//===--- Rule3bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3bCheck.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Token.h"
#include <iostream>
#include <vector>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {

Rule3bCheck::Rule3bCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule3bCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(stmt().bind("relex"), this);
    Finder->addMatcher(decl().bind("relex"), this);
    Finder->addMatcher(binaryOperator().bind("binary_operator"), this);
}

void Rule3bCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();
    const SourceManager &SM = *Result.SourceManager;
    SourceLocation loc;
    std::string name = "";
    std::string type = "";

    if (auto MatchedDecl = Result.Nodes.getNodeAs<BinaryOperator>("binary_operator")) {
        if (not SM.isWrittenInMainFile(MatchedDecl->getOperatorLoc())) {
            return;
        }
        loc = MatchedDecl->getOperatorLoc();
        name = MatchedDecl->getOpcodeStr().str();
        type = "Binary Operator";

        if (MatchedDecl->isPtrMemOp()) { // Only want to check this if it
                                         // isn't a -> op
                                         // Nothing
        } else {
            // Set up the lexer
            CharSourceRange Range = CharSourceRange::getTokenRange(
                MatchedDecl->getBeginLoc(), MatchedDecl->getEndLoc());
            const SourceManager &Sources = *Result.SourceManager;
            std::pair<FileID, unsigned> LocInfo =
                Sources.getDecomposedLoc(Range.getBegin());
            StringRef File = Sources.getBufferData(LocInfo.first);
            const char *TokenBegin = File.data() + LocInfo.second;
            Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                           Result.Context->getLangOpts(), File.begin(), TokenBegin,
                           File.end());

            RawLexer.SetKeepWhitespaceMode(true);

            // Lex the file
            Token tok;
            bool donext = false;
            Token next;
            std::vector<Token> tokens;

            while (!RawLexer.LexFromRawLexer(tok)) {
                if (tok.getLocation() == MatchedDecl->getOperatorLoc()) {
                    if (SM.isWrittenInMainFile(tok.getLocation())) {
                        std::string match(SM.getCharacterData(tok.getLocation()),
                                          SM.getCharacterData(tok.getEndLoc()));
                        // std::cout << "GOT MATCH: |" << match << "|" <<
                        // std::endl;
                    }

                    if (RawLexer.LexFromRawLexer(next)) {
                        break;
                    }
                    donext = true;

                    if (not(tokens.empty()) &&
                        SM.isWrittenInMainFile(tokens.back().getLocation())) {
                        std::string match(
                            SM.getCharacterData(tokens.back().getLocation()),
                            SM.getCharacterData(tokens.back().getEndLoc()));
                        // std::cout << "PRECEEDING MATCH: |" << match <<
                        // "|" << std::endl;
                    }

                    if (not(tokens.empty()) &&
                        SM.isWrittenInMainFile(tokens.back().getLocation()) &&
                        not MatchedDecl->isCommaOp() &&
                        std::string(SM.getCharacterData(tokens.back().getLocation()),
                                    SM.getCharacterData(tokens.back().getEndLoc())) !=
                            " " &&
                        not tok.isAtStartOfLine()) {
                        auto errmsg =
                            diag(tok.getLocation(), "Leading space required.");
                        errmsg << FixItHint::CreateInsertion(tok.getLocation(), " ");
                    }

                    if (SM.isWrittenInMainFile(next.getLocation())) {
                        std::string match(SM.getCharacterData(next.getLocation()),
                                          SM.getCharacterData(next.getEndLoc()));

                        if (match != " " && match.find('\n') == std::string::npos) {
                            auto errmsg =
                                diag(tok.getEndLoc(), "Trailing space required.");
                            errmsg << FixItHint::CreateInsertion(tok.getEndLoc(), " ");
                        }
                    }

                    return;
                }
                if (SM.isWrittenInMainFile(tok.getLocation())) {
                    tokens.push_back(tok);
                    if (donext && SM.isWrittenInMainFile(next.getLocation())) {
                        donext = false;
                        tokens.push_back(next);
                    }
                }
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

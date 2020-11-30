
//===--- Rule3cCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3cCheck.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule3cCheck::registerMatchers(MatchFinder * Finder) {
                Finder->addMatcher(functionDecl().bind("function_decl"), this);
            }
            void Rule3cCheck::check(const MatchFinder::MatchResult & Result) {
                const SourceManager & SM = *Result.SourceManager;
                const ASTContext * Context = Result.Context;

                if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
                    if (not MatchedDecl->isDefined()) {
                        return;
                    }
                    const FunctionDecl * FunctionDefinition = MatchedDecl->getDefinition();
                    std::string fname = FunctionDefinition->getNameInfo().getName().getAsString();
                    if (FunctionDefinition) {
                        SourceRange FunctionDefinitionRange = FunctionDefinition->getSourceRange();
                        std::pair<FileID, unsigned> LocInfo = SM.getDecomposedLoc(FunctionDefinitionRange.getBegin());
                        SourceLocation StartOfFile = SM.getLocForStartOfFile(SM.getFileID(FunctionDefinitionRange.getBegin()));
                        StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
                        const char * TokenBegin = File.data();

                        Lexer RawLexer(SM.getLocForStartOfFile(LocInfo.first),
                                Context->getLangOpts(), File.begin(), TokenBegin, File.end());

                        RawLexer.SetKeepWhitespaceMode(true);

                        if (not this->checked) {
                            Token tok;
                            while (!RawLexer.LexFromRawLexer(tok)) {
                                if (tok.isOneOf(tok::semi, tok::comma)) {
                                    Token report = tok;
                                    if (!RawLexer.LexFromRawLexer(tok)) {
                                        std::string next_content(SM.getCharacterData(tok.getLocation()),
                                                SM.getCharacterData(tok.getEndLoc()));
                                        if (next_content == " " or (next_content != "" and next_content[0] == '\n')) {
                                            continue;
                                        }
                                        diag(report.getEndLoc(), "Expected single space.");
                                    }
                                }
                            }
                            this->checked = true;
                        }
                    }
                }
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

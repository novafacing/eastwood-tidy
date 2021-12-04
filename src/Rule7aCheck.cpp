
//===--- Rule7aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule7aCheck.h"
#include <iostream>
#include <regex>
#include <vector>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            Rule7aCheck::Rule7aCheck(StringRef Name, ClangTidyContext *Context)
                : ClangTidyCheck(Name, Context),
                  debug_enabled(Options.get("debug", "false")) {
                if (this->debug_enabled == "true") {
                    this->debug = true;
                }
            }
            void Rule7aCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(functionDecl().bind("function_decl"), this);
            }
            void Rule7aCheck::check(const MatchFinder::MatchResult &Result) {

                const SourceManager &SM = *Result.SourceManager;
                const ASTContext *Context = Result.Context;

                if (auto MatchedDecl =
                        Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
                    if (not MatchedDecl->isDefined()) {
                        return;
                    }
                    const FunctionDecl *FunctionDefinition =
                        MatchedDecl->getDefinition();
                    std::string fname =
                        FunctionDefinition->getNameInfo().getName().getAsString();
                    if (FunctionDefinition) {
                        SourceRange FunctionDefinitionRange =
                            FunctionDefinition->getSourceRange();
                        std::pair<FileID, unsigned> LocInfo =
                            SM.getDecomposedLoc(FunctionDefinitionRange.getBegin());
                        SourceLocation StartOfFile = SM.getLocForStartOfFile(
                            SM.getFileID(FunctionDefinitionRange.getBegin()));
                        StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
                        const char *TokenBegin = File.data();

                        Lexer RawLexer(SM.getLocForStartOfFile(LocInfo.first),
                                       Context->getLangOpts(), File.begin(), TokenBegin,
                                       File.end());

                        RawLexer.SetKeepWhitespaceMode(true);

                        Token tok;
                        std::vector<Token> tokens;
                        while (!RawLexer.LexFromRawLexer(tok)) {
                            if (tok.getLocation() ==
                                FunctionDefinitionRange.getBegin()) {
                                /*std::cout << "Stopping lex at token |" <<
                                   std::string(SM.getCharacterData(tok.getLocation()),
                                    SM.getCharacterData(tok.getEndLoc())) << "|" <<
                                   std::endl; */
                                break;
                            }
                            tokens.push_back(tok);
                        }
                        /*std::cout << "Token range: |" <<
                           std::string(SM.getCharacterData(tokens.at(tokens.size() -
                           3).getLocation()),
                                    SM.getCharacterData(tokens.back().getEndLoc())) <<
                           "|" << std::endl; */
                        if (tokens.size() >= 3) {
                            std::vector<Token>::reverse_iterator rit = tokens.rbegin();

                            size_t comment_ct = 0;
                            for (size_t i = 0; rit != tokens.rend() and i < 3;
                                 rit++, i++) {
                                if (rit->getKind() == tok::comment) {
                                    comment_ct++;
                                }
                            }
                            if (comment_ct == 0) {
                                diag(tokens.back().getEndLoc(),
                                     "Missing header comment.");
                                return;
                            }

                            if (not(std::string(SM.getCharacterData(
                                                    tokens.back().getLocation()),
                                                SM.getCharacterData(
                                                    tokens.back().getEndLoc())) ==
                                    "\n\n")) {
                                diag(tokens.back().getEndLoc(), "Empty line required.");
                                return;
                            }

                            if (not(std::string(
                                        SM.getCharacterData(
                                            tokens.at(tokens.size() - 3).getLocation()),
                                        SM.getCharacterData(tokens.at(tokens.size() - 3)
                                                                .getEndLoc())) ==
                                    "\n\n")) {
                                diag(tokens.at(tokens.size() - 2).getLocation(),
                                     "Empty line required.");
                                return;
                            }
                            // Make sure the last token is just a newline and the
                            // previous is a comment. Then, check for comment formatting
                            if (not isWhitespace(*SM.getCharacterData(
                                    tokens.back().getLocation()))) {
                                diag(tokens.back().getEndLoc(), "Empty line required.");
                                return;
                            }
                            if (not isWhitespace(*SM.getCharacterData(
                                    tokens.at(tokens.size() - 3).getLocation()))) {
                                diag(tokens.at(tokens.size() - 2).getLocation(),
                                     "Empty line required.");
                                return;
                            }
                            if (tokens.at(tokens.size() - 2).getKind() ==
                                tok::comment) {
                                std::string raw_header_comment(
                                    SM.getCharacterData(
                                        tokens.at(tokens.size() - 2).getLocation()),
                                    SM.getCharacterData(
                                        tokens.at(tokens.size() - 2).getEndLoc()));

                                std::regex pre_regex{
                                    R"([\/][*]([^\n]*[\n][ ][*])([ ]*[^ ][^\n]*[\n][ ][*]){1,}[\/])"};
                                std::smatch result;
                                if (std::regex_match(raw_header_comment, result,
                                                     pre_regex)) {
                                    return;
                                }
                                // std::cout << "Print A" << std::endl;
                                diag(tokens.at(tokens.size() - 2).getLocation(),
                                     "Malformed header comment.");
                                return;
                            }
                            diag(tokens.back().getEndLoc(), "Missing header comment.");
                            return;
                        }
                        // std::cout << "Pring B" << std::endl;
                        diag(tok.getLocation(), "Malformed header comment.");
                    }
                }
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

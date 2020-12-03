
//===--- Rule5dCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule5dCheck.h"
#include <iostream>
#include <vector>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule5dCheck::registerMatchers(MatchFinder * Finder) {
                Finder->addMatcher(functionDecl().bind("function_decl"), this);
            }
            void Rule5dCheck::check(const MatchFinder::MatchResult & Result) {
                const SourceManager & SM = *Result.SourceManager;
                const ASTContext * Context = Result.Context;

                if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
                    if (not MatchedDecl->isDefined()) {
                        return;

                    }
                    const FunctionDecl * FunctionDefinition = MatchedDecl->getDefinition();
                    auto NI = FunctionDefinition->getNameInfo();
                    std::string fname = NI.getName().getAsString();
                    if (FunctionDefinition) {
                        SourceRange FunctionDefinitionRange = FunctionDefinition->getBody()->getSourceRange();
                        std::pair<FileID, unsigned> LocInfo = SM.getDecomposedLoc(FunctionDefinitionRange.getEnd());
                        SourceLocation StartOfFile = SM.getLocForStartOfFile(SM.getFileID(FunctionDefinitionRange.getEnd()));
                        StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
                        const char * TokenBegin = File.data() + LocInfo.second;

                        Lexer RawLexer(SM.getLocForStartOfFile(LocInfo.first),
                                Context->getLangOpts(), File.begin(), TokenBegin, File.end());

                        RawLexer.SetKeepWhitespaceMode(true);

                        Token tok;
                        std::vector<Token> ws_tokens;
                        size_t ws_ct = 0;
                        if (RawLexer.LexFromRawLexer(tok)) {
                            // TODO: maybe diag here but this would indicate a syntax error anyway
                            return;
                        }
                        while (!RawLexer.LexFromRawLexer(tok) and isWhitespace(*SM.getCharacterData(tok.getLocation()))) {
                            ws_ct++;
                            ws_tokens.push_back(tok);
                        }
                        if (not ws_tokens.empty()) {
                            if (ws_ct != 1 or std::string(SM.getCharacterData(ws_tokens.back().getLocation()), 
                                        SM.getCharacterData(ws_tokens.back().getEndLoc())).size() > 1) {
                                if (SM.getSpellingLineNumber(ws_tokens.back().getLocation()) == SM.getSpellingLineNumber(ws_tokens.back().getEndLoc())) {
                                    diag(tok.getLocation(), "Function footer comment must be separated by 1 space");
                                }
                            }
                        }
                        if (tok.getKind() == tok::comment) {
                            std::string comment_raw(SM.getCharacterData(tok.getLocation()), SM.getCharacterData(tok.getEndLoc()));
                            std::string correct_annotation("/* " + fname + "() */");
                            if (comment_raw != "/* " + fname + "() */") {
                                diag(tok.getLocation(), std::string("End of function comment is malformed. Got \"") + comment_raw + "\" Expected \"/* " + fname + "() */\".");
                                return;
                            } else {
                                return;
                            }
                        }
                        diag(FunctionDefinitionRange.getEnd(), "Missing function footer comment");
                    }
                }
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

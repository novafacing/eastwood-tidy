
//===--- Rule3fCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3fCheck.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule3fCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(functionDecl().bind("function_decl"), this);
            }
            void Rule3fCheck::check(const MatchFinder::MatchResult &Result) {
                const SourceManager &SM = *Result.SourceManager;
                const ASTContext *Context = Result.Context;

                if (auto MatchedDecl =
                        Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
                    DeclarationNameInfo NameInfo = MatchedDecl->getNameInfo();
                    SourceLocation NameEnd(NameInfo.getEndLoc());
                    SourceRange ParamsRange = MatchedDecl->getParametersSourceRange();
                    std::pair<FileID, unsigned> LocInfo = SM.getDecomposedLoc(NameEnd);
                    SourceLocation StartOfFile =
                        SM.getLocForStartOfFile(SM.getFileID(NameEnd));
                    StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
                    const char *TokenBegin = File.data() + LocInfo.second;

                    Lexer RawLexer(SM.getLocForStartOfFile(LocInfo.first),
                                   Context->getLangOpts(), File.begin(), TokenBegin,
                                   File.end());

                    RawLexer.SetKeepWhitespaceMode(true);

                    Token tok;
                    while (not RawLexer.LexFromRawLexer(tok)) {
                        if (tok.getLocation() == NameInfo.getEndLoc()) {
                            if (RawLexer.LexFromRawLexer(tok)) {
                                return;
                            }
                            if (std::string(SM.getCharacterData(tok.getLocation()),
                                            SM.getCharacterData(tok.getEndLoc())) !=
                                "(") {
                                std::string match(
                                    SM.getCharacterData(tok.getLocation()),
                                    SM.getCharacterData(tok.getEndLoc()));
                                // std::cout << "Matched offender: |" << match << "|" <<
                                // std::endl;
                                if (SM.isWrittenInMainFile(tok.getLocation())) {
                                    diag(tok.getLocation(),
                                         "No space permitted between function name and "
                                         "parameter list.");
                                }

                            } else {
                                std::string match(
                                    SM.getCharacterData(tok.getLocation()),
                                    SM.getCharacterData(tok.getEndLoc()));
                                // std::cout << "Matched offender: |" << match << "|" <<
                                // std::endl;
                            }
                            return;
                        }
                    }
                }
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

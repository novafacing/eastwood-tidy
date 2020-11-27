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

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {

            void Rule3bCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(binaryOperator().bind("binary_operator"), this);
            }

            void Rule3bCheck::check(const MatchFinder::MatchResult &Result) {
                SourceLocation loc;
                std::string name = "";
                std::string type = "";

                if (auto MatchedDecl = Result.Nodes.getNodeAs<BinaryOperator>("binary_operator")) {
                    loc = MatchedDecl->getOperatorLoc();
                    name = MatchedDecl->getOpcodeStr().str();
                    type = "Binary Operator";

                    if (MatchedDecl->isPtrMemOp()) { // Only want to check this if it isn't a -> op
                        // Nothing
                    } else {
                        // Set up the lexer
                        CharSourceRange Range = CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc(), 
                                MatchedDecl->getEndLoc());
                        const SourceManager & Sources = *Result.SourceManager;
                        std::pair<FileID, unsigned> LocInfo = Sources.getDecomposedLoc(Range.getBegin());
                        StringRef File = Sources.getBufferData(LocInfo.first);
                        const char * TokenBegin = File.data() + LocInfo.second;
                        Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                                Result.Context->getLangOpts(), File.begin(), TokenBegin,
                                File.end());

                        // Lex the file
                        Token tok;
                        llvm::Optional<Token> ConstTok;
                        while (!RawLexer.LexFromRawLexer(tok)) {

                        }



                        if (MatchedDecl->isCommaOp()) { // Only check for space on the RHS of operator
                            SourceLocation end = MatchedDecl->getEndLoc();
                        } else { // Check normally (spaces on both sides of operator)
                        }
                    }
                } else {
                    return;
                }

                if ((Result.SourceManager)->isWrittenInMainFile(loc)) {
                }
            }

        } // namespace eastwood
    } // namespace tidy
} // namespace clang

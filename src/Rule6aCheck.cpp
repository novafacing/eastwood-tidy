
//===--- Rule6aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule6aCheck.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule6aCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(binaryOperator(hasAnyOperatorName("&&", "||")).bind("op"), this);
            }

            void Rule6aCheck::checkBinaryParens(Expr *E, SourceManager &SM) {
                if (const BinaryOperator *BO = dyn_cast<BinaryOperator>(E)) {
                    SourceLocation beg = BO->getBeginLoc();
                    SourceLocation end = BO->getEndLoc();
                    std::string FullOperation(SM.getCharacterData(beg), SM.getCharacterData(end));
                    if (FullOperation.size() >= 2) {
                        if (FullOperation[0] != '(' or FullOperation[FullOperation.size() - 1] != ')') {
                            diag(BO->getOperatorLoc(), "This sub-expression must be surrounded by parentheses.");
                        }
                    } else {
                        diag(BO->getOperatorLoc(), "This sub-expression must be surrounded by parentheses.");
                    }
                    this->checkBinaryParens(BO->getLHS(), SM);
                    this->checkBinaryParens(BO->getRHS(), SM);
                }
            }

            void Rule6aCheck::check(const MatchFinder::MatchResult &Result) {
                ASTContext *Context = Result.Context;
                SourceManager &SM = *Result.SourceManager;

                if (auto MatchedDecl = Result.Nodes.getNodeAs<BinaryOperator>("op")) {
                    Expr *LHS = MatchedDecl->getLHS();
                    Expr *RHS = MatchedDecl->getRHS();
                    this->checkBinaryParens(LHS, SM);
                    this->checkBinaryParens(RHS, SM);
                }
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

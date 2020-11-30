
//===--- Rule4bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule4bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule4bCheck::registerMatchers(MatchFinder * Finder) {
                Finder->addMatcher(functionDecl().bind("function_decl"), this);
            }
            void Rule4bCheck::check(const MatchFinder::MatchResult & Result) {
                const SourceManager & SM = *Result.SourceManager;
                if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
                    if (MatchedDecl->param_size() < 2) {
                        return;
                    }
                    unsigned FirstCol = SM.getSpellingColumnNumber(MatchedDecl->getParamDecl(0)->getBeginLoc());
                    unsigned FirstLineNum = SM.getSpellingLineNumber(MatchedDecl->getParamDecl(0)->getBeginLoc());
                    unsigned CurrentLine = FirstLineNum;
                    for (auto Param : MatchedDecl->parameters()) {
                        if (SM.getSpellingLineNumber(Param->getBeginLoc()) > CurrentLine) {
                            CurrentLine = SM.getSpellingLineNumber(Param->getBeginLoc());
                            if (SM.getSpellingColumnNumber(Param->getBeginLoc()) != FirstCol) {
                                diag(Param->getBeginLoc(), "Line-broken parameter is not aligned with first parameter.");
                            }
                        }
                    }
                }
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

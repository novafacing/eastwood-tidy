//===--- Rule11eCheck.cpp - clang-tidy ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule11eCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {

            void Rule11eCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(gotoStmt().bind("goto"), this);
            }

            void Rule11eCheck::check(const MatchFinder::MatchResult &Result) {
                const auto * MatchedDecl = Result.Nodes.getNodeAs<GotoStmt>("goto");
                diag(MatchedDecl->getBeginLoc(), "Use of goto is prohibited.");
            }

        } // namespace eastwood
    } // namespace tidy
} // namespace clang

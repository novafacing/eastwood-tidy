
//===--- Rule11eCheck.cpp - clang-tidy
//-------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule11eCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
void Rule11eCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(gotoStmt().bind("goto"), this);
}
void Rule11eCheck::check(const MatchFinder::MatchResult &Result) {
    if (auto MatchedDecl = Result.Nodes.getNodeAs<GotoStmt>("goto")) {
        diag(MatchedDecl->getBeginLoc(), "Use of goto.");
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

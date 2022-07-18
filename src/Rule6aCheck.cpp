
//===--- Rule6aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule6aCheck.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule6aCheck::Rule6aCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule6aCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(binaryOperator(hasAnyOperatorName("&&", "||")).bind("op"), this);
}

void Rule6aCheck::checkBinaryParens(BinaryOperator *BO, SourceManager &SM,
                                    size_t depth) {

    Expr *LHS = BO->getLHS();
    Expr *RHS = BO->getRHS();
    BinaryOperator *LHS_BO = nullptr;
    BinaryOperator *RHS_BO = nullptr;

    // Sanity checks that will cause segv
    if ((LHS_BO = dyn_cast<BinaryOperator>(LHS->IgnoreParenCasts())) &&
        LHS_BO->isLogicalOp()) {
        this->checkBinaryParens(LHS_BO, SM, depth + 1);

    } else {
        if (!dyn_cast<ParenExpr>(LHS)) {
            if (auto NL_OP = dyn_cast<BinaryOperator>(LHS->IgnoreParenCasts())) {
                auto errmsg =
                    diag(NL_OP->getOperatorLoc(), "This sub-expression must be "
                                                  "surrounded by parentheses");
                errmsg << FixItHint::CreateInsertion(
                    LHS->getBeginLoc().getLocWithOffset(-1), "(");
                errmsg << FixItHint::CreateInsertion(
                    LHS->getEndLoc().getLocWithOffset(1), ")");
            }
        }
    }

    if ((RHS_BO = dyn_cast<BinaryOperator>(RHS->IgnoreParenCasts())) &&
        RHS_BO->isLogicalOp()) {
        this->checkBinaryParens(RHS_BO, SM, depth + 1);

    } else {
        if (!dyn_cast<ParenExpr>(RHS)) {
            if (auto NL_OP = dyn_cast<BinaryOperator>(RHS->IgnoreParenCasts())) {
                auto errmsg =
                    diag(NL_OP->getOperatorLoc(), "This sub-expression must be "
                                                  "surrounded by parentheses");
                errmsg << FixItHint::CreateInsertion(
                    RHS->getBeginLoc().getLocWithOffset(-1), "(");
                errmsg << FixItHint::CreateInsertion(
                    RHS->getEndLoc().getLocWithOffset(1), ")");
            }
        }
    }
}

void Rule6aCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    SourceManager &SM = *Result.SourceManager;

    if (auto MatchedDecl = Result.Nodes.getNodeAs<BinaryOperator>("op")) {
        Expr *LHS = MatchedDecl->getLHS();
        Expr *RHS = MatchedDecl->getRHS();
        if (LHS && RHS) {
            this->checkBinaryParens(const_cast<BinaryOperator *>(MatchedDecl), SM);
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

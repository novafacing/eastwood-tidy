//===--- Rule2bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Functions must be less than 2 pages (240 lines)

#include "Rule2bCheck.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule2bCheck::Rule2bCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}
void Rule2bCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(functionDecl().bind("function"), this);
}

void Rule2bCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        std::string name = MatchedDecl->getNameAsString();

        const FunctionDecl *FunctionDefinition = MatchedDecl->getDefinition();
        if (!MatchedDecl->doesThisDeclarationHaveABody()) {
            return;
        }
        unsigned func_start_line = 0;
        if (this->source_manager->isWrittenInMainFile(
                FunctionDefinition->getReturnTypeSourceRange().getBegin())) {
            func_start_line = this->source_manager->getSpellingLineNumber(
                FunctionDefinition->getReturnTypeSourceRange().getBegin());
        } else {
            func_start_line = this->source_manager->getSpellingLineNumber(
                FunctionDefinition->getNameInfo().getLoc());
        }
        SourceLocation end = FunctionDefinition->getSourceRange().getEnd();
        unsigned end_ln = this->source_manager->getSpellingLineNumber(end);

        if (end_ln - func_start_line >= MAX_FN_SIZE) {
            auto errmsg = diag(FunctionDefinition->getSourceRange().getBegin(),
                               "Function %0 is over the maximum function size"
                               " of %1 lines")
                          << name << MAX_FN_SIZE;
        }
    } else {
        return;
    }
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

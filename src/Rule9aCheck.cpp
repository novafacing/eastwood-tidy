
//===--- Rule9aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule9aCheck.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule9aCheck::Rule9aCheck(StringRef name, ClangTidyContext *Context)
    : ClangTidyCheck(name, Context), EastwoodTidyCheckBase(name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}
void Rule9aCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(
        varDecl(hasInitializer(callExpr(callee(functionDecl(
                    anyOf(hasName("fopen"), hasName("malloc"), hasName("calloc"),
                          hasName("fread"), hasName("fwrite"), hasName("system")))))))
            .bind("var_from_call"),
        this);
}
void Rule9aCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();
    if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("var_from_call")) {
        // MatchedDecl.
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang


//===--- Rule12bCheck.cpp - clang-tidy
//-------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule12bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule12bCheck::Rule12bCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule12bCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(varDecl().bind("var_decl"), this);
}

void Rule12bCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("var_decl")) {
        if (MatchedDecl->isLocalVarDeclOrParm() && not MatchedDecl->isLocalVarDecl()) {
            // This is a function parameter
            return;
        }
        if (not MatchedDecl->hasExternalStorage()) {
            if (not MatchedDecl->hasInit()) {
                if ((Result.SourceManager)
                        ->isWrittenInMainFile(MatchedDecl->getLocation())) {
                    diag(MatchedDecl->getLocation(),
                         "Variable declaration without definition is "
                         "forbidden.");
                }
            }
            if (MatchedDecl->getInitializingDeclaration() != MatchedDecl) {
                diag(MatchedDecl->getLocation(),
                     "Variable declaration without definition is "
                     "forbidden.");
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

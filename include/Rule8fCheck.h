
//===--- Rule8fCheck.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE8FCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE8FCHECK_H

#include "../ClangTidyCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace clang {
    namespace tidy {
        namespace eastwood {

            class Rule8fCheck : public ClangTidyCheck {
                public:
                    /* Constructors */
                    Rule8fCheck(StringRef Name, ClangTidyContext *Context)
                        : ClangTidyCheck(Name, Context) {}

                    /* Overrides */
                    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
                    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
            }; // Rule8fCheck
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE8FCHECK_H
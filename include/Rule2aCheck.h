
//===--- Rule2aCheck.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE2ACHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE2ACHECK_H

#include "../ClangTidyCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <vector>

namespace clang {
    namespace tidy {
        namespace eastwood {

            class Rule2aCheck : public ClangTidyCheck {
                public:
                    /* Constructors */
                    Rule2aCheck(StringRef Name, ClangTidyContext *Context)
                        : ClangTidyCheck(Name, Context), checked(false) {}

                    /* Overrides */
                    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
                    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

                    std::vector<size_t> over_length_lines;
                    std::vector<size_t> broken_lines;
                    bool checked;

            }; // Rule2aCheck
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE2ACHECK_H

//===--- Rule4cCheck.h - clang-tidy -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE4CCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE4CCHECK_H

#include "../ClangTidyCheck.h"

namespace clang {
    namespace tidy {
        namespace eastwood {

            /// Checks that bodies of `if` statements and loops (`for`, `range-for`,
            /// `do-while`, and `while`) are inside braces
            ///
            class Rule4cCheck : public ClangTidyCheck {
                public:
                    Rule4cCheck(StringRef Name, ClangTidyContext *Context);
                    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
                    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

                private:
                    bool checkStmt(const ast_matchers::MatchFinder::MatchResult &Result,
                            const Stmt *S, SourceLocation StartLoc,
                            SourceLocation EndLocHint = SourceLocation());
                    template <typename IfOrWhileStmt>
                        SourceLocation findRParenLoc(const IfOrWhileStmt *S, const SourceManager &SM,
                                const ASTContext *Context);

            };

        } // namespace eastwood
    } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE4CCHECK_H

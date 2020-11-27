
//===--- Rule3aCheck.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE3ACHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE3ACHECK_H

#include "../ClangTidyCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {

            class Rule3aCheck : public ClangTidyCheck {
                public:
                    /* Constructors */
                    Rule3aCheck(StringRef Name, ClangTidyContext *Context)
                        : ClangTidyCheck(Name, Context) {}

                    /* Overrides */
                    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
                    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
                    void checkStmt(const MatchFinder::MatchResult & Result,
                            const Stmt * S,
                            SourceLocation InitialLoc, SourceLocation EndLocHint);
                    template<typename IfOrWhileStmt>
                    SourceLocation findRParenLoc(const IfOrWhileStmt * S,
                            const SourceManager & SM,
                            const ASTContext * Context);
            }; // Rule3aCheck
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE3ACHECK_H


//===--- Rule5bCheck.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE5BCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE5BCHECK_H

#include "../ClangTidyCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/Token.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <vector>
#include <iostream>

namespace clang {
    namespace tidy {
        namespace eastwood {
            class Rule5bVisitor : public RecursiveASTVisitor<Rule5bVisitor> {
                public:
                    explicit Rule5bVisitor(ASTContext * Context) : Context(Context) {};
                    std::vector<SourceLocation> function_decl_ends;
                    bool VisitDecl(Decl * d) {
                        if (d->isFunctionOrFunctionTemplate()) {
                            FunctionDecl * FD = d->getAsFunction();
                            if (FD->isDefined()) {
                                FunctionDecl * FDef = FD->getDefinition();
                                SourceRange FunctionDefinitionRange = FDef->getBody()->getSourceRange();
                                this->function_decl_ends.push_back(FunctionDefinitionRange.getEnd());
                            }
                        }
                        return true;
                    }
                private:
                    ASTContext * Context;
            };

            class Rule5bCheck : public ClangTidyCheck {
                public:
                    /* Constructors */
                    Rule5bCheck(StringRef Name, ClangTidyContext *Context)
                        : ClangTidyCheck(Name, Context), visited(false), checked(false) {}

                    /* Overrides */
                    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
                    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
                    bool visited;
                    bool checked;
                    std::vector<SourceLocation> function_decl_ends;

            }; // Rule5bCheck
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE5BCHECK_H

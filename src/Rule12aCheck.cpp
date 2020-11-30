
//===--- Rule12aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule12aCheck.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule12aCheck::registerMatchers(MatchFinder * Finder) {
                Finder->addMatcher(varDecl().bind("var_decl"), this);
            }

            void Rule12aCheck::check(const MatchFinder::MatchResult & Result) {
                if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("var_decl")) {
                    if (MatchedDecl->isLocalVarDeclOrParm() and not MatchedDecl->isLocalVarDecl()) {
                        // This is a function parameter
                        return;
                    }
                    const SourceManager & SM = *Result.SourceManager;
                    const ASTContext * Context = Result.Context;
                    if (not MatchedDecl->hasExternalStorage()) {
                        unsigned line_num = SM.getSpellingLineNumber(MatchedDecl->getLocation());
                        if (this->first) {
                            this->first = false;
                        } else {
                            if (line_num == this->last_line) {
                                if (SM.isWrittenInMainFile(MatchedDecl->getLocation())) {
                                    diag(MatchedDecl->getLocation(), "Multiple variable declarations on a single line are not allowed.");
                                }
                            }
                        }
                        this->last_line = line_num;
                    }
                }
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

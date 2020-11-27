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
            void Rule2bCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(functionDecl().bind("function"), this);
            }

            void Rule2bCheck::check(const MatchFinder::MatchResult &Result) {
                if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
                    SourceLocation loc = MatchedDecl->getLocation();
                    std::string name = MatchedDecl->getName().str();
                    std::string type = "Function";
                    if ((Result.SourceManager)->isWrittenInMainFile(loc)) {
                        SourceRange range = MatchedDecl->getSourceRange();
                        SourceLocation beg = range.getBegin();
                        SourceLocation end = range.getEnd();
                        unsigned beg_ln = (Result.SourceManager)->getSpellingLineNumber(beg, nullptr);
                        unsigned end_ln = (Result.SourceManager)->getSpellingLineNumber(end, nullptr);
                        if (end_ln - beg_ln >= MAX_FN_SIZE) {
                            diag(loc, "%0 %1 is over the maximum function size"
                                    " of %2 lines") << type << name << MAX_FN_SIZE;
                        }

                    }
                } else {
                    return;
                }

            }

        } // namespace eastwood
    } // namespace tidy
} // namespace clang

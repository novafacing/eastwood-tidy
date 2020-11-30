
//===--- Rule5bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule5bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule5bCheck::registerMatchers(MatchFinder * Finder) {
            }
            void Rule5bCheck::check(const MatchFinder::MatchResult & Result) {
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

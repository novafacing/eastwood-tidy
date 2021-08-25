import os

SRCTEMPLATE="""
//===--- Rule{}Check.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule{}Check.h"

using namespace clang::ast_matchers;

namespace clang {{
    namespace tidy {{
        namespace eastwood {{
            void Rule{}Check::registerMatchers(MatchFinder * Finder) {{
            }}
            void Rule{}Check::check(const MatchFinder::MatchResult & Result) {{
            }}
        }} // namespace eastwood
    }} // namespace tidy
}} // namespace clang
"""

HEADERTEMPLATE="""
//===--- Rule{}Check.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE{}CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE{}CHECK_H

#include "@CLANG_TIDY_TOPLEVEL@/ClangTidyCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace clang {{
    namespace tidy {{
        namespace eastwood {{

            class Rule{}Check : public ClangTidyCheck {{
                public:
                    /* Constructors */
                    Rule{}Check(StringRef Name, ClangTidyContext *Context)
                        : ClangTidyCheck(Name, Context) {{}}

                    /* Overrides */
                    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
                    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
            }}; // Rule{}Check
        }} // namespace eastwood
    }} // namespace tidy
}} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE{}CHECK_H
"""

for check in [
            "2A",
            "3A",
            "3C",
            "3D",
            "3E",
            "3F",
            "4B",
            "5A",
            "5B",
            "5C",
            "5D",
            "5E",
            "6A",
            "7A",
            "8A",
            "8B",
            "8C",
            "8D",
            "8E",
            "8F",
            "8G",
            "9A",
            "9B",
            "9C",
            "9D",
            "9E",
            "10A",
            "11A",
            "11B",
            "11C",
            "11D",
            "11E",
            "12A",
            "12B",
            "12C"
        ]:
    with open(os.path.join('src', 'Rule{}Check.cpp'.format(check.lower())), 'w') as srcfile:
        srcfile.write(SRCTEMPLATE.format(check.lower(), check.lower(), check.lower(), check.lower()))

    with open(os.path.join('include', 'Rule{}Check.h'.format(check.lower())), 'w') as headerfile:
        headerfile.write(HEADERTEMPLATE.format(check.lower(), check, check, check.lower(),
            check.lower(), check.lower(), check))


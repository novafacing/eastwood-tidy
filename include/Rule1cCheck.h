//===--- Rule1cCheck.h - clang-tidy--------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE1CCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE1CCHECK_H

#include "../ClangTidy.h"
#include "../ClangTidyCheck.h"
#include "clang/Lex/Preprocessor.h"
#include <string>
#include <regex>
#include <map>
#include <utility>
#include <iostream>
#include <iomanip>
#include <vector>

namespace clang {
    namespace tidy {
        namespace eastwood {

            /// Find macro usage that is considered problematic because better language
            /// constructs exist for the task.
            ///
            /// For the user-facing documentation see:
            /// http://clang.llvm.org/extra/clang-tidy/checks/eastwood-macro-usage.html
            class Rule1cCheck : public ClangTidyCheck {
                private:
                    std::map<std::string, std::vector<SourceLocation>> EmbeddedConstants;
                    std::vector<SourceRange> DeclarationRanges;
                    std::string Dump;
                public:
                    Rule1cCheck(StringRef Name, ClangTidyContext *Context);
                    void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                            Preprocessor *ModuleExpanderPP) override;
                    void registerMatchers(ast_matchers::MatchFinder * Finder) override;
                    void saveEmbeddedConstant(SourceLocation loc, std::string type);
                    void check(const ast_matchers::MatchFinder::MatchResult & Result) override;
                    void onEndOfTranslationUnit() override;
                    void storeOptions(ClangTidyOptions::OptionMap & Opts) override;
            };

        } // namespace eastwood
    } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE1CCHECK_H

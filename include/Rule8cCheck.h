
//===--- Rule8cCheck.h - clang-tidy -----------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE8CCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE8CCHECK_H

#include "../ClangTidyCheck.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include <utility>
#include <vector>
#include <map>

namespace clang {
    namespace tidy {
        namespace eastwood {

            class Rule8cCheck : public ClangTidyCheck {
                public:
                    /* Constructors */
                    Rule8cCheck(StringRef Name, ClangTidyContext *Context)
                        : ClangTidyCheck(Name, Context) {}

                    /* Overrides */
                    void registerPPCallbacks(const SourceManager & SM,
                            Preprocessor * PP, Preprocessor * ModuleExpanderPP) override;
                    void onEndOfTranslationUnit() override;
                    void registerMatchers(ast_matchers::MatchFinder *Finder) override;
                    void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
                    std::vector<std::pair<std::string, SourceLocation>> required_guards;
                    std::vector<std::string> found_ifndef_checks;
                    std::vector<std::string> found_defined_macro_guards;
                    std::map<std::string, std::string> filenames_guards;
            }; // Rule8cCheck
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE8CCHECK_H

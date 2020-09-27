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

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace clang {
    namespace tidy {
        namespace eastwood {

            class Rule1cCheck : public ClangTidyCheck {
                private:
                    std::map<std::string, std::vector<SourceLocation>> embeddedConstants;
                    std::vector<SourceRange> declarationRanges;
                    std::string dump;

                public:
                    /* Constructors */
                    Rule1cCheck(StringRef Name, ClangTidyContext *Context);

                    /* Overrides */
                    void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                            Preprocessor *ModuleExpanderPP) override;
                    void registerMatchers(ast_matchers::MatchFinder * Finder) override;
                    void check(const ast_matchers::MatchFinder::MatchResult & Result) override;
                    void onEndOfTranslationUnit() override;
                    void storeOptions(ClangTidyOptions::OptionMap & Opts) override;

                    void saveEmbeddedConstant(SourceLocation loc, std::string type);
            }; // Rule1cCheck

        } // namespace eastwood
    } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE1CCHECK_H


//===--- Rule2aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Each line must be <= 80 columns. If it is more, it should be broken up and
// indented
// >= 2 spaces

#include "Rule2aCheck.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "clang/Lex/Lexer.h"
#include "clang/Lex/Token.h"

#define MAX_LINE_LEN (80)

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule2aCheck::Rule2aCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule2aCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
}

void Rule2aCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();
}

void Rule2aCheck::onEndOfTranslationUnit() {
    if (!this->source_manager) {
        this->dout() << "No source manager, not checking line lengths" << std::endl;
    }
    for (size_t i = 2; i < this->tokens.size(); i++) {
        if (this->tokens.at(i).isAtStartOfLine()) {
            unsigned col_num = this->source_manager->getSpellingColumnNumber(
                this->tokens.at(i - 2).getEndLoc());
            this->dout() << "Checking line with end column " << col_num << "\n";
            if (col_num > MAX_LINE_LEN) {
                this->diag(this->tokens.at(i - 2).getEndLoc(),
                           "Line length must be less than %0 characters")
                    << MAX_LINE_LEN;
            }
        }
    }
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

//===--- Rule3eCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3eCheck.h"
#include "clang/Basic/TokenKinds.h"
#include <iomanip>
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {

Rule3eCheck::Rule3eCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule3eCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
}

void Rule3eCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();
}

void Rule3eCheck::onEndOfTranslationUnit() {
    std::vector<std::tuple<Token, std::string>> whitespaces;

    std::string ws_str;
    for (size_t i = 0; i < this->tokens.size(); i++) {
        ws_str += *this->tok_string(*this->source_manager, this->tokens.at(i));
    }

    size_t str_loc = 0;
    SourceLocation loc = this->tokens.at(0).getLocation();
    while ((str_loc = ws_str.find(" \n", str_loc)) != std::string::npos) {
        diag(loc.getLocWithOffset(str_loc), "Trailing whitespace.");
        str_loc += 2;
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

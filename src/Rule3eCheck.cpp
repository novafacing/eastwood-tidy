
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
    std::vector<std::tuple<SourceLocation, std::string>> whitespaces;

    for (size_t i = 0; i < this->tokens.size(); i++) {
        Token tok = this->tokens.at(i);
        std::string tok_str = *this->tok_string(*this->source_manager, tok);

        if (tok.isAtStartOfLine()) {
            whitespaces.push_back(std::make_tuple(tok.getLocation(), ""));
        }

        if (std::regex_match(*this->tok_string(*this->source_manager, tok),
                             std::regex("[ \\t\\n\\r]+")) ||
            // If a comment is multiline, we need to see if it contains any trailing
            // whitespace
            (tok.is(tok::comment) &&
             this->source_manager->getSpellingLineNumber(tok.getLocation()) <
                 this->source_manager->getSpellingLineNumber(tok.getEndLoc()))

        ) {
            std::get<1>(whitespaces.back()) += tok_str;
        }

        // Debug print all whitespaces
        for (auto &ws : whitespaces) {
            SourceLocation loc = std::get<0>(ws);
            std::string str = std::get<1>(ws);
            this->dout() << this->source_manager->getSpellingLineNumber(loc) << ": "
                         << str << "\n";
        }

        for (auto &ws : whitespaces) {
            SourceLocation loc = std::get<0>(ws);
            std::string ws_str = std::get<1>(ws);
            size_t str_loc = 0;

            std::stringstream ss;
            ss << std::hex << std::setw(2) << std::setfill('0');
            for (size_t i = 0; i < ws_str.size(); i++) {
                ss << (unsigned int)ws_str[i] << " ";
            }

            this->dout() << this->source_manager->getSpellingLineNumber(loc) << ": "
                         << ss.str() << "\n";

            while (ws_str.find(" \n", str_loc) != std::string::npos) {
                this->dout() << "Got a space/nl at " << str_loc;
                diag(loc.getLocWithOffset(str_loc), "Trailing whitespace.");
                str_loc = ws_str.find(" \n", str_loc) + 1;
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

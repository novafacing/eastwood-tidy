
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

    for (size_t i = 0; i < this->tokens.size(); i++) {
        Token tok = this->tokens.at(i);

        if (tok.isAtStartOfLine()) {
            whitespaces.push_back(std::make_tuple(tok, ""));
        }

        std::smatch match;
        for (ssize_t j = i - 1; j > 0; j--) {
            Token itok = this->tokens.at(j);
            std::string tok_str = *this->tok_string(*this->source_manager, itok);
            std::get<0>(whitespaces.back()) = itok;
            if (std::regex_match(tok_str, std::regex("[ \\t\\n\\r]+")) ||
                tok.is(tok::comment)) {
                std::get<1>(whitespaces.back()).insert(0, tok_str);
            } else if (std::regex_match(tok_str, match,
                                        std::regex("[^ \\t\\n\\r]+([ \\t\\n\\r]+)"))) {
                std::get<1>(whitespaces.back()).insert(0, match[0]);
                break;
            } else {
                break;
            }
        }
    }

    // Debug print all whitespaces

    for (auto &ws : whitespaces) {
        SourceLocation loc = std::get<0>(ws).getLocation();
        std::string ws_str = std::get<1>(ws);

        this->dout() << this->source_manager->getSpellingLineNumber(loc) << ": "
                     << ws_str << "\n";

        size_t str_loc = 0;

        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0');

        for (size_t i = 0; i < ws_str.size(); i++) {
            ss << (unsigned int)ws_str.at(i) << " ";
        }

        this->dout() << this->source_manager->getSpellingLineNumber(loc) << ": "
                     << ss.str() << "\n";

        while ((str_loc = ws_str.find(" \n", str_loc)) != std::string::npos) {
            this->dout() << "Got a space/nl at " << str_loc;
            diag(loc.getLocWithOffset(str_loc), "Trailing whitespace.");
            str_loc += 2;
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

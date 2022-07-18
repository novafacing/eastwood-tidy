
//===--- Rule3cCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3cCheck.h"
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule3cCheck::Rule3cCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}
void Rule3cCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(functionDecl().bind("function_decl"), this);
}
void Rule3cCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();
}

void Rule3cCheck::onEndOfTranslationUnit() {

    for (size_t i = 0; i < this->tokens.size(); i++) {
        Token tok = this->tokens.at(i);

        if (tok.isOneOf(tok::semi, tok::comma) && i + 1 < this->tokens.size()) {
            size_t j = i + 1;
            std::vector<Token> ntokens;
            std::string fullstr;

            for (; j < this->tokens.size(); j++) {
                std::string ntok_str =
                    *this->tok_string(*this->source_manager, this->tokens.at(j));

                if (!std::regex_match(ntok_str, std::regex("[ \\t\\n\\r]+")) &&
                    !this->tokens.at(j).is(tok::comment)) {
                    break;
                }

                ntokens.push_back(this->tokens.at(j));
                fullstr += ntok_str;
            }

            if (j >= this->tokens.size() || this->tokens.at(j).isAtStartOfLine() ||
                fullstr == " " || fullstr == "\n") {
                // It's ok if the semicolon was at the end of the line and some stuff is
                // after, or if the spacing is correct
                continue;
            }

            auto errmsg = diag(tok.getEndLoc(), "Single space required after '%0'.")
                          << *this->tok_string(*this->source_manager, tok);
            errmsg << FixItHint::CreateReplacement(
                SourceRange(tok.getEndLoc(), ntokens.empty()
                                                 ? tok.getEndLoc()
                                                 : ntokens.back().getEndLoc()),
                " ");
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

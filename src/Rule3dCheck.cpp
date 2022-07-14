
//===--- Rule3dCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3dCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
static size_t spc_ct(std::string s) {
    size_t ct = 0;
    for (auto c : s) {
        if (c == ' ') {
            ct++;
        }
    }
    return ct;
}

void Rule3dCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
}
void Rule3dCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();
}
void Rule3dCheck::onEndOfTranslationUnit() {
    for (size_t i = 0; i < this->tokens.size(); i++) {
        auto tok = this->tokens.at(i);
        std::string ws("");
        if (i > 0) {
            ws = *this->sourcerange_string(
                *this->source_manager,
                SourceRange(this->tokens.at(i - 1).getLocation(), tok.getLocation()));
        }

        std::string raw_tok_data = *this->tok_string(*this->source_manager, tok);
        if (raw_tok_data.rfind("#", 0) == 0 && tok.isAtStartOfLine() &&
            spc_ct(ws) > 0) {
            diag(tok.getLocation(), "Incorrect indentation level for preprocessor "
                                    "directive. Expected %0, got %1.")
                << 0 << spc_ct(ws);
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

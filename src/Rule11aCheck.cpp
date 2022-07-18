//===--- Rule11aCheck.cpp - clang-tidy
//-------------------------------------===//
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

#include "Rule11aCheck.h"
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
Rule11aCheck::Rule11aCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule11aCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(stmt().bind("relex"), this);
    Finder->addMatcher(decl().bind("relex"), this);
    Finder->addMatcher(functionDecl().bind("function"), this);
}

void Rule11aCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();

    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        if (not this->checked) {
            for (size_t i = 0; i < this->tokens.size(); i++) {
                if (this->tokens.at(i).isAtStartOfLine() && i > 0) {
                    std::string ws(*this->tok_string(*this->source_manager,
                                                     this->tokens.at(i - 1)));

                    for (auto c : ws) {
                        if (c != ' ' && c != '\n') {
                            auto errmsg =
                                diag(this->tokens.at(i).getLocation(),
                                     "Indentation must consist of spaces only.");
                            errmsg << FixItHint::CreateReplacement(
                                SourceRange(this->tokens.at(i - 1).getLocation(),
                                            this->tokens.at(i).getEndLoc()),
                                "    ");
                        }
                    }
                }
            }
        }
        this->checked = true;
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang


//===--- Rule3fCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3fCheck.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule3fCheck::Rule3fCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}
void Rule3fCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(stmt().bind("relex"), this);
    Finder->addMatcher(decl().bind("relex"), this);
    Finder->addMatcher(functionDecl().bind("function_decl"), this);
}
void Rule3fCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();
    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
        DeclarationNameInfo NameInfo = MatchedDecl->getNameInfo();
        SourceLocation NameEnd(NameInfo.getEndLoc());

        for (size_t i = 0; i < this->tokens.size(); i++) {
            Token tok = this->tokens.at(i);

            if (tok.getLocation() == NameInfo.getEndLoc()) {
                if (i >= this->tokens.size()) {
                    return;
                }

                tok = this->tokens.at(i + 1);

                std::string token_str = *this->tok_string(*this->source_manager, tok);

                if (token_str != "(") {
                    if (this->source_manager->isWrittenInMainFile(tok.getLocation())) {
                        auto errmsg =
                            diag(tok.getLocation(),
                                 "No space permitted between function name and "
                                 "parameter list.");
                        errmsg << FixItHint::CreateRemoval(SourceRange(
                            this->tokens.at(i).getEndLoc(), tok.getEndLoc()));
                    }
                }

                return;
            }
        }
    }
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

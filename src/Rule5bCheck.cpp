
//===--- Rule5bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule5bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule5bCheck::Rule5bCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule5bCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(functionDecl(isDefinition()).bind("function"), this);
    Finder->addMatcher(decl().bind("decl"), this);
    Finder->addMatcher(ifStmt().bind("if"), this);
    Finder->addMatcher(switchCase().bind("case"), this);
}

size_t count_newlines(std::string s) {
    size_t ct = 0;
    for (auto c : s) {
        if (c == '\n') {
            ct++;
        }
    }
    return ct;
}
void Rule5bCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();

    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        this->alongside_ok_lines.push_back(MatchedDecl->getSourceRange().getEnd());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<Decl>("decl")) {
        this->alongside_ok_lines.push_back(MatchedDecl->getEndLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
        if (auto els = MatchedDecl->getElse()) {
            this->alongside_ok_lines.push_back(els->getBeginLoc());
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchCase>("case")) {
        this->alongside_ok_lines.push_back(MatchedDecl->getEndLoc());
    }
}

void Rule5bCheck::onEndOfTranslationUnit(void) {
    for (auto token : this->tokens) {
        if (token.getKind() == tok::comment) {
            auto it = this->alongside_ok_lines.begin();
            for (; it != this->alongside_ok_lines.end(); it++) {
                if (this->source_manager->getSpellingLineNumber(token.getLocation()) ==
                    this->source_manager->getSpellingLineNumber(*it)) {
                    break;
                }
            }
            if (it == this->alongside_ok_lines.end() && !token.isAtStartOfLine()) {
                this->diag(token.getLocation(),
                           "Comments must appear above code except "
                           "for else, case, or declarations");
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

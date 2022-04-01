
//===--- Rule3aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3aCheck.h"

#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule3aCheck::Rule3aCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule3aCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(whileStmt().bind("while"), this);
    Finder->addMatcher(forStmt().bind("for"), this);
    Finder->addMatcher(ifStmt().bind("if"), this);
    Finder->addMatcher(doStmt().bind("do"), this);
    Finder->addMatcher(switchStmt().bind("switch"), this);
}

void Rule3aCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();
    std::vector<SourceLocation> left;
    std::vector<SourceLocation> right;
    std::string type;

    if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while")) {
        if (not this->source_manager->isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
            return;
        }
        left.push_back(MatchedDecl->getLParenLoc());
        right.push_back(MatchedDecl->getRParenLoc());
        type = "while";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
        if (not this->source_manager->isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
            return;
        }
        left.push_back(MatchedDecl->getLParenLoc());
        right.push_back(MatchedDecl->getRParenLoc());
        type = "for";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
        if (not this->source_manager->isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
            return;
        }
        left.push_back(MatchedDecl->getLParenLoc());
        right.push_back(MatchedDecl->getRParenLoc());
        type = "if";

        if (auto els = MatchedDecl->getElse()) {
            size_t idx = this->token_index(els->getBeginLoc());
            std::vector<Token> else_space_tokens;
            for (size_t i = idx + 1; i < this->tokens.size(); i++) {
                if (this->tokens.at(i).getKind() == tok::r_brace) {
                    if (else_space_tokens.size() != 1 ||
                        *this->tok_string(*this->source_manager,
                                          else_space_tokens.at(0)) != " ") {
                        this->diag(els->getBeginLoc(),
                                   "There must be exactly one space between 'else' and "
                                   "open brace");
                    }
                    break;
                }
            }
        }

    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("do")) {
        if (not this->source_manager->isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
            return;
        }
        left.push_back(MatchedDecl->getLParenLoc());
        right.push_back(MatchedDecl->getRParenLoc());
        type = "do";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
        if (not this->source_manager->isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
            return;
        }
        left.push_back(MatchedDecl->getLParenLoc());
        right.push_back(MatchedDecl->getRParenLoc());
        type = "switch";
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    auto lit = left.begin();
    auto rit = right.begin();

    if (left.size() < 1 || right.size() < 1) {
        return;
    }

    for (auto tt = this->tokens.begin(); tt != this->tokens.end(); tt++) {
        std::string raw_trav_tok_data = Lexer::getSpelling(
            *tt, *this->source_manager, this->ast_context->getLangOpts());

        if (lit <= left.end() && *lit == tt->getLocation()) {
            auto rtok_it = std::make_reverse_iterator(tt);
            // rtok_it++;
            size_t ws = 0;
            bool bad = false;
            std::string total = "";
            while (rtok_it != tokens.rend()) {
                if (rtok_it->getKind() == tok::unknown) {
                    std::string raw_tok_data =
                        Lexer::getSpelling(*rtok_it, *this->source_manager,
                                           this->ast_context->getLangOpts());

                    total += raw_tok_data;

                    rtok_it++;

                } else {
                    break;
                }
            }
            for (auto c : total) {
                if (c == ' ') {
                    ws++;
                } else {
                    bad = true;
                }
            }
            if (bad || ws != 1) {
                diag(*lit, "There must be exactly one space "
                           "between %0 and open parenthesis.")
                    << type;
            }

            lit++;
        }

        if (rit <= right.end() && *rit == tt->getLocation()) {
            auto rtok_it = tt;
            rtok_it++;
            size_t ws = 0;
            bool bad = false;
            std::string total = "";
            while (rtok_it != tokens.end()) {
                if (rtok_it->getKind() == tok::unknown) {
                    std::string raw_tok_data =
                        Lexer::getSpelling(*rtok_it, *this->source_manager,
                                           this->ast_context->getLangOpts());

                    total += raw_tok_data;

                    rtok_it++;

                } else {
                    break;
                }
            }

            for (auto c : total) {
                if (c == ' ') {
                    ws++;
                } else {
                    bad = true;
                }
            }

            if (bad || ws != 1) {
                diag(*rit, "There must be exactly one space between "
                           "parenthesis and open brace.")
                    << type;
            }

            rit++;
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang
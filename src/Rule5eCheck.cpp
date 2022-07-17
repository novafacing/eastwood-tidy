
//===--- Rule5eCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule5eCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule5eCheck::Rule5eCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}
void Rule5eCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(stmt().bind("relex"), this);
    Finder->addMatcher(decl().bind("relex"), this);
    Finder->addMatcher(functionDecl().bind("function_decl"), this);
}
void Rule5eCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();

    const SourceManager &SM = *Result.SourceManager;

    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
        if (not MatchedDecl->isDefined()) {
            return;
        }
        const FunctionDecl *FunctionDefinition = MatchedDecl->getDefinition();
        std::string fname = FunctionDefinition->getNameInfo().getName().getAsString();
        SourceRange FunctionDefinitionRange = FunctionDefinition->getSourceRange();
        unsigned int func_start_line = 0;
        SourceLocation func_start_loc;
        if (SM.isWrittenInMainFile(
                FunctionDefinition->getReturnTypeSourceRange().getBegin())) {
            func_start_loc = FunctionDefinition->getReturnTypeSourceRange().getBegin();
            func_start_line = SM.getSpellingLineNumber(func_start_loc);
        } else {
            // if the return type is not in the main file, then we can't
            // use it to check preceeding lines, grab the function name
            // location instead
            func_start_loc = FunctionDefinition->getNameInfo().getLoc();
            func_start_line = SM.getSpellingLineNumber(func_start_loc);
        }
        if (MatchedDecl->doesThisDeclarationHaveABody() && FunctionDefinition) {
            std::vector<Token> toks = this->tokens;
            std::vector<Token> tokens;
            for (auto t : toks) {
                // Make sure the lines we grab backward from the decl are on
                // lines above and come before in the TU - this fixes #73
                if (SM.getSpellingLineNumber(t.getLocation()) < func_start_line) {
                    tokens.push_back(t);
                } else {
                    break;
                }
            }

            if (tokens.size() >= 3) {
                std::vector<Token>::reverse_iterator rit = tokens.rbegin();

                size_t comment_ct = 0;
                for (size_t i = 0; rit != tokens.rend() && i < 3; rit++, i++) {
                    this->dout(fname + " preheader check tok")
                        << *this->tok_string(SM, *rit) << "\n";
                    if (rit->getKind() == tok::comment) {
                        comment_ct++;
                    }
                }
                std::string before_tok_str =
                    *this->tok_string(SM, tokens.at(tokens.size() - 3));
                if (std::count(before_tok_str.begin(), before_tok_str.end(), '\n') <
                    2) {
                    diag(tokens.at(tokens.size() - 2).getLocation(),
                         "At least one empty line required before function "
                         "header "
                         "comment for function " +
                             fname + ".");
                }

                if (*this->tok_string(SM, tokens.back()) != "\n\n") {
                    diag(tokens.back().getEndLoc(),
                         "Exactly one empty line required after function "
                         "header "
                         "comment for function " +
                             fname + ".");
                }

                if (!isWhitespace(*SM.getCharacterData(
                        tokens.at(tokens.size() - 3).getLocation()))) {
                    diag(tokens.at(tokens.size() - 2).getLocation(),
                         "At least one empty line required before function "
                         "header "
                         "comment for function " +
                             fname + ".");
                }

                if (!isWhitespace(*SM.getCharacterData(tokens.back().getLocation()))) {
                    diag(tokens.back().getEndLoc(),
                         "Exactly one empty line required after function "
                         "header "
                         "comment for function " +
                             fname + ".");
                }
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

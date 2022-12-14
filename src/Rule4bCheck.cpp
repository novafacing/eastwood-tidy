
//===--- Rule4bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule4bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule4bCheck::Rule4bCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule4bCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(functionDecl().bind("function_decl"), this);
}
void Rule4bCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    const SourceManager &SM = *Result.SourceManager;
    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
        if (MatchedDecl->param_size() < 2) {
            return;
        }
        unsigned FirstCol =
            SM.getSpellingColumnNumber(MatchedDecl->getParamDecl(0)->getBeginLoc());
        unsigned FirstLineNum =
            SM.getSpellingLineNumber(MatchedDecl->getParamDecl(0)->getBeginLoc());
        unsigned CurrentLine = FirstLineNum;
        for (auto Param : MatchedDecl->parameters()) {
            if (SM.getSpellingLineNumber(Param->getBeginLoc()) > CurrentLine) {
                CurrentLine = SM.getSpellingLineNumber(Param->getBeginLoc());
                if (SM.getSpellingColumnNumber(Param->getBeginLoc()) != FirstCol &&
                    SM.isWrittenInMainFile(Param->getBeginLoc())) {
                    size_t colnum = SM.getSpellingColumnNumber(Param->getBeginLoc());
                    auto errmsg =
                        diag(
                            Param->getBeginLoc(),
                            "Line-broken parameter is not aligned with first "
                            "parameter. Parameters should be aligned in column %0 (got "
                            "%1).")
                        << FirstCol << colnum;
                    if (FirstCol < colnum) {
                        errmsg << FixItHint::CreateRemoval(SourceRange(
                            Param->getBeginLoc().getLocWithOffset(FirstCol - colnum),
                            Param->getBeginLoc()));

                    } else {
                        errmsg << FixItHint::CreateInsertion(
                            Param->getBeginLoc(), std::string(FirstCol - colnum, ' '));
                    }
                }
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

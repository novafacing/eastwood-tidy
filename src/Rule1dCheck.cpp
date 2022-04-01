//===--- Rule1DCheck.cpp - clang-tidy ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// global variables must start with g_

#include "Rule1dCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule1dCheck::Rule1dCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule1dCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(varDecl(hasGlobalStorage()).bind("variable"), this);
}

void Rule1dCheck::check(const MatchFinder::MatchResult &Result) {
    const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable");
    if (!(Result.SourceManager)->isWrittenInMainFile(MatchedDecl->getLocation())) {
        return;
    }

    if (MatchedDecl->getName().startswith("g_")) {
        return;
    } else {
        diag(MatchedDecl->getLocation(),
             "Global variable %0 doesn't conform to global naming scheme.")
            << MatchedDecl
            << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "g_");
    }
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

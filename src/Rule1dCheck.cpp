//===--- Rule1DCheck.cpp - clang-tidy ------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Rule1dCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {

void Rule1DCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl(hasGlobalStorage()).bind("variable"), this);
}

void Rule1DCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable");

  if (MatchedDecl->getName().startswith("g_")) {
    return;
  } else {

  diag(MatchedDecl->getLocation(), "[Rule I.D] Global variable %0 doesn't conform to global naming scheme")
      << MatchedDecl
      << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "g_");
  }
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

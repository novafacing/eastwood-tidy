//===--- Rule1aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule1aCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {

void Rule1aCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(varDecl().bind("variable"), this);
}

void Rule1aCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable");
  if (!MatchedDecl || !MatchedDecl->getLocation().isValid()) {
    return;
  }

  std::regex local_regex{R"([a-z][a-z0-9_]*)"};
  std::smatch results;
  std::string var_name = MatchedDecl->getName().str();
  if (std::regex_match(var_name, results, local_regex)) {
    return;
  } else {
    // Error
    diag(MatchedDecl->getLocation(), "[Rule 1.A] Variable %0 is not all lowercase"
                                     " and separated by underscores")
        << MatchedDecl;
  } 
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

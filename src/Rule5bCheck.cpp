
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
      visited(false), checked(false),
      debug_enabled(Options.get("debug", "false")) {
  if (this->debug_enabled == "true") {
    this->debug = true;
  }
}

void Rule5bCheck::registerMatchers(MatchFinder *Finder) {
  Finder->addMatcher(stmt().bind("relex"), this);
  Finder->addMatcher(decl().bind("relex"), this);
  Finder->addMatcher(functionDecl().bind("function_decl"), this);
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
  RELEX();
  const SourceManager &SM = *Result.SourceManager;
  ASTContext *Context = Result.Context;

  if (auto MatchedDecl =
          Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
    std::vector<Token> tokens;
  }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

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
    Finder->addMatcher(functionDecl().bind("function"), this);
    Finder->addMatcher(varDecl(hasGlobalStorage()).bind("variable"), this);
}

void Rule1dCheck::check(const MatchFinder::MatchResult &Result) {
    if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable")) {
        if (!(Result.SourceManager)->isWrittenInMainFile(MatchedDecl->getLocation())) {
            return;
        }

        if (!MatchedDecl->getName().startswith("g_")) {
            diag(MatchedDecl->getLocation(),
                 "Global variable %0 doesn't conform to global naming scheme.")
                << MatchedDecl
                << FixItHint::CreateInsertion(MatchedDecl->getLocation(), "g_");
        }

        if (this->first_function_line != -1 &&
            (Result.SourceManager)->getSpellingLineNumber(MatchedDecl->getLocation()) >
                this->first_function_line) {
            diag(MatchedDecl->getLocation(), "Global variable %0 must be declared at "
                                             "the top of the file before any function.")
                << MatchedDecl->getName()
                << FixItHint::CreateRemoval(MatchedDecl->getSourceRange());
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        if (!(Result.SourceManager)->isWrittenInMainFile(MatchedDecl->getLocation())) {
            return;
        }

        if (this->first_function_line == -1) {
            this->dout() << "Setting first_function_line to "
                         << (Result.SourceManager)
                                ->getSpellingLineNumber(MatchedDecl->getLocation())
                         << "\n";
            this->first_function_line =
                (Result.SourceManager)
                    ->getSpellingLineNumber(MatchedDecl->getBeginLoc());
        }
    }
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

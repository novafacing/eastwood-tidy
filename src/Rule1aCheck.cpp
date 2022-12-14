//===--- Rule1aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Rule Description: Variable names must be in all lowercase, separated by
// underscores

#include "Rule1aCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
Rule1aCheck::Rule1aCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule1aCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(varDecl().bind("variable"), this);
    Finder->addMatcher(functionDecl().bind("function"), this);
    Finder->addMatcher(enumDecl().bind("enum"), this);
    Finder->addMatcher(recordDecl(isUnion()).bind("union"), this);
    Finder->addMatcher(recordDecl(isStruct()).bind("struct"), this);
    Finder->addMatcher(fieldDecl().bind("field"), this);
    Finder->addMatcher(typedefDecl().bind("typedef"), this);
}

void Rule1aCheck::checkName(SourceLocation loc, std::string name, std::string type) {
    if (!loc.isValid()) {
        return;
    }

    std::regex localRegex{R"([a-z][a-z0-9_]*)"};
    std::smatch results;
    std::string newname(name.begin(), name.end());
    std::transform(newname.begin(), newname.end(), newname.begin(), ::tolower);
    std::replace_if(
        newname.begin(), newname.end(),
        [](char c) { return !((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A')); },
        '_');

    if (std::regex_match(name, results, localRegex)) {
        return;
    } else if (name != "") {
        // Error
        auto errmsg = diag(loc, "%0 %1 is not all lowercase"
                                " and separated by underscores")
                      << type << name;
        errmsg << FixItHint::CreateReplacement(
            SourceRange(loc, loc.getLocWithOffset(name.length())), newname);
    }
}

void Rule1aCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    SourceLocation loc;
    std::string name = "";
    std::string type = "";

    if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable")) {
        loc = MatchedDecl->getLocation();
        name = MatchedDecl->getNameAsString();
        type = "Variable";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        loc = MatchedDecl->getLocation();
        name = MatchedDecl->getNameAsString();
        type = "Function";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
        loc = MatchedDecl->getLocation();
        name = MatchedDecl->getNameAsString();
        type = "Enum";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("struct")) {
        if (MatchedDecl->isAnonymousStructOrUnion()) {
            return;
        }

        loc = MatchedDecl->getLocation();
        name = MatchedDecl->getNameAsString();
        type = "Struct";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("union")) {
        if (MatchedDecl->isAnonymousStructOrUnion()) {
            return;
        }

        loc = MatchedDecl->getLocation();
        name = MatchedDecl->getNameAsString();
        type = "Union";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FieldDecl>("field")) {
        if (MatchedDecl->isAnonymousStructOrUnion()) {
            return;
        }

        loc = MatchedDecl->getLocation();
        name = MatchedDecl->getNameAsString();
        type = "Field";
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
        loc = MatchedDecl->getLocation();
        name = MatchedDecl->getNameAsString();
        type = "Enum";
    } else {
        return;
    }

    if ((Result.SourceManager)->isWrittenInMainFile(loc)) {
        checkName(loc, name, type);
    }
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

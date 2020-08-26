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
                Finder->addMatcher(functionDecl().bind("function"), this);
                Finder->addMatcher(enumDecl().bind("enum"), this);
                Finder->addMatcher(recordDecl(isUnion()).bind("union"), this);
                Finder->addMatcher(recordDecl(isStruct()).bind("struct"), this);
                Finder->addMatcher(fieldDecl().bind("field"), this);
                Finder->addMatcher(typedefDecl().bind("typedef"), this);
            }

            void Rule1aCheck::check_name(SourceLocation loc, std::string name, std::string type) {
                if (!loc.isValid()) {
                    return;
                }

                std::regex local_regex{R"([a-z][a-z0-9_]*)"};
                std::smatch results;

                if (std::regex_match(name, results, local_regex)) {
                    return;
                } else {
                    // Error
                    diag(loc, "[Rule 1.A] %0 %1 is not all lowercase"
                            " and separated by underscores")
                        << type << name;
                }
            }

            void Rule1aCheck::check(const MatchFinder::MatchResult &Result) {

                SourceLocation loc;
                std::string name = "";
                std::string type = "";

                if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable")) {
                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "Variable";
                    check_name(loc, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "Function";
                    check_name(loc, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "Enum";
                    check_name(loc, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("struct")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "Struct";
                    check_name(loc, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("union")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "Union";
                    check_name(loc, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FieldDecl>("field")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "Field";
                    check_name(loc, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "Enum";
                    check_name(loc, name, type);
                } else {
                    return;
                }
            }

        } // namespace eastwood
    } // namespace tidy
} // namespace clang

//===--- Rule1bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule1bCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <regex>
#include <map>
#include <utility>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {

            Rule1bCheck::Rule1bCheck(StringRef Name, ClangTidyContext * Context) :
                ClangTidyCheck(Name, Context), Dump(Options.get("Dump", "false")) {
                if (this->Dump == "true") {
                    for (auto ty : {"variable", "function", "enum", "union", "struct", "field", "typedef"}) {
                        std::vector<SourceLocation> v{};
                        this->Declarations.insert(std::make_pair(ty, v));
                    }
                }
            }

            void Rule1bCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(varDecl().bind("variable"), this);
                Finder->addMatcher(functionDecl().bind("function"), this);
                Finder->addMatcher(enumDecl().bind("enum"), this);
                Finder->addMatcher(recordDecl(isUnion()).bind("union"), this);
                Finder->addMatcher(recordDecl(isStruct()).bind("struct"), this);
                Finder->addMatcher(fieldDecl().bind("field"), this);
                Finder->addMatcher(typedefDecl().bind("typedef"), this);
            }

            void Rule1bCheck::saveVar(SourceLocation loc, std::string type) {
                this->Declarations[type].push_back(loc);
            }

            void Rule1bCheck::check(const MatchFinder::MatchResult &Result) {
                SourceLocation loc;
                std::string type = "";
                if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable")) {
                    loc = MatchedDecl->getLocation();
                    type = "variable";
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
                    loc = MatchedDecl->getLocation();
                    type = "function";
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
                    loc = MatchedDecl->getLocation();
                    type = "enum";
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("struct")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    type = "struct";
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("union")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    type = "union";
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FieldDecl>("field")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    type = "field";
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
                    loc = MatchedDecl->getLocation();
                    type = "enum";
                } else {
                    return;
                }

                if ((Result.SourceManager)->isWrittenInMainFile(loc)) {
                    this->saveVar(loc, type);
                } else {
                    // Just for debugging purposes I'm leaving this here
                    /*
                    if (loc.isValid()) {
                        diag(loc, "'%0' is not being saved") << type;
                    }
                    */
                }
            }

            void Rule1bCheck::onEndOfTranslationUnit() {
                ClangTidyCheck::onEndOfTranslationUnit();
                if (this->Dump == "true") {
                    std::ios init(NULL);
                    init.copyfmt(std::cout);
                    for (auto ty : {"variable", "function", "enum", "union", "struct", "field", "typedef"}) {
                        for (auto dec : this->Declarations.at(ty)) {
                            if (dec.isValid()) {
                                diag(dec, "'%0' declaration.", DiagnosticIDs::Note) << ty;
                            }
                        }
                    }
                    std::cout.copyfmt(init);
                }
            }

            void Rule1bCheck::storeOptions(ClangTidyOptions::OptionMap & Opts) {
                Options.store(Opts, "Dump", this->Dump);
            } 

        } // namespace eastwood
    } // namespace tidy
} // namespace clang 

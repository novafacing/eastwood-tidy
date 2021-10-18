//===--- Rule1bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Variable names should be descriptive and meaningful. Note: This only dumps variable
// names, it does not error

#include "Rule1bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {

            Rule1bCheck::Rule1bCheck(StringRef Name, ClangTidyContext *Context)
                : ClangTidyCheck(Name, Context), dump(Options.get("dump", "false")) {
                if (this->dump == "true") {
                    for (auto type : {"variable", "function", "enum", "union", "struct",
                                      "field", "typedef"}) {
                        std::vector<std::string> sources{};
                        this->declarations.insert(std::make_pair(type, sources));
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

            void Rule1bCheck::saveVar(std::string name, std::string type) {
                this->declarations[type].push_back(name);
            }

            void Rule1bCheck::check(const MatchFinder::MatchResult &Result) {
                SourceLocation loc;
                std::string type = "";
                std::string name = "";

                if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable")) {
                    if (!MatchedDecl->isLocalVarDecl()) {
                        // This is a parameter, we need to check if we actually have a
                        // name
                        if (!MatchedDecl->getIdentifier()) {
                            // There is no identifier for this variable, don't report
                            // it.
                            return;
                        }
                    }
                    loc = MatchedDecl->getLocation();
                    type = "variable";
                    if (!MatchedDecl->getIdentifier()) {
                        return;
                    }
                    name = MatchedDecl->getIdentifier()->getName().str();
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<FunctionDecl>("function")) {
                    loc = MatchedDecl->getLocation();
                    type = "function";
                    if (!MatchedDecl->getIdentifier()) {
                        return;
                    }
                    name = MatchedDecl->getIdentifier()->getName().str();
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<EnumDecl>("enum")) {
                    loc = MatchedDecl->getLocation();
                    type = "enum";
                    if (!MatchedDecl->getIdentifier()) {
                        return;
                    }
                    name = MatchedDecl->getIdentifier()->getName().str();
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<RecordDecl>("struct")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    type = "struct";
                    if (!MatchedDecl->getIdentifier()) {
                        return;
                    }
                    name = MatchedDecl->getIdentifier()->getName().str();
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<RecordDecl>("union")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    type = "union";
                    if (!MatchedDecl->getIdentifier()) {
                        return;
                    }
                    name = MatchedDecl->getIdentifier()->getName().str();
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<FieldDecl>("field")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    type = "field";
                    if (!MatchedDecl->getIdentifier()) {
                        return;
                    }
                    name = MatchedDecl->getIdentifier()->getName().str();
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
                    loc = MatchedDecl->getLocation();
                    type = "enum";
                    if (!MatchedDecl->getIdentifier()) {
                        return;
                    }
                    name = MatchedDecl->getIdentifier()->getName().str();
                } else {
                    return;
                }

                if ((Result.SourceManager)->isWrittenInMainFile(loc)) {
                    if (name != "") {
                        this->saveVar(name, type);
                    }
                } else {
                }
            }

            void Rule1bCheck::onEndOfTranslationUnit() {
                if (this->dump == "true") {
                    std::ios init(NULL);
                    init.copyfmt(std::cout);
                    size_t max_decl_len = 0;
                    for (auto type : {"variable", "function", "enum", "union", "struct",
                                      "field", "typedef"}) {
                        for (auto declaration : this->declarations.at(type)) {
                            if (declaration.length() > max_decl_len) {
                                max_decl_len = declaration.length();
                            }
                        }
                    }
                    std::string header(" TYPE    | NAME    ");
                    std::cout << std::string(
                                     std::max(max_decl_len + 10, header.length()), '=')
                              << std::endl;
                    std::cout << "  TYPE   |   VARIABLE NAME" << std::endl;
                    std::cout << std::string(
                                     std::max(max_decl_len + 10, header.length()), '=')
                              << std::endl;
                    for (auto type : {"variable", "function", "enum", "union", "struct",
                                      "field", "typedef"}) {
                        for (auto declaration : this->declarations.at(type)) {
                            std::cout << type << " | " << declaration << std::endl;
                        }
                    }
                    std::cout << std::string(
                                     std::max(max_decl_len + 10, header.length()), '=')
                              << std::endl;
                    std::cout << std::endl;
                    std::cout.copyfmt(init);
                }
            }

            void Rule1bCheck::storeOptions(ClangTidyOptions::OptionMap &Opts) {
                Options.store(Opts, "dump", this->dump);
            }

        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

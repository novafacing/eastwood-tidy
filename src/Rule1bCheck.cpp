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
#include <string>
#include <iostream>
#include <iomanip>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {

            Rule1bCheck::Rule1bCheck(StringRef Name, ClangTidyContext * Context) :
                ClangTidyCheck(Name, Context), Dump(Options.get("Dump", "false")) {
                std::cout << "Options value is equal to: " << Options.get("Dump", "false") << std::endl;
                if (this->Dump == "true") {
                    for (auto ty : {"variable", "function", "enum", "union", "struct", "field", "typedef"}) {
                        std::vector<std::pair<unsigned, std::string>> v{};
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

            void Rule1bCheck::saveVar(unsigned lineNum, std::string name, std::string type) {
                this->Declarations[type].push_back(std::pair<unsigned, std::string>(lineNum, name));
            }

            void Rule1bCheck::check(const MatchFinder::MatchResult &Result) {
                SourceLocation loc;
                std::string name = "";
                std::string type = "";
                if (auto MatchedDecl = Result.Nodes.getNodeAs<VarDecl>("variable")) {
                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "variable";
                    unsigned lineNum = Result.SourceManager->getSpellingLineNumber(loc);
                    this->saveVar(lineNum, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "function";
                    unsigned lineNum = Result.SourceManager->getSpellingLineNumber(loc);
                    this->saveVar(lineNum, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "enum";
                    unsigned lineNum = Result.SourceManager->getSpellingLineNumber(loc);
                    this->saveVar(lineNum, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("struct")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "struct";
                    unsigned lineNum = Result.SourceManager->getSpellingLineNumber(loc);
                    this->saveVar(lineNum, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("union")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "union";
                    unsigned lineNum = Result.SourceManager->getSpellingLineNumber(loc);
                    this->saveVar(lineNum, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FieldDecl>("field")) {
                    if (MatchedDecl->isAnonymousStructOrUnion()) {
                        return;
                    }

                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "field";
                    unsigned lineNum = Result.SourceManager->getSpellingLineNumber(loc);
                    this->saveVar(lineNum, name, type);
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<TypedefDecl>("typedef")) {
                    loc = MatchedDecl->getLocation();
                    name = MatchedDecl->getName().str();
                    type = "enum";
                    unsigned lineNum = Result.SourceManager->getSpellingLineNumber(loc);
                    this->saveVar(lineNum, name, type);
                } else {
                    return;
                }
            }

            void Rule1bCheck::onEndOfTranslationUnit() {
                ClangTidyCheck::onEndOfTranslationUnit();
                if (this->Dump == "true") {
                    std::ios init(NULL);
                    init.copyfmt(std::cout);
                    std::cout << "[Rule I.B] Dumping variables from translation unit:" << std::endl;
                    for (auto ty : {"variable", "function", "enum", "union", "struct", "field", "typedef"}) {
                        for (auto dec : this->Declarations.at(ty)) {
                            /* Format:     enum:00000013 degrees */
                            if (dec.first != 0) {
                                /* Included declarations will have a value of 0 */
                                std::cout << std::setw(8) << ty << ":" << std::setfill('0')
                                    << std::setw(8) << dec.first << " "<< std::setfill(' ') << dec.second << std::endl;
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

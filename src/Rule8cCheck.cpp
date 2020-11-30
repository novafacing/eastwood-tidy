
//===--- Rule8cCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8cCheck.h"
#include <iostream>
#include <algorithm>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            class Rule8cPPCallBack : public PPCallbacks {
                private:
                    Rule8cCheck * Check;
                    Preprocessor * PP;
                    const SourceManager & SM;
                    bool checked;
                public:

                    Rule8cPPCallBack(Rule8cCheck * Check, Preprocessor * PP, const SourceManager & SM) : Check(Check), 
                        PP(PP), SM(SM), checked(false) {};

                    void Ifndef (SourceLocation Loc, const Token &MacroNameTok, const MacroDefinition &MD) override {
                        std::string macro_name(this->SM.getCharacterData(MacroNameTok.getLocation()),
                                this->SM.getCharacterData(MacroNameTok.getEndLoc()));
                        std::string basename(macro_name);
                        if (basename.find_last_of("/") != std::string::npos) {
                            basename = basename.substr(basename.find_last_of("/") + 1);
                        }
                        std::transform(basename.begin(), basename.end(),basename.begin(), ::toupper);
                        std::replace(basename.begin(), basename.end(), '.', '_');
                        std::replace(basename.begin(), basename.end(), '-', '_');
                        this->Check->found_ifndef_checks.push_back(basename);
                    }

                    void MacroDefined (const Token &MacroNameTok, const MacroDirective *MD) override {
                        std::string macro_name(this->SM.getCharacterData(MacroNameTok.getLocation()),
                                this->SM.getCharacterData(MacroNameTok.getEndLoc()));
                        std::string basename(macro_name);
                        if (basename.find_last_of("/") != std::string::npos) {
                            basename = basename.substr(basename.find_last_of("/") + 1);
                        }
                        std::transform(basename.begin(), basename.end(),basename.begin(), ::toupper);
                        std::replace(basename.begin(), basename.end(), '.', '_');
                        std::replace(basename.begin(), basename.end(), '-', '_');
                        this->Check->found_defined_macro_guards.push_back(basename);
                    }

                    void InclusionDirective(SourceLocation HashLoc, const Token & IncludeTok,
                            StringRef FileName, bool isAngled, CharSourceRange FilenameRange, const FileEntry * File,
                            StringRef SearchPath, StringRef RelativePath, const Module * Imported,
                            SrcMgr::CharacteristicKind FileType) override {
                        if (this->SM.isWrittenInMainFile(HashLoc)) {
                            if (not isAngled) {
                                std::string basename(FileName.str());
                                if (basename.find_last_of("/") != std::string::npos) {
                                    basename = basename.substr(basename.find_last_of("/") + 1);
                                }
                                std::transform(basename.begin(), basename.end(),basename.begin(), ::toupper);
                                std::replace(basename.begin(), basename.end(), '.', '_');
                                std::replace(basename.begin(), basename.end(), '-', '_');
                                std::cout << "Found include. Guard is: " << basename << std::endl;
                                this->Check->required_guards.push_back(std::make_pair(basename, HashLoc));
                            }
                        }
                    }
            };

            void Rule8cCheck::registerPPCallbacks(const SourceManager & SM, Preprocessor * PP,
                    Preprocessor * ModuleExpanderPP) {
                PP->addPPCallbacks(std::make_unique<Rule8cPPCallBack>(this, PP, SM));
            }

            void Rule8cCheck::registerMatchers(MatchFinder * Finder) {
                Finder->addMatcher(functionDecl().bind("function"), this);
            }

            void Rule8cCheck::check(const MatchFinder::MatchResult & Result) {
                return;
            }

            void Rule8cCheck::onEndOfTranslationUnit() {
                std::cout << "Have " << this->required_guards.size() << " required guards" << std::endl;
                for (auto req : this->required_guards) {
                    std::cout << "Required guard: " << req.first << std::endl;
                    if (std::find(this->found_ifndef_checks.begin(), this->found_ifndef_checks.end(), req.first)
                            == this->found_ifndef_checks.end()) {
                        diag(req.second, "Expected ifndef define guard check in included header file");
                    }
                    if (std::find(this->found_defined_macro_guards.begin(), this->found_defined_macro_guards.end(), req.first)
                            == this->found_defined_macro_guards.end()) {
                        diag(req.second, "Expected define guard in included header file");
                    }
                }
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang


//===--- Rule8bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            class Rule8bPPCallBack : public PPCallbacks {
            private:
                Rule8bCheck *Check;
                Preprocessor *PP;
                const SourceManager &SM;
                bool checked;

            public:
                Rule8bPPCallBack(Rule8bCheck *Check, Preprocessor *PP, const SourceManager &SM) : Check(Check),
                                                                                                  PP(PP), SM(SM), checked(false){};
                void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                                        StringRef FileName, bool isAngled, CharSourceRange FilenameRange, const FileEntry *File,
                                        StringRef SearchPath, StringRef RelativePath, const Module *Imported,
                                        SrcMgr::CharacteristicKind FileType) override {

                    if (not isAngled) {
                        if (File and File->isValid()) {
                            std::string file_path(File->tryGetRealPathName().str());
                            std::string ext(file_path.substr(file_path.find_last_of(".") + 1));
                            if (ext != "h") {
                                this->Check->diag(HashLoc, "All non-global header files must end in '.h'.");
                            }
                        }
                    }
                }
            };
            void Rule8bCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                                  Preprocessor *ModuleExpanderPP) {
                PP->addPPCallbacks(std::make_unique<Rule8bPPCallBack>(this, PP, SM));
            }
            void Rule8bCheck::registerMatchers(MatchFinder *Finder) {
            }
            void Rule8bCheck::check(const MatchFinder::MatchResult &Result) {
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang


//===--- Rule8aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8aCheck.h"
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            std::string getFileName(const std::string &s) {

                char sep = '/';

                size_t i = s.rfind(sep, s.length());
                if (i != std::string::npos) {
                    return (s.substr(i + 1, s.length() - i));
                }

                return ("");
            }

            class Rule8aPPCallBack : public PPCallbacks {
            private:
                Rule8aCheck *Check;
                Preprocessor *PP;
                const SourceManager &SM;
                bool checked;

            public:
                Rule8aPPCallBack(Rule8aCheck *Check, Preprocessor *PP,
                                 const SourceManager &SM)
                    : Check(Check), PP(PP), SM(SM), checked(false){};
                void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                                        StringRef FileName, bool isAngled,
                                        CharSourceRange FilenameRange,
                                        const FileEntry *File, StringRef SearchPath,
                                        StringRef RelativePath, const Module *Imported,
                                        SrcMgr::CharacteristicKind FileType) override {

                    if (!this->checked) {
                        std::string main_file_path(
                            this->SM.getFileEntryForID(this->SM.getFileID(HashLoc))
                                ->tryGetRealPathName()
                                .str());
                        if (File and File->isValid()) {
                            std::string file_path(File->tryGetRealPathName().str());
                            /* std::filesystem won't link for SOME reason */
                            std::string header_file_name(getFileName(file_path));
                            header_file_name = header_file_name.substr(
                                0, header_file_name.find_last_of("."));
                            std::string main_file_name(getFileName(main_file_path));
                            main_file_name = main_file_name.substr(
                                0, main_file_name.find_last_of("."));

                            if (main_file_name != header_file_name) {
                                this->Check->diag(
                                    HashLoc,
                                    "Corresponding header file include must be first "
                                    "and have same basename as source file.");
                            }
                            std::string ppath("../");
                            std::string cdirpath("./");
                            auto pp_mm_res(std::mismatch(ppath.begin(), ppath.end(),
                                                         file_path.begin()));
                            auto cd_mm_res(std::mismatch(
                                cdirpath.begin(), cdirpath.end(), file_path.begin()));
                            if (pp_mm_res.first == ppath.end() or
                                cd_mm_res.first == cdirpath.end()) {
                                // diag(HashLoc, "Relative include paths are
                                // forbidden");
                            }
                        }
                        this->checked = true;
                    }
                }
            };

            Rule8aCheck::Rule8aCheck(StringRef Name, ClangTidyContext *Context)
                : ClangTidyCheck(Name, Context),
                  debug_enabled(Options.get("debug", "false")) {
                if (this->debug_enabled == "true") {
                    this->debug = true;
                }
            }

            void Rule8aCheck::registerPPCallbacks(const SourceManager &SM,
                                                  Preprocessor *PP,
                                                  Preprocessor *ModuleExpanderPP) {
                PP->addPPCallbacks(std::make_unique<Rule8aPPCallBack>(this, PP, SM));
            }

            void Rule8aCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(functionDecl().bind("function_decl"), this);
            }
            void Rule8aCheck::check(const MatchFinder::MatchResult &Result) {
                if (auto MatchedDecl =
                        Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
                    const SourceManager &SM = *Result.SourceManager;
                    if (SM.isInMainFile(MatchedDecl->getLocation())) {
                        const FileEntry *MainFile = SM.getFileEntryForID(
                            SM.getFileID(MatchedDecl->getLocation()));
                        if (MainFile and MainFile->isValid()) {
                            std::string file_path(MainFile->tryGetRealPathName().str());
                        }
                    }
                }
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

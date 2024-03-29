
//===--- Rule8dCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8dCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
class Rule8dPPCallBack : public PPCallbacks {
private:
    Rule8dCheck *Check;

public:
    Rule8dPPCallBack(Rule8dCheck *Check) : Check(Check){};

    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool isAngled,
                            CharSourceRange FilenameRange, OptionalFileEntryRef File,
                            StringRef SearchPath, StringRef RelativePath,
                            const Module *Imported,
                            SrcMgr::CharacteristicKind FileType) override {

        if (not isAngled) {
            std::string header_file_path(FileName.str());
            std::string ppath("../");
            std::string cdirpath("./");
            auto pp_mm_res(
                std::mismatch(ppath.begin(), ppath.end(), header_file_path.begin()));
            auto cd_mm_res(std::mismatch(cdirpath.begin(), cdirpath.end(),
                                         header_file_path.begin()));
            if (pp_mm_res.first == ppath.end() || cd_mm_res.first == cdirpath.end()) {
                Check->diag(HashLoc, "Relative include paths are forbidden");
            }
        }
    }
};

Rule8dCheck::Rule8dCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule8dCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                      Preprocessor *ModuleExpanderPP) {
    PP->addPPCallbacks(std::make_unique<Rule8dPPCallBack>(this));
}
void Rule8dCheck::registerMatchers(MatchFinder *Finder) {}
void Rule8dCheck::check(const MatchFinder::MatchResult &Result) {}
} // namespace eastwood
} // namespace tidy
} // namespace clang

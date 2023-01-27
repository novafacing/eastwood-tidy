
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

public:
    Rule8bPPCallBack(Rule8bCheck *Check) : Check(Check){};
    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool isAngled,
                            CharSourceRange FilenameRange, OptionalFileEntryRef File,
                            StringRef SearchPath, StringRef RelativePath,
                            const Module *Imported,
                            SrcMgr::CharacteristicKind FileType) override {

        if (not isAngled) {
            if (File) {
                std::string file_path(File->getFileEntry().tryGetRealPathName().str());
                std::string ext(file_path.substr(file_path.find_last_of(".") + 1));
                if (ext != "h") {
                    Check->diag(HashLoc,
                                "All non-global header files must end in '.h'.");
                }
            }
        }
    }
};
Rule8bCheck::Rule8bCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}
void Rule8bCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                      Preprocessor *ModuleExpanderPP) {
    PP->addPPCallbacks(std::make_unique<Rule8bPPCallBack>(this));
}
void Rule8bCheck::registerMatchers(MatchFinder *Finder) {}
void Rule8bCheck::check(const MatchFinder::MatchResult &Result) {}
} // namespace eastwood
} // namespace tidy
} // namespace clang

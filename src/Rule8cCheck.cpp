
//===--- Rule8cCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8cCheck.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
class Rule8cPPCallBack : public PPCallbacks {
private:
    Rule8cCheck *Check;
    Preprocessor *PP;
    const SourceManager &SM;

public:
    Rule8cPPCallBack(Rule8cCheck *Check, Preprocessor *PP, const SourceManager &SM)
        : Check(Check), PP(PP), SM(SM){};

    void Ifndef(SourceLocation Loc, const Token &MacroNameTok,
                const MacroDefinition &MD) override {}

    void MacroDefined(const Token &MacroNameTok, const MacroDirective *MD) override {}

    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool isAngled,
                            CharSourceRange FilenameRange, const FileEntry *File,
                            StringRef SearchPath, StringRef RelativePath,
                            const Module *Imported,
                            SrcMgr::CharacteristicKind FileType) override {
        if (!File || !File->isValid()) {
            this->Check->diag(HashLoc, "Header file does not exist.");
            return;
        }

        if (this->SM.isWrittenInMainFile(HashLoc)) {
            if (not isAngled) {
                std::string basename(FileName.str());
                if (basename.find_last_of("/") != std::string::npos) {
                    basename = basename.substr(basename.find_last_of("/") + 1);
                }
                std::transform(basename.begin(), basename.end(), basename.begin(),
                               ::toupper);
                std::replace(basename.begin(), basename.end(), '.', '_');
                std::replace(basename.begin(), basename.end(), '-', '_');
                auto filename = File->tryGetRealPathName().str();
                std::ifstream headerfs(filename);
                std::vector<std::string> lines;
                std::string line;
                while (std::getline(headerfs, line)) {
                    lines.push_back(line);
                }
                std::regex firstline("^#ifndef " + basename + "$");
                std::regex secondline("^#define " + basename + "$");
                std::regex lastline("^#endif // " + basename + "$");
                std::smatch results;
                if (lines.size() < 1) {
                    this->Check->diag(HashLoc,
                                      "Header file missing ifndef component of "
                                      "include guard in header file.");
                    this->Check->diag(HashLoc,
                                      "Header file missing define component of "
                                      "include guard in header file.");
                    this->Check->diag(HashLoc, "Header file missing endif component "
                                               "of include guard in header file.");

                } else {
                    if (not std::regex_match(lines.at(0), results, firstline)) {
                        this->Check->diag(HashLoc, "Malformed ifndef component of "
                                                   "include guard in header file.");
                    }
                }
                if (lines.size() < 2) {
                    this->Check->diag(HashLoc,
                                      "Header file missing define component of "
                                      "include guard in header file.");
                    this->Check->diag(HashLoc, "Header file missing endif component "
                                               "of include guard in header file.");
                } else {
                    if (not std::regex_match(lines.at(1), results, secondline)) {
                        this->Check->diag(HashLoc, "Malformed define component of "
                                                   "include guard in header file.");
                    }
                }
                if (lines.size() < 3) {
                    this->Check->diag(HashLoc, "Header file missing endif component "
                                               "of include guard in header file.");
                } else {
                    if (not std::regex_match(lines.back(), results, lastline)) {
                        this->Check->diag(HashLoc, "Malformed endif component of "
                                                   "include guard in header file.");
                    }
                }
            }
        }
    }
};

Rule8cCheck::Rule8cCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule8cCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                      Preprocessor *ModuleExpanderPP) {
    PP->addPPCallbacks(std::make_unique<Rule8cPPCallBack>(this, PP, SM));
}

void Rule8cCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(functionDecl().bind("function"), this);
}

void Rule8cCheck::check(const MatchFinder::MatchResult &Result) { return; }

void Rule8cCheck::onEndOfTranslationUnit() {}
} // namespace eastwood
} // namespace tidy
} // namespace clang


//===--- Rule8gCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule8gCheck.h"
#include <ftw.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace clang::ast_matchers;

std::vector<std::string> *files;

static int AddFile(const char *fpath, const struct stat *sb, int typeFlag) {

  if (typeFlag == FTW_F) {
    std::string s_fpath(fpath);
    s_fpath = s_fpath.substr(s_fpath.find_last_of("/") + 1);
    files->push_back(s_fpath);
  }
  return 0;
}
namespace clang {
namespace tidy {
namespace eastwood {
class Rule8gPPCallBack : public PPCallbacks {
private:
  Rule8gCheck *Check;
  Preprocessor *PP;
  const SourceManager &SM;
  bool checked;

public:
  Rule8gPPCallBack(Rule8gCheck *Check, Preprocessor *PP,
                   const SourceManager &SM)
      : Check(Check), PP(PP), SM(SM), checked(false){};

  void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                          StringRef FileName, bool isAngled,
                          CharSourceRange FilenameRange, const FileEntry *File,
                          StringRef SearchPath, StringRef RelativePath,
                          const Module *Imported,
                          SrcMgr::CharacteristicKind FileType) override {

    if (not this->checked) {
      files = new std::vector<std::string>();
      std::string main_file_path(
          this->SM.getFileEntryForID(this->SM.getFileID(HashLoc))
              ->tryGetRealPathName()
              .str());
      std::string main_file_dir(
          main_file_path.substr(0, main_file_path.find_last_of("/")));
      ftw(main_file_dir.c_str(), AddFile, 0x10);
      this->checked = true;
    }
    if (not File || not File->isValid()) {
      return;
    }

    std::string header_file_path(File->tryGetRealPathName().str());
    std::string header_file_name(
        header_file_path.substr(header_file_path.find_last_of("/") + 1));
    for (auto fn : *files) {
      // std::cout << "Checking header file: " << fn << std::endl;
    }
    if (isAngled) {
      for (auto fn : *files) {
        if (fn == header_file_name) {
          this->Check->diag(HashLoc, "Local includes must be included "
                                     "with \"...\", not angle braces.");
        }
      }
    } else {
      for (auto fn : *files) {
        if (fn == header_file_name) {
          return;
        }
      }
      if (SM.isWrittenInMainFile(HashLoc)) {
        this->Check->diag(HashLoc, "Non-local include must be included "
                                   "with <...>, not double quotes.");
      }
    }
  }
};

Rule8gCheck::Rule8gCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
  if (this->debug_enabled == "true") {
    this->debug = true;
  }
}
void Rule8gCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                      Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<Rule8gPPCallBack>(this, PP, SM));
}

void Rule8gCheck::registerMatchers(MatchFinder *Finder) {}
void Rule8gCheck::check(const MatchFinder::MatchResult &Result) {}
} // namespace eastwood
} // namespace tidy
} // namespace clang

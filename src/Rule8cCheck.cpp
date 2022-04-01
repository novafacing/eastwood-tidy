
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
  bool checked;

public:
  Rule8cPPCallBack(Rule8cCheck *Check, Preprocessor *PP,
                   const SourceManager &SM)
      : Check(Check), PP(PP), SM(SM), checked(false){};

  void Ifndef(SourceLocation Loc, const Token &MacroNameTok,
              const MacroDefinition &MD) override {
    /*
        std::string
       macro_name(this->SM.getCharacterData(MacroNameTok.getLocation()),
                this->SM.getCharacterData(MacroNameTok.getEndLoc()));
        std::string basename(macro_name);
        if (basename.find_last_of("/") != std::string::npos) {
            basename = basename.substr(basename.find_last_of("/") + 1);
        }
        std::transform(basename.begin(),
       basename.end(),basename.begin(), ::toupper);
        std::replace(basename.begin(), basename.end(), '.', '_');
        std::replace(basename.begin(), basename.end(), '-', '_');
        this->Check->found_ifndef_checks.push_back(basename);
        */
  }

  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    /*
        std::string
       macro_name(this->SM.getCharacterData(MacroNameTok.getLocation()),
                this->SM.getCharacterData(MacroNameTok.getEndLoc()));
        std::string basename(macro_name);
        if (basename.find_last_of("/") != std::string::npos) {
            basename = basename.substr(basename.find_last_of("/") + 1);
        }
        std::transform(basename.begin(),
       basename.end(),basename.begin(), ::toupper);
        std::replace(basename.begin(), basename.end(), '.', '_');
        std::replace(basename.begin(), basename.end(), '-', '_');
        this->Check->found_defined_macro_guards.push_back(basename);
        */
  }

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
        // basename += "_H";
        auto filename = File->tryGetRealPathName().str();
        std::ifstream headerfs(filename);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(headerfs, line)) {
          // std::cout << "read line from file: " << line <<
          // std::endl;
          lines.push_back(line);
        }
        std::regex firstline("^#ifndef " + basename + "$");
        std::regex secondline("^#define " + basename + "$");
        std::regex lastline("^#endif // " + basename + "$");
        std::smatch results;
        for (auto s : lines) {
          // std::cout << "Line in include file: " << s <<
          // std::endl;
        }
        if (lines.size() < 1) {
          this->Check->diag(HashLoc, "Header file missing ifndef component of "
                                     "include guard in header file.");
          this->Check->diag(HashLoc, "Header file missing define component of "
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
          this->Check->diag(HashLoc, "Header file missing define component of "
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

void Rule8cCheck::onEndOfTranslationUnit() {
  /*
  std::cout << "Have " << this->required_guards.size() << " required
  guards" << std::endl; for (auto r : this->required_guards) { std::cout
  << "Guard required: " << r.first << std::endl;
  }
  std::cout << "Have " << this->found_ifndef_checks.size() << " ifdef
  checks:" << std::endl; for (auto r : this->found_ifndef_checks) {
      std::cout << "Ifdef check: " << r << std::endl;
  }

  for (auto req : this->required_guards) {
      std::cout << "Required guard: " << req.first << std::endl;
      if (std::find(this->found_ifndef_checks.begin(),
  this->found_ifndef_checks.end(), req.first)
              == this->found_ifndef_checks.end()) {
          diag(req.second, "Expected ifndef define guard check in included
  header file");
      }
      if (std::find(this->found_defined_macro_guards.begin(),
  this->found_defined_macro_guards.end(), req.first)
              == this->found_defined_macro_guards.end()) {
          diag(req.second, "Expected define guard in included header
  file");
      }
  }
  */
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

//===--- Rule1cCheck.cpp - clang-tidy----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule1cCheck.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/PPCallbacks.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/Regex.h"
#include <algorithm>
#include <cctype>
#include <iostream>

namespace clang {
namespace tidy {
namespace eastwood {

namespace {

bool isCapsOnly(StringRef Name) {
  if (Name.size() < 2) {
    return false;
  }

  return std::all_of(Name.begin(), Name.end(), [](const char c) {
    if (std::isupper(c) || std::isdigit(c) || c == '_')
      return true;
    return false;
  });
}

class MacroUsageCallbacks : public PPCallbacks {
public:
  MacroUsageCallbacks(Rule1cCheck *Check, const SourceManager &SM,
                      StringRef RegExp, bool CapsOnly, bool IgnoreCommandLine)
      : Check(Check), SM(SM), RegExp(RegExp), CheckCapsOnly(CapsOnly),
        IgnoreCommandLineMacros(IgnoreCommandLine) {}
  void MacroDefined(const Token &MacroNameTok,
                    const MacroDirective *MD) override {
    if (SM.isWrittenInBuiltinFile(MD->getLocation()) ||
        MD->getMacroInfo()->isUsedForHeaderGuard() ||
        MD->getMacroInfo()->isFunctionLike() ||
        MD->getMacroInfo()->getNumTokens() == 0)
      return;

    StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
    if (!isCapsOnly(MacroName)) {
      Check->warnNaming(MD, MacroName);
    }

    SourceLocation loc = MD->getMacroInfo()->getDefinitionEndLoc();
    bool found_errors = false;
    // TODO: Ensure that the following conventions are followed:
    //    - "strings are like this"
    //    - (10)
    //    - 'c' TODO: Check with Jeff on this 
    
    /*
    if (*SM.getCharacterData(loc) != '\'' &&
        *SM.getCharacterData(loc) != '\"') {

      if (*SM.getCharacterData(loc) != '(' ||
          *SM.getCharacterData(loc.getLocWithOffset(1)) == '\'' ||
          *SM.getCharacterData(loc.getLocWithOffset(1)) == '\"') {
        Check->warnFormatting(loc, MacroName);
      }
    }
    */
  }

private:
  Rule1cCheck *Check;
  const SourceManager &SM;
  StringRef RegExp;
  bool CheckCapsOnly;
  bool IgnoreCommandLineMacros;
};
} // namespace

void Rule1cCheck::registerPPCallbacks(const SourceManager &SM,
                                          Preprocessor *PP,
                                          Preprocessor *ModuleExpanderPP) {
  PP->addPPCallbacks(std::make_unique<MacroUsageCallbacks>(
      this, SM, AllowedRegexp, CheckCapsOnly, IgnoreCommandLineMacros));
}

void Rule1cCheck::warnNaming(const MacroDirective *MD,
                                 StringRef MacroName) {
  diag(MD->getLocation(), "[Rule I.C] Macro definition does not define the macro name "
                          "'%0' using all uppercase characters or is only one character"
                          " long")
      << MacroName;
}

void Rule1cCheck::warnFormatting(const SourceLocation loc,
                                 StringRef MacroName) {
  diag(loc,               "[Rule I.C] Constant strings and characters must not "
                          "be surrounded in parentheses, constant numbers must "
                          "be surrounded in parentheses")
      << MacroName;
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

//===--- Rule1cCheck.h - clang-tidy--------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE1CCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE1CCHECK_H

#include "../ClangTidy.h"
#include "clang/Lex/Preprocessor.h"
#include <string>

namespace clang {
namespace tidy {
namespace eastwood {

/// Find macro usage that is considered problematic because better language
/// constructs exist for the task.
///
/// For the user-facing documentation see:
/// http://clang.llvm.org/extra/clang-tidy/checks/eastwood-macro-usage.html
class Rule1cCheck : public ClangTidyCheck {
public:
  Rule1cCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                           Preprocessor *ModuleExpanderPP) override;
  void warnNaming(const MacroDirective *MD, StringRef MacroName);
  void warnFormatting(const SourceLocation loc, StringRef MacroName);

private:
  /// A regular expression that defines how allowed macros must look like.
  std::string AllowedRegexp;
  /// Control if only the check shall only test on CAPS_ONLY macros.
  bool CheckCapsOnly;
  /// Should the macros without a valid location be diagnosed?
  bool IgnoreCommandLineMacros;
};

} // namespace eastwood
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_EASTWOOD_RULE1CCHECK_H

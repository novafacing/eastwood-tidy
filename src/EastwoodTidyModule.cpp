//===--- EastoodTidyModule.cpp - clang-tidy ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "EastwoodTidyModule.h"

#include "Rule10aCheck.h"
#include "Rule11aCheck.h"
#include "Rule11bCheck.h"
#include "Rule11cCheck.h"
#include "Rule11dCheck.h"
#include "Rule11eCheck.h"
#include "Rule12aCheck.h"
#include "Rule12bCheck.h"
#include "Rule12cCheck.h"
#include "Rule1aCheck.h"
#include "Rule1bCheck.h"
#include "Rule1cCheck.h"
#include "Rule1dCheck.h"
#include "Rule2aCheck.h"
#include "Rule2bCheck.h"
#include "Rule3aCheck.h"
#include "Rule3bCheck.h"
#include "Rule3cCheck.h"
#include "Rule3dCheck.h"
#include "Rule3eCheck.h"
#include "Rule3fCheck.h"
#include "Rule4aCheck.h"
#include "Rule4bCheck.h"
#include "Rule4cCheck.h"
#include "Rule5aCheck.h"
#include "Rule5bCheck.h"
#include "Rule5cCheck.h"
#include "Rule5dCheck.h"
#include "Rule5eCheck.h"
#include "Rule6aCheck.h"
#include "Rule7aCheck.h"
#include "Rule8aCheck.h"
#include "Rule8bCheck.h"
#include "Rule8cCheck.h"
#include "Rule8dCheck.h"
#include "Rule8eCheck.h"
#include "Rule8fCheck.h"
#include "Rule8gCheck.h"
#include "Rule9aCheck.h"
#include "Rule9bCheck.h"
#include "Rule9cCheck.h"
#include "Rule9dCheck.h"
#include "Rule9eCheck.h"
#include <llvm/Support/PrettyStackTrace.h>
namespace clang {
    namespace tidy {
        namespace eastwood {

            class EastwoodModule : public ClangTidyModule {
            public:
                void
                addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
                    this->setup();
                    CheckFactories.registerCheck<Rule1aCheck>("eastwood-Rule1aCheck");
                    CheckFactories.registerCheck<Rule1bCheck>("eastwood-Rule1bCheck");
                    CheckFactories.registerCheck<Rule1cCheck>("eastwood-Rule1cCheck");
                    CheckFactories.registerCheck<Rule1dCheck>("eastwood-Rule1dCheck");
                    CheckFactories.registerCheck<Rule2aCheck>("eastwood-Rule2aCheck");
                    CheckFactories.registerCheck<Rule2bCheck>("eastwood-Rule2bCheck");
                    CheckFactories.registerCheck<Rule3aCheck>("eastwood-Rule3aCheck");
                    CheckFactories.registerCheck<Rule3bCheck>("eastwood-Rule3bCheck");
                    CheckFactories.registerCheck<Rule3cCheck>("eastwood-Rule3cCheck");
                    CheckFactories.registerCheck<Rule3dCheck>("eastwood-Rule3dCheck");
                    CheckFactories.registerCheck<Rule3eCheck>("eastwood-Rule3eCheck");
                    CheckFactories.registerCheck<Rule3fCheck>("eastwood-Rule3fCheck");
                    CheckFactories.registerCheck<Rule4aCheck>("eastwood-Rule4aCheck");
                    CheckFactories.registerCheck<Rule4bCheck>("eastwood-Rule4bCheck");
                    CheckFactories.registerCheck<Rule4cCheck>("eastwood-Rule4cCheck");
                    CheckFactories.registerCheck<Rule5aCheck>("eastwood-Rule5aCheck");
                    CheckFactories.registerCheck<Rule5bCheck>("eastwood-Rule5bCheck");
                    CheckFactories.registerCheck<Rule5cCheck>("eastwood-Rule5cCheck");
                    CheckFactories.registerCheck<Rule5dCheck>("eastwood-Rule5dCheck");
                    CheckFactories.registerCheck<Rule5eCheck>("eastwood-Rule5eCheck");
                    CheckFactories.registerCheck<Rule6aCheck>("eastwood-Rule6aCheck");
                    CheckFactories.registerCheck<Rule7aCheck>("eastwood-Rule7aCheck");
                    CheckFactories.registerCheck<Rule8aCheck>("eastwood-Rule8aCheck");
                    CheckFactories.registerCheck<Rule8bCheck>("eastwood-Rule8bCheck");
                    CheckFactories.registerCheck<Rule8cCheck>("eastwood-Rule8cCheck");
                    CheckFactories.registerCheck<Rule8dCheck>("eastwood-Rule8dCheck");
                    CheckFactories.registerCheck<Rule8eCheck>("eastwood-Rule8eCheck");
                    CheckFactories.registerCheck<Rule8fCheck>("eastwood-Rule8fCheck");
                    CheckFactories.registerCheck<Rule8gCheck>("eastwood-Rule8gCheck");
                    CheckFactories.registerCheck<Rule9aCheck>("eastwood-Rule9aCheck");
                    CheckFactories.registerCheck<Rule9bCheck>("eastwood-Rule9bCheck");
                    CheckFactories.registerCheck<Rule9cCheck>("eastwood-Rule9cCheck");
                    CheckFactories.registerCheck<Rule9dCheck>("eastwood-Rule9dCheck");
                    CheckFactories.registerCheck<Rule9eCheck>("eastwood-Rule9eCheck");
                    CheckFactories.registerCheck<Rule10aCheck>("eastwood-Rule10aCheck");
                    CheckFactories.registerCheck<Rule11aCheck>("eastwood-Rule11aCheck");
                    CheckFactories.registerCheck<Rule11bCheck>("eastwood-Rule11bCheck");
                    CheckFactories.registerCheck<Rule11cCheck>("eastwood-Rule11cCheck");
                    CheckFactories.registerCheck<Rule11dCheck>("eastwood-Rule11dCheck");
                    CheckFactories.registerCheck<Rule11eCheck>("eastwood-Rule11eCheck");
                    CheckFactories.registerCheck<Rule12aCheck>("eastwood-Rule12aCheck");
                    CheckFactories.registerCheck<Rule12bCheck>("eastwood-Rule12bCheck");
                    CheckFactories.registerCheck<Rule12cCheck>("eastwood-Rule12cCheck");
                }

            private:
                void setup(void) {
                    llvm::setBugReportMsg(
                        "=============================================================="
                        "==================\nPLEASE submit a bug report to:\nRowan "
                        "Hart "
                        "on Campuswire OR send an email to hart111@purdue.edu\nIn your "
                        "email, "
                        "please include:\n  - The entire stack trace printed below.\n  "
                        "- Your code that is causing the "
                        "crash.\n======================================================"
                        "==========================\n");
                }
            };
        } // namespace eastwood

        // Register the EastwoodTidyModule using this statically initialized variable.
        static ClangTidyModuleRegistry::Add<eastwood::EastwoodModule>
            X("eastwood-module", "Adds CS240 linter checks");

        // This anchor is used to force the linker to link in the generated object file
        // and thus register the EastwoodModule
        volatile int EastwoodModuleAnchorSource = 0;

    } // namespace tidy
} // namespace clang
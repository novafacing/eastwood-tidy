//===--- EastoodTidyModule.cpp - clang-tidy ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "Rule11eCheck.h"
#include "Rule1aCheck.h"
#include "Rule1bCheck.h"
#include "Rule1cCheck.h"
#include "Rule1dCheck.h"
#include "Rule4aCheck.h"
#include "Rule4cCheck.h"

namespace clang {
    namespace tidy {
        namespace eastwood {

            class EastwoodModule : public ClangTidyModule {
                public:
                    void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
                        CheckFactories.registerCheck<Rule11eCheck>(
                                "eastwood-Rule11E");
                        CheckFactories.registerCheck<Rule1aCheck>(
                                "eastwood-Rule1A");
                        CheckFactories.registerCheck<Rule1bCheck>(
                                "eastwood-Rule1B");
                        CheckFactories.registerCheck<Rule1cCheck>(
                                "eastwood-Rule1C");
                        CheckFactories.registerCheck<Rule1dCheck>(
                                "eastwood-Rule1D");
                        CheckFactories.registerCheck<Rule4aCheck>(
                                "eastwood-Rule4A");
                        CheckFactories.registerCheck<Rule4cCheck>(
                                "eastwood-Rule4C");
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

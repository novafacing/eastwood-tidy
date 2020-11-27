//===--- Rule1cCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// All constants must be uppercase, contain at least 2 characters, and be declared with define

#include "Rule1cCheck.h"

using namespace clang::ast_matchers;

/* This may be overly permissive */
static bool isSurroundedLeft(const clang::Token & T) {
    return T.isOneOf(clang::tok::l_paren, clang::tok::l_brace, clang::tok::l_square, clang::tok::comma, clang::tok::semi);
}

/* This may be overly permissive */
static bool isSurroundedRight(const clang::Token & T) {
    return T.isOneOf(clang::tok::r_paren, clang::tok::r_brace, clang::tok::r_square, clang::tok::comma, clang::tok::semi);
}

namespace clang {
    namespace tidy {
        namespace eastwood {

            class Rule1cPPCallBack : public PPCallbacks {
                private:
                    Rule1cCheck * Check;
                    Preprocessor * PP;

                public:
                    Rule1cPPCallBack(Rule1cCheck * Check, Preprocessor * PP) 
                        : Check(Check), PP(PP) {
                        }

                    /* Can also implement:
                     *  - MacroExpands (whenever macro is expanded)
                     *  - MacroUndefined (whenever #undef <x>
                     *  - Defined (whenever `defined` token
                     *  - Ifdef
                     *  - Ifndef
                     */
                    void MacroDefined(const Token & MacroNameTok, const MacroDirective * MD) override {
                        if (this->PP->getSourceManager().isWrittenInBuiltinFile(MD->getLocation()) ||
                                !this->PP->getSourceManager().isWrittenInMainFile(MD->getLocation()) ||
                                MD->getMacroInfo()->isUsedForHeaderGuard() ||
                                MD->getMacroInfo()->getNumTokens() == 0) {
                            return;
                        }
                        std::regex defineNameRegex{R"([A-Z0-9_][A-Z0-9_]+)"};
                        std::smatch results;
                        std::string name = this->PP->getSpelling(MacroNameTok);
                        if (!std::regex_match(name, results, defineNameRegex)) {
                            DiagnosticBuilder Diag = this->Check->diag(MD->getLocation(), 
                                    "'%0' is not all uppercase, separated by underscores, and >= 2 characters in length.");
                            Diag << name;
                        }


                        /* A constant declaration with #define will either be "string" or (val) (1 / 3 toks) */
                        // TODO: Still failing the correct case....not sure why. TODO!!!!
                        if (MD->getMacroInfo()->getNumTokens() == 1 || MD->getMacroInfo()->getNumTokens() == 3) {
                            const Token & start = *(MD->getMacroInfo()->tokens_begin());
                            const Token & primary = *(MD->getMacroInfo()->tokens_begin() + 1);
                            const Token & end = *(MD->getMacroInfo()->tokens_begin() + 2);
                            if (tok::isLiteral(primary.getKind()) && !tok::isStringLiteral(primary.getKind())) {
                                if (isSurroundedLeft(start) && isSurroundedRight(end)) {
                                    return;
                                } else {
                                    DiagnosticBuilder Diag = this->Check->diag(MD->getLocation(),
                                            "'%0' initializer is non-string constant and not surrounded by parentheses.");
                                    Diag << name << primary.getLiteralData();
                                }
                            } else if (tok::isLiteral(start.getKind()) && !tok::isStringLiteral(start.getKind()) && MD->getMacroInfo()->getNumTokens() == 1) {
                                DiagnosticBuilder Diag = this->Check->diag(MD->getLocation(),
                                        "'%0' initializer is non-string constant and not surrounded by parentheses.");
                                Diag << name;

                            }
                        }
                    }
            };

            Rule1cCheck::Rule1cCheck(StringRef Name, ClangTidyContext * Context) :
                ClangTidyCheck(Name, Context), dump(Options.get("dump", "false")) {

                    if (this->dump == "true") {
                        for (auto ty : {"characterLiteral", "floatLiteral", "imaginaryLiteral", 
                                "integerLiteral", "userDefinedLiteral", "fixedPointLiteral", "compoundLiteral"}) {
                            std::vector<SourceLocation> v{};
                            this->embeddedConstants.insert(std::make_pair(ty, v));
                        }
                    }
                }

            void Rule1cCheck::registerPPCallbacks(const SourceManager & SM, Preprocessor * PP,
                    Preprocessor * ModuleExpanderPP) {
                PP->addPPCallbacks(std::make_unique<Rule1cPPCallBack>(this, PP));
            }

            void Rule1cCheck::registerMatchers(MatchFinder *Finder) {
                /* We want to be able to ignore varDecls */
                Finder->addMatcher(varDecl().bind("variable"), this);
                /* Embedded Constants Matchers */
                Finder->addMatcher(characterLiteral().bind("characterLiteral"), this);
                Finder->addMatcher(floatLiteral().bind("floatLiteral"), this);
                Finder->addMatcher(imaginaryLiteral().bind("imaginaryLiteral"), this);
                Finder->addMatcher(integerLiteral().bind("integerLiteral"), this);
                Finder->addMatcher(userDefinedLiteral().bind("userDefinedLiteral"), this);
                Finder->addMatcher(fixedPointLiteral().bind("fixedPointLiteral"), this);
                Finder->addMatcher(compoundLiteralExpr().bind("compoundLiteralExpr"), this);
                /* If checking CXX, add cxxNullPtrLiteralExpr and cxxBoolLiteral */
            }

            void Rule1cCheck::saveEmbeddedConstant(SourceLocation loc, std::string type) {
                this->embeddedConstants[type].push_back(loc);
            }

            void Rule1cCheck::check(const MatchFinder::MatchResult &Result) {

                std::string type = "";
                SourceLocation loc;

                if (auto Match = Result.Nodes.getNodeAs<VarDecl>("variable")) {
                    if (Match->hasDefinition(*Result.Context)) {
                        /* If we aren't defining, we definitely don't need to check that the literal is in it */
                        this->declarationRanges.push_back(Match->getSourceRange());
                    }
                }

                if (auto Match = Result.Nodes.getNodeAs<Stmt>("characterLiteral")) {
                    type = "characterLiteral";
                    loc = Match->getBeginLoc();
                } else if (auto Match = Result.Nodes.getNodeAs<Stmt>("floatLiteral")) {
                    type = "floatLiteral";
                    loc = Match->getBeginLoc();
                } else if (auto Match = Result.Nodes.getNodeAs<Stmt>("imaginaryLiteral")) {
                    type = "imaginaryLiteral";
                    loc = Match->getBeginLoc();
                } else if (auto Match = Result.Nodes.getNodeAs<Stmt>("integerLiteral")) {
                    type = "integerLiteral";
                    loc = Match->getBeginLoc();
                } else if (auto Match = Result.Nodes.getNodeAs<Stmt>("userDefinedLiteral")) {
                    type = "userDefinedLiteral";
                    loc = Match->getBeginLoc();
                } else if (auto Match = Result.Nodes.getNodeAs<Stmt>("fixedPointLiteral")) {
                    type = "fixedPointLiteral";
                    loc = Match->getBeginLoc();
                } else if (auto Match = Result.Nodes.getNodeAs<Stmt>("compoundLiteralExpr")) {
                    type = "compoundLiteralExpr";
                    loc = Match->getBeginLoc();
                } else {
                    return;
                }

                if ((Result.SourceManager)->isWrittenInMainFile(loc)) {
                    if (loc.isValid()) {
                        this->saveEmbeddedConstant(loc, type);
                    }
                }
            }

            void Rule1cCheck::onEndOfTranslationUnit() {
                ClangTidyCheck::onEndOfTranslationUnit();
                if (this->dump == "true") {
                    std::ios init(NULL);
                    init.copyfmt(std::cout);
                    for (auto ty : {"characterLiteral", "floatLiteral", "imaginaryLiteral", "integerLiteral", 
                            "userDefinedLiteral", "fixedPointLiteral", "compoundLiteral"}) {
                        for (auto constant : this->embeddedConstants.at(ty)) {
                            if (constant.isValid()) {
                                if (this->declarationRanges.empty()) {
                                    diag(constant, "embedded constant of type '%0'.", DiagnosticIDs::Note) << ty;
                                } else {
                                    bool toPrint = true;
                                    for (auto range : this->declarationRanges) {
                                        if (range.fullyContains(SourceRange(constant))) {
                                            toPrint = false;

                                        }
                                    }
                                    if (toPrint) {
                                        diag(constant, "embedded constant of type '%0'.", DiagnosticIDs::Note) << ty;
                                    }
                                }
                            }
                        }
                    }
                    std::cout.copyfmt(init);
                }
            }

            void Rule1cCheck::storeOptions(ClangTidyOptions::OptionMap & Opts) {
                Options.store(Opts, "dump", this->dump);
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

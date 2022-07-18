
//===--- Rule5bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule5bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
class Rule5bPPCallBack : public PPCallbacks {
private:
    Rule5bCheck *Check;
    const SourceManager &SM;

public:
    Rule5bPPCallBack(Rule5bCheck *Check, const SourceManager &SM)
        : Check(Check), SM(SM){};

    void MacroDefined(const Token &MacroNameTok, const MacroDirective *MD) override {
        SourceLocation Loc = MacroNameTok.getLocation();
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->alongside_ok_lines.push_back(Loc);
    }
};

Rule5bCheck::Rule5bCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule5bCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                      Preprocessor *ModuleExpanderPP) {
    PP->addPPCallbacks(std::make_unique<Rule5bPPCallBack>(this, SM));
}

void Rule5bCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(functionDecl(isDefinition()).bind("function"), this);
    Finder->addMatcher(decl().bind("decl"), this);
    Finder->addMatcher(ifStmt().bind("if"), this);
    Finder->addMatcher(switchCase().bind("case"), this);
    Finder->addMatcher(forStmt().bind("for"), this);
}

size_t count_newlines(std::string s) {
    size_t ct = 0;
    for (auto c : s) {
        if (c == '\n') {
            ct++;
        }
    }
    return ct;
}
void Rule5bCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();

    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        if (!this->source_manager->isWrittenInMainFile(MatchedDecl->getLocation())) {
            return;
        }

        this->alongside_ok_lines.push_back(MatchedDecl->getSourceRange().getEnd());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<Decl>("decl")) {
        if (!this->source_manager->isWrittenInMainFile(MatchedDecl->getEndLoc()) ||
            (MatchedDecl->getKind() != Decl::Kind::Var &&
             MatchedDecl->getKind() != Decl::Kind::Field)) {
            return;
        }
        this->alongside_ok_lines.push_back(MatchedDecl->getEndLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
        if (!this->source_manager->isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
            return;
        }
        this->alongside_ok_lines.push_back(MatchedDecl->getBeginLoc());

        if (auto els = MatchedDecl->getElse()) {
            if (!this->source_manager->isWrittenInMainFile(els->getBeginLoc())) {
                return;
            }
            this->alongside_ok_lines.push_back(els->getBeginLoc());
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchCase>("case")) {
        if (!this->source_manager->isWrittenInMainFile(MatchedDecl->getEndLoc())) {
            return;
        }
        this->alongside_ok_lines.push_back(MatchedDecl->getEndLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
        if (!this->source_manager->isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
            return;
        }
        this->explicit_not_ok_lines.push_back(MatchedDecl->getBeginLoc());
    }
}

void Rule5bCheck::onEndOfTranslationUnit(void) {
    for (auto token : this->tokens) {
        if (token.getKind() == tok::comment) {
            auto git = this->alongside_ok_lines.begin();
            auto bit = this->explicit_not_ok_lines.begin();

            for (; git != this->alongside_ok_lines.end(); git++) {
                if (this->source_manager->getSpellingLineNumber(token.getLocation()) ==
                    this->source_manager->getSpellingLineNumber(*git)) {
                    break;
                }
            }

            for (; bit != this->explicit_not_ok_lines.end(); bit++) {
                if (this->source_manager->getSpellingLineNumber(token.getLocation()) ==
                    this->source_manager->getSpellingLineNumber(*bit)) {
                    break;
                }
            }

            if ((git == this->alongside_ok_lines.end() ||
                 bit != this->explicit_not_ok_lines.end()) &&
                !token.isAtStartOfLine()) {
                auto errmsg = diag(token.getLocation(),
                                   "Comments must appear above code except "
                                   "for else, case, #defines, or declarations");
                errmsg << FixItHint::CreateRemoval(
                    SourceRange(token.getLocation(), token.getEndLoc()));
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

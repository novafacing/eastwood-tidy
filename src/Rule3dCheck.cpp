
//===--- Rule3dCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3dCheck.h"
#include "clang/Basic/Diagnostic.h"
#include <cstring>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {

class Rule3dPPCallBack : public PPCallbacks {
private:
    Rule3dCheck *Check;
    const SourceManager &SM;

public:
    Rule3dPPCallBack(Rule3dCheck *Check, const SourceManager &SM)
        : Check(Check), SM(SM){};

    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange, Optional<FileEntryRef> File,
                            StringRef SearchPath, StringRef RelativePath,
                            const Module *Imported,
                            SrcMgr::CharacteristicKind FileType) override {}

    void Ident(SourceLocation Loc, StringRef str) override {
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void PragmaDirective(SourceLocation Loc, PragmaIntroducerKind Introducer) override {
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void MacroDefined(const Token &MacroNameTok, const MacroDirective *MD) override {
        SourceLocation Loc = MacroNameTok.getLocation();
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void MacroUndefined(const Token &MacroNameTok, const MacroDefinition &MD,
                        const MacroDirective *Undef) override {

        SourceLocation Loc = MacroNameTok.getLocation();
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void If(SourceLocation Loc, SourceRange ConditionRange,
            ConditionValueKind ConditionValue) override {
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void Elif(SourceLocation Loc, SourceRange ConditionRange,
              ConditionValueKind ConditionValue, SourceLocation IfLoc) override {
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void Ifdef(SourceLocation Loc, const Token &MacroNameTok,
               const MacroDefinition &MD) override {
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void Ifndef(SourceLocation Loc, const Token &MacroNameTok,
                const MacroDefinition &MD) override {
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void Else(SourceLocation Loc, SourceLocation IfLoc) override {
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }

    void Endif(SourceLocation Loc, SourceLocation IfLoc) override {
        if (!SM.isWrittenInMainFile(Loc)) {
            return;
        }
        Check->directive_lines.push_back(SM.getSpellingLineNumber(Loc));
    }
};

static size_t spc_ct(std::string s) {
    size_t ct = 0;
    for (auto c : s) {
        if (c == ' ') {
            ct++;
        }
    }
    return ct;
}

Rule3dCheck::Rule3dCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule3dCheck::registerPPCallbacks(const SourceManager &SM, Preprocessor *PP,
                                      Preprocessor *ModuleExpanderPP) {
    PP->addPPCallbacks(std::make_unique<Rule3dPPCallBack>(this, SM));
}

void Rule3dCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(functionDecl().bind("function"), this);
}

void Rule3dCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();

    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        if (MatchedDecl->isThisDeclarationADefinition()) {
            this->function_boundaries.push_back(std::make_pair(
                this->source_manager->getSpellingLineNumber(MatchedDecl->getBeginLoc()),
                this->source_manager->getSpellingLineNumber(
                    MatchedDecl->getBodyRBrace())));
        }
    }
}

void Rule3dCheck::onEndOfTranslationUnit() {
    this->dout() << "Rule3dCheck::onEndOfTranslationUnit()" << std::endl;
    for (size_t i = 0; i < this->tokens.size(); i++) {
        auto tok = this->tokens.at(i);
        std::string ws("");
        if (i > 0) {
            ws = *this->sourcerange_string(
                *this->source_manager,
                SourceRange(this->tokens.at(i - 1).getLocation(), tok.getLocation()));
        }

        std::string raw_tok_data = *this->tok_string(*this->source_manager, tok);
        if (raw_tok_data.rfind("#", 0) == 0 && tok.isAtStartOfLine() &&
            spc_ct(ws) > 0) {
            auto errmsg =
                diag(tok.getLocation(), "Incorrect indentation level for preprocessor "
                                        "directive. Expected %0, got %1.")
                << 0 << spc_ct(ws);
            errmsg << FixItHint::CreateRemoval(SourceRange(
                tok.getLocation().getLocWithOffset(-spc_ct(ws)), tok.getLocation()));
        }
    }

    // For each preprocessor line, we need to check:
    // 1. That for the main translation unit, and for each function, all preprocessor
    // lines
    //    are consecutive
    // 2. That for each group of preprocessor lines, the first line is preceeded by an
    // empty line
    //    and succeeded by an empty line
    // std::sort(this->directive_lines.begin(), this->directive_lines.end());
    std::sort(this->function_boundaries.begin(), this->function_boundaries.end());
    // this->directive_lines.erase(
    //     std::unique(this->directive_lines.begin(), this->directive_lines.end()),
    //     this->directive_lines.end());

    std::vector<std::pair<size_t, size_t>> directive_groups;

    for (size_t i = 0; i < this->directive_lines.size(); i++) {
        this->dout() << "Directive line #" << i << ": " << this->directive_lines.at(i)
                     << std::endl;
        if (directive_groups.empty()) {
            directive_groups.push_back(std::make_pair(this->directive_lines.at(i),
                                                      this->directive_lines.at(i)));
        } else if (this->directive_lines.at(i) <= directive_groups.back().second + 1) {
            directive_groups.back().second = this->directive_lines.at(i);
        } else if (this->directive_lines.at(i) > directive_groups.back().second + 1) {
            directive_groups.push_back(std::make_pair(this->directive_lines.at(i),
                                                      this->directive_lines.at(i)));
        }
    }

    for (auto g : directive_groups) {
        this->dout() << "Got directive group: " << g.first << ":" << g.second
                     << std::endl;
    }

    for (auto b : this->function_boundaries) {
        this->dout() << "Got function boundary: " << b.first << ":" << b.second
                     << std::endl;
    }

    bool first_func = true;
    bool first_group = true;
    size_t token_idx = 0;
    SourceLocation last_start_loc =
        this->tokens.empty() ? SourceLocation() : this->tokens.at(0).getLocation();

    for (auto b : this->function_boundaries) {
        this->dout() << "Checking function boundary " << b.first << "-" << b.second
                     << std::endl;

        first_group = true;

        // Check groups before the start of this function
        while (!directive_groups.empty() && directive_groups.front().first <= b.first) {

            while (token_idx < this->tokens.size() &&
                   this->source_manager->getSpellingLineNumber(
                       this->tokens.at(token_idx).getLocation()) <=
                       directive_groups.front().second) {
                if (this->tokens.at(token_idx).isAtStartOfLine() &&
                    this->source_manager->getSpellingLineNumber(
                        this->tokens.at(token_idx).getLocation()) ==
                        directive_groups.front().first &&
                    token_idx > 0) {
                    // This is the token at the start of the beginning of the group
                    std::string prec_tok_str = *this->tok_string(
                        *this->source_manager, this->tokens.at(token_idx - 1));

                    if (std::count(prec_tok_str.begin(), prec_tok_str.end(), '\n') <
                        2) {
                        // There is no newline preceeding this group
                        diag(this->tokens.at(token_idx).getLocation(),
                             "Global preprocessor directive should be preceeded by at "
                             "least one "
                             "blank line.");
                    }
                }

                if (this->tokens.at(token_idx).isAtStartOfLine() &&
                    this->source_manager->getSpellingLineNumber(
                        this->tokens.at(token_idx).getLocation()) ==
                        directive_groups.front().second &&
                    token_idx < this->tokens.size()) {
                    // This is the token at the start of the last line in the group
                    for (size_t i = token_idx + 1; i < this->tokens.size(); i++) {
                        if (this->tokens.at(i).isAtStartOfLine()) {
                            // This is the token at the next start of line
                            std::string prec_tok_str = *this->tok_string(
                                *this->source_manager, this->tokens.at(i - 1));

                            if (std::count(prec_tok_str.begin(), prec_tok_str.end(),
                                           '\n') < 2) {
                                diag(this->tokens.at(token_idx).getLocation(),
                                     "Global preprocessor directive should be followed "
                                     "by at "
                                     "least one "
                                     "blank line.");
                            }

                            break;
                        }
                    }
                }

                token_idx++;
                if (token_idx < this->tokens.size() &&
                    this->tokens.at(token_idx).isAtStartOfLine() &&
                    this->tok_string(*this->source_manager, this->tokens.at(token_idx))
                            ->rfind('#', 0) != std::string::npos) {
                    last_start_loc = this->tokens.at(token_idx).getLocation();
                }
            }

            directive_groups.erase(directive_groups.begin());

            if (!first_group || !first_func) {
                diag(last_start_loc,
                     "All preprocessor directives outside of a function should be "
                     "placed at the top of the file after include directives.");
            }

            first_group = false;
        }

        // Check the groups inside this function
        first_group = true;
        while (!directive_groups.empty() && directive_groups.front().first >= b.first &&
               directive_groups.front().second <= b.second) {

            while (token_idx < this->tokens.size() &&
                   this->source_manager->getSpellingLineNumber(
                       this->tokens.at(token_idx).getLocation()) <=
                       directive_groups.front().second) {

                if (this->tokens.at(token_idx).isAtStartOfLine() &&
                    this->source_manager->getSpellingLineNumber(
                        this->tokens.at(token_idx).getLocation()) ==
                        directive_groups.front().first &&
                    token_idx < this->tokens.size() && token_idx > 0) {
                    // This is the token at the start of the beginning of the group
                    std::string prec_tok_str = *this->tok_string(
                        *this->source_manager, this->tokens.at(token_idx - 1));

                    if (std::count(prec_tok_str.begin(), prec_tok_str.end(), '\n') <
                        2) {
                        // There is no newline preceeding this group
                        diag(this->tokens.at(token_idx).getLocation(),
                             "Preprocessor directive should be preceeded by at "
                             "least one "
                             "blank line.");
                    }
                }

                if (this->tokens.at(token_idx).isAtStartOfLine() &&
                    this->source_manager->getSpellingLineNumber(
                        this->tokens.at(token_idx).getLocation()) ==
                        directive_groups.front().second &&
                    token_idx < this->tokens.size()) {
                    // This is the token at the start of the last line in the group
                    for (size_t i = token_idx + 1; i < this->tokens.size(); i++) {
                        if (this->tokens.at(i).isAtStartOfLine()) {
                            // This is the token at the next start of line
                            std::string prec_tok_str = *this->tok_string(
                                *this->source_manager, this->tokens.at(i - 1));

                            if (std::count(prec_tok_str.begin(), prec_tok_str.end(),
                                           '\n') < 2) {
                                diag(this->tokens.at(token_idx).getLocation(),
                                     "Preprocessor directive should be followed by at "
                                     "least one "
                                     "blank line.");
                            }

                            break;
                        }
                    }
                }

                token_idx++;

                if (token_idx < this->tokens.size() &&
                    this->tokens.at(token_idx).isAtStartOfLine() &&
                    this->tok_string(*this->source_manager, this->tokens.at(token_idx))
                            ->rfind('#', 0) != std::string::npos) {
                    last_start_loc = this->tokens.at(token_idx).getLocation();
                }
            }

            directive_groups.erase(directive_groups.begin());

            if (!first_group) {
                diag(last_start_loc,
                     "All preprocessor directives inside a function should be "
                     "grouped together.");
            }
            first_group = false;
        }

        first_func = false;
    }
}

} // namespace eastwood
} // namespace tidy
} // namespace clang

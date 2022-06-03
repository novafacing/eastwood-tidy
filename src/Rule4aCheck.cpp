//===--- Rule4aCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule4aCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Lex/Lexer.h"

#include <iomanip>
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {

Rule4aCheck::Rule4aCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")), indent_level(0) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}
void Rule4aCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(recordDecl().bind("record"), this);
    Finder->addMatcher(enumDecl().bind("enum"), this);
    Finder->addMatcher(functionDecl().bind("function"), this);
    Finder->addMatcher(doStmt().bind("do"), this);
    Finder->addMatcher(forStmt().bind("for"), this);
    Finder->addMatcher(ifStmt().bind("if"), this);
    Finder->addMatcher(switchStmt().bind("switch"), this);
    Finder->addMatcher(caseStmt().bind("case"), this);
    Finder->addMatcher(defaultStmt().bind("default"), this);
    Finder->addMatcher(whileStmt().bind("while"), this);
    Finder->addMatcher(compoundStmt().bind("compound"), this);
}

void Rule4aCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);
    RELEX();

    if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("record")) {
        CHECK_LOC(MatchedDecl);

        if (MatchedDecl->isCompleteDefinition()) {
            SourceRange BraceRange = MatchedDecl->getBraceRange();

            LOG_OPEN("record", BraceRange.getBegin());
            this->opens.push_back(BraceRange.getBegin());

            LOG_CLOSE("record", BraceRange.getEnd().getLocWithOffset(-1));
            this->closes.push_back(BraceRange.getEnd().getLocWithOffset(-1));

            if (this->source_manager->getSpellingLineNumber(this->opens.back()) !=
                this->source_manager->getSpellingLineNumber(
                    MatchedDecl->getLocation())) {
                diag(this->opens.back(), "Open brace must be located "
                                         "on same line as record.");
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
        CHECK_LOC(MatchedDecl);

        if (MatchedDecl->isCompleteDefinition()) {
            SourceRange BraceRange = MatchedDecl->getBraceRange();

            LOG_OPEN("enum", BraceRange.getBegin());
            this->opens.push_back(BraceRange.getBegin());
            LOG_CLOSE("enum", BraceRange.getEnd().getLocWithOffset(-1));
            this->closes.push_back(BraceRange.getEnd().getLocWithOffset(-1));

            if (this->source_manager->getSpellingLineNumber(this->opens.back()) !=
                this->source_manager->getSpellingLineNumber(
                    MatchedDecl->getLocation())) {
                diag(this->opens.back(), "Open brace must be located "
                                         "on same line as record.");
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        CHECK_LOC(MatchedDecl);

        /* This is just a hack to set up the lexer in a known location
         */
        if (MatchedDecl->isThisDeclarationADefinition() &&
            MatchedDecl->doesThisDeclarationHaveABody()) {
            size_t start, end;
            SourceLocation StartBrace = MatchedDecl->getBody()->getBeginLoc();
            SourceLocation EndBrace = MatchedDecl->getBodyRBrace();
            LOG_OPEN("function", StartBrace);
            this->opens.push_back(StartBrace);
            LOG_CLOSE("function", EndBrace.getLocWithOffset(-1));
            this->closes.push_back(EndBrace.getLocWithOffset(-1));
            if (MatchedDecl->getNumParams() > 1 &&
                ((start = this->source_manager->getSpellingLineNumber(
                      MatchedDecl->getLocation()))) !=
                    (end = this->source_manager->getSpellingLineNumber(
                         MatchedDecl->getParamDecl(MatchedDecl->getNumParams() - 1)
                             ->getEndLoc()))) {
                for (size_t i = start + 1; i <= end; i++) {
                    this->broken_lines.push_back(i);
                }
            }

            if ((MatchedDecl->getNumParams() == 0 &&
                 this->source_manager->getSpellingLineNumber(this->opens.back()) !=
                     this->source_manager->getSpellingLineNumber(
                         MatchedDecl->getLocation())) ||
                (MatchedDecl->getNumParams() > 0 &&
                 this->source_manager->getSpellingLineNumber(
                     MatchedDecl->getParamDecl(MatchedDecl->getNumParams() - 1)
                         ->getEndLoc()) !=
                     this->source_manager->getSpellingLineNumber(this->opens.back()))) {

                diag(this->opens.back(),
                     "Open brace on line %0 must be located on same "
                     "line "
                     "as function "
                     "declaration or after parameters on line %1.")
                    << this->source_manager->getSpellingLineNumber(this->opens.back())
                    << this->source_manager->getSpellingLineNumber(
                           MatchedDecl->getParametersSourceRange().getEnd());
            }
        }

        SourceRange param_range = MatchedDecl->getParametersSourceRange();
        if (this->source_manager->getSpellingLineNumber(param_range.getBegin()) !=
            this->source_manager->getSpellingLineNumber(param_range.getEnd())) {
            this->broken_ranges.push_back(param_range);
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("do")) {
        CHECK_LOC(MatchedDecl);

        LOG_OPEN("do", MatchedDecl->getBody()->getBeginLoc());
        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
        LOG_CLOSE("do", MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));
        this->closes.push_back(
            MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));

        if (this->source_manager->getSpellingLineNumber(this->opens.back()) !=
            this->source_manager->getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
            diag(this->opens.back(), "Open brace must be located on same line as do.");
        }
        SourceRange condition_range = SourceRange(MatchedDecl->getCond()->getBeginLoc(),
                                                  MatchedDecl->getCond()->getEndLoc());
        if (this->source_manager->getSpellingLineNumber(condition_range.getBegin()) !=
            this->source_manager->getSpellingLineNumber(condition_range.getEnd())) {
            this->broken_ranges.push_back(condition_range);
        }

    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
        CHECK_LOC(MatchedDecl);

        size_t start, end;
        LOG_OPEN("for", MatchedDecl->getBody()->getBeginLoc());
        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
        LOG_CLOSE("for", MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));
        this->closes.push_back(
            MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));

        if ((start = this->source_manager->getSpellingLineNumber(
                 MatchedDecl->getLParenLoc())) !=
            (end = this->source_manager->getSpellingLineNumber(
                 MatchedDecl->getRParenLoc()))) {
            for (size_t i = start + 1; i <= end; i++) {
                this->broken_lines.push_back(i);
            }
        }
        if (this->source_manager->getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
            this->source_manager->getSpellingLineNumber(this->opens.back())) {
            diag(this->opens.back(), "Open brace must be located on "
                                     "same line as for or after "
                                     "split contents.");
        }
        SourceRange condition_range =
            SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc());
        if (this->source_manager->getSpellingLineNumber(condition_range.getBegin()) !=
            this->source_manager->getSpellingLineNumber(condition_range.getEnd())) {
            this->broken_ranges.push_back(condition_range);
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
        CHECK_LOC(MatchedDecl)
        size_t start, end;
        const IfStmt *If = MatchedDecl;
        const Stmt *Else = MatchedDecl->getElse();

        SourceLocation StartIf = If->getThen()->getBeginLoc();
        SourceLocation EndIf = If->getThen()->getEndLoc();

        LOG_OPEN("if", StartIf);
        this->opens.push_back(StartIf);
        LOG_CLOSE("if", EndIf.getLocWithOffset(-1));
        this->closes.push_back(EndIf.getLocWithOffset(-1));

        if (this->source_manager->getSpellingLineNumber(this->opens.back()) !=
            this->source_manager->getSpellingLineNumber(MatchedDecl->getRParenLoc())) {
            diag(this->opens.back(), "Open brace must be located on same line as if.");
        }

        if ((start = this->source_manager->getSpellingLineNumber(
                 MatchedDecl->getLParenLoc())) !=
            (end = this->source_manager->getSpellingLineNumber(
                 MatchedDecl->getRParenLoc()))) {
            for (size_t i = start + 1; i <= end; i++) {
                this->broken_lines.push_back(i);
            }
        }

        if (Else) {
            if (this->source_manager->getSpellingLineNumber(
                    If->getThen()->getEndLoc()) !=
                this->source_manager->getSpellingLineNumber(Else->getBeginLoc()) - 1) {
                diag(Else->getBeginLoc().getLocWithOffset(-1),
                     "Else must be on the line after the associated "
                     "'if' "
                     "statement's closing brace.");
            }
            if (const auto *ChildIf = dyn_cast<IfStmt>(Else)) {
                SourceLocation StartElse = ChildIf->getThen()->getBeginLoc();

                if (this->source_manager->getSpellingLineNumber(StartElse) !=
                    this->source_manager->getSpellingLineNumber(
                        ChildIf->getRParenLoc())) {
                    diag(this->opens.back(),
                         "Open brace must be located on same line as "
                         "else.");
                }
            } else {
                LOG_OPEN("else", Else->getBeginLoc());
                this->opens.push_back(Else->getBeginLoc());
                LOG_CLOSE("else", Else->getEndLoc().getLocWithOffset(-1));
                this->closes.push_back(Else->getEndLoc().getLocWithOffset(-1));
            }
        }

    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
        CHECK_LOC(MatchedDecl);

        this->dbgdump(MatchedDecl, *this->ast_context);

        // Add an open/close for the switch itself
        LOG_OPEN("switch", MatchedDecl->getBeginLoc());
        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
        // Don't really need a close because it's a compound anyway

        // Add an open/close for each case
        std::deque<const Stmt *> switch_children(MatchedDecl->getBody()->child_begin(),
                                                 MatchedDecl->getBody()->child_end());
        const Stmt *first_child = switch_children.front();

        while (!switch_children.empty()) {
            auto child = switch_children.front();
            switch_children.pop_front();
            if (auto _case = dyn_cast<SwitchCase>(child)) {
                LOG_OPEN("case", _case->getColonLoc().getLocWithOffset(-1));
                this->opens.push_back(_case->getColonLoc().getLocWithOffset(1));
                if (child != first_child) {
                    LOG_CLOSE("case", _case->getKeywordLoc().getLocWithOffset(-1));
                    this->closes.push_back(_case->getKeywordLoc().getLocWithOffset(-1));
                }
                for (auto case_child : _case->children()) {
                    switch_children.push_back(case_child);
                }
            }
        }

        // Add the close for the last entry in the switch.
        LOG_CLOSE("switch", switch_children.back()->getEndLoc());
        this->closes.push_back(switch_children.back()->getEndLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while")) {
        CHECK_LOC(MatchedDecl);
        size_t start, end;
        LOG_OPEN("while", MatchedDecl->getBody()->getBeginLoc());
        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());

        LOG_CLOSE("while", MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));
        this->closes.push_back(
            MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));

        if (this->source_manager->getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
            this->source_manager->getSpellingLineNumber(this->opens.back())) {
            diag(this->opens.back(),
                 "Open brace must be located on same line as while.");
        }
        if ((start = this->source_manager->getSpellingLineNumber(
                 MatchedDecl->getLParenLoc())) !=
            (end = this->source_manager->getSpellingLineNumber(
                 MatchedDecl->getRParenLoc()))) {
            for (size_t i = start + 1; i <= end; i++) {
                this->broken_lines.push_back(i);
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<CompoundStmt>("compound")) {
        CHECK_LOC(MatchedDecl);
        ParentMapContext &PMC =
            const_cast<ASTContext *>(this->ast_context)->getParentMapContext();
        DynTypedNode node = DynTypedNode::create<CompoundStmt>(*MatchedDecl);
        DynTypedNodeList Parents = PMC.getParents(node);
        bool is_case = false;
        for (auto it = Parents.begin(); it != Parents.end(); it++) {
            // If the parent is a case, this is handled there...
            if (it->getNodeKind().asStringRef().str() == "CaseStmt") {
                is_case = true;
            }
        }

        LOG_OPEN("compound", MatchedDecl->getLBracLoc());
        this->opens.push_back(MatchedDecl->getLBracLoc());
        LOG_CLOSE("compound", MatchedDecl->getRBracLoc().getLocWithOffset(-1));
        this->closes.push_back(MatchedDecl->getRBracLoc().getLocWithOffset(-1));

        if (is_case) {
            // Little trick to allow this:
            /*
               case 1: {
                 something...
               }
                 break;
               case 2: ...
             */
            LOG_OPEN("compound-case", MatchedDecl->getRBracLoc().getLocWithOffset(1));
            this->opens.push_back(MatchedDecl->getRBracLoc().getLocWithOffset(1));
        }
    }
}

static size_t spc_ct(std::string s) {
    size_t ct = 0;
    for (auto c : s) {
        if (c == ' ') {
            ct++;
        }
    }
    return ct;
}

void Rule4aCheck::onEndOfTranslationUnit(void) {
    size_t indent_amount = 0;
    std::vector<Token> checked_tokens;
    std::vector<Token> eol_tokens;

    std::deque<size_t> open_lines;
    std::deque<size_t> close_lines;

    for (SourceLocation loc : this->opens) {
        size_t line = this->source_manager->getSpellingLineNumber(loc);
        open_lines.push_back(line);
    }
    for (SourceLocation loc : this->closes) {
        size_t line = this->source_manager->getSpellingLineNumber(loc);
        close_lines.push_back(line);
    }

    // Sort open and close locations and remove duplicates
    std::sort(opens.begin(), opens.end());
    std::sort(open_lines.begin(), open_lines.end());
    opens.erase(std::unique(opens.begin(), opens.end()), opens.end());
    std::sort(closes.begin(), closes.end());
    std::sort(close_lines.begin(), close_lines.end());
    closes.erase(std::unique(closes.begin(), closes.end()), closes.end());

    this->dout() << "Checking indentation over " << this->tokens.size() << " tokens"
                 << std::endl;

    for (size_t i = 0; i < this->tokens.size(); i++) {
        auto tok = this->tokens.at(i);

        if (not this->source_manager->isWrittenInMainFile(tok.getLocation()) ||
            not this->source_manager->isWrittenInMainFile(tok.getEndLoc())) {
            continue;
        }

        std::string raw_tok_data = Lexer::getSpelling(tok, *this->source_manager,
                                                      this->ast_context->getLangOpts());
        size_t tok_line_number =
            this->source_manager->getSpellingLineNumber(tok.getLocation());

        while (open_lines.front() < tok_line_number) {
            open_lines.pop_front();
            indent_amount += INDENT_AMOUNT;
        }

        while (close_lines.front() < tok_line_number) {
            close_lines.pop_front();
            indent_amount -= INDENT_AMOUNT;
        }

        std::string ws("");
        if (i > 0) {
            ws = *this->sourcerange_string(
                *this->source_manager,
                SourceRange(this->tokens.at(i - 1).getLocation(), tok.getLocation()));
        }

        if (raw_tok_data.find('\n') != std::string::npos) {
            if (i > 0) {
                eol_tokens.push_back(this->tokens.at(i - 1));
            }
        }

        bool breakable = false;

        if (tok.isAtStartOfLine()) {
            if (this->tok_string(*this->source_manager, tok)->rfind("#", 0) == 0) {
                // This is a preprocessor directive, so it must be on
                // the left edge.
                if (spc_ct(ws) != 0) {
                    diag(tok.getLocation(),
                         "Incorrect indentation level. Preprocessor "
                         "directives should not be indented. Expected "
                         "%0, got %1")
                        << 0 << spc_ct(ws);
                }
            } else if (spc_ct(ws) != indent_amount) {
                if (std::find(this->broken_lines.begin(), this->broken_lines.end(),
                              this->source_manager->getSpellingLineNumber(
                                  tok.getLocation())) != this->broken_lines.end()) {
                    if (spc_ct(ws) < indent_amount + 2) {
                        diag(tok.getLocation(),
                             "Incorrect indentation level. Expected at "
                             "least %0, got %1")
                            << std::to_string(indent_amount + 2)
                            << std::to_string(spc_ct(ws));
                    }
                    // Is the end of the previous line something other
                    // than:
                    // - ';'
                    // - '{'
                    // - A comment
                    // - A macro define, ifndef, endif, include
                } else if (breakable) {
                    if (spc_ct(ws) < indent_amount + 2) {
                        diag(tok.getLocation(),
                             "Incorrect indentation level for broken "
                             "line. "
                             "Expected at "
                             "least %0, got %1")
                            << std::to_string(indent_amount + 2)
                            << std::to_string(spc_ct(ws));
                    }

                } else if (tok.getKind() == tok::comment) {
                    if (spc_ct(ws) < indent_amount) {
                        diag(tok.getLocation(),
                             "Incorrect indentation level for comment. "
                             "Expected at "
                             "least %0, got %1")
                            << std::to_string(indent_amount + 2)
                            << std::to_string(spc_ct(ws));
                    }
                } else {
                    diag(tok.getLocation(), "Incorrect indentation level. Expected %0, "
                                            "got %1")
                        << std::to_string(indent_amount) << std::to_string(spc_ct(ws));
                }
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

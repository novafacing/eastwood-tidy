//===--- Rule4aCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule4aCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/Decl.h"
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

static bool hasCompoundChildRec(const Stmt *stmt) {
    if (auto compound = dyn_cast<CompoundStmt>(stmt)) {
        return true;
    }

    for (auto child : stmt->children()) {
        return hasCompoundChildRec(child);
    }

    if (stmt->children().empty()) {
        return false;
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

    /* Matchers for broken lines:
     * - varDecl
     * - functionDecl->getParametersSourceRange
     * - callExpr->getRParenLoc, callExpr->getBeginLoc
     * - doStmt->getWhileLoc, doStmt->getRParenLoc
     * - forStmt->getLParenLoc, forStmt->getRParenLoc
     * - ifStmt->getLParenLoc, ifStmt->getRParenLoc
     * - parenExpr->getLParen, parenExpr->getRParen
     * - parenListExpr->getLParenLoc, parenListExpr->getRParenLoc
     * - switchStmt->getLParenLoc, switchStmt->getRParenLoc
     * - whileStmt->getLParenLoc, whileStmt->getRParenLoc
     * - stmt without compoundstmt child
     */
    Finder->addMatcher(functionDecl().bind("functionSplit"), this);
    Finder->addMatcher(callExpr().bind("callSplit"), this);
    Finder->addMatcher(doStmt().bind("doSplit"), this);
    Finder->addMatcher(forStmt().bind("forSplit"), this);
    Finder->addMatcher(ifStmt().bind("ifSplit"), this);
    Finder->addMatcher(switchStmt().bind("switchSplit"), this);
    Finder->addMatcher(parenExpr().bind("parenSplit"), this);
    Finder->addMatcher(parenListExpr().bind("parenListSplit"), this);
    Finder->addMatcher(switchStmt().bind("switchSplit"), this);
    Finder->addMatcher(whileStmt().bind("whileSplit"), this);
    Finder->addMatcher(stmt().bind("stmtSplit"), this);
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

            LOG_CLOSE("record", BraceRange.getEnd());
            this->closes.push_back(BraceRange.getEnd());

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

        if (MatchedDecl->isThisDeclarationADefinition() &&
            MatchedDecl->doesThisDeclarationHaveABody()) {
            size_t start, end;
            SourceLocation StartBrace = MatchedDecl->getBody()->getBeginLoc();
            SourceLocation EndBrace = MatchedDecl->getBodyRBrace();
            LOG_OPEN("function", StartBrace);
            this->opens.push_back(StartBrace);
            LOG_CLOSE("function", EndBrace);
            this->closes.push_back(EndBrace);
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
                 this->source_manager->getSpellingLineNumber(StartBrace) !=
                     this->source_manager->getSpellingLineNumber(
                         MatchedDecl->getLocation())) ||
                (MatchedDecl->getNumParams() > 0 &&
                 this->source_manager->getSpellingLineNumber(
                     MatchedDecl->getParamDecl(MatchedDecl->getNumParams() - 1)
                         ->getEndLoc()) !=
                     this->source_manager->getSpellingLineNumber(StartBrace))) {

                diag(StartBrace, "Open brace on line %0 must be located on same "
                                 "line "
                                 "as function "
                                 "declaration or after parameters on line %1.")
                    << this->source_manager->getSpellingLineNumber(StartBrace)
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

        // Add an open/close for the switch itself
        LOG_OPEN("switch", MatchedDecl->getBody()->getBeginLoc());
        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
        // Don't really need a close because it's a compound anyway
        LOG_CLOSE("switch", MatchedDecl->getBody()->getEndLoc());
        this->closes.push_back(MatchedDecl->getBody()->getEndLoc());

        // Add an open/close for each case
        auto switch_children = MatchedDecl->getSwitchCaseList();
        while (switch_children) {
            auto child = switch_children;
            LOG_OPEN("case", child->getColonLoc().getLocWithOffset(1));
            this->opens.push_back(child->getColonLoc().getLocWithOffset(1));

            switch_children = switch_children->getNextSwitchCase();
            if (switch_children) {
                LOG_CLOSE("case", child->getKeywordLoc().getLocWithOffset(-1));
                this->closes.push_back(child->getKeywordLoc().getLocWithOffset(-1));
            }
        }
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
        bool is_last = false;
        bool is_ignore = false;
        const CaseStmt *case_stmt = nullptr;
        const SwitchStmt *switch_stmt = nullptr;
        for (auto it = Parents.begin(); it != Parents.end(); it++) {
            // If the parent is a case, this is handled there...
            if (it->getNodeKind().asStringRef().str() == "CaseStmt") {
                is_case = true;
                case_stmt = it->get<CaseStmt>();
                this->dout() << "Got node as case_stmt: " << case_stmt << std::endl;

            } else {
                auto ptype = it->getNodeKind().asStringRef().str();
                if (ptype == "FunctionDecl" || ptype == "RecordDecl" ||
                    ptype == "EnumDecl" || ptype == "IfStmt" || ptype == "ForStmt" ||
                    ptype == "DoStmt" || ptype == "WhileStmt") {
                    is_ignore = true;
                }
            }
        }
        if (is_ignore) {
            return;
        }
        if (case_stmt) {
            DynTypedNode parents_node = DynTypedNode::create<CaseStmt>(*case_stmt);
            while (!switch_stmt) {
                DynTypedNodeList parents = PMC.getParents(parents_node);
                for (auto it = parents.begin(); it != parents.end(); it++) {
                    if (it->getNodeKind().asStringRef().str() == "SwitchStmt") {
                        switch_stmt = it->get<SwitchStmt>();
                        const SwitchCase *last_case = switch_stmt->getSwitchCaseList();
                        if (last_case) {
                            this->dout() << "This source range: "
                                         << *this->sourcerange_string(
                                                *this->source_manager,
                                                case_stmt->getSourceRange())
                                         << std::endl;
                            this->dout() << "Last source range: "
                                         << *this->sourcerange_string(
                                                *this->source_manager,
                                                last_case->getSourceRange())
                                         << std::endl;
                            is_last = case_stmt->getSourceRange() ==
                                      last_case->getSourceRange();
                            this->dout() << "Got switch stmt. This casestmt is last?: "
                                         << is_last << std::endl;
                        }
                    } else {
                        parents_node = *it;
                    }
                }
            }
        }

        if (is_case && !is_last) {
            // Little trick to allow this:
            /*
               case 1: {
                 something...
               }
                 break;
               case 2: ...
             */
            // However we need to avoid adding the open for the last case statement, so
            // if this does not have any children after the compoundstmt, we don't add
            // it...

            LOG_CLOSE("compound-case", MatchedDecl->getRBracLoc().getLocWithOffset(-1));
            this->closes.push_back(MatchedDecl->getRBracLoc().getLocWithOffset(-1));
            LOG_OPEN("compound-case", MatchedDecl->getRBracLoc().getLocWithOffset(1));
            this->opens.push_back(MatchedDecl->getRBracLoc().getLocWithOffset(1));
        } else if (is_case) {
            LOG_CLOSE("compound-case", MatchedDecl->getRBracLoc().getLocWithOffset(-1));
            this->closes.push_back(MatchedDecl->getRBracLoc().getLocWithOffset(-1));

        } else {
            LOG_OPEN("compound", MatchedDecl->getLBracLoc());
            this->opens.push_back(MatchedDecl->getLBracLoc());
            LOG_CLOSE("compound", MatchedDecl->getRBracLoc().getLocWithOffset(-1));
            this->closes.push_back(MatchedDecl->getRBracLoc().getLocWithOffset(-1));
        }
    }

    /* Matchers for broken lines:
     * - varDecl
     * - functionDecl->getParametersSourceRange
     * - callExpr->getRParenLoc, callExpr->getBeginLoc
     * - doStmt->getWhileLoc, doStmt->getRParenLoc
     * - forStmt->getLParenLoc, forStmt->getRParenLoc
     * - ifStmt->getLParenLoc, ifStmt->getRParenLoc
     * - parenExpr->getLParen, parenExpr->getRParen
     * - parenListExpr->getLParenLoc, parenListExpr->getRParenLoc
     * - switchStmt->getLParenLoc, switchStmt->getRParenLoc
     * - whileStmt->getLParenLoc, whileStmt->getRParenLoc
     * - stmt without compoundstmt child
     */
    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("functionSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(MatchedDecl->getParametersSourceRange());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<CallExpr>("callSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(
            SourceRange(MatchedDecl->getBeginLoc(), MatchedDecl->getRParenLoc()));
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("doSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(
            SourceRange(MatchedDecl->getWhileLoc(), MatchedDecl->getRParenLoc()));
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("forSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(
            SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc()));
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("ifSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(
            SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc()));
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ParenExpr>("parenSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(
            SourceRange(MatchedDecl->getLParen(), MatchedDecl->getRParen()));
    } else if (auto MatchedDecl =
                   Result.Nodes.getNodeAs<ParenListExpr>("parenListSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(
            SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc()));
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switchSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(
            SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc()));
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("whileSplit")) {
        CHECK_LOC(MatchedDecl);
        this->broken_ranges.push_back(
            SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc()));
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<Stmt>("stmtSplit")) {
        CHECK_LOC(MatchedDecl);
        if (!hasCompoundChildRec(MatchedDecl)) {
            this->broken_ranges.push_back(MatchedDecl->getSourceRange());
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

    std::vector<size_t> open_lines;
    std::vector<size_t> close_lines;

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
    open_lines.erase(std::unique(open_lines.begin(), open_lines.end()),
                     open_lines.end());

    std::sort(closes.begin(), closes.end());
    std::sort(close_lines.begin(), close_lines.end());
    closes.erase(std::unique(closes.begin(), closes.end()), closes.end());
    close_lines.erase(std::unique(close_lines.begin(), close_lines.end()),
                      close_lines.end());

    for (auto o : open_lines) {
        this->dout() << "Open after line " << o << std::endl;
    }
    for (auto o : close_lines) {
        this->dout() << "Close before line " << o << std::endl;
    }

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

        this->dout() << "Token: " << raw_tok_data << " at line " << tok_line_number
                     << std::endl;

        while (open_lines.front() < tok_line_number && !open_lines.empty()) {
            size_t open_line = open_lines.front();
            open_lines.erase(open_lines.begin());
            indent_amount += INDENT_AMOUNT;
            this->dout() << "Opening at line " << open_line << " with indent "
                         << indent_amount << std::endl;
        }

        while (close_lines.front() <= tok_line_number && !close_lines.empty()) {
            size_t close_line = close_lines.front();
            close_lines.erase(close_lines.begin());
            indent_amount -= INDENT_AMOUNT;
            this->dout() << "Closing at line " << close_line << " with indent "
                         << indent_amount << std::endl;
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

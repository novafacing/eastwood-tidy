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
    if (dyn_cast<WhileStmt>(stmt) || dyn_cast<DoStmt>(stmt) ||
        dyn_cast<ForStmt>(stmt) || dyn_cast<IfStmt>(stmt) ||
        dyn_cast<SwitchStmt>(stmt)) {
        return true;
    }
    if (auto compound = dyn_cast<CompoundStmt>(stmt)) {
        return true;
    }

    for (auto child : stmt->children()) {
        return hasCompoundChildRec(child);
    }

    return false;
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

            if (this->source_manager->getSpellingLineNumber(BraceRange.getBegin()) !=
                this->source_manager->getSpellingLineNumber(
                    MatchedDecl->getLocation())) {
                diag(BraceRange.getBegin(), "Open brace must be located "
                                            "on same line as record.");
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
        CHECK_LOC(MatchedDecl);

        if (MatchedDecl->isCompleteDefinition()) {
            SourceRange BraceRange = MatchedDecl->getBraceRange();

            LOG_OPEN("enum", BraceRange.getBegin());
            this->opens.push_back(BraceRange.getBegin());
            LOG_CLOSE("enum", BraceRange.getEnd());
            this->closes.push_back(BraceRange.getEnd());

            if (this->source_manager->getSpellingLineNumber(BraceRange.getBegin()) !=
                this->source_manager->getSpellingLineNumber(
                    MatchedDecl->getLocation())) {
                diag(BraceRange.getBegin(), "Open brace must be located "
                                            "on same line as record.");
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
        CHECK_LOC(MatchedDecl);

        if (MatchedDecl->isThisDeclarationADefinition() &&
            MatchedDecl->doesThisDeclarationHaveABody()) {
            SourceLocation StartBrace = MatchedDecl->getBody()->getBeginLoc();

            if ( // No parameters and the function decl isn't on the same line as the
                 // open brace
                (MatchedDecl->getNumParams() == 0 &&
                 this->source_manager->getSpellingLineNumber(StartBrace) !=
                     this->source_manager->getSpellingLineNumber(
                         MatchedDecl->getLocation())) ||
                // There are parameters and the open brace isn't on the same line as the
                // last parameter
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

    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("do")) {
        CHECK_LOC(MatchedDecl);

        if (this->source_manager->getSpellingLineNumber(
                MatchedDecl->getBody()->getBeginLoc()) !=
            this->source_manager->getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
            diag(MatchedDecl->getBody()->getBeginLoc(),
                 "Open brace must be located on same line as do.");
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
        CHECK_LOC(MatchedDecl);

        if (this->source_manager->getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
            this->source_manager->getSpellingLineNumber(
                MatchedDecl->getBody()->getBeginLoc())) {
            diag(MatchedDecl->getBody()->getBeginLoc(), "Open brace must be located on "
                                                        "same line as for or after "
                                                        "split contents.");
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
        CHECK_LOC(MatchedDecl)
        const IfStmt *If = MatchedDecl;
        const Stmt *Else = MatchedDecl->getElse();

        if (this->source_manager->getSpellingLineNumber(If->getThen()->getBeginLoc()) !=
            this->source_manager->getSpellingLineNumber(MatchedDecl->getRParenLoc())) {
            diag(this->opens.back(), "Open brace must be located on same line as if.");
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
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
        CHECK_LOC(MatchedDecl);
        this->dbgdump(MatchedDecl, *this->ast_context);

        // Add an open/close for each case
        auto switch_children = MatchedDecl->getSwitchCaseList();
        Stmt *last_child = nullptr;

        for (const Stmt *child : switch_children->children()) {
            last_child = const_cast<Stmt *>(child);
        }

        /* we actually always want to dedent because if we have:
        case 1:
          a = 1;
          {
            break;
          }
        we still need to dedent from the final compoundstmt child of the case stmt
        */
        LOG_CLOSE("case", MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));
        this->closes.push_back(
            MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));

        // This iterates from bottom to top, and we only close the switch case if it
        // is not the first switch case because we are closing *before* the case not
        // after
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

        if (this->source_manager->getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
            this->source_manager->getSpellingLineNumber(
                MatchedDecl->getBody()->getBeginLoc())) {
            diag(this->opens.back(),
                 "Open brace must be located on same line as while.");
        }

    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<CompoundStmt>("compound")) {
        CHECK_LOC(MatchedDecl);
        ParentMapContext &PMC =
            const_cast<ASTContext *>(this->ast_context)->getParentMapContext();
        DynTypedNode node = DynTypedNode::create<CompoundStmt>(*MatchedDecl);
        DynTypedNodeList Parents = PMC.getParents(node);
        const SwitchCase *case_stmt = nullptr;
        const SwitchStmt *switch_stmt = nullptr;

        for (auto it = Parents.begin(); it != Parents.end(); it++) {
            // If the parent is a case, this is handled there...
            if (it->getNodeKind().asStringRef().str() == "CaseStmt") {
                case_stmt = it->get<SwitchCase>();
                this->dout() << "Got node as case_stmt: " << case_stmt << std::endl;
            } else if (it->getNodeKind().asStringRef().str() == "DefaultStmt") {
                case_stmt = it->get<SwitchCase>();
                this->dout() << "Got node as case_stmt: " << case_stmt << std::endl;
            }
        }

        if (case_stmt) {
            size_t child_num = 0;
            for (auto case_child : case_stmt->children()) {
                if (auto case_child_compound = dyn_cast<CompoundStmt>(case_child)) {
                    // 1 because the compound will be the first child of the case
                    // *after* the case constexpr
                    if (child_num == 1 &&
                        this->source_manager->getSpellingLineNumber(
                            case_stmt->getColonLoc()) ==
                            this->source_manager->getSpellingLineNumber(
                                case_child_compound->getLBracLoc())) {
                        // This case child is the compound stmt, so if it is the first
                        // thing after the case and it needs a special case of not
                        // modifying the indentation at all other than its close brace
                        this->closes.push_back(case_child_compound->getRBracLoc());
                        this->opens.push_back(case_child_compound->getRBracLoc());
                        return;
                    }
                }

                if (child_num++ >= 1) {
                    break;
                }
            }
        }

        LOG_OPEN("compound", MatchedDecl->getLBracLoc());
        this->opens.push_back(MatchedDecl->getLBracLoc());
        LOG_CLOSE("compound", MatchedDecl->getRBracLoc());
        this->closes.push_back(MatchedDecl->getRBracLoc());
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
    SourceRange *range = nullptr;
    ;
    if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("functionSplit")) {
        CHECK_LOC(MatchedDecl);

        range = new SourceRange(MatchedDecl->getParametersSourceRange());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<CallExpr>("callSplit")) {
        CHECK_LOC(MatchedDecl);
        range =
            new SourceRange(MatchedDecl->getBeginLoc(), MatchedDecl->getRParenLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("doSplit")) {
        CHECK_LOC(MatchedDecl);
        range =
            new SourceRange(MatchedDecl->getWhileLoc(), MatchedDecl->getRParenLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("forSplit")) {
        CHECK_LOC(MatchedDecl);
        range =
            new SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("ifSplit")) {
        CHECK_LOC(MatchedDecl);
        range =
            new SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ParenExpr>("parenSplit")) {
        CHECK_LOC(MatchedDecl);
        range = new SourceRange(MatchedDecl->getLParen(), MatchedDecl->getRParen());
    } else if (auto MatchedDecl =
                   Result.Nodes.getNodeAs<ParenListExpr>("parenListSplit")) {
        CHECK_LOC(MatchedDecl);
        range =
            new SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switchSplit")) {
        CHECK_LOC(MatchedDecl);
        range =
            new SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("whileSplit")) {
        CHECK_LOC(MatchedDecl);
        range =
            new SourceRange(MatchedDecl->getLParenLoc(), MatchedDecl->getRParenLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<Stmt>("stmtSplit")) {
        CHECK_LOC(MatchedDecl);
        if (!hasCompoundChildRec(MatchedDecl)) {
            range = new SourceRange(MatchedDecl->getSourceRange());
        }
    }

    if (range != nullptr) {
        if (this->source_manager->isWrittenInMainFile(range->getBegin()) &&
            this->source_manager->isWrittenInMainFile(range->getEnd()) &&
            this->source_manager->getSpellingLineNumber(range->getBegin()) !=
                this->source_manager->getSpellingLineNumber(range->getEnd())) {
            this->dout() << "Adding range: " << std::endl;
            this->broken_ranges.push_back(*range);
        }
        delete range;
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

    std::sort(open_lines.begin(), open_lines.end());

    std::sort(close_lines.begin(), close_lines.end());

    std::sort(this->broken_ranges.begin(), this->broken_ranges.end(),
              [this](SourceRange a, SourceRange b) {
                  return this->source_manager->getSpellingLineNumber(a.getBegin()) <
                         this->source_manager->getSpellingLineNumber(b.getBegin());
              });

    this->broken_ranges.erase(
        std::unique(
            this->broken_ranges.begin(), this->broken_ranges.end(),
            [this](SourceRange a, SourceRange b) {
                return this->source_manager->getSpellingLineNumber(a.getBegin()) ==
                           this->source_manager->getSpellingLineNumber(b.getBegin()) &&
                       this->source_manager->getSpellingLineNumber(a.getEnd()) ==
                           this->source_manager->getSpellingLineNumber(b.getEnd());
            }),
        this->broken_ranges.end());

    for (auto o : open_lines) {
        this->dout() << "Open after line " << o << std::endl;
    }

    for (auto o : close_lines) {
        this->dout() << "Close before line " << o << std::endl;
    }

    for (auto b : broken_ranges) {
        this->dout() << "Broken range: ("
                     << this->source_manager->getSpellingLineNumber(b.getBegin())
                     << ", " << this->source_manager->getSpellingLineNumber(b.getEnd())
                     << ")" << std::endl;
    }

    this->dout() << "Checking indentation over " << this->tokens.size() << " tokens"
                 << std::endl;

    for (size_t i = 0; i < this->tokens.size(); i++) {
        auto tok = this->tokens.at(i);
        bool breakable = false;

        if (!this->source_manager->isWrittenInMainFile(tok.getLocation()) ||
            !this->source_manager->isWrittenInMainFile(tok.getEndLoc())) {
            continue;
        }

        std::string raw_tok_data = Lexer::getSpelling(tok, *this->source_manager,
                                                      this->ast_context->getLangOpts());
        size_t tok_line_number =
            this->source_manager->getSpellingLineNumber(tok.getLocation());

        // Check if we're inside a broken range (but not on the first line of a range)
        for (auto r : broken_ranges) {
            if (this->source_manager->isBeforeInTranslationUnit(r.getBegin(),
                                                                tok.getLocation()) &&
                this->source_manager->isBeforeInTranslationUnit(tok.getLocation(),
                                                                r.getEnd())) {
                this->dout() << "Token " << raw_tok_data << " on line "
                             << tok_line_number << " is in broken range" << std::endl;
                breakable = true;
                break;
            }
        }

        // Basically, set an open on the line before the next indented line
        // and set a close on the line that needs to be dedented
        while (open_lines.front() < tok_line_number && !open_lines.empty()) {
            size_t open_line = open_lines.front();
            open_lines.erase(open_lines.begin());
            indent_amount += INDENT_AMOUNT;
            this->dout() << "Opening at line " << open_line << " with indent "
                         << indent_amount << ":" << raw_tok_data << std::endl;
        }

        while (close_lines.front() <= tok_line_number && !close_lines.empty()) {
            size_t close_line = close_lines.front();
            close_lines.erase(close_lines.begin());
            indent_amount -= INDENT_AMOUNT;
            this->dout() << "Closing at line " << close_line << " with indent "
                         << indent_amount << ":" << raw_tok_data << std::endl;
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

        if (tok.isAtStartOfLine()) {
            if (this->tok_string(*this->source_manager, tok)->rfind("#", 0) == 0) {
                // This is a preprocessor directive, so it must be on
                // the left edge. This is checked by rule 3.D
                if (spc_ct(ws) != 0) {
                    // diag(tok.getLocation(),
                    //      "Incorrect indentation level. Preprocessor "
                    //      "directives should not be indented. Expected "
                    //      "%0, got %1")
                    //     << 0 << spc_ct(ws);
                }
            } else if (breakable) {
                // This error falls under 2.A and is displayed in that check
                if (spc_ct(ws) != indent_amount + INDENT_AMOUNT) {
                    // diag(tok.getLocation(), "Incorrect indentation level for broken "
                    //                         "line. Expected %0, got %1")
                    //     << indent_amount + INDENT_AMOUNT << spc_ct(ws);
                }
            } else if (spc_ct(ws) != indent_amount) {
                diag(tok.getLocation(),
                     "Incorrect indentation level. Expected %0, got %1")
                    << indent_amount << spc_ct(ws);
            } else if (tok.getKind() == tok::comment) {
                if (spc_ct(ws) < indent_amount) {
                    diag(tok.getLocation(), "Incorrect indentation level for comment. "
                                            "Expected at "
                                            "least %0, got %1")
                        << std::to_string(indent_amount + 2)
                        << std::to_string(spc_ct(ws));
                }
            }
        }
    }
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

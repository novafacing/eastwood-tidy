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
#include "clang/Lex/Lexer.h"

#include <iomanip>
#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {

Rule4aCheck::Rule4aCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name), indent_level(0),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}
void Rule4aCheck::registerMatchers(MatchFinder *Finder) {
    Finder->addMatcher(stmt().bind("relex"), this);
    Finder->addMatcher(decl().bind("relex"), this);
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
    RELEX();
    const SourceManager &SM = *Result.SourceManager;
    this->SMan = Result.SourceManager;
    ASTContext *Context = Result.Context;
    this->ctx = Context;
    if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("record")) {
        CHECK_LOC(MatchedDecl);

        if (MatchedDecl->isCompleteDefinition()) {
            SourceRange BraceRange = MatchedDecl->getBraceRange();

            this->opens.push_back(BraceRange.getBegin());
            this->closes.push_back(BraceRange.getEnd().getLocWithOffset(-1));

            if (SM.getSpellingLineNumber(this->opens.back()) !=
                SM.getSpellingLineNumber(MatchedDecl->getLocation())) {
                diag(this->opens.back(), "Open brace must be located "
                                         "on same line as record.");
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
        CHECK_LOC(MatchedDecl);

        if (MatchedDecl->isCompleteDefinition()) {
            SourceRange BraceRange = MatchedDecl->getBraceRange();

            this->opens.push_back(BraceRange.getBegin());
            this->closes.push_back(BraceRange.getEnd().getLocWithOffset(-1));

            if (SM.getSpellingLineNumber(this->opens.back()) !=
                SM.getSpellingLineNumber(MatchedDecl->getLocation())) {
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
            this->opens.push_back(StartBrace);
            this->closes.push_back(EndBrace.getLocWithOffset(-1));
            if (MatchedDecl->getNumParams() > 1 &&
                ((start = SM.getSpellingLineNumber(MatchedDecl->getLocation()))) !=
                    (end = SM.getSpellingLineNumber(
                         MatchedDecl->getParamDecl(MatchedDecl->getNumParams() - 1)
                             ->getEndLoc()))) {
                for (size_t i = start + 1; i <= end; i++) {
                    this->broken_lines.push_back(i);
                }
            }

            if ((MatchedDecl->getNumParams() == 0 &&
                 SM.getSpellingLineNumber(this->opens.back()) !=
                     SM.getSpellingLineNumber(MatchedDecl->getLocation())) ||
                (MatchedDecl->getNumParams() > 0 &&
                 SM.getSpellingLineNumber(
                     MatchedDecl->getParamDecl(MatchedDecl->getNumParams() - 1)
                         ->getEndLoc()) !=
                     SM.getSpellingLineNumber(this->opens.back()))) {

                diag(this->opens.back(),
                     "Open brace on line %0 must be located on same "
                     "line "
                     "as function "
                     "declaration or after parameters on line %1.")
                    << SM.getSpellingLineNumber(this->opens.back())
                    << SM.getSpellingLineNumber(
                           MatchedDecl->getParametersSourceRange().getEnd());
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("do")) {
        CHECK_LOC(MatchedDecl);

        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
        this->closes.push_back(
            MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));

        if (SM.getSpellingLineNumber(this->opens.back()) !=
            SM.getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
            diag(this->opens.back(), "Open brace must be located on same line as do.");
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
        CHECK_LOC(MatchedDecl);

        size_t start, end;
        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
        this->closes.push_back(
            MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));

        if ((start = SM.getSpellingLineNumber(MatchedDecl->getLParenLoc())) !=
            (end = SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()))) {
            for (size_t i = start + 1; i <= end; i++) {
                this->broken_lines.push_back(i);
            }
        }
        if (SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
            SM.getSpellingLineNumber(this->opens.back())) {
            diag(this->opens.back(), "Open brace must be located on "
                                     "same line as for or after "
                                     "split contents.");
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
        CHECK_LOC(MatchedDecl)
        size_t start, end;
        const IfStmt *If = MatchedDecl;
        const Stmt *Else = MatchedDecl->getElse();

        SourceLocation StartIf = If->getThen()->getBeginLoc();
        SourceLocation EndIf = If->getThen()->getEndLoc();

        this->opens.push_back(StartIf);
        this->closes.push_back(EndIf.getLocWithOffset(-1));

        if (SM.getSpellingLineNumber(this->opens.back()) !=
            SM.getSpellingLineNumber(MatchedDecl->getRParenLoc())) {
            diag(this->opens.back(), "Open brace must be located on same line as if.");
        }

        if ((start = SM.getSpellingLineNumber(MatchedDecl->getLParenLoc())) !=
            (end = SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()))) {
            for (size_t i = start + 1; i <= end; i++) {
                this->broken_lines.push_back(i);
            }
        }

        if (Else) {
            if (SM.getSpellingLineNumber(If->getThen()->getEndLoc()) !=
                SM.getSpellingLineNumber(Else->getBeginLoc()) - 1) {
                diag(Else->getBeginLoc().getLocWithOffset(-1),
                     "Else must be on the line after the associated "
                     "'if' "
                     "statement's closing brace.");
            }
            if (const auto *ChildIf = dyn_cast<IfStmt>(Else)) {
                SourceLocation StartElse = ChildIf->getThen()->getBeginLoc();
                SourceLocation EndElse = ChildIf->getThen()->getEndLoc();

                if (SM.getSpellingLineNumber(StartElse) !=
                    SM.getSpellingLineNumber(ChildIf->getRParenLoc())) {
                    diag(this->opens.back(),
                         "Open brace must be located on same line as "
                         "else.");
                }
            } else {
                this->opens.push_back(Else->getBeginLoc());
                this->closes.push_back(Else->getEndLoc().getLocWithOffset(-1));
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
        CHECK_LOC(MatchedDecl);

        // Add an open/close for the switch itself
        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
        // Don't really need a close because it's a compound anyway

        this->dbgdump(MatchedDecl, *Context);

        // Add an open/close for each case
        std::vector<const Stmt *> switch_children(MatchedDecl->getBody()->child_begin(),
                                                  MatchedDecl->getBody()->child_end());

        for (auto child = switch_children.begin(); child != switch_children.end();
             child++) {
            if (auto Case = dyn_cast<SwitchCase>(*child)) {
                this->opens.push_back(Case->getColonLoc().getLocWithOffset(1));
                if (child != switch_children.begin()) {
                    this->closes.push_back(Case->getKeywordLoc().getLocWithOffset(-1));
                }
            }
        }

        // Add the close for the last entry in the switch.
        this->closes.push_back(switch_children.back()->getEndLoc());
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while")) {
        CHECK_LOC(MatchedDecl);
        size_t start, end;
        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());

        this->closes.push_back(
            MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));

        if (SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
            SM.getSpellingLineNumber(this->opens.back())) {
            diag(this->opens.back(),
                 "Open brace must be located on same line as while.");
        }
        if ((start = SM.getSpellingLineNumber(MatchedDecl->getLParenLoc())) !=
            (end = SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()))) {
            for (size_t i = start + 1; i <= end; i++) {
                this->broken_lines.push_back(i);
            }
        }
    } else if (auto MatchedDecl = Result.Nodes.getNodeAs<CompoundStmt>("compound")) {
        CHECK_LOC(MatchedDecl);
        ParentMapContext &PMC = Context->getParentMapContext();
        DynTypedNode node = DynTypedNode::create<CompoundStmt>(*MatchedDecl);
        DynTypedNodeList Parents = PMC.getParents(node);
        bool is_case = false;
        for (auto it = Parents.begin(); it != Parents.end(); it++) {
            // If the parent is a case, this is handled there...
            if (it->getNodeKind().asStringRef().str() == "CaseStmt") {
                is_case = true;
            }
        }

        if (!is_case) {
            this->opens.push_back(MatchedDecl->getLBracLoc());
        } else {
            // Little trick to allow this:
            /*
               case 1: {
                 something...
               }
                 break;
               case 2: ...
             */
            this->opens.push_back(MatchedDecl->getRBracLoc().getLocWithOffset(1));
        }

        this->closes.push_back(MatchedDecl->getRBracLoc().getLocWithOffset(-1));
    }
}

size_t spc_ct(std::string s) {
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
    // Sort open and close locations and remove duplicates
    std::sort(opens.begin(), opens.end());
    opens.erase(std::unique(opens.begin(), opens.end()), opens.end());
    std::sort(closes.begin(), closes.end());
    closes.erase(std::unique(closes.begin(), closes.end()), closes.end());
    for (auto tok : this->tokens) {
        this->dout() << "Token: " << *this->tok_string(*this->SMan, tok) << " "
                     << tok.getLocation().printToString(*this->SMan) << "\n";
        if (not this->SMan->isWrittenInMainFile(tok.getLocation()) ||
            not this->SMan->isWrittenInMainFile(tok.getEndLoc())) {
            continue;
        }

        std::string raw_tok_data =
            Lexer::getSpelling(tok, *this->SMan, this->ctx->getLangOpts());

        // This is a line break, so we push the last token into the
        // vector of EOL tokens
        if (raw_tok_data.find('\n') != std::string::npos) {
            if (checked_tokens.size() > 0) {
                eol_tokens.push_back(checked_tokens.back());
            }
        }
        checked_tokens.push_back(tok);
        SourceRange TokenSourceRange(tok.getLocation(), tok.getEndLoc());

        while (not opens.empty() && this->SMan->isBeforeInTranslationUnit(
                                        opens.front(), tok.getLocation())) {

            opens.pop_front();
            indent_amount += 2;
            this->dout() << "++ (" + std::to_string(indent_amount) + ") |"
                         << std::string(this->SMan->getCharacterData(
                                            eol_tokens.back().getLocation()),
                                        this->SMan->getCharacterData(tok.getEndLoc()))
                         << "|" << tok.getLocation().printToString(*this->SMan)
                         << std::endl;
        }

        while (not closes.empty() && this->SMan->isBeforeInTranslationUnit(
                                         closes.front(), tok.getLocation())) {
            closes.pop_front();
            indent_amount -= 2;
            this->dout() << "-- (" + std::to_string(indent_amount) + ") |"
                         << std::string(this->SMan->getCharacterData(
                                            eol_tokens.back().getLocation()),
                                        this->SMan->getCharacterData(tok.getEndLoc()))
                         << "|" << tok.getLocation().printToString(*this->SMan)
                         << std::endl;
        }
        if (tok.isAtStartOfLine() && checked_tokens.size() > 1) {
            std::string ws(*this->tok_string(
                *this->SMan, checked_tokens.at(checked_tokens.size() - 2)));
            bool breakable = false;
            for (auto t : eol_tokens) {
                // This EOL token is the one for the line before the
                // current line
                if (this->SMan->getSpellingLineNumber(t.getLocation()) ==
                    this->SMan->getSpellingLineNumber(tok.getLocation()) - 1) {
                    if (t.getKind() != tok::l_brace && t.getKind() != tok::semi &&
                        t.getKind() != tok::comment && t.getKind() != tok::colon) {
                        breakable = true;
                    }

                    break;
                }
            }

            if (this->tok_string(*this->SMan, tok)->rfind("#", 0) == 0) {
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
                              this->SMan->getSpellingLineNumber(tok.getLocation())) !=
                    this->broken_lines.end()) {
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

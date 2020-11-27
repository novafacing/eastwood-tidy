
//===--- Rule3aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule3aCheck.h"

#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {

            tok::TokenKind getTokenKind(SourceLocation Loc, const SourceManager &SM,
                    const ASTContext *Context) {
                Token Tok;
                SourceLocation Beginning =
                    Lexer::GetBeginningOfToken(Loc, SM, Context->getLangOpts());
                const bool Invalid =
                    Lexer::getRawToken(Beginning, Tok, SM, Context->getLangOpts());
                assert(!Invalid && "Expected a valid token.");

                if (Invalid) {
                    return tok::NUM_TOKENS;
                }

                return Tok.getKind();
            }

            SourceLocation forwardSkipWhitespaceAndComments(SourceLocation Loc,
                    const SourceManager &SM,
                    const ASTContext *Context) {
                assert(Loc.isValid());
                for (;;) {
                    while (isWhitespace(*SM.getCharacterData(Loc)))
                        Loc = Loc.getLocWithOffset(1);

                    tok::TokenKind TokKind = getTokenKind(Loc, SM, Context);
                    if (TokKind != tok::comment)
                        return Loc;

                    // Fast-forward current token.
                    Loc = Lexer::getLocForEndOfToken(Loc, 0, SM, Context->getLangOpts());
                }
            }

            template<typename IfOrWhileStmt>
                SourceLocation Rule3aCheck::findRParenLoc(const IfOrWhileStmt * S,
                        const SourceManager & SM,
                        const ASTContext * Context) {
                    if (S->getBeginLoc().isMacroID()) {
                        return SourceLocation();
                    }

                    SourceLocation CondEndLoc = S->getCond()->getEndLoc();
                    if (const DeclStmt * CondVar = S->getConditionVariableDeclStmt()) {
                        CondEndLoc = CondVar->getEndLoc();
                    }

                    if (not CondEndLoc.isValid()) {
                        return SourceLocation();
                    }

                    SourceLocation PastCondEndLoc =
                        Lexer::getLocForEndOfToken(CondEndLoc, 0, SM, Context->getLangOpts());
                    if (PastCondEndLoc.isInvalid()) {
                        return SourceLocation();
                    }

                    SourceLocation RParenLoc = 
                        forwardSkipWhitespaceAndComments(PastCondEndLoc, SM, Context);

                    if (RParenLoc.isInvalid()) {
                        return SourceLocation();
                    }

                    tok::TokenKind TokKind = getTokenKind(RParenLoc, SM, Context);

                    if (TokKind != tok::r_paren) {
                        return SourceLocation();
                    }

                    return RParenLoc;
                }

            void Rule3aCheck::registerMatchers(MatchFinder * Finder) {
                Finder->addMatcher(whileStmt().bind("while"), this);
                Finder->addMatcher(forStmt().bind("for"), this);
                Finder->addMatcher(ifStmt().bind("if"), this);
                Finder->addMatcher(doStmt().bind("do"), this);
                Finder->addMatcher(switchStmt().bind("switch"), this);
            }

            void Rule3aCheck::checkStmt(const MatchFinder::MatchResult & Result,
                    const Stmt * S,
                    SourceLocation InitialLoc, SourceLocation EndLocHint) {

                if (!InitialLoc.isValid()) {
                    return;
                }

                const SourceManager & SM = *Result.SourceManager;
                const ASTContext * Context = Result.Context;

                CharSourceRange FileRange = Lexer::makeFileCharRange(
                        CharSourceRange::getTokenRange(S->getSourceRange()), SM,
                        Context->getLangOpts());

                if (FileRange.isInvalid()) {
                    return;
                }

                InitialLoc = Lexer::makeFileCharRange(
                        CharSourceRange::getCharRange(InitialLoc, S->getBeginLoc()),
                        SM, Context->getLangOpts())
                    .getBegin();

                if (InitialLoc.isInvalid()) {
                    return;
                }

            }

            void Rule3aCheck::check(const MatchFinder::MatchResult & Result) {
                const SourceManager & SM = *Result.SourceManager;
                const ASTContext * Context = Result.Context;

                SourceLocation loc;
                std::string name = "";
                std::string type = "";
                if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while")) {
                    loc = MatchedDecl->getWhileLoc();
                    type = "While";
                    SourceLocation RParenLoc = this->findRParenLoc(MatchedDecl, SM, Context);
                    if (RParenLoc.isInvalid()) {
                        return;
                    }

                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
                    loc = MatchedDecl->getForLoc();
                    type = "For";
                    SourceLocation LParenLoc = MatchedDecl->getLParenLoc();
                    SourceLocation RParenLoc = MatchedDecl->getRParenLoc();
                    size_t l_ws_ct = 0;
                    size_t r_ws_ct = 0;

                    assert(LParenLoc.isValid());
                    assert(RParenLoc.isValid());

                    /*
                       while (true) {
                       while (isWhitespace(*SM.getCharacterData(LParenLoc))) {
                       LParenLoc = Loc.getLocWithOffset(-1);
                       }
                       tok::TokenKind TokKind = getToken(LParenLoc, SM, Context);
                       if (TokKind != tok::comment) {
                       break;
                       }
                       auto PreviousTokenComment = [&Start, &Context]() {
                       Token T;
                       T = lexer::getPreviousToken(Start, Context.getSourceManager(), Context.getLangOpts());
                       return T.is(tok::comment);
                       };
                    //LParenLoc = Lexer::getLocForEndOfToken(LParenLoc, 0, SM, Context->getLangOpts());
                    while(LParenLoc.isValid() && (PreviousTokenComment() || isWhitespace(*SM.getCharacterData(
                    }
                    */
                    while (true) {
                        while (isWhitespace(*SM.getCharacterData(RParenLoc))) {
                            RParenLoc = RParenLoc.getLocWithOffset(1);
                            r_ws_ct++;
                        }

                        tok::TokenKind RPTokKind = getTokenKind(RParenLoc, SM, Context);
                        if (RPTokKind != tok::comment) {
                            break;
                        }
                        RParenLoc = Lexer::getLocForEndOfToken(RParenLoc, 0, SM, Context->getLangOpts());
                    }

                    std::cout << "Right side paren whitespace count: " << r_ws_ct << std::endl;
                    if (r_ws_ct != 1) {
                        diag(loc, "%0 statement expression is not followed by a whitespace") << type;
                    }

                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
                    loc = MatchedDecl->getIfLoc();
                    type = "If";
                    SourceLocation RParenLoc = this->findRParenLoc(MatchedDecl, SM, Context);
                    if (RParenLoc.isInvalid()) {
                        return;
                    }
                    const Stmt * Else = MatchedDecl->getElse();


                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("do")) {
                    /*
                       loc = MatchedDecl->getLocation();
                       type = "Do";
                       SourceLocation LParenLoc = MatchedDecl->getLParenLoc();
                       SourceLocation RParenLoc = MatchedDecl->getRParenLoc();
                       */
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
                    loc = MatchedDecl->getSwitchLoc();
                    type = "Switch";
                    /*
                    SourceLocation LParenLoc = MatchedDecl->getLParenLoc();
                    SourceLocation RParenLoc = MatchedDecl->getRParenLoc();
                    */

                }
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

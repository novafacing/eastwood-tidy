
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
                    Loc =
                        Lexer::getLocForEndOfToken(Loc, 0, SM, Context->getLangOpts());
                }
            }

            template <typename IfOrWhileStmt>
            SourceLocation Rule3aCheck::findRParenLoc(const IfOrWhileStmt *S,
                                                      const SourceManager &SM,
                                                      const ASTContext *Context) {
                if (S->getBeginLoc().isMacroID()) {
                    return SourceLocation();
                }

                SourceLocation CondEndLoc = S->getCond()->getEndLoc();
                if (const DeclStmt *CondVar = S->getConditionVariableDeclStmt()) {
                    CondEndLoc = CondVar->getEndLoc();
                }

                if (not CondEndLoc.isValid()) {
                    return SourceLocation();
                }

                SourceLocation PastCondEndLoc = Lexer::getLocForEndOfToken(
                    CondEndLoc, 0, SM, Context->getLangOpts());
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

            void Rule3aCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(whileStmt().bind("while"), this);
                Finder->addMatcher(forStmt().bind("for"), this);
                Finder->addMatcher(ifStmt().bind("if"), this);
                Finder->addMatcher(doStmt().bind("do"), this);
                Finder->addMatcher(switchStmt().bind("switch"), this);
            }

            void Rule3aCheck::checkStmt(const MatchFinder::MatchResult &Result,
                                        const Stmt *S, SourceLocation InitialLoc,
                                        SourceLocation EndLocHint) {

                if (!InitialLoc.isValid()) {
                    return;
                }

                const SourceManager &SM = *Result.SourceManager;
                const ASTContext *Context = Result.Context;

                CharSourceRange FileRange = Lexer::makeFileCharRange(
                    CharSourceRange::getTokenRange(S->getSourceRange()), SM,
                    Context->getLangOpts());

                if (FileRange.isInvalid()) {
                    return;
                }

                InitialLoc = Lexer::makeFileCharRange(CharSourceRange::getCharRange(
                                                          InitialLoc, S->getBeginLoc()),
                                                      SM, Context->getLangOpts())
                                 .getBegin();

                if (InitialLoc.isInvalid()) {
                    return;
                }
            }

            void Rule3aCheck::check(const MatchFinder::MatchResult &Result) {
                const SourceManager &SM = *Result.SourceManager;
                const ASTContext *Context = Result.Context;

                std::vector<SourceLocation> left;
                std::vector<SourceLocation> right;

                if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while")) {
                    if (SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    left.push_back(MatchedDecl->getLParenLoc());
                    right.push_back(MatchedDecl->getRParenLoc());
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
                    if (SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    left.push_back(MatchedDecl->getLParenLoc());
                    right.push_back(MatchedDecl->getRParenLoc());
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
                    if (SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    left.push_back(MatchedDecl->getLParenLoc());
                    right.push_back(MatchedDecl->getRParenLoc());
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
                    if (SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    left.push_back(MatchedDecl->getLParenLoc());
                    right.push_back(MatchedDecl->getRParenLoc());
                }

                std::sort(left.begin(), left.end());
                std::sort(right.begin(), right.end());
                auto lit = left.begin();
                auto rit = right.begin();

                std::pair<FileID, unsigned> LocInfo =
                    SM.getDecomposedLoc(Range.getEnd());
                StringRef File = SM.getBufferData(LocInfo.first);
                const char *TokenBegin = File.data() + LocInfo.second;
                if (not Sources.getLocForStartOfFile(LocInfo.first).isValid() or
                    not SM.isWrittenInMainFile(
                        Sources.getLocForStartOfFile(LocInfo.first))) {
                    return;
                }

                Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                               Result.Context->getLangOpts(), File.begin(), TokenBegin,
                               File.end());

                RawLexer.SetKeepWhitespaceMode(true);
                std::vector<Token> tokens;
                Token tok;
                while (!RawLexer.LexFromRawLexer(tok)) {
                    if (not SM->isWrittenInMainFile(tok.getLocation()) or
                        not SM->isWrittenInMainFile(tok.getEndLoc())) {
                        continue;
                    }

                    tokens.push_back(tok);
                }
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

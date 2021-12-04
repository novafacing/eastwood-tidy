
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
            Rule3aCheck(StringRef Name, ClangTidyContext *Context)
                : ClangTidyCheck(Name, Context),
                  debug_enabled(Options.get("debug", "false")) {
                if (this->debug_enabled == "true") {
                    this->debug = true;
                }
            }

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
                std::string type;

                if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    left.push_back(MatchedDecl->getLParenLoc());
                    right.push_back(MatchedDecl->getRParenLoc());
                    type = "while";
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    left.push_back(MatchedDecl->getLParenLoc());
                    right.push_back(MatchedDecl->getRParenLoc());
                    type = "for";
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    left.push_back(MatchedDecl->getLParenLoc());
                    right.push_back(MatchedDecl->getRParenLoc());
                    type = "if";
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    left.push_back(MatchedDecl->getLParenLoc());
                    right.push_back(MatchedDecl->getRParenLoc());
                    type = "switch";
                }

                std::sort(left.begin(), left.end());
                std::sort(right.begin(), right.end());
                auto lit = left.begin();
                auto rit = right.begin();

                if (left.size() < 1 or right.size() < 1) {
                    return;
                }

                SourceRange Range(*left.begin(), *right.begin());
                std::pair<FileID, unsigned> LocInfo =
                    SM.getDecomposedLoc(Range.getEnd());
                StringRef File = SM.getBufferData(LocInfo.first);
                const char *TokenBegin = File.data();
                if (not SM.getLocForStartOfFile(LocInfo.first).isValid() or
                    not SM.isWrittenInMainFile(
                        SM.getLocForStartOfFile(LocInfo.first))) {
                    return;
                }

                Lexer RawLexer(SM.getLocForStartOfFile(LocInfo.first),
                               Result.Context->getLangOpts(), File.begin(), TokenBegin,
                               File.end());

                RawLexer.SetKeepWhitespaceMode(true);
                std::vector<Token> tokens;
                Token t;

                while (!RawLexer.LexFromRawLexer(t)) {
                    if (not SM.isWrittenInMainFile(t.getLocation()) or
                        not SM.isWrittenInMainFile(t.getEndLoc())) {
                        continue;
                    }
                    tokens.push_back(t);
                }

                if (tokens.size() < 2) {
                    return;
                }

                for (auto tt = tokens.begin(); tt != tokens.end(); tt++) {
                    std::string raw_trav_tok_data =
                        Lexer::getSpelling(*tt, SM, Context->getLangOpts());

                    if (lit <= left.end() and *lit == tt->getLocation()) {
                        auto rtok_it = std::make_reverse_iterator(tt);
                        // rtok_it++;
                        size_t ws = 0;
                        bool bad = false;
                        std::string total = "";
                        while (rtok_it != tokens.rend()) {
                            if (rtok_it->getKind() == tok::unknown) {
                                std::string raw_tok_data = Lexer::getSpelling(
                                    *rtok_it, SM, Context->getLangOpts());

                                total += raw_tok_data;

                                rtok_it++;

                            } else {
                                break;
                            }
                        }
                        for (auto c : total) {
                            if (c == ' ') {
                                ws++;
                            } else {
                                bad = true;
                            }
                        }
                        if (bad or ws != 1) {
                            diag(*lit, "There must be exactly one space "
                                       "between %0 and open parenthesis.")
                                << type;
                        }

                        lit++;
                    }

                    if (rit <= right.end() and *rit == tt->getLocation()) {
                        auto rtok_it = tt;
                        rtok_it++;
                        size_t ws = 0;
                        bool bad = false;
                        std::string total = "";
                        while (rtok_it != tokens.end()) {
                            if (rtok_it->getKind() == tok::unknown) {
                                std::string raw_tok_data = Lexer::getSpelling(
                                    *rtok_it, SM, Context->getLangOpts());

                                total += raw_tok_data;

                                rtok_it++;

                            } else {
                                break;
                            }
                        }

                        for (auto c : total) {
                            if (c == ' ') {
                                ws++;
                            } else {
                                bad = true;
                            }
                        }

                        if (bad or ws != 1) {
                            diag(*rit, "There must be exactly one space between "
                                       "parenthesis and open brace.")
                                << type;
                        }

                        rit++;
                    }
                }
            }
        }
    } // namespace eastwood
} // namespace tidy
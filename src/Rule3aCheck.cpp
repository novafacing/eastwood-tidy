
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
                Finder->addMatcher(whileStmt().bind("while_end"), this);
                Finder->addMatcher(whileStmt().bind("while_beg"), this);
                Finder->addMatcher(forStmt().bind("for_end"), this);
                Finder->addMatcher(forStmt().bind("for_beg"), this);
                Finder->addMatcher(ifStmt().bind("if_end"), this);
                Finder->addMatcher(ifStmt().bind("if_beg"), this);
                Finder->addMatcher(doStmt().bind("do"), this);
                Finder->addMatcher(switchStmt().bind("switch_end"), this);
                Finder->addMatcher(switchStmt().bind("switch_beg"), this);
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
                if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while_end")) {
                    // Set up the lexer
                    CharSourceRange Range = CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc(), 
                            MatchedDecl->getRParenLoc().getLocWithOffset(1));
                    const SourceManager & Sources = *Result.SourceManager;
                    std::pair<FileID, unsigned> LocInfo = Sources.getDecomposedLoc(Range.getEnd());
                    StringRef File = Sources.getBufferData(LocInfo.first);
                    const char * TokenBegin = File.data() + LocInfo.second;
                    Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                            Result.Context->getLangOpts(), File.begin(), TokenBegin,
                            File.end());

                    RawLexer.SetKeepWhitespaceMode(true);
                    Token tok;
                    std::vector<Token> tokens;
                    while (!RawLexer.LexFromRawLexer(tok)) {
                        if (tok.getKind() == tok::l_brace) {
                            if (tokens.size() != 1 or std::string(SM.getCharacterData(tokens.back().getLocation()), 
                                        SM.getCharacterData(tokens.back().getEndLoc())) != " ") {
                                diag(tok.getLocation(), "Open brace after while must be preceeded by exactly one space.");
                                return;
                            }
                            return;
                        }
                        tokens.push_back(tok);
                    }

                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while_beg")) {
                    // Set up the lexer
                    CharSourceRange Range = CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc().getLocWithOffset(1), 
                            MatchedDecl->getLParenLoc());

                    const SourceManager & Sources = *Result.SourceManager;
                    std::pair<FileID, unsigned> LocInfo = Sources.getDecomposedLoc(Range.getBegin());
                    StringRef File = Sources.getBufferData(LocInfo.first);
                    const char * TokenBegin = File.data() + LocInfo.second;
                    Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                            Result.Context->getLangOpts(), File.begin(), TokenBegin,
                            File.end());

                    RawLexer.SetKeepWhitespaceMode(true);
                    Token tok;
                    std::vector<Token> tokens;
                    while (!RawLexer.LexFromRawLexer(tok)) {
                        if (tok.getLocation() == Range.getEnd()) {
                            if (tokens.empty() or std::string(SM.getCharacterData(tokens.back().getLocation()), 
                                        SM.getCharacterData(tokens.back().getEndLoc())) != " ") {
                                diag(tok.getLocation(), "While keyword must be followed by exactly one space.");
                                return;
                            }
                            return;
                        }
                        tokens.push_back(tok);
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for_end")) {
                    CharSourceRange Range = CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc(), 
                            MatchedDecl->getRParenLoc().getLocWithOffset(1));
                    const SourceManager & Sources = *Result.SourceManager;
                    std::pair<FileID, unsigned> LocInfo = Sources.getDecomposedLoc(Range.getEnd());
                    StringRef File = Sources.getBufferData(LocInfo.first);
                    const char * TokenBegin = File.data() + LocInfo.second;
                    Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                            Result.Context->getLangOpts(), File.begin(), TokenBegin,
                            File.end());

                    RawLexer.SetKeepWhitespaceMode(true);
                    Token tok;
                    std::vector<Token> tokens;
                    while (!RawLexer.LexFromRawLexer(tok)) {
                        if (tok.getKind() == tok::l_brace) {
                            if (tokens.size() != 1 or std::string(SM.getCharacterData(tokens.back().getLocation()), 
                                        SM.getCharacterData(tokens.back().getEndLoc())) != " ") {
                                diag(tok.getLocation(), "Open brace after for must be preceeded by exactly one space.");
                                return;
                            }
                            return;
                        }
                        tokens.push_back(tok);
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for_beg")) {
                    CharSourceRange Range = CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc().getLocWithOffset(1), 
                            MatchedDecl->getLParenLoc());

                    const SourceManager & Sources = *Result.SourceManager;
                    std::pair<FileID, unsigned> LocInfo = Sources.getDecomposedLoc(Range.getBegin());
                    StringRef File = Sources.getBufferData(LocInfo.first);
                    const char * TokenBegin = File.data() + LocInfo.second;
                    Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                            Result.Context->getLangOpts(), File.begin(), TokenBegin,
                            File.end());

                    RawLexer.SetKeepWhitespaceMode(true);
                    Token tok;
                    std::vector<Token> tokens;
                    while (!RawLexer.LexFromRawLexer(tok)) {
                        if (tok.getLocation() == Range.getEnd()) {
                            if (tokens.empty() or std::string(SM.getCharacterData(tokens.back().getLocation()), 
                                        SM.getCharacterData(tokens.back().getEndLoc())) != " ") {
                                diag(tok.getLocation(), "For keyword must be followed by exactly one space.");
                                return;
                            }
                            return;
                        }
                        tokens.push_back(tok);
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if_end")) {
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if_beg")) {
                    CharSourceRange Range = CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc().getLocWithOffset(1), 
                            MatchedDecl->getLParenLoc());

                    const SourceManager & Sources = *Result.SourceManager;
                    std::pair<FileID, unsigned> LocInfo = Sources.getDecomposedLoc(Range.getBegin());
                    StringRef File = Sources.getBufferData(LocInfo.first);
                    const char * TokenBegin = File.data() + LocInfo.second;
                    Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                            Result.Context->getLangOpts(), File.begin(), TokenBegin,
                            File.end());

                    RawLexer.SetKeepWhitespaceMode(true);
                    Token tok;
                    std::vector<Token> tokens;
                    while (!RawLexer.LexFromRawLexer(tok)) {
                        if (tok.getLocation() == Range.getEnd()) {
                            if (tokens.empty() or std::string(SM.getCharacterData(tokens.back().getLocation()), 
                                        SM.getCharacterData(tokens.back().getEndLoc())) != " ") {
                                diag(tok.getLocation(), "If keyword must be followed by exactly one space.");
                                return;
                            }
                            return;
                        }
                        tokens.push_back(tok);
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("do")) {
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switch_end")) {
                    CharSourceRange Range = CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc(), 
                            MatchedDecl->getRParenLoc().getLocWithOffset(1));
                    const SourceManager & Sources = *Result.SourceManager;
                    //std::cout << "SWITCH MATCH: |" << std::string(SM.getCharacterData(Range.getBegin()),
                    //        SM.getCharacterData(Range.getEnd())) << "|" << std::endl;
                    std::pair<FileID, unsigned> LocInfo = Sources.getDecomposedLoc(Range.getEnd());
                    StringRef File = Sources.getBufferData(LocInfo.first);
                    const char * TokenBegin = File.data() + LocInfo.second;
                    Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                            Result.Context->getLangOpts(), File.begin(), TokenBegin,
                            File.end());

                    RawLexer.SetKeepWhitespaceMode(true);
                    Token tok;
                    std::vector<Token> tokens;
                    while (!RawLexer.LexFromRawLexer(tok)) {
                        if (tok.getKind() == tok::l_brace) {
                            if (tokens.size() != 1 or std::string(SM.getCharacterData(tokens.back().getLocation()), 
                                        SM.getCharacterData(tokens.back().getEndLoc())) != " ") {
                                diag(tok.getLocation(), "Open brace after switch must be preceeded by exactly one space.");
                                return;
                            }
                            return;
                        }
                        tokens.push_back(tok);
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switch_beg")) {
                    CharSourceRange Range = CharSourceRange::getTokenRange(MatchedDecl->getBeginLoc().getLocWithOffset(1), 
                            MatchedDecl->getLParenLoc());

                    const SourceManager & Sources = *Result.SourceManager;
                    std::pair<FileID, unsigned> LocInfo = Sources.getDecomposedLoc(Range.getBegin());
                    StringRef File = Sources.getBufferData(LocInfo.first);
                    const char * TokenBegin = File.data() + LocInfo.second;
                    Lexer RawLexer(Sources.getLocForStartOfFile(LocInfo.first),
                            Result.Context->getLangOpts(), File.begin(), TokenBegin,
                            File.end());

                    RawLexer.SetKeepWhitespaceMode(true);
                    Token tok;
                    std::vector<Token> tokens;
                    while (!RawLexer.LexFromRawLexer(tok)) {
                        if (tok.getLocation() == Range.getEnd()) {
                            if (tokens.empty() or std::string(SM.getCharacterData(tokens.back().getLocation()), 
                                        SM.getCharacterData(tokens.back().getEndLoc())) != " ") {
                                diag(tok.getLocation(), "Switch keyword must be followed by exactly one space.");
                                return;
                            }
                            return;
                        }
                        tokens.push_back(tok);
                    }
                }
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang

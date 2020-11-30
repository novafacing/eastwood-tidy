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

#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule4aCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(recordDecl().bind("record"), this);
                Finder->addMatcher(enumDecl().bind("enum"), this);
                Finder->addMatcher(functionDecl().bind("function"), this);
                Finder->addMatcher(doStmt().bind("do"), this);
                Finder->addMatcher(forStmt().bind("for"), this);
                Finder->addMatcher(ifStmt().bind("if"), this);
                Finder->addMatcher(switchStmt().bind("switch"), this);
                Finder->addMatcher(caseStmt().bind("case"), this);
                Finder->addMatcher(whileStmt().bind("while"), this);
            }

            void Rule4aCheck::check(const MatchFinder::MatchResult &Result) {
                const SourceManager &SM = *Result.SourceManager;
                this->SMan = Result.SourceManager;
                ASTContext *Context = Result.Context;
                if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("record")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (MatchedDecl->isCompleteDefinition()) {
                        SourceRange BraceRange = MatchedDecl->getBraceRange();
                        if (this->opens.empty() or BraceRange.getBegin() != this->opens.back()) {
                            this->opens.push_back(BraceRange.getBegin());
                        }

                        if (this->closes.empty() or BraceRange.getEnd() != this->closes.back()) {
                            this->closes.push_back(BraceRange.getEnd());
                        }

                        if (SM.getSpellingLineNumber(this->opens.back()) !=
                                SM.getSpellingLineNumber(MatchedDecl->getLocation())) {
                            diag(this->opens.back(), "Open brace must be located on same line as record.");
                        }

                        // // std::cout << "RECORD BODY: |" << std::string(SM.getCharacterData(opens.back()),
                                //SM.getCharacterData(closes.back())) << "|" << std::endl;
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<EnumDecl>("enum")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (MatchedDecl->isCompleteDefinition()) {
                        SourceRange BraceRange = MatchedDecl->getBraceRange();
                        if (this->opens.empty() or BraceRange.getBegin() != this->opens.back()) {
                            this->opens.push_back(BraceRange.getBegin());
                        }
                        if (this->closes.empty() or BraceRange.getEnd() != this->closes.back()) {

                            this->closes.push_back(BraceRange.getEnd());
                        }
                        if (SM.getSpellingLineNumber(this->opens.back()) !=
                                SM.getSpellingLineNumber(MatchedDecl->getLocation())) {
                            diag(this->opens.back(), "Open brace must be located on same line as enum.");
                        }
                        // std::cout << "ENUM BODY: |" << std::string(SM.getCharacterData(opens.back()),
                        //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (not this->lexer_initialized) {
                        SourceRange FunctionDefinitionRange = MatchedDecl->getSourceRange();
                        std::pair<FileID, unsigned> LocInfo = SM.getDecomposedLoc(FunctionDefinitionRange.getBegin());
                        SourceLocation StartOfFile = SM.getLocForStartOfFile(SM.getFileID(FunctionDefinitionRange.getBegin()));
                        StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
                        const char * TokenBegin = File.data();
                        this->lexer = new Lexer(SM.getLocForStartOfFile(LocInfo.first),
                                Context->getLangOpts(), File.begin(), TokenBegin, File.end());
                        this->lexer->SetKeepWhitespaceMode(true);
                        this->lexer_initialized = true;
                    }
                    if (MatchedDecl->isThisDeclarationADefinition() and MatchedDecl->doesThisDeclarationHaveABody()) {
                        SourceLocation StartBrace = MatchedDecl->getBody()->getBeginLoc();
                        SourceLocation EndBrace = MatchedDecl->getBodyRBrace();
                        if (this->opens.empty() or StartBrace != this->opens.back()) {
                            this->opens.push_back(StartBrace);
                        }
                        if (this->closes.empty() or EndBrace != this->closes.back()) {
                            this->closes.push_back(EndBrace);
                        }
                        if (SM.getSpellingLineNumber(this->opens.back()) !=
                                SM.getSpellingLineNumber(MatchedDecl->getLocation())
                                and (MatchedDecl->param_empty() 
                                    or SM.getSpellingLineNumber(MatchedDecl->parameters().back()->getSourceRange().getEnd()) !=
                                    SM.getSpellingLineNumber(this->opens.back()))) {

                            diag(this->opens.back(), "Open brace must be located on same line as function declaration or after parameters.");
                        }
                        // std::cout << "FUNCTION BODY: |" << std::string(SM.getCharacterData(opens.back()),
                        //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("do")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (this->opens.empty() or MatchedDecl->getBody()->getBeginLoc() != this->opens.back()) {
                        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
                    }
                    if (this->closes.empty() or MatchedDecl->getBody()->getEndLoc() != this->closes.back()) {
                        this->closes.push_back(MatchedDecl->getBody()->getEndLoc());
                    }
                    if (SM.getSpellingLineNumber(this->opens.back()) !=
                            SM.getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
                        diag(this->opens.back(), "Open brace must be located on same line as do.");
                    }
                    // std::cout << "DO BODY: |" << std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (this->opens.empty() or MatchedDecl->getBody()->getBeginLoc() != this->opens.back()) {
                        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
                    }
                    if (this->closes.empty() or MatchedDecl->getBody()->getEndLoc() != this->closes.back()) {
                        this->closes.push_back(MatchedDecl->getBody()->getEndLoc());
                    }
                    if (SM.getSpellingLineNumber(this->opens.back()) !=
                            SM.getSpellingLineNumber(MatchedDecl->getBeginLoc())
                            and SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
                            SM.getSpellingLineNumber(this->opens.back())) {
                        diag(this->opens.back(), "Open brace must be located on same line as for or after split contents.");
                    }
                    // std::cout << "FOR BODY: |" << std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    const IfStmt * If = MatchedDecl;
                    const Stmt * Else = MatchedDecl->getElse(); 

                    SourceLocation StartIf = If->getThen()->getBeginLoc();
                    SourceLocation EndIf = If->getThen()->getEndLoc();

                    if (this->opens.empty() or StartIf != this->opens.back()) {
                        this->opens.push_back(StartIf);
                    }

                    if (this->closes.empty() or EndIf != this->closes.back()) {
                        this->closes.push_back(EndIf);
                    }

                    if (SM.getSpellingLineNumber(this->opens.back()) !=
                            SM.getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
                        diag(this->opens.back(), "Open brace must be located on same line as if.");
                    }

                    // std::cout << "IF BODY: |" << std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                    if (Else) {
                        if (SM.getSpellingLineNumber(If->getThen()->getEndLoc()) !=
                                SM.getSpellingLineNumber(Else->getBeginLoc()) - 1) {
                            diag(Else->getBeginLoc().getLocWithOffset(-1), "Else must be on a new line.");
                        }
                        if (const auto * ChildIf = dyn_cast<IfStmt>(Else)) {
                            SourceLocation StartElse = ChildIf->getThen()->getBeginLoc();
                            SourceLocation EndElse = ChildIf->getThen()->getEndLoc();
                            if (this->opens.empty() or StartElse != this->opens.back()) {
                                this->opens.push_back(StartElse);
                            }

                            if (this->closes.empty() or EndElse != this->closes.back()) {
                                this->closes.push_back(EndElse);
                            }

                            if (SM.getSpellingLineNumber(this->opens.back()) !=
                                    SM.getSpellingLineNumber(ChildIf->getBeginLoc())) {
                                diag(this->opens.back(), "Open brace must be located on same line as else.");
                            }
                            // std::cout << "ELSE BODY: |" << std::string(SM.getCharacterData(opens.back()),
                            //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                        }
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (this->opens.empty() or MatchedDecl->getBody()->getBeginLoc() != this->opens.back()) {
                        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
                    }

                    if (this->closes.empty() or MatchedDecl->getBody()->getEndLoc() != this->closes.back()) {
                        this->closes.push_back(MatchedDecl->getBody()->getEndLoc());
                    }

                    if (SM.getSpellingLineNumber(this->opens.back()) !=
                            SM.getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
                        diag(this->opens.back(), "Open brace must be located on same line as switch.");
                    }

                    // std::cout << "SWITCH BODY: |" << std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<CaseStmt>("case")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (this->opens.empty() or MatchedDecl->getBeginLoc() != this->opens.back()) {
                        this->opens.push_back(MatchedDecl->getBeginLoc());
                    }

                    if (this->closes.empty() or MatchedDecl->getEndLoc() != this->closes.back()) {
                        this->closes.push_back(MatchedDecl->getEndLoc());
                    }

                    // std::cout << "CASE BODY: |" << std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<WhileStmt>("while")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (this->opens.empty() or MatchedDecl->getBody()->getBeginLoc() != this->opens.back()) {
                        this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
                    }

                    if (this->closes.empty() or MatchedDecl->getBody()->getEndLoc() != this->closes.back()) {
                        this->closes.push_back(MatchedDecl->getBody()->getEndLoc());
                    }

                    // std::cout << "WHILE BODY: |" << std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;

                    if (SM.getSpellingLineNumber(this->opens.back()) !=
                            SM.getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
                        diag(this->opens.back(), "Open brace must be located on same line as while.");
                    }
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
                if (not this->lexer_initialized) {
                    return;
                }
                size_t indent_amount = 0;
                Token tok;
                std::vector<Token> tokens;
                while (!this->lexer->LexFromRawLexer(tok)) {
                    if (not this->SMan->isWrittenInMainFile(tok.getLocation()) or
                            not this->SMan->isWrittenInMainFile(tok.getEndLoc())) {
                        continue;
                    }
                    tokens.push_back(tok);
                    SourceRange TokenSourceRange(tok.getLocation(), tok.getEndLoc());
                    /*
                       if (TokenSourceRange.fullyContains(SourceRange(opens.front()))) {
                       */
                    while (not opens.empty() and this->SMan->isBeforeInTranslationUnit(opens.front(), tok.getLocation())) {
                        std::string ws(this->SMan->getCharacterData(opens.front()),
                                this->SMan->getCharacterData(tok.getEndLoc()));
                        // std::cout << "Increasing indent at location " << ws << std::endl;
                        opens.pop_front();
                        indent_amount += 2;
                    }
                    /*
                       if (TokenSourceRange.fullyContains(SourceRange(closes.front()))) {
                       */
                    while (not closes.empty() and this->SMan->isBeforeInTranslationUnit(closes.front(), tok.getLocation())) {
                        std::string ws(this->SMan->getCharacterData(closes.front()),
                                this->SMan->getCharacterData(tok.getEndLoc()));
                        // std::cout << "Decreasing indent at location " << ws << std::endl;
                        closes.pop_front();
                        indent_amount -= 2;
                    }
                    if (tok.isAtStartOfLine() and tokens.size() > 1) {
                        std::string ws(this->SMan->getCharacterData(tokens.at(tokens.size() - 2).getLocation()),
                                this->SMan->getCharacterData(tokens.at(tokens.size() - 2).getEndLoc()));
                        if (spc_ct(ws) != indent_amount) {
                            diag(tok.getLocation(), "Incorrect indentation level. Expected %0, got %1") << std::to_string(indent_amount) << std::to_string(spc_ct(ws));
                        }
                    }
                }
                // std::cout << "Opens to consume: " << opens.size() << std::endl;
                // std::cout << "closes to consume: " << closes.size() << std::endl;
            }
        } // namespace eastwood
    } // namespace tidy
} // namespace clang


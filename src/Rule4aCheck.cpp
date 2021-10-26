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
            void Rule4aCheck::registerMatchers(MatchFinder *Finder) {
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
            }

            void Rule4aCheck::check(const MatchFinder::MatchResult &Result) {
                const SourceManager &SM = *Result.SourceManager;
                this->SMan = Result.SourceManager;
                ASTContext *Context = Result.Context;
                this->ctx = Context;
                if (auto MatchedDecl = Result.Nodes.getNodeAs<RecordDecl>("record")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (MatchedDecl->isCompleteDefinition()) {
                        SourceRange BraceRange = MatchedDecl->getBraceRange();
                        this->opens.push_back(BraceRange.getBegin());
                        // std::string
                        // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                        // SM.getCharacterData(this->opens.back()));

                        this->closes.push_back(
                            BraceRange.getEnd().getLocWithOffset(-1));
                        // std::string
                        // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                        // SM.getCharacterData(this->closes.back())); std::cout <<
                        // "Opening at: |" << file_str << "| OPEN" << std::endl <<
                        // "Closing at: |" << file_str_close << "| CLOSE" << std::endl;

                        if (SM.getSpellingLineNumber(this->opens.back()) !=
                            SM.getSpellingLineNumber(MatchedDecl->getLocation())) {
                            diag(this->opens.back(),
                                 "Open brace must be located on same line as record.");
                        }

                        // // std::cout << "RECORD BODY: |" <<
                        // std::string(SM.getCharacterData(opens.back()),
                        // SM.getCharacterData(closes.back())) << "|" << std::endl;
                    }
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<EnumDecl>("enum")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (MatchedDecl->isCompleteDefinition()) {
                        SourceRange BraceRange = MatchedDecl->getBraceRange();
                        this->opens.push_back(BraceRange.getBegin());
                        // std::string
                        // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                        // SM.getCharacterData(this->opens.back()));

                        this->closes.push_back(
                            BraceRange.getEnd().getLocWithOffset(-1));
                        // std::string
                        // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                        // SM.getCharacterData(this->closes.back()));

                        // std::cout << "Opening at: |" << file_str << "| OPEN" <<
                        // std::endl << "Closing at: |" << file_str_close << "| CLOSE"
                        // << std::endl;
                        if (SM.getSpellingLineNumber(this->opens.back()) !=
                            SM.getSpellingLineNumber(MatchedDecl->getLocation())) {
                            diag(this->opens.back(),
                                 "Open brace must be located on same line as enum.");
                        }
                        // std::cout << "ENUM BODY: |" <<
                        // std::string(SM.getCharacterData(opens.back()),
                        //        SM.getCharacterData(closes.back())) << "|" <<
                        //        std::endl;
                    }
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<FunctionDecl>("function")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    if (not this->lexer_initialized) {
                        SourceRange FunctionDefinitionRange =
                            MatchedDecl->getSourceRange();
                        std::pair<FileID, unsigned> LocInfo =
                            SM.getDecomposedLoc(FunctionDefinitionRange.getBegin());
                        SourceLocation StartOfFile = SM.getLocForStartOfFile(
                            SM.getFileID(FunctionDefinitionRange.getBegin()));
                        StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
                        const char *TokenBegin = File.data();
                        this->lexer = new Lexer(SM.getLocForStartOfFile(LocInfo.first),
                                                Context->getLangOpts(), File.begin(),
                                                TokenBegin, File.end());
                        this->lexer->SetKeepWhitespaceMode(true);
                        this->lexer_initialized = true;
                    }
                    if (MatchedDecl->isThisDeclarationADefinition() and
                        MatchedDecl->doesThisDeclarationHaveABody()) {
                        size_t start, end;
                        SourceLocation StartBrace =
                            MatchedDecl->getBody()->getBeginLoc();
                        SourceLocation EndBrace = MatchedDecl->getBodyRBrace();
                        this->opens.push_back(StartBrace);
                        // std::string
                        // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                        // SM.getCharacterData(this->opens.back()));
                        this->closes.push_back(EndBrace.getLocWithOffset(-1));
                        // std::string
                        // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                        // SM.getCharacterData(this->closes.back()));
                        // std::cout << "Opening at: |" << file_str << "| OPEN" <<
                        // std::endl << "Closing at: |" << file_str_close << "| CLOSE"
                        // << std::endl;
                        if (MatchedDecl->getNumParams() > 1 &&
                            ((start = SM.getSpellingLineNumber(
                                  MatchedDecl->getLocation()))) !=
                                (end = SM.getSpellingLineNumber(
                                     MatchedDecl
                                         ->getParamDecl(MatchedDecl->getNumParams() - 1)
                                         ->getEndLoc()))) {
                            for (size_t i = start + 1; i <= end; i++) {
                                this->broken_lines.push_back(i);
                            }
                        }

                        if ((MatchedDecl->getNumParams() == 0 and
                             SM.getSpellingLineNumber(this->opens.back()) !=
                                 SM.getSpellingLineNumber(
                                     MatchedDecl->getLocation())) or
                            (MatchedDecl->getNumParams() > 0 &&
                             SM.getSpellingLineNumber(
                                 MatchedDecl
                                     ->getParamDecl(MatchedDecl->getNumParams() - 1)
                                     ->getEndLoc()) !=
                                 SM.getSpellingLineNumber(this->opens.back()))) {

                            diag(this->opens.back(),
                                 "Open brace on line %0 must be located on same line "
                                 "as function "
                                 "declaration or after parameters on line %1.")
                                << SM.getSpellingLineNumber(this->opens.back())
                                << SM.getSpellingLineNumber(
                                       MatchedDecl->getParametersSourceRange()
                                           .getEnd());
                        }
                        // std::cout << "FUNCTION BODY: |" <<
                        // std::string(SM.getCharacterData(opens.back()),
                        //        SM.getCharacterData(closes.back())) << "|" <<
                        //        std::endl;
                    }
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<DoStmt>("do")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
                    // std::string
                    // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->opens.back()));
                    this->closes.push_back(
                        MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));
                    // std::string
                    // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->closes.back()));
                    // std::cout << "Opening at: |" << file_str << "| OPEN" << std::endl
                    // << "Closing at: |" << file_str_close << "| CLOSE" << std::endl;

                    if (SM.getSpellingLineNumber(this->opens.back()) !=
                        SM.getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
                        diag(this->opens.back(),
                             "Open brace must be located on same line as do.");
                    }
                    // std::cout << "DO BODY: |" <<
                    // std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<ForStmt>("for")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    size_t start, end;
                    this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
                    // std::string
                    // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->opens.back()));
                    this->closes.push_back(
                        MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));
                    // std::string
                    // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->closes.back()));
                    // std::cout << "Opening at: |" << file_str << "| OPEN" << std::endl
                    // << "Closing at: |" << file_str_close << "| CLOSE" << std::endl;

                    if ((start =
                             SM.getSpellingLineNumber(MatchedDecl->getLParenLoc())) !=
                        (end = SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()))) {
                        for (size_t i = start + 1; i <= end; i++) {
                            this->broken_lines.push_back(i);
                        }
                    }
                    if (SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
                        SM.getSpellingLineNumber(this->opens.back())) {
                        diag(this->opens.back(),
                             "Open brace must be located on same line as for or after "
                             "split contents.");
                    }
                    // std::cout << "FOR BODY: |" <<
                    // std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                } else if (auto MatchedDecl = Result.Nodes.getNodeAs<IfStmt>("if")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    size_t start, end;
                    const IfStmt *If = MatchedDecl;
                    const Stmt *Else = MatchedDecl->getElse();

                    SourceLocation StartIf = If->getThen()->getBeginLoc();
                    SourceLocation EndIf = If->getThen()->getEndLoc();

                    this->opens.push_back(StartIf);
                    // std::string
                    // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->opens.back()));

                    this->closes.push_back(EndIf.getLocWithOffset(-1));
                    // std::string
                    // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->closes.back()));
                    // std::cout << "Opening at: |" << file_str << "| OPEN" << std::endl
                    // << "Closing at: |" << file_str_close << "| CLOSE" << std::endl;

                    if (SM.getSpellingLineNumber(this->opens.back()) !=
                        SM.getSpellingLineNumber(MatchedDecl->getRParenLoc())) {
                        diag(this->opens.back(),
                             "Open brace must be located on same line as if.");
                    }

                    if ((start =
                             SM.getSpellingLineNumber(MatchedDecl->getLParenLoc())) !=
                        (end = SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()))) {
                        for (size_t i = start + 1; i <= end; i++) {
                            this->broken_lines.push_back(i);
                        }
                    }

                    // std::cout << "IF BODY: |" <<
                    // std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                    if (Else) {
                        if (SM.getSpellingLineNumber(If->getThen()->getEndLoc()) !=
                            SM.getSpellingLineNumber(Else->getBeginLoc()) - 1) {
                            diag(Else->getBeginLoc().getLocWithOffset(-1),
                                 "Else must be on the line after the associated 'if' "
                                 "statement's closing brace.");
                        }
                        if (const auto *ChildIf = dyn_cast<IfStmt>(Else)) {
                            SourceLocation StartElse =
                                ChildIf->getThen()->getBeginLoc();
                            SourceLocation EndElse = ChildIf->getThen()->getEndLoc();

                            // this->opens.push_back(StartElse);
                            // std::string
                            // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                            // SM.getCharacterData(this->opens.back()));

                            // this->closes.push_back(EndElse.getLocWithOffset(-1));
                            // std::string
                            // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                            // SM.getCharacterData(this->closes.back()));
                            // std::cout << "Opening at: |" << file_str << "| OPEN" <<
                            // std::endl << "Closing at: |" << file_str_close << "|
                            // CLOSE" << std::endl;

                            if (SM.getSpellingLineNumber(StartElse) !=
                                SM.getSpellingLineNumber(ChildIf->getBeginLoc())) {
                                diag(
                                    this->opens.back(),
                                    "Open brace must be located on same line as else.");
                            }
                            // std::cout << "ELSE BODY: |" <<
                            // std::string(SM.getCharacterData(opens.back()),
                            //        SM.getCharacterData(closes.back())) << "|" <<
                            //        std::endl;
                        } else {
                            this->opens.push_back(Else->getBeginLoc());
                            this->closes.push_back(
                                Else->getEndLoc().getLocWithOffset(-1));
                        }
                    }
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<SwitchStmt>("switch")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
                    // std::string
                    // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->opens.back()));

                    this->closes.push_back(
                        MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));
                    // std::string
                    // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->closes.back()));
                    // std::cout << "Opening at: |" << file_str << "| OPEN" << std::endl
                    // << "Closing at: |" << file_str_close << "| CLOSE" << std::endl;

                    if (SM.getSpellingLineNumber(this->opens.back()) !=
                        SM.getSpellingLineNumber(MatchedDecl->getBeginLoc())) {
                        diag(this->opens.back(),
                             "Open brace must be located on same line as switch.");
                    }

                    // std::cout << "SWITCH BODY: |" <<
                    // std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<CaseStmt>("case")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    this->opens.push_back(MatchedDecl->getBeginLoc());
                    // std::string
                    // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->opens.back()));

                    this->closes.push_back(
                        MatchedDecl->getEndLoc().getLocWithOffset(-1));
                    // std::string
                    // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->closes.back()));
                    // std::cout << "Opening at: |" << file_str << "| OPEN" << std::endl
                    // << "Closing at: |" << file_str_close << "| CLOSE" << std::endl;

                    // std::cout << "CASE BODY: |" <<
                    // std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<DefaultStmt>("default")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    this->opens.push_back(MatchedDecl->getBeginLoc());
                    // std::string
                    // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->opens.back()));

                    this->closes.push_back(
                        MatchedDecl->getEndLoc().getLocWithOffset(-1));
                    // std::string
                    // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->closes.back()));
                    // std::cout << "Opening at: |" << file_str << "| OPEN" << std::endl
                    // << "Closing at: |" << file_str_close << "| CLOSE" << std::endl;
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<WhileStmt>("while")) {
                    if (not SM.isWrittenInMainFile(MatchedDecl->getBeginLoc()) or
                        not MatchedDecl->getBeginLoc().isValid() or
                        SM.isMacroArgExpansion(MatchedDecl->getBeginLoc()) or
                        SM.isMacroBodyExpansion(MatchedDecl->getBeginLoc())) {
                        return;
                    }
                    size_t start, end;
                    this->opens.push_back(MatchedDecl->getBody()->getBeginLoc());
                    // std::string
                    // file_str(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->opens.back()));

                    this->closes.push_back(
                        MatchedDecl->getBody()->getEndLoc().getLocWithOffset(-1));
                    // std::string
                    // file_str_close(SM.getCharacterData(SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getBeginLoc()))),
                    // SM.getCharacterData(this->closes.back()));
                    // std::cout << "Opening at: |" << file_str << "| OPEN" << std::endl
                    // << "Closing at: |" << file_str_close << "| CLOSE" << std::endl;

                    // std::cout << "WHILE BODY: |" <<
                    // std::string(SM.getCharacterData(opens.back()),
                    //        SM.getCharacterData(closes.back())) << "|" << std::endl;

                    if (SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()) !=
                        SM.getSpellingLineNumber(this->opens.back())) {
                        diag(this->opens.back(),
                             "Open brace must be located on same line as while.");
                    }
                    if ((start =
                             SM.getSpellingLineNumber(MatchedDecl->getLParenLoc())) !=
                        (end = SM.getSpellingLineNumber(MatchedDecl->getRParenLoc()))) {
                        for (size_t i = start + 1; i <= end; i++) {
                            this->broken_lines.push_back(i);
                        }
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
                std::vector<Token> eol_tokens;
                std::sort(opens.begin(), opens.end());
                std::sort(closes.begin(), closes.end());
                while (!this->lexer->LexFromRawLexer(tok)) {
                    if (not this->SMan->isWrittenInMainFile(tok.getLocation()) or
                        not this->SMan->isWrittenInMainFile(tok.getEndLoc())) {
                        continue;
                    }

                    std::string raw_tok_data =
                        Lexer::getSpelling(tok, *this->SMan, this->ctx->getLangOpts());

                    // This is a line break, so we push the last token into the vector
                    // of EOL tokens
                    if (raw_tok_data.find('\n') != std::string::npos) {
                        if (tokens.size() > 0) {
                            eol_tokens.push_back(tokens.back());
                        }
                    }
                    tokens.push_back(tok);
                    SourceRange TokenSourceRange(tok.getLocation(), tok.getEndLoc());
                    /*
                       if (TokenSourceRange.fullyContains(SourceRange(opens.front()))) {
                       */
                    while (not opens.empty() and
                           this->SMan->isBeforeInTranslationUnit(opens.front(),
                                                                 tok.getLocation())) {
                        /*
                           std::string ws(this->SMan->getCharacterData(opens.front()),
                           this->SMan->getCharacterData(tok.getEndLoc()));
                        std::string
                        cur(this->SMan->getCharacterData(tokens.at(0).getLocation()),
                                this->SMan->getCharacterData(tok.getLocation()));
                           */
                        opens.pop_front();
                        indent_amount += 2;
                        // std::cout << "Increasing indent at location |" << cur << "|
                        // ++Indent " << indent_amount << std::endl;
                    }
                    /*
                       if (TokenSourceRange.fullyContains(SourceRange(closes.front())))
                       {
                       */
                    while (not closes.empty() and
                           this->SMan->isBeforeInTranslationUnit(closes.front(),
                                                                 tok.getLocation())) {
                        /*
                           std::string ws(this->SMan->getCharacterData(closes.front()),
                           this->SMan->getCharacterData(tok.getEndLoc()));
                        std::string
                        cur(this->SMan->getCharacterData(tokens.at(0).getLocation()),
                                this->SMan->getCharacterData(tok.getLocation()));
                           */
                        closes.pop_front();
                        indent_amount -= 2;
                        // std::cout << "Decreasing indent at location |" << cur <<
                        // "|---Indent " << indent_amount << std::endl;
                    }
                    if (tok.isAtStartOfLine() and tokens.size() > 1) {
                        std::string ws(this->SMan->getCharacterData(
                                           tokens.at(tokens.size() - 2).getLocation()),
                                       this->SMan->getCharacterData(
                                           tokens.at(tokens.size() - 2).getEndLoc()));
                        /*
                        std::cout << "WS: |";
                        for (auto c : ws) {
                            std::cout << "," << std::hex << (int)c;
                        }
                        std::cout << "|" << std::endl;
                        */

                        bool breakable = false;
                        for (auto t : eol_tokens) {
                            // This EOL token is the one for the line before the current
                            // line
                            if (this->SMan->getSpellingLineNumber(t.getLocation()) ==
                                this->SMan->getSpellingLineNumber(tok.getLocation()) -
                                    1) {
                                if (t.getKind() != tok::l_brace and
                                    t.getKind() != tok::semi and
                                    t.getKind() != tok::comment) {
                                    breakable = true;
                                }

                                break;
                            }
                        }
                        if (spc_ct(ws) != indent_amount) {
                            // Is this a known broken line?
                            if (std::find(this->broken_lines.begin(),
                                          this->broken_lines.end(),
                                          this->SMan->getSpellingLineNumber(
                                              tok.getLocation())) !=
                                this->broken_lines.end()) {
                                if (spc_ct(ws) < indent_amount + 2) {
                                    diag(tok.getLocation(),
                                         "Incorrect indentation level. Expected at "
                                         "least %0, got %1")
                                        << std::to_string(indent_amount + 2)
                                        << std::to_string(spc_ct(ws));
                                }
                                // Is the end of the previous line something other than:
                                // - ';'
                                // - '{'
                                // - A comment
                                // - A macro define, ifndef, endif, include
                            } else if (breakable) {
                                if (spc_ct(ws) < indent_amount + 2) {
                                    diag(tok.getLocation(),
                                         "Incorrect indentation level. Expected at "
                                         "least %0, got %1")
                                        << std::to_string(indent_amount + 2)
                                        << std::to_string(spc_ct(ws));
                                }

                            } else {
                                diag(tok.getLocation(),
                                     "Incorrect indentation level. Expected %0, got %1")
                                    << std::to_string(indent_amount)
                                    << std::to_string(spc_ct(ws));
                            }
                            // std::cout << "reporting incorrect indentation level.
                            // Expected " << indent_amount << " got " << spc_ct(ws) <<
                            // std::endl;
                        }
                    }
                }
                // std::cout << "Opens to consume: " << opens.size() << std::endl;
                // std::cout << "closes to consume: " << closes.size() << std::endl;
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

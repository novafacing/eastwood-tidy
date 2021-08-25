
//===--- Rule5bCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule5bCheck.h"

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            void Rule5bCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(functionDecl().bind("function_decl"), this);
            }
            size_t count_newlines(std::string s) {
                size_t ct = 0;
                for (auto c : s) {
                    if (c == '\n') {
                        ct++;
                    }
                }
                return ct;
            }
            void Rule5bCheck::check(const MatchFinder::MatchResult &Result) {
                const SourceManager &SM = *Result.SourceManager;
                ASTContext *Context = Result.Context;

                if (auto MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("function_decl")) {
                    if (not this->checked) {
                        SourceLocation Location = MatchedDecl->getSourceRange().getBegin();
                        SourceLocation StartOfFile = SM.getLocForStartOfFile(SM.getFileID(Location));
                        StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
                        const char *TokenBegin = File.data();
                        Lexer RawLexer(StartOfFile, Context->getLangOpts(), File.begin(), TokenBegin, File.end());

                        RawLexer.SetKeepWhitespaceMode(true);

                        std::vector<Token> tokens;
                        std::vector<Token> line_begin_tokens;

                        Token tok;
                        Token next;
                        Token nnext;
                        bool add_next = false;
                        while (!RawLexer.LexFromRawLexer(tok)) {
                            if (tok.getKind() == tok::comment) {
                                //std::cout << "checking comment token" << std::endl;
                                // OK if:
                                // - At start of line
                                // - After an if, else, or case statement
                                // - Has blank line above and below
                                if (tok.isAtStartOfLine()) {
                                    if (tokens.size() != 0) {
                                        if (count_newlines(std::string(SM.getCharacterData(tokens.back().getLocation()),
                                                                       SM.getCharacterData(tokens.back().getEndLoc()))) < 2) {
                                            std::string contents(SM.getCharacterData(tokens.back().getLocation()),
                                                                 SM.getCharacterData(tokens.back().getEndLoc()));
                                            //std::cout << "CONTENTS OF UNMATCHED \\n\\n STRING: |" << contents << "|" << std::endl;
                                            if (tokens.at(tokens.size() - 2).getKind() != tok::l_brace and tokens.at(tokens.size() - 2).getKind() != tok::comment) {
                                                diag(tok.getLocation(), "Comment must be preceeded by a blank line, comment, or closing brace.");
                                            }
                                        }
                                    }
                                    // Check if blank lines
                                    if (RawLexer.LexFromRawLexer(next)) {
                                        // This is EOF, which is fine.
                                        continue;
                                    } else {
                                        if (RawLexer.LexFromRawLexer(nnext)) {
                                            continue;
                                        }
                                        add_next = true;
                                        if (count_newlines(std::string(SM.getCharacterData(next.getLocation()),
                                                                       SM.getCharacterData(next.getEndLoc()))) < 2 and
                                            nnext.getKind() != tok::comment) {
                                            std::string contents(SM.getCharacterData(next.getLocation()),
                                                                 SM.getCharacterData(next.getEndLoc()));
                                            //std::cout << "CONTENTS OF UNMATCHED \\n\\n STRING: |" << contents << "|" << std::endl;
                                            diag(tok.getEndLoc(), "Comment must be followed by a blank line or comment.");
                                        }
                                    }
                                } else {
                                    Token last_start = line_begin_tokens.back();
                                    if (last_start.isOneOf(tok::kw_if, tok::kw_else, tok::kw_case) or
                                        (last_start.isAnyIdentifier() and (last_start.getRawIdentifier().str() == "if" or
                                                                           last_start.getRawIdentifier().str() == "else" or
                                                                           last_start.getRawIdentifier().str() == "case"))) {
                                        // OK
                                        //std::cout << "Last start is OK" << std::endl;
                                    } else {
                                        //std::cout << "Last start is " << last_start.getName() << std::endl;
                                        if (not this->visited) {
                                            Rule5bVisitor visitor(Context);
                                            if (not visitor.TraverseDecl(Context->getTranslationUnitDecl())) {
                                                //std::cout << "Error traversing declaration" << std::endl;
                                                // TODO: Handle error
                                            }
                                            for (auto SL : visitor.function_decl_ends) {
                                                this->function_decl_ends.push_back(SL);
                                            }
                                            this->visited = true;
                                        }
                                        for (auto SL : this->function_decl_ends) {
                                            if (last_start.getLocation() == SL) {
                                                //std::cout << "Last start is function body end" << std::endl;
                                                goto end;
                                            }
                                        }
                                        diag(tok.getLocation(), "Inline comment may only follow a function, if, else, or case statement.");
                                    }
                                }
                            }

                        end:
                            tokens.push_back(tok);
                            if (tok.isAtStartOfLine()) {
                                line_begin_tokens.push_back(tok);
                            }
                            if (add_next and next.isAtStartOfLine()) {
                                line_begin_tokens.push_back(next);
                            }
                            if (add_next and nnext.isAtStartOfLine()) {
                                line_begin_tokens.push_back(nnext);
                            }
                            if (add_next) {
                                tokens.push_back(next);
                                tokens.push_back(nnext);
                                add_next = false;
                            }
                        }
                        this->checked = true;
                    }
                }
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

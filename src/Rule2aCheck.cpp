
//===--- Rule2aCheck.cpp - clang-tidy -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Each line must be <= 80 columns. If it is more, it should be broken up and indented
// >= 2 spaces

#include "Rule2aCheck.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "clang/Lex/Lexer.h"
#include "clang/Lex/Token.h"

#define MAX_LINE_LEN (80)

using namespace clang::ast_matchers;

namespace clang {
    namespace tidy {
        namespace eastwood {
            Rule2aCheck::Rule2aCheck(StringRef Name, ClangTidyContext *Context)
                : ClangTidyCheck(Name, Context), checked(false),
                  debug_enabled(Options.get("debug", "false")) {
                if (this->debug_enabled == "true") {
                    this->debug = true;
                }
            }

            void Rule2aCheck::registerMatchers(MatchFinder *Finder) {
                Finder->addMatcher(functionDecl().bind("function"), this);
                Finder->addMatcher(translationUnitDecl().bind("translation_unit"),
                                   this);
            }

            void Rule2aCheck::check(const MatchFinder::MatchResult &Result) {

                const SourceManager &SM = *Result.SourceManager;
                const ASTContext *Context = Result.Context;

                if (auto MatchedDecl =
                        Result.Nodes.getNodeAs<FunctionDecl>("function")) {
                    if (not this->checked) {
                        SourceLocation Location =
                            MatchedDecl->getSourceRange().getBegin();
                        SourceLocation StartOfFile =
                            SM.getLocForStartOfFile(SM.getFileID(Location));
                        StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
                        const char *TokenBegin = File.data();
                        Lexer RawLexer(StartOfFile, Context->getLangOpts(),
                                       File.begin(), TokenBegin, File.end());

                        RawLexer.SetKeepWhitespaceMode(true);

                        Token tok;
                        llvm::Optional<Token> ConstTok;
                        size_t lines = 0;
                        std::vector<std::pair<Token, std::string>> tq;
                        std::regex trailing_ws{R"([ \t\f\p\v])"};
                        std::smatch results;
                        size_t indentation_level = 0;
                        while (!RawLexer.LexFromRawLexer(tok)) {
                            if (tok.getKind() == tok::l_brace) {
                                indentation_level += 2;
                            }
                            if (tok.getKind() == tok::r_brace) {
                                indentation_level -= 2;
                            }
                            std::string raw_tok_data =
                                Lexer::getSpelling(tok, SM, Context->getLangOpts());
                            if (raw_tok_data == "\n") {
                                // std::cout << "Got newline token" << std::endl;
                                if (SM.getSpellingColumnNumber(tok.getLocation()) >
                                    80) {
                                    diag(tok.getLocation(),
                                         "Line exceeds 80 characters.");
                                }
                            }
                            if (tok.isAtStartOfLine()) {
                                lines++;
                                if (lines >= 2) {
                                    if (tq.at(tq.size() - 1).second == "\r") {
                                        // XI.B Unix newline
                                        // diag(tq.at(tq.size() -
                                        // 1).first.getLocation(), "Non-Unix newlines
                                        // are not permitted. Please run dos2unix on
                                        // your source file.");
                                    } else if (std::regex_match(
                                                   tq.at(tq.size() - 2).second, results,
                                                   trailing_ws)) {
                                        // III.E Trailing Whitespace
                                        diag(tq.at(tq.size() - 1).first.getLocation(),
                                             "Trailing whitespace is not permitted.");
                                    }
                                    unsigned lcol = SM.getSpellingColumnNumber(
                                        tq.at(tq.size() - 2).first.getLocation());
                                    unsigned col =
                                        SM.getSpellingColumnNumber(tok.getLocation());
                                    if (lcol >= MAX_LINE_LEN) {
                                        diag(tq.at(tq.size() - 1).first.getLocation(),
                                             "Lines must be <= " +
                                                 std::to_string(MAX_LINE_LEN) +
                                                 " characters in length.");
                                    }
                                }
                                std::string Indentation =
                                    Lexer::getIndentationForLine(tok.getLocation(), SM)
                                        .str();
                                for (auto c : Indentation) {
                                    if (c != ' ') {
                                        // diag(tok.getLocation(), "Indentation must
                                        // consist of spaces only.");
                                    }
                                }
                            }
                            tq.push_back(std::make_pair(tok, raw_tok_data));
                        }
                    }
                    this->checked = true;
                } else if (auto MatchedDecl =
                               Result.Nodes.getNodeAs<TranslationUnitDecl>(
                                   "translation_unit")) {
                    std::cout << "Got translation unit" << std::endl;
                    std::string match(SM.getCharacterData(MatchedDecl->getBeginLoc()),
                                      SM.getCharacterData(MatchedDecl->getEndLoc()));
                    std::cout << match << std::endl;
                }
            }
        } // namespace eastwood
    }     // namespace tidy
} // namespace clang

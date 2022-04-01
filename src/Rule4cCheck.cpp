//===--- Rule4cCheck.cpp - clang-tidy ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Rule4cCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/Lexer.h"

#include <iostream>

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace eastwood {
namespace {

tok::TokenKind getTokenKind(SourceLocation Loc, const SourceManager &SM,
                            const ASTContext *Context) {
    Token Tok;
    SourceLocation Beginning =
        Lexer::GetBeginningOfToken(Loc, SM, Context->getLangOpts());
    const bool Invalid = Lexer::getRawToken(Beginning, Tok, SM, Context->getLangOpts());
    assert(!Invalid && "Expected a valid token.");

    if (Invalid)
        return tok::NUM_TOKENS;

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
        if (TokKind == tok::NUM_TOKENS || TokKind != tok::comment)
            return Loc;

        // Fast-forward current token.
        Loc = Lexer::getLocForEndOfToken(Loc, 0, SM, Context->getLangOpts());
    }
}

SourceLocation findEndLocation(SourceLocation LastTokenLoc, const SourceManager &SM,
                               const ASTContext *Context) {
    SourceLocation Loc =
        Lexer::GetBeginningOfToken(LastTokenLoc, SM, Context->getLangOpts());
    // Loc points to the beginning of the last (non-comment non-ws)
    // token before end or ';'.
    assert(Loc.isValid());
    bool SkipEndWhitespaceAndComments = true;
    tok::TokenKind TokKind = getTokenKind(Loc, SM, Context);
    if (TokKind == tok::NUM_TOKENS || TokKind == tok::semi || TokKind == tok::r_brace) {
        // If we are at ";" or "}", we found the last token. We could
        // use as well `if (isa<NullStmt>(S))`, but it wouldn't work for
        // nested statements.
        SkipEndWhitespaceAndComments = false;
    }

    Loc = Lexer::getLocForEndOfToken(Loc, 0, SM, Context->getLangOpts());
    // Loc points past the last token before end or after ';'.
    if (SkipEndWhitespaceAndComments) {
        Loc = forwardSkipWhitespaceAndComments(Loc, SM, Context);
        tok::TokenKind TokKind = getTokenKind(Loc, SM, Context);
        if (TokKind == tok::semi)
            Loc = Lexer::getLocForEndOfToken(Loc, 0, SM, Context->getLangOpts());
    }

    for (;;) {
        assert(Loc.isValid());
        while (isHorizontalWhitespace(*SM.getCharacterData(Loc))) {
            Loc = Loc.getLocWithOffset(1);
        }

        if (isVerticalWhitespace(*SM.getCharacterData(Loc))) {
            // EOL, insert brace before.
            break;
        }
        tok::TokenKind TokKind = getTokenKind(Loc, SM, Context);
        if (TokKind != tok::comment) {
            // Non-comment token, insert brace before.
            break;
        }

        SourceLocation TokEndLoc =
            Lexer::getLocForEndOfToken(Loc, 0, SM, Context->getLangOpts());
        SourceRange TokRange(Loc, TokEndLoc);
        StringRef Comment = Lexer::getSourceText(
            CharSourceRange::getTokenRange(TokRange), SM, Context->getLangOpts());
        if (Comment.startswith("/*") && Comment.find('\n') != StringRef::npos) {
            // Multi-line block comment, insert brace before.
            break;
        }
        // else: Trailing comment, insert brace after the newline.

        // Fast-forward current token.
        Loc = TokEndLoc;
    }
    return Loc;
}

} // namespace

Rule4cCheck::Rule4cCheck(StringRef Name, ClangTidyContext *Context)
    : ClangTidyCheck(Name, Context), EastwoodTidyCheckBase(Name),
      debug_enabled(Options.get("debug", "false")) {
    if (this->debug_enabled == "true") {
        this->debug = true;
    }
}

void Rule4cCheck::registerMatchers(MatchFinder *Finder) {
    this->register_relex_matchers(Finder, this);
    Finder->addMatcher(doStmt().bind("do"), this);
}

void Rule4cCheck::check(const MatchFinder::MatchResult &Result) {
    this->acquire_common(Result);

    if (auto S = Result.Nodes.getNodeAs<DoStmt>("do")) {
        checkStmt(Result, S->getBody(), S->getDoLoc(), S->getWhileLoc());
    }
}

bool Rule4cCheck::checkStmt(const MatchFinder::MatchResult &Result, const Stmt *S,
                            SourceLocation InitialLoc, SourceLocation EndLocHint) {

    // Treat macros.
    CharSourceRange FileRange = Lexer::makeFileCharRange(
        CharSourceRange::getTokenRange(S->getSourceRange()), *this->source_manager,
        this->ast_context->getLangOpts());
    if (FileRange.isInvalid())
        return false;

    // Convert InitialLoc to file location, if it's on the same macro
    // expansion level as the start of the statement. We also need file
    // locations for Lexer::getLocForEndOfToken working properly.
    InitialLoc = Lexer::makeFileCharRange(
                     CharSourceRange::getCharRange(InitialLoc, S->getBeginLoc()),
                     *this->source_manager, this->ast_context->getLangOpts())
                     .getBegin();
    if (InitialLoc.isInvalid())
        return false;

    // StartLoc points at the location of the opening brace to be inserted.
    SourceLocation EndLoc;
    if (EndLocHint.isValid()) {
        EndLoc = EndLocHint;
    } else {
        const auto FREnd = FileRange.getEnd().getLocWithOffset(-1);
        EndLoc = findEndLocation(FREnd, *this->source_manager, this->ast_context);
    }

    assert(EndLoc.isValid());
    assert(*this->source_manager->getCharacterData(EndLoc) == 'w');

    EndLoc = EndLoc.getLocWithOffset(-1);

    // Check that
    while (*this->source_manager->getCharacterData(EndLoc) != '}') {
        if (*this->source_manager->getCharacterData(EndLoc) == '\n') {
            diag(EndLoc, "While statement in a do-while must be on same "
                         "line as the closing brace.");
            return true;
        }

        EndLoc = EndLoc.getLocWithOffset(-1);
    }

    return false;
}
} // namespace eastwood
} // namespace tidy
} // namespace clang

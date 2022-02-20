#include "EastwoodTidyUtil.h"

EastwoodTidyDebugStream &EastwoodTidyCheckBase::dout() {
    if (this->debugStream == nullptr) {
        this->debugStream = new EastwoodTidyDebugStream(this->debug);
    }
    return *this->debugStream;
}

/*
 * Get the string representation of a token.
 */
std::shared_ptr<std::string> EastwoodTidyCheckBase::tok_string(const SourceManager &SM,
                                                               const Token &tok) {
    if (!tok.getLocation().isValid()) {
        return std::make_shared<std::string>("(INVALID TOKEN LOCATION)");
    } else if (!SM.isWrittenInMainFile(tok.getLocation())) {
        return std::make_shared<std::string>("(NOT IN MAIN FILE TOKEN LOCATION)");
    }
    return std::make_shared<std::string>(SM.getCharacterData(tok.getLocation()),
                                         SM.getCharacterData(tok.getEndLoc()));
}

std::shared_ptr<std::string>
EastwoodTidyCheckBase::sourcerange_string(const SourceManager &SM,
                                          const SourceRange &range) {
    if (!range.isValid() || (!SM.isWrittenInMainFile(range.getBegin()) &&
                             !SM.isWrittenInMainFile(range.getEnd()))) {
        return std::make_shared<std::string>("(INVALID SOURCE RANGE)");
    }
    return std::make_shared<std::string>(SM.getCharacterData(range.getBegin()),
                                         SM.getCharacterData(range.getEnd()));
}

/*
 * Relex a source file containing a match result into a vector of tokens.
 */
std::shared_ptr<std::vector<Token>>
EastwoodTidyCheckBase::relex_file(const MatchFinder::MatchResult &Result,
                                  const std::string &match_name, bool keep_whitespace) {
    const SourceManager &SM = *Result.SourceManager;
    const ASTContext *Context = Result.Context;
    auto MatchedDecl = Result.Nodes.getNodeAs<Decl>(match_name);

    if (!MatchedDecl) {
        return std::make_shared<std::vector<Token>>();
    }

    std::shared_ptr<std::vector<Token>> tokens = std::make_shared<std::vector<Token>>();
    std::pair<FileID, unsigned> LocInfo =
        SM.getDecomposedLoc(MatchedDecl->getLocation());
    SourceLocation StartOfFile =
        SM.getLocForStartOfFile(SM.getFileID(MatchedDecl->getLocation()));
    StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
    const char *TokenBegin = File.data();
    Lexer RawLexer(SM.getLocForStartOfFile(LocInfo.first), Context->getLangOpts(),
                   File.begin(), TokenBegin, File.end());
    RawLexer.SetKeepWhitespaceMode(keep_whitespace);

    Token tok;
    while (!RawLexer.LexFromRawLexer(tok)) {
        if (tok.getLocation().isValid() && SM.isWrittenInMainFile(tok.getLocation())) {
            tokens->push_back(tok);
        }
    }
    return tokens;
}
#include "EastwoodTidyUtil.h"

EastwoodTidyCheckBase::EastwoodTidyCheckBase(StringRef Name)
    : name(Name.str()), debug(false), checked(false) {}

EastwoodTidyDebugStream &EastwoodTidyCheckBase::dout(const std::string &s) {
    if (this->debugStream == nullptr) {
        this->debugStream = new EastwoodTidyDebugStream(this->debug);
    }

    std::string st;
    if (s != "") {
        st += " " + s;
    } else {
        st += s;
    }

    (*this->debugStream) << "[" << this->name << st << "]:  ";
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

void EastwoodTidyCheckBase::relex_file(const MatchFinder::MatchResult &Result,
                                       const std::string &match_name,
                                       const Optional<SourceLocation> &loc_override,
                                       bool keep_whitespace) {

    const SourceManager &SM = *Result.SourceManager;
    const ASTContext *Context = Result.Context;

    SourceLocation loc;
    auto MatchedStmt = Result.Nodes.getNodeAs<Stmt>(match_name.c_str());
    auto MatchedDecl = Result.Nodes.getNodeAs<Decl>(match_name.c_str());

    if (MatchedDecl) {
        loc = MatchedDecl->getBeginLoc();
    } else if (MatchedStmt) {
        loc = MatchedStmt->getBeginLoc();
    } else {
        return;
    }

    if (!SM.isWrittenInMainFile(loc) || !loc.isValid()) {
        return;
    }

    /* If the location is overridden, use that instead. Useful if the Result
     * location is not in the main file or something like that. */
    if (loc_override.hasValue()) {
        loc = loc_override.getValue();
    }

    this->tokens.clear();
    std::pair<FileID, unsigned> LocInfo = SM.getDecomposedLoc(loc);
    SourceLocation StartOfFile = SM.getLocForStartOfFile(SM.getFileID(loc));
    StringRef File = SM.getBufferData(SM.getFileID(StartOfFile));
    const char *TokenBegin = File.data();
    Lexer RawLexer(SM.getLocForStartOfFile(LocInfo.first), Context->getLangOpts(),
                   File.begin(), TokenBegin, File.end());

    RawLexer.SetKeepWhitespaceMode(keep_whitespace);

    Token tok;
    while (!RawLexer.LexFromRawLexer(tok)) {
        if (tok.getLocation().isValid() && SM.isWrittenInMainFile(tok.getLocation())) {
            this->tokens.push_back(tok);
        }
    }
    this->dout() << "Relexed file for match: " << match_name << " with "
                 << this->tokens.size() << " tokens.\n";

    this->relexed = true;
}
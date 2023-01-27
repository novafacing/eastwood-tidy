#include "EastwoodTidyUtil.h"

EastwoodTidyCheckBase::EastwoodTidyCheckBase(StringRef Name)
    : name(Name.str()), debug(false), checked(false) {}

EastwoodTidyDebugStream &EastwoodTidyCheckBase::dout(const std::string &s) {
    if (this->debug_stream == nullptr) {
        this->debug_stream = new EastwoodTidyDebugStream(this->debug);
    }

    std::string st;
    if (s != "") {
        st += " " + s;
    } else {
        st += s;
    }

    (*this->debug_stream) << "[" << this->name << st << "]:  ";
    return *this->debug_stream;
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

void EastwoodTidyCheckBase::register_relex_matchers(MatchFinder *Finder,
                                                    ClangTidyCheck *Check) {
    Finder->addMatcher(stmt().bind("relex"), Check);
    Finder->addMatcher(decl().bind("relex"), Check);
}

ssize_t EastwoodTidyCheckBase::token_index(const Token &t) {
    ssize_t idx = -1;
    for (size_t i = 0; i < this->tokens.size(); i++) {
        if (!this->source_manager->isBeforeInTranslationUnit(
                this->tokens.at(i).getLocation(), t.getLocation())) {
            idx = i - 1;
            break;
        }
    }
    return idx;
}

ssize_t EastwoodTidyCheckBase::token_index(const SourceLocation &loc) {
    ssize_t idx = -1;
    for (size_t i = 0; i < this->tokens.size(); i++) {
        if (!this->source_manager->isBeforeInTranslationUnit(
                this->tokens.at(i).getLocation(), loc)) {
            idx = i - 1;
            break;
        }
    }
    return idx;
}

void EastwoodTidyCheckBase::relex_file(const MatchFinder::MatchResult &Result,
                                       const std::string &match_name,
                                       const Optional<SourceLocation> &loc_override,
                                       bool keep_whitespace) {

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

    if (!this->source_manager->isWrittenInMainFile(loc) || !loc.isValid()) {
        return;
    }

    /* If the location is overridden, use that instead. Useful if the Result
     * location is not in the main file or something like that. */
    if (loc_override.has_value()) {
        loc = loc_override.value();
    }

    this->tokens.clear();
    std::pair<FileID, unsigned> LocInfo = this->source_manager->getDecomposedLoc(loc);
    SourceLocation StartOfFile = this->source_manager->getLocForStartOfFile(
        this->source_manager->getFileID(loc));
    StringRef File = this->source_manager->getBufferData(
        this->source_manager->getFileID(StartOfFile));
    const char *TokenBegin = File.data();
    Lexer RawLexer(this->source_manager->getLocForStartOfFile(LocInfo.first),
                   this->ast_context->getLangOpts(), File.begin(), TokenBegin,
                   File.end());

    RawLexer.SetKeepWhitespaceMode(keep_whitespace);

    Token tok;
    while (!RawLexer.LexFromRawLexer(tok)) {
        if (tok.getLocation().isValid() &&
            this->source_manager->isWrittenInMainFile(tok.getLocation())) {
            this->tokens.push_back(tok);
        }
    }
    this->dout() << "Relexed file for match: " << match_name << " with "
                 << this->tokens.size() << " tokens.\n";

    this->relexed = true;
}

void EastwoodTidyCheckBase::acquire_common(const MatchFinder::MatchResult &Result) {
    this->source_manager = Result.SourceManager;
    this->ast_context = Result.Context;
}
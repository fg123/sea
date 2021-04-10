#include "Lexer.h"

#define MATCH_KEYWORD(word, token) \
if (iterator.Match(word)) { \
    tokenList.emplace_back(token, iterator.GetLastMatch(), fileName, \
        iterator.GetLastMatchLine(), iterator.GetLastMatchCol()); \
    return true; \
}

bool Lexer::MatchKeyword() {
         MATCH_KEYWORD("break", TokenType::BREAK)
    else MATCH_KEYWORD("case", TokenType::CASE)
    else MATCH_KEYWORD("catch", TokenType::CATCH)
    else MATCH_KEYWORD("class", TokenType::CLASS)
    else MATCH_KEYWORD("const", TokenType::CONST)
    else MATCH_KEYWORD("let", TokenType::LET)
    else MATCH_KEYWORD("continue", TokenType::CONTINUE)
    else MATCH_KEYWORD("do", TokenType::DO)
    else MATCH_KEYWORD("else", TokenType::ELSE)
    else MATCH_KEYWORD("extends", TokenType::EXTENDS)
    else MATCH_KEYWORD("final", TokenType::FINAL)
    else MATCH_KEYWORD("finally", TokenType::FINALLY)
    else MATCH_KEYWORD("function", TokenType::FUNCTION)
    else MATCH_KEYWORD("for", TokenType::FOR)
    else MATCH_KEYWORD("if", TokenType::IF)
    else MATCH_KEYWORD("implements", TokenType::IMPLEMENTS)
    else MATCH_KEYWORD("import", TokenType::IMPORT)
    else MATCH_KEYWORD("as?", TokenType::SAFE_AS)
    else MATCH_KEYWORD("as", TokenType::AS)
    else MATCH_KEYWORD("is", TokenType::IS)
    else MATCH_KEYWORD("in", TokenType::IN)
    else MATCH_KEYWORD("interface", TokenType::INTERFACE)
    else MATCH_KEYWORD("new", TokenType::NEW)
    else MATCH_KEYWORD("public", TokenType::PUBLIC)
    else MATCH_KEYWORD("return", TokenType::RETURN)
    else MATCH_KEYWORD("static", TokenType::STATIC)
    else MATCH_KEYWORD("super", TokenType::SUPER)
    else MATCH_KEYWORD("switch", TokenType::SWITCH)
    else MATCH_KEYWORD("this", TokenType::THIS)
    else MATCH_KEYWORD("throw", TokenType::THROW)
    else MATCH_KEYWORD("throws", TokenType::THROWS)
    else MATCH_KEYWORD("try", TokenType::TRY)
    else MATCH_KEYWORD("void", TokenType::VOID)
    else MATCH_KEYWORD("while", TokenType::WHILE)
    else MATCH_KEYWORD("true", TokenType::TRUE_LITERAL)
    else MATCH_KEYWORD("false", TokenType::FALSE_LITERAL)
    else MATCH_KEYWORD("null", TokenType::NULL_LITERAL)
    
    /* Multi Character Matching First */
    else MATCH_KEYWORD("==" ,TokenType::EQ)
    else MATCH_KEYWORD(">=" ,TokenType::GE)
    else MATCH_KEYWORD("<=" ,TokenType::LE)
    else MATCH_KEYWORD("!=" ,TokenType::NE)
    else MATCH_KEYWORD("&&" ,TokenType::AND)
    else MATCH_KEYWORD("||" ,TokenType::OR)
    else MATCH_KEYWORD("->" ,TokenType::RANGE)
    else MATCH_KEYWORD("++" ,TokenType::INC)
    else MATCH_KEYWORD("--" ,TokenType::DEC)
    else MATCH_KEYWORD("+=" ,TokenType::ADD_EQ)
    else MATCH_KEYWORD("-=" ,TokenType::SUB_EQ)
    else MATCH_KEYWORD("*=" ,TokenType::MULT_EQ)
    else MATCH_KEYWORD("/=" ,TokenType::DIV_EQ)
    else MATCH_KEYWORD("%=" ,TokenType::DIV_EQ)
    else MATCH_KEYWORD('?:' ,TokenType::ELVIS)
    else MATCH_KEYWORD('(' ,TokenType::LPAREN)
    else MATCH_KEYWORD(')' ,TokenType::RPAREN)
    else MATCH_KEYWORD('{' ,TokenType::LBRACE)
    else MATCH_KEYWORD('}' ,TokenType::RBRACE)
    else MATCH_KEYWORD('[' ,TokenType::LBRACKET)
    else MATCH_KEYWORD(']' ,TokenType::RBRACKET)
    else MATCH_KEYWORD(';' ,TokenType::SEMICOLON)
    else MATCH_KEYWORD(',' ,TokenType::COMMA)
    else MATCH_KEYWORD('.' ,TokenType::DOT)
    else MATCH_KEYWORD('?' ,TokenType::QUESTION)
    else MATCH_KEYWORD('=' ,TokenType::ASSIGNMENT)
    else MATCH_KEYWORD('>' ,TokenType::GT)
    else MATCH_KEYWORD('<' ,TokenType::LT)
    else MATCH_KEYWORD('!' ,TokenType::NOT)
    else MATCH_KEYWORD('~' ,TokenType::TILDE)
    else MATCH_KEYWORD(':' ,TokenType::COLON)
    else MATCH_KEYWORD('+' ,TokenType::ADD)
    else MATCH_KEYWORD('-' ,TokenType::SUB)
    else MATCH_KEYWORD('*' ,TokenType::MULT)
    else MATCH_KEYWORD('/' ,TokenType::DIV)
    else MATCH_KEYWORD('^' ,TokenType::XOR)
    else MATCH_KEYWORD('%' ,TokenType::MOD)
    return false;
}

bool IsIdentifierCharacter(char c) {
    return c == '_' || std::isalnum(c);
}

void Lexer::Expect(char c, std::string errorMessage) {
    if (!iterator.Match(c)) {
        throw SeaError(fileName, iterator.GetLine(), iterator.GetCol(), errorMessage);
    }
}

char Lexer::Next(std::string errorMessage) {
    if (iterator.IsEnd()) {
        throw SeaError(fileName, iterator.GetLine(), iterator.GetCol(), errorMessage);
    }
    char c = iterator.Peek();
    iterator.Advance();
    return c;
}

bool Lexer::MatchCharLiteral() {
    if (iterator.Match('\'')) {
        size_t startLine = iterator.GetLastMatchLine();
        size_t startCol = iterator.GetLastMatchCol();
        if (iterator.Match('\\')) {
            char next = Next("Expected character to escape!");
            char actual;
            switch (next) {
                case 't': actual = '\t'; break;
                case 'b': actual = '\b'; break;
                case 'n': actual = '\n'; break;
                case 'r': actual = '\r'; break;
                case '\'': actual = '\''; break;
                case '"': actual = '"'; break;
                case '\\': actual = '\\'; break;
                default:
                    throw SeaError(fileName, startLine, startCol, "Unexpected escape sequence!");
            }
            tokenList.emplace_back(TokenType::CHAR_LITERAL,
                std::string(1, actual), fileName, startLine, startCol);
        }
        else {
            char next = Next("Expected character in character literal!");
            tokenList.emplace_back(TokenType::CHAR_LITERAL,
                std::string(1, next), fileName, startLine, startCol);
        }
        Expect('\'', "Expected closing \"'\" for char literal!");
        return true;
    }
    return false;
}

bool Lexer::MatchStringLiteral() {
    if (iterator.Match('"')) {
        size_t startLine = iterator.GetLastMatchLine();
        size_t startCol = iterator.GetLastMatchCol();
        std::string strLiteral;
        while (!iterator.IsEnd() && iterator.Peek() != '\n' && iterator.Peek() != '"') {
            char c = Next("???");
            if (c == '\\') {
                char next = Next("Expected character to escape!");
                switch (next) {
                    case 't': c = '\t'; break;
                    case 'b': c = '\b'; break;
                    case 'n': c = '\n'; break;
                    case 'r': c = '\r'; break;
                    case '\'': c = '\''; break;
                    case '"': c = '"'; break;
                    case '\\': c = '\\'; break;
                    default:
                        throw SeaError(fileName, startLine, startCol, "Unexpected escape sequence!");
                }
            }
            
            strLiteral += c;
        }
        Expect('"', "Expected closing '\"' for string literal!");
        tokenList.emplace_back(TokenType::STRING_LITERAL,
                    strLiteral, fileName, startLine, startCol);
        return true;
    }
    return false;
}

bool Lexer::MatchIdentifier() {
    if (std::isalpha(iterator.Peek())) {
        size_t startLine = iterator.GetLine();
        size_t startCol = iterator.GetCol();
        std::string value;
        while (!iterator.IsEnd() && IsIdentifierCharacter(iterator.Peek())) {
            value += iterator.Peek();
            iterator.Advance();
        }
        tokenList.emplace_back(TokenType::IDENTIFIER, value, fileName, startLine, startCol);
        return true;
    }
    return false;
}

bool Lexer::MatchNumber() {
    if (std::isdigit(iterator.Peek())) {
        size_t startLine = iterator.GetLine();
        size_t startCol = iterator.GetCol();
        std::string value;
        while (!iterator.IsEnd() && std::isdigit(iterator.Peek())) {
            value += iterator.Peek();
            iterator.Advance();
        }
        if (iterator.Match('.')) {
            // Decimal?
            value += iterator.GetLastMatch();
            while (!iterator.IsEnd() && std::isdigit(iterator.Peek())) {
                value += iterator.Peek();
                iterator.Advance();
            }
        }
        // PostFixes
        if (iterator.Match('f')) {
            // Float Designator
            value += iterator.GetLastMatch();
        }
        else if (iterator.Match('l')) {
            // Long Designator
            value += iterator.GetLastMatch();
        }
        else if (iterator.Match("ul")) {
            // unsigned long Designator
            value += iterator.GetLastMatch();
        }
        else if (iterator.Match("u")) {
            // unsigned Designator
            value += iterator.GetLastMatch();
        }
        tokenList.emplace_back(TokenType::NUMBER_LITERAL, value, fileName, startLine, startCol);
        return true;
    }
    return false;
}

bool Lexer::MatchComment() {
    if (iterator.Match("//")) {
        while (!iterator.IsEnd() && iterator.Peek() != '\n') {
            iterator.Advance();
        }
        return true;
    }
    return false;
}

bool Lexer::MatchWhitespace() {
    if (std::isspace(iterator.Peek())) {
        iterator.Advance();
        return true;
    }
    return false;
}


void Lexer::Lex() {
    while (!iterator.IsEnd()) {
        // LOG_DEBUG("Line " << (iterator.GetLine() + 1) <<
        //           " Col " << (iterator.GetCol() + 1));

        if (MatchComment()) { }
        else if (MatchWhitespace()) { }
        else if (MatchKeyword()) { }
        else if (MatchNumber()) { }
        else if (MatchStringLiteral()) { }
        else if (MatchCharLiteral()) { }
        else if (MatchIdentifier()) { }
        else {
            // LOG_DEBUG(ListToString(tokenList, "\n"));
            throw SeaError(fileName, iterator.GetLine(), iterator.GetCol(),
                "Unexpected character '" + std::string(1, iterator.Peek()) +
                "'!");
        }
    }
    tokenList.emplace_back(TokenType::END_OF_FILE, "", fileName, iterator.GetLine(), iterator.GetCol());
}

bool FileSourceIterator::IsEnd() {
    // Add a newline at the very end
    return line >= source.srcLines.size() - 1;
}

void FileSourceIterator::Advance() {
    if (IsEnd()) {
        throw GeneralError("Cannot advance past end!");
    }
    col += 1;
    if (col >= source.srcLines.at(line).size()) {
        col = 0;
        line += 1;
    }
}

char FileSourceIterator::Peek() {
    if (IsEnd()) {
        throw GeneralError("Cannot peek past end!");
    }
    return source.srcLines.at(line).at(col);
}


bool FileSourceIterator::Match(char c) {
    if (IsEnd()) return false;
    if (Peek() == c) {
        lastMatchLine = line;
        lastMatchCol = col;
        Advance();
        lastMatch = std::string(1, c);
        return true;
    }
    return false;
}

bool FileSourceIterator::Match(std::string str) {
    size_t save_line = line;
    size_t save_col = col;
    for (size_t i = 0; i < str.length(); i++) {
        if (!Match(str[i])) {
            // Error, rewind
            line = save_line;
            col = save_col;
            return false;
        }
    }
    lastMatch = str;
    lastMatchLine = save_line;
    lastMatchCol = save_col;
    return true;
}

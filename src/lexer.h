#pragma once

#include "Token.h"
#include "Error.h"
#include "CompilationContext.h"

#include <vector>
#include <string>
#include <fstream>
#include <streambuf>

class FileSourceIterator {
    const FileSource& source;
    std::string lastMatch;
    size_t lastMatchLine = 0;
    size_t lastMatchCol = 0;

    size_t line = 0;
    size_t col = 0;
public:
    FileSourceIterator(const FileSource& source) : source(source) {}

    bool IsEnd();
    void Advance();

    char Peek();
    bool Match(char c);
    bool Match(std::string str);

    std::string PeekWord();

    std::string GetLastMatch() { return lastMatch; }
    size_t GetLastMatchLine() { return lastMatchLine; }
    size_t GetLastMatchCol() { return lastMatchCol; }

    size_t GetLine() { return line; }
    size_t GetCol() { return col; }
};

class Lexer {
    std::vector<Token> tokenList;
    FileSourceIterator iterator;
    std::string fileName;

    void Expect(char c, std::string errorMessage);
    char Next(std::string errorMessage);

    bool MatchKeyword();
    bool MatchStringLiteral();
    bool MatchCharLiteral();
    bool MatchNumber();
    bool MatchIdentifier();
    bool MatchComment();
    bool MatchWhitespace();

public:
    Lexer(CompilationContext& context, const std::string& fileName) :
        iterator(context.files[fileName]),
        fileName(fileName) {

    }

    void Lex();

    const std::vector<Token>& GetTokenList() { return tokenList; }
};


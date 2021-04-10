#include "Parser.h"
#include "Error.h"

#define SEA_ERROR(msg) SeaError(tokenList[ptr].fileName, \
    tokenList[ptr].lineNumber, tokenList[ptr].colNumber, msg);

SeaType Parser::ParseType() {
    SeaType type;
    
    const Token& t = Expect(TokenType::IDENTIFIER, "Expected type");
    type.name.path.push_back(t.value);
    while (Match(TokenType::DOT)) {
        const Token& tok = Expect(TokenType::IDENTIFIER, "Expected next part of name");
        type.name.path.push_back(tok.value);
    }
    
    if (Match(TokenType::QUESTION)) {
        type.nullable = true;
    }
    return type;
}

Statement* Parser::ParseStatement() {
    Statement* outStatement;
    if (Match(TokenType::CONST) || Match(TokenType::LET)) {
        // Local Variable Declaration
        LocalVariableDeclarationStatement* stmt = MakeNode<LocalVariableDeclarationStatement>();
        stmt->isConstant = (lastMatch == TokenType::CONST);
        const Token& idTok = Expect(TokenType::IDENTIFIER, "Expected identifier in local variable declaration");
        stmt->id = idTok.value;

        if (Match(TokenType::COLON)) {
            // Declared Type
            stmt->type = ParseType();
        }

        if (Match(TokenType::ASSIGNMENT)) {
            stmt->initializer = ParseExpression();
        }
        outStatement = stmt;
    }
    else if (Match(TokenType::FOR)) {
        // For Loop
        ForStatement* stmt = MakeNode<ForStatement>();
        Expect(TokenType::LPAREN, "Expected '(' after 'for'");
        const Token& idTok = Expect(TokenType::IDENTIFIER, "Expected identifier for iterator name");
        stmt->iteratorId = idTok.value;
        if (Match(TokenType::COLON)) {
            // Declared Type
            stmt->iteratorType = ParseType();
        }
        Expect(TokenType::IN, "Expected 'in' after iterator declaration");
        stmt->rhs = ParseExpression();
        Expect(TokenType::RPAREN, "Expected ')'");
        stmt->body = ParseStatement();
        outStatement = stmt;
    }
    else if (Match(TokenType::WHILE)) {
        // While Loop
        WhileStatement* stmt = MakeNode<WhileStatement>();
        Expect(TokenType::LPAREN, "Expected '(' after 'while'");
        stmt->condition = ParseExpression();
        Expect(TokenType::RPAREN, "Expected ')'");
        stmt->body = ParseStatement();
        outStatement = stmt;
    }
    else if (Match(TokenType::RETURN)) {
        // Return Statement
        ReturnStatement* stmt = MakeNode<ReturnStatement>();
        if (Peek() != TokenType::SEMICOLON) {
            stmt->value = ParseExpression();
        }
        outStatement = stmt;
    }
    else if (Match(TokenType::LBRACE)) {
        // Block Statement
        BlockStatement* stmt = MakeNode<BlockStatement>();
        while (!Match(TokenType::RBRACE)) {
            stmt->statements.push_back(ParseStatement());
        }
        // Don't check for semicolon
        return stmt;
    }
    else {
        // Try Matching an expression statement
        ExpressionStatement* stmt = MakeNode<ExpressionStatement>();
        stmt->expression = ParseExpression();
        outStatement = stmt;
    }
    Expect(TokenType::SEMICOLON, "Expected ';' to denote end of statement");
    return outStatement;
}

FunctionDeclaration* Parser::ParseFunctionDeclaration() {
    // LOG_DEBUG("Parse Function Declaration");
    FunctionDeclaration* decl = MakeNode<FunctionDeclaration>();
    const Token& idToken = Expect(TokenType::IDENTIFIER, "Expected identifier");
    decl->id = idToken.value;
    Expect(TokenType::LPAREN, "Expected '(' to start argument list");
    if (!Match(TokenType::RPAREN)) {
        do {
            FunctionArgument fArg;
            fArg.argumentType = ParseType();
            const Token& argName = Expect(TokenType::IDENTIFIER, "Expected argument name");
            fArg.id = argName.value;
            decl->arguments.push_back(fArg);
        }
        while (Match(TokenType::COMMA));
        Expect(TokenType::RPAREN, "Expected ')' to close argument list");
    }
    
    if (Match(TokenType::COLON)) {
        decl->returnType = ParseType();
    }

    decl->body = ParseStatement();
    return decl;
}

ClassDeclaration* Parser::ParseClassDeclaration() {
    // LOG_DEBUG("Parse Class Declaration");
    ClassDeclaration* decl = MakeNode<ClassDeclaration>();
    const Token& idToken = Expect(TokenType::IDENTIFIER, "Expected identifier");
    decl->id = idToken.value;
    
    Expect(TokenType::LBRACE, "Expected class body declarations!");
    while (!IsEnd() && Peek() == TokenType::RBRACE) {
        decl->declarations.push_back(ParseFunctionDeclaration());
    }
    Expect(TokenType::RBRACE, "Expected '}'!");
    return decl;
}

InterfaceDeclaration* Parser::ParseInterfaceDeclaration() {
    // LOG_DEBUG("Parse Interface Declaration");
    InterfaceDeclaration* decl = MakeNode<InterfaceDeclaration>();
    const Token& idToken = Expect(TokenType::IDENTIFIER, "Expected identifier");
    decl->id = idToken.value;
    
    Expect(TokenType::LBRACE, "Expected class body declarations!");
    while (!IsEnd() && Peek() == TokenType::RBRACE) {
        decl->declarations.push_back(ParseFunctionDeclaration());
    }
    Expect(TokenType::RBRACE, "Expected '}'!");
    return decl;
}

    
Declaration* Parser::ParseDeclaration() {
    // LOG_DEBUG("Parse Declaration");
    bool isPublic = false;
    if (Match(TokenType::PUBLIC)) {
        isPublic = true;
    }
    Declaration* decl = nullptr;
    if (Match(TokenType::FUNCTION)) {
        decl = ParseFunctionDeclaration();
    }
    else if (Match(TokenType::CLASS)) {
        decl = ParseClassDeclaration();
    }
    else if (Match(TokenType::INTERFACE)) {
        decl = ParseInterfaceDeclaration();
    }
    else {
        throw SEA_ERROR("Could not parse declaration!");
    }
    decl->isPublic = isPublic;
    return decl;
}

void Parser::ParseCompilationUnit() {
    while (!IsEnd()) {
        if (Match(TokenType::IMPORT)) {
            const Token& token = Expect(TokenType::STRING_LITERAL,
                "Expected string literal after import");
            ImportDeclaration decl;
            decl.import = Name(token.value);
            compilationUnit->imports.push_back(decl);
        }
        else {
            compilationUnit->declarations.push_back(
                ParseDeclaration()
            );
        }
    }
}

std::unique_ptr<CompilationUnit> Parser::Parse() {
    compilationUnit = std::make_unique<CompilationUnit>();
    ParseCompilationUnit();
    return std::move(compilationUnit);
}


const Token& Parser::Expect(TokenType c, std::string errorMessage) {
    if (!Match(c)) {
        throw SEA_ERROR(errorMessage);
    }
    return tokenList[ptr - 1];
}

const Token& Parser::Next(std::string errorMessage) {
    if (IsEnd()) {
        throw SEA_ERROR(errorMessage);
    }
    return tokenList[ptr++];
}

TokenType Parser::Peek() {
    if (IsEnd()) {
        throw GeneralError("Tried to peek past the end!");
    }
    return tokenList[ptr].type;
}

bool Parser::Match(TokenType type) {
    if (IsEnd()) {
        return false;
    }
    if (Peek() == type) {
        lastMatch = type;
        ptr++;
        return true;
    }
    return false;
}

bool Parser::IsEnd() {
    // The last token is an EOF token!
    return ptr >= tokenList.size() - 1;
}
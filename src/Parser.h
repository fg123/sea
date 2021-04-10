#pragma once

#include "AST.h"
#include "Token.h"

class Parser {
    std::unique_ptr<CompilationUnit> compilationUnit;

    TokenType lastMatch;

    const std::vector<Token>& tokenList;
    size_t ptr = 0;

    const Token& Expect(TokenType c, std::string errorMessage);
    const Token& Next(std::string errorMessage);
    TokenType Peek();
    bool Match(TokenType type);
    bool IsEnd();

    SeaType ParseType();
    Expression* MakeBinary(Expression* lhs, Expression* rhs, BinaryExpression::Operator op);
    
    Expression* ParsePrimaryExpression();
    Expression* ParseUnaryExpression();
    Expression* ParseAsExpression();
    Expression* ParseMultiplicativeExpression();
    Expression* ParseAdditiveExpression();
    Expression* ParseRangeExpression();
    Expression* ParseInfixFunctionCall();
    Expression* ParseElvisExpression();
    Expression* ParseInfixOperation();
    Expression* ParseComparison();
    Expression* ParseEquality();
    Expression* ParseConjunction();
    Expression* ParseDisjunction();

    Expression* ParseExpression();
    Statement* ParseStatement();

    FunctionDeclaration* ParseFunctionDeclaration();
    ClassDeclaration* ParseClassDeclaration();
    InterfaceDeclaration* ParseInterfaceDeclaration();

    Declaration* ParseDeclaration();
    void ParseCompilationUnit();

    template<class T>
    T* MakeNode() {
        static_assert(std::is_base_of<ASTNode, T>::value, "T must inherit from ASTNode");
        T* node = new T;
        node->fileName = tokenList[ptr].fileName;
        node->lineNumber = tokenList[ptr].lineNumber;
        node->colNumber = tokenList[ptr].colNumber;

        compilationUnit->allNodes.insert(node);
        return node;
    }

public:
    Parser(const std::vector<Token>& tokenList) :
        tokenList(tokenList) {}
    
    std::unique_ptr<CompilationUnit> Parse();
};
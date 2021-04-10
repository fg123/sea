#pragma once

#include "Token.h"
#include "ASTBase.h"

class FunctionDeclaration;

struct Expression : public ASTNode {
    SeaType expressionType;
    AST_VISITOR_DECL
};

struct LiteralExpression : public Expression {
    Token token;
};

struct BinaryExpression : public Expression {
    enum class Operator {
        PLUS,       // +
        MINUS,      // -
        TIMES,      // *
        DIVIDE,     // /
        MODULO,     // %
        RANGE,      // ->
        GET,        // [ ]
        CONTAINS,   // in
        EQ,         // ==
        NEQ,        // !=
        LT,         // <
        GT,         // >
        LTE,        // <=
        GTE,        // >=
        DOT,        // .
        SAFE_DOT,   // ?.
        ELVIS,      // ?:
        OR,         // ||
        AND,        // &&
    } op;
    Expression* lhs = nullptr;
    Expression* rhs = nullptr;
    FunctionDeclaration* functionToCall = nullptr;

    AST_VISITOR_DEFN({
        if (!lhs->Accept(visitor)) return false;
        if (!rhs->Accept(visitor)) return false;
    })
};

struct BinaryTypeExpression : public Expression {
    enum class Operator {
        AS,
        IS,
        SAFE_AS
    } op;
    Expression* lhs = nullptr;
    SeaType rhs;
    FunctionDeclaration* functionToCall = nullptr;

    AST_VISITOR_DEFN({
        if (!lhs->Accept(visitor)) return false;
    })
};

struct InfixCallExpression : public Expression {
    Expression* lhs = nullptr;
    Expression* rhs = nullptr;

    std::string functionName;
    FunctionDeclaration* functionToCall = nullptr;

    AST_VISITOR_DEFN({
        if (!lhs->Accept(visitor)) return false;
    })
};

struct UnaryExpression : public Expression {
    enum class Operator {
        NEGATE,
        NOT,
        PRE_INC,
        PRE_DEC,
        POST_INC,
        POST_DEC
    } op;

    Expression* lhs = nullptr;
    FunctionDeclaration* functionToCall = nullptr;
    
    AST_VISITOR_DEFN({
        if (!lhs->Accept(visitor)) return false;
    })
};

struct IfExpression : public Expression {

};

struct WhenExpression : public Expression {

};

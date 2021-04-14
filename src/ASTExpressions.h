#pragma once

#include "Token.h"
#include "ASTBase.h"

struct FunctionDeclaration;
struct Statement;

struct LiteralExpression : public Expression {
    Token token;

    AST_VISITOR_DEFN({})
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
        AST_VISIT_CHILD(lhs)
    })
};

struct InfixCallExpression : public Expression {
    Expression* lhs = nullptr;
    Expression* rhs = nullptr;

    std::string functionName;
    FunctionDeclaration* functionToCall = nullptr;

    AST_VISITOR_DEFN({
        AST_VISIT_CHILD(lhs)
        AST_VISIT_CHILD(rhs)
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
        AST_VISIT_CHILD(lhs)
    })
};

struct IfExpression : public Expression {
    Expression* condition = nullptr;
    Statement* trueStatement = nullptr;
    Statement* falseStatement = nullptr;

    AST_VISITOR_DEFN({
        AST_VISIT_CHILD(condition)
        AST_VISIT_CHILD(trueStatement)
        AST_VISIT_CHILD(falseStatement)
    })
};

struct WhenEntry {
    std::vector<Expression*> tests;
    Statement* body;
};

struct WhenExpression : public Expression {
    std::string bindingId;
    Expression* subject = nullptr;

    std::vector<WhenEntry> entries;
    Statement* elseEntry = nullptr;

    AST_VISITOR_DEFN({
        AST_VISIT_CHILD(subject)
        for (auto& entry : entries) {
            for (auto& test : entry.tests) {
                AST_VISIT_CHILD(test)
            }
            AST_VISIT_CHILD(entry.body)
        }
        AST_VISIT_CHILD(elseEntry)
    })
};  

struct NameExpression : public Expression {
    std::string name;

    AST_VISITOR_DEFN({})
};

struct ThisExpression : public Expression {
    AST_VISITOR_DEFN({})
};  

struct SuperExpression : public Expression {
    AST_VISITOR_DEFN({})
};  

struct TryExpression : public Expression {
    // TODO: Exceptions
    AST_VISITOR_DEFN({})
};

struct CallExpression : public Expression {
    Expression* lhs = nullptr;
    std::vector<Expression*> args;

    AST_VISITOR_DEFN({
        AST_VISIT_CHILD(lhs)
        for (auto& arg : args) { 
            AST_VISIT_CHILD(arg)
        }
    })
};

struct NavigationExpression : public Expression {
    Expression* lhs = nullptr;
    std::string id;

    bool safeNavigation = false;
    
    AST_VISITOR_DEFN({
        AST_VISIT_CHILD(lhs)
    })
};
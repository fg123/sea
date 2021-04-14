#pragma once

#include "ASTBase.h"
#include "ASTExpressions.h"

#include <optional>

struct LocalVariableDeclarationStatement : public Statement {
    bool isConstant = false;

    std::optional<SeaType> type;
    std::string id;

    Expression* initializer = nullptr;
    
    AST_VISITOR_DEFN({
        if (!initializer->Accept(visitor)) return false;
    })
};

struct ExpressionStatement : public Statement {
    Expression* expression = nullptr;
    AST_VISITOR_DEFN({
        if (!expression->Accept(visitor)) return false;
    })
};

struct ForStatement : public Statement {
    std::optional<SeaType> iteratorType;
    std::string iteratorId;

    Expression* rhs = nullptr;

    Statement* body = nullptr;

    AST_VISITOR_DEFN({
        if (!rhs->Accept(visitor)) return false;
        if (!body->Accept(visitor)) return false;
    })
};

struct WhileStatement : public Statement {
    Expression* condition = nullptr;
    Statement* body = nullptr;

    AST_VISITOR_DEFN({
        if (!condition->Accept(visitor)) return false;
        if (!body->Accept(visitor)) return false;
    })
};

struct ReturnStatement : public Statement {
    Expression* value = nullptr;

    AST_VISITOR_DEFN({
        if (!value->Accept(visitor)) return false;
    })
};

struct BlockStatement : public Statement {
    std::vector<Statement*> statements;

    AST_VISITOR_DEFN({
        for (auto& statement : statements) {
            if (!statement->Accept(visitor)) return false;
        }
    })
};

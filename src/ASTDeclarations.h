#pragma once

#include "ASTBase.h"
#include "ASTType.h"

#include <vector>
#include <string>

#include "ASTExpressions.h"
#include "ASTStatements.h"

struct Declaration : public ASTNode {
    bool isPublic = false;
    AST_VISITOR_DECL
};

// Class or Interface, represents a "Type"
struct TypeDeclaration : public Declaration {};

struct FunctionArgument {
    SeaType argumentType;
    std::string id;
    Expression* defaultValue = nullptr;
};

struct FunctionDeclaration : public Declaration {
    bool isOperator;
    SeaType returnType;
    std::string id;
    std::vector<FunctionArgument> arguments;

    Statement* body;

    AST_VISITOR_DEFN({
        if (!body->Accept(visitor)) return false;
    });
};

struct ClassDeclaration : public TypeDeclaration {
    std::string id;
    std::vector<FunctionDeclaration*> declarations;
    AST_VISITOR_DEFN({
        for (auto& declaration : declarations) {
            if (!declaration->Accept(visitor)) return false;
        }
    })
};

struct InterfaceDeclaration : public TypeDeclaration {
    std::string id;
    std::vector<FunctionDeclaration*> declarations;
    AST_VISITOR_DEFN({
        for (auto& declaration : declarations) {
            if (!declaration->Accept(visitor)) return false;
        }
    })
};

struct ImportDeclaration {
    Name import;
};
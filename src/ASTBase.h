#pragma once

#include "ASTVisitor.h"
#include "ASTType.h"

struct ASTNode {
    ASTNode* parent = nullptr;
    std::string fileName;
    size_t lineNumber;
    size_t colNumber;
    virtual ~ASTNode() {}
};

struct Statement : public ASTNode {
    AST_VISITOR_DECL
};

struct Expression : public ASTNode {
    // Filled out by TypeResolver
    SeaType expressionType;
    
    virtual ~Expression() {}

    AST_VISITOR_DECL
};

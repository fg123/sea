#pragma once

#include "ASTVisitor.h"

struct ASTNode {
    ASTNode* parent = nullptr;
    std::string fileName;
    size_t lineNumber;
    size_t colNumber;
    virtual ~ASTNode() {}
};

#define AST_VISITOR_DECL virtual bool Accept(ASTVisitor* visitor) = 0;
#define AST_VISITOR_DEFN(...) virtual bool Accept(ASTVisitor* visitor) {       \
    visitor->currentNode = this;                                               \
    if (!visitor->PreVisit(this)) return false;                                \
    if (!visitor->skipVisitChildren) {                                         \
        visitor->PreVisitChildren();                                           \
        __VA_ARGS__                                                            \
        visitor->PostVisitChildren();                                          \
    }                                                                          \
    visitor->skipVisitChildren = false;                                        \
    if(!visitor->PostVisit(this)) return false;                                \
    return true;                                                               \
}

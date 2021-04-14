#pragma once

#include "Error.h"

// Declarations
struct CompilationUnit;
struct ASTNode;
struct ClassDeclaration;
struct InterfaceDeclaration;
struct FunctionDeclaration;

// Expressions
struct LiteralExpression;
struct BinaryExpression;
struct BinaryTypeExpression;
struct UnaryExpression;
struct IfExpression;
struct WhenExpression;
struct TryExpression;
struct InfixCallExpression;
struct SuperExpression;
struct ThisExpression;
struct NameExpression;
struct CallExpression;
struct NavigationExpression;

// Statements
struct WhileStatement;
struct ReturnStatement;
struct ForStatement;
struct BlockStatement;
struct LocalVariableDeclarationStatement;
struct ExpressionStatement;

#define AST_DECL_ACCEPTOR(type) \
    virtual bool PreVisit (type) { return true; } \
    virtual bool PostVisit(type) { return true; } \
    virtual bool PreVisitChild(type, unsigned int) { return true; } \
    virtual bool PostVisitChild(type, unsigned int) { return true; }


#define AST_VISITOR_DECL virtual bool Accept(ASTVisitor* visitor) = 0;
#define AST_VISITOR_DEFN(...) virtual bool Accept(ASTVisitor* visitor) {       \
    visitor->currentNode = this;                                               \
    if (!visitor->PreVisit(this)) return false;                                \
    if (!visitor->skipVisitChildren) {                                         \
        unsigned int childCounter = 0;                                               \
        UNUSED(childCounter);                                                  \
        visitor->PreVisitChildren();                                           \
        __VA_ARGS__                                                            \
        visitor->PostVisitChildren();                                          \
    }                                                                          \
    visitor->skipVisitChildren = false;                                        \
    if(!visitor->PostVisit(this)) return false;                                \
    return true;                                                               \
}

#define AST_VISIT_CHILD(field) {                                               \
    if (visitor->PreVisitChild(this, childCounter)) {                          \
        if ((field) && !(field)->Accept(visitor)) return false;                \
        visitor->PostVisitChild(this, childCounter);                           \
        childCounter += 1;                                                     \
    }                                                                          \
}

class ASTVisitor {
public:
    ASTNode* currentNode = nullptr;

    bool skipVisitChildren = false;

    void Start(CompilationUnit* base);

    virtual void PreVisitChildren() { }
    virtual void PostVisitChildren() { }

    SeaError VisitorError(const std::string& message);

    // Declarations
    AST_DECL_ACCEPTOR(CompilationUnit*)
    AST_DECL_ACCEPTOR(ClassDeclaration*)
    AST_DECL_ACCEPTOR(InterfaceDeclaration*)
    AST_DECL_ACCEPTOR(FunctionDeclaration*)

    // Expressions
    AST_DECL_ACCEPTOR(LiteralExpression*)
    AST_DECL_ACCEPTOR(BinaryExpression*)
    AST_DECL_ACCEPTOR(BinaryTypeExpression*)
    AST_DECL_ACCEPTOR(UnaryExpression*)
    AST_DECL_ACCEPTOR(IfExpression*)
    AST_DECL_ACCEPTOR(WhenExpression*)
    AST_DECL_ACCEPTOR(InfixCallExpression*)
    AST_DECL_ACCEPTOR(TryExpression*)
    AST_DECL_ACCEPTOR(ThisExpression*)
    AST_DECL_ACCEPTOR(SuperExpression*)
    AST_DECL_ACCEPTOR(NameExpression*)
    AST_DECL_ACCEPTOR(CallExpression*)
    AST_DECL_ACCEPTOR(NavigationExpression*)

    // Statements
    AST_DECL_ACCEPTOR(WhileStatement*)
    AST_DECL_ACCEPTOR(ReturnStatement*)
    AST_DECL_ACCEPTOR(ForStatement*)
    AST_DECL_ACCEPTOR(BlockStatement*)
    AST_DECL_ACCEPTOR(LocalVariableDeclarationStatement*)
    AST_DECL_ACCEPTOR(ExpressionStatement*)
};
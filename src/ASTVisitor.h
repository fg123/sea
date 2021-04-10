#pragma once

// Declarations
class CompilationUnit;
class ASTNode;
class ClassDeclaration;
class InterfaceDeclaration;
class FunctionDeclaration;

// Expressions
class BinaryExpression;
class BinaryTypeExpression;
class UnaryExpression;
class IfExpression;
class WhenExpression;

// Statements
class WhileStatement;
class ReturnStatement;
class ForStatement;
class BlockStatement;
class LocalVariableDeclarationStatement;
class ExpressionStatement;

class ASTVisitor {
public:
    ASTNode* currentNode = nullptr;

    bool skipVisitChildren = false;

    void Start(CompilationUnit* base);
    
    virtual void PreVisitChildren() {}
    virtual void PostVisitChildren() {}

    // Declarations
    virtual bool PreVisit (CompilationUnit*) { return true; }
    virtual bool PostVisit(CompilationUnit*) { return true; }
    virtual bool PreVisit (ClassDeclaration*) { return true; }
    virtual bool PostVisit(ClassDeclaration*) { return true; }
    virtual bool PreVisit (InterfaceDeclaration*) { return true; }
    virtual bool PostVisit(InterfaceDeclaration*) { return true; }
    virtual bool PreVisit (FunctionDeclaration*) { return true; }
    virtual bool PostVisit(FunctionDeclaration*) { return true; }

    // Expressions
    virtual bool PreVisit (BinaryExpression*) { return true; }
    virtual bool PostVisit(BinaryExpression*) { return true; }
    virtual bool PreVisit (BinaryTypeExpression*) { return true; }
    virtual bool PostVisit(BinaryTypeExpression*) { return true; }
    virtual bool PreVisit (UnaryExpression*) { return true; }
    virtual bool PostVisit(UnaryExpression*) { return true; }
    virtual bool PreVisit (IfExpression*) { return true; }
    virtual bool PostVisit(IfExpression*) { return true; }
    virtual bool PreVisit (WhenExpression*) { return true; }
    virtual bool PostVisit(WhenExpression*) { return true; }
    // virtual bool PreVisit(ClassInstanceCreationExpression*) { return true; }
    // virtual bool PostVisit(ClassInstanceCreationExpression*) { return true; }
    // virtual bool PreVisit(ArrayAccessExpression*) { return true; }
    // virtual bool PostVisit(ArrayAccessExpression*) { return true; }
    // virtual bool PreVisit(AmbiguousNameExpression*) { return true; }
    // virtual bool PostVisit(AmbiguousNameExpression*) { return true; }
    // virtual bool PreVisit(ThisExpression*) { return true; }
    // virtual bool PostVisit(ThisExpression*) { return true; }
    // virtual bool PreVisit(LiteralExpression*) { return true; }
    // virtual bool PostVisit(LiteralExpression*) { return true; }
    // virtual bool PreVisit(FieldAccessExpression*) { return true; }
    // virtual bool PostVisit(FieldAccessExpression*) { return true; }
    // virtual bool PreVisit(MethodInvocationExpression*) { return true; }
    // virtual bool PostVisit(MethodInvocationExpression*) { return true; }
    // virtual bool PreVisit(AssignmentExpression*) { return true; }
    // virtual bool PostVisit(AssignmentExpression*) { return true; }
    // virtual bool PreVisit(VariableExpression*) { return true; }
    // virtual bool PostVisit(VariableExpression*) { return true; }

    // Statements
    virtual bool PreVisit (WhileStatement*) { return true; }
    virtual bool PostVisit(WhileStatement*) { return true; }
    virtual bool PreVisit (ReturnStatement*) { return true; }
    virtual bool PostVisit(ReturnStatement*) { return true; }
    virtual bool PreVisit (ForStatement*) { return true; }
    virtual bool PostVisit(ForStatement*) { return true; }
    virtual bool PreVisit (BlockStatement*) { return true; }
    virtual bool PostVisit(BlockStatement*) { return true; }
    virtual bool PreVisit (LocalVariableDeclarationStatement*) { return true; }
    virtual bool PostVisit(LocalVariableDeclarationStatement*) { return true; }
    virtual bool PreVisit (ExpressionStatement*) { return true; }
    virtual bool PostVisit(ExpressionStatement*) { return true; }
    
};
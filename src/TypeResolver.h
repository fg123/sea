#pragma once

#include "ASTVisitor.h"
#include "CompilationContext.h"
#include "Error.h"
#include "AST.h"

#include <stack>
#include <string>
#include <unordered_map>

// This visitor is responsible for binding TypeDeclarations* to all 
//   SeaTypes, as well as inferring types about any expressions

struct Scope {
    std::unordered_map<std::string, TypeDeclaration*> types;
};

struct ScopeManager {
    std::vector<Scope> scopeStack;

    void PushScope() {
        scopeStack.emplace_back();
    }

    void PopScope() {
        scopeStack.pop_back();
    }

    bool RegisterName(const std::string& name, TypeDeclaration* type) {
        for (size_t i = 0; i < scopeStack.size(); i++) {
            if (scopeStack[i].types.find(name) !=
                scopeStack[i].types.end()) {
                return false;
            }
        }
        scopeStack.back().types[name] = type;
        return true;
    }

    TypeDeclaration* TypeOfName(const std::string& name) {
        for (size_t i = 0; i < scopeStack.size(); i++) {
            Scope& scope = scopeStack[scopeStack.size() - 1 - i];
            if (scope.types.find(name) != scope.types.end()) {
                return scope.types[name];
            }
        }
        return nullptr;
    }
};

class TypeResolver : public ASTVisitor {
    CompilationContext& context;

    CompilationUnit* currentCompilationUnit = nullptr;

    ScopeManager scopeManager;

    void LinkType(SeaType& type) {
        if (!type.IsNothing()) {
            LOG_WARN("Called LinkType on a type that is already linked!");
        }
        if (currentCompilationUnit->knownTypes.find(type.name.ToString()) ==
            currentCompilationUnit->knownTypes.end()) {
            throw VisitorError("Could not resolve type '" + type.name.ToString() + "'! Did you forget an import?");
        }
        type.type = currentCompilationUnit->knownTypes[type.name.ToString()];
    }

    TypeDeclaration* TypeOfName(const std::string& name) {
        TypeDeclaration* decl = scopeManager.TypeOfName(name);
        if (!decl) {
            throw VisitorError("Could not resolve name: " + name);
        }
        return decl;
    }

    void RegisterName(const std::string& name, TypeDeclaration* type) {
        if (!scopeManager.RegisterName(name, type)) {
            throw VisitorError("Duplicate declaration of name: " + name);
        }
    }

public:
    TypeResolver(CompilationContext& context) : context(context) {
        for (auto& unit : context.compilationUnits) {
            auto& knownTypes = unit.second->knownTypes;
            for (auto& selfDecl : unit.second->declarations) {
                TypeDeclaration* type = dynamic_cast<TypeDeclaration*>(selfDecl.second);
                if (!type) continue; // Ignore Function Declarations
                knownTypes[selfDecl.first] = type;
            }
            for (auto& import : unit.second->imports) {
                for (auto& importDecl : import.importedUnit->declarations) {
                    TypeDeclaration* type = dynamic_cast<TypeDeclaration*>(importDecl.second);
                    if (!type) continue; // Ignore Function Declarations
                    if (!type->isPublic) continue; // Ignore private classes
                    if (knownTypes.find(importDecl.first) !=
                        knownTypes.end()) {
                        throw SeaError(importDecl.second->fileName,
                            importDecl.second->lineNumber,
                            importDecl.second->colNumber,
                            "Conflicting declaration of '" + importDecl.first + "'!");
                    }
                    knownTypes[importDecl.first] = type;
                }
            }
        }
    }

    bool PreVisit(CompilationUnit* unit) override {
        currentCompilationUnit = unit;
        return true;
    }

    bool PreVisit(ClassDeclaration* decl) override {
        if (!decl->extendsType) {
            decl->extendsType = context.GetAnyType();
        }
        else {
            LinkType(*decl->extendsType);
        }
        for (auto& type : decl->implementsInterfaces) {
            LinkType(type);
        }
        return true;
    }

    bool PreVisit(InterfaceDeclaration* decl) override {
        for (auto& type : decl->extendsInterfaces) {
            LinkType(type);
        }
        return true;
    }

    bool PreVisit(FunctionDeclaration*) override {
        scopeManager.PushScope();
        return true;
    }

    bool PostVisit(FunctionDeclaration*) override {
        scopeManager.PopScope();
        return true;
    }

    bool PreVisit(LocalVariableDeclarationStatement* stmt) override {
        if (stmt->type) {
            LinkType(*stmt->type);
        }
        return true;
    }

    bool PostVisit(LocalVariableDeclarationStatement* stmt) override {
        if (!stmt->type) {
            if (!stmt->initializer) {
                throw VisitorError("Could not deduce type without an initializer!");
            }
            stmt->type = stmt->initializer->expressionType;
            if (stmt->type->IsNothing()) {
                throw VisitorError("Deduced type cannot be Nothing");
            }
        }
        RegisterName(stmt->id, stmt->type->type);
        return true;
    }

    // Expressions, fill out expressionType
    bool PostVisit(LiteralExpression* expr) override {
        switch (expr->token.type) {
            case TokenType::INT_LITERAL:
                expr->expressionType = context.GetPrimitiveInt();
                break;
            case TokenType::DOUBLE_LITERAL:
                expr->expressionType = context.GetPrimitiveDouble();
                break;
            case TokenType::FLOAT_LITERAL:
                expr->expressionType = context.GetPrimitiveFloat();
                break;
            case TokenType::LONG_LITERAL:
                expr->expressionType = context.GetPrimitiveLong();
                break;
            case TokenType::TRUE_LITERAL:
                expr->expressionType = context.GetPrimitiveBoolean();
                break;
            case TokenType::FALSE_LITERAL:
                expr->expressionType = context.GetPrimitiveBoolean();
                break;
            case TokenType::CHAR_LITERAL:
                expr->expressionType = context.GetPrimitiveChar();
                break;
            case TokenType::STRING_LITERAL:
                expr->expressionType = context.GetPrimitiveString();
                break;
            case TokenType::NULL_LITERAL:
                expr->expressionType = context.GetNullType();
                break;
            default:
                throw GeneralError("Invalid Literal Type " +
                    std::string(TokenTypeString[(int) expr->token.type]));
        }
        return true;
    }

    bool PostVisit(BinaryExpression* expr) override {
        return true;
    }

    bool PostVisit(BinaryTypeExpression* expr) override {
        if (expr->op == BinaryTypeExpression::Operator::AS) {
            expr->expressionType = expr->rhs;
        }
        else if (expr->op == BinaryTypeExpression::Operator::SAFE_AS) {
            expr->expressionType = expr->rhs;
            expr->expressionType.nullable = true;
        }
        else if (expr->op == BinaryTypeExpression::Operator::IS) {
            expr->expressionType = context.GetPrimitiveBoolean();
        }
        return true;
    }

    bool PostVisit(UnaryExpression* expr) override {
        return true;
    }

    bool PostVisit(IfExpression* expr) override {
        // Both branches must exist and be ExpressionStatement
        //   and must converge to the same base-type (could be Any)
        ExpressionStatement* trueStmt = dynamic_cast<ExpressionStatement*>(expr->trueStatement);
        ExpressionStatement* falseStmt = dynamic_cast<ExpressionStatement*>(expr->falseStatement);
        if (!trueStmt || !falseStmt) {
            expr->expressionType.SetNothing();
            return true;
        }

        expr->expressionType = context.GetBaseTypeBetween(
            trueStmt->expression->expressionType,
            falseStmt->expression->expressionType
        );
        return true;
    }

    bool PostVisit(WhenExpression* expr) override {
        return true;
    }

    bool PostVisit(InfixCallExpression* expr) override {
        return true;
    }

    bool PostVisit(TryExpression* expr) override {
        return true;
    }

    bool PostVisit(ThisExpression* expr) override {
        return true;
    }

    bool PostVisit(SuperExpression* expr) override {
        return true;
    }

    bool PostVisit(NameExpression* expr) override {
        expr->expressionType.type = TypeOfName(expr->name);
        return true;
    }

    bool PostVisit(CallExpression* expr) override {
        return true;
    }

    bool PostVisit(NavigationExpression* expr) override {
        TypeDeclaration* lhsType = expr->lhs->expressionType.type;
        if (!lhsType) {
            throw VisitorError("No type on lhs of Navigation Expression!");
        }
        TypeDeclaration* decl = lhsType->PerformNavigate(expr->id);
        if (!decl) {
            throw VisitorError("Invalid navigation on " + expr->id);
        }
        // TODO: function cases
        expr->expressionType = SeaType(decl);
        return true;
    }
};
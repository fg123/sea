#pragma once

#include "ASTBase.h"
#include "ASTType.h"

#include <vector>
#include <string>

#include "ASTExpressions.h"
#include "ASTStatements.h"

struct Declaration : public ASTNode {
    bool isPublic = false;

    virtual std::string GetId() const = 0;
    AST_VISITOR_DECL
};

// Class or Interface, represents a "Type"
struct TypeDeclaration : public Declaration {
    virtual TypeDeclaration* PerformNavigate(std::string navigate) = 0;
};

struct FunctionArgument {
    SeaType argumentType;
    std::string id;
    Expression* defaultValue = nullptr;
};

struct FunctionDeclaration : public TypeDeclaration {
    bool isOperator;
    SeaType returnType;
    std::string id;
    std::vector<FunctionArgument> arguments;

    Statement* body;
    
    std::string GetId() const override { return id; }

    AST_VISITOR_DEFN({
        if (!body->Accept(visitor)) return false;
    });

    TypeDeclaration* PerformNavigate(std::string) override {
        return nullptr;
    }
};

struct ClassDeclaration : public TypeDeclaration {
    std::string id;
    std::vector<FunctionDeclaration*> declarations;

    std::optional<SeaType> extendsType;
    std::vector<SeaType> implementsInterfaces;

    std::string GetId() const override { return id; }

    AST_VISITOR_DEFN({
        for (auto& declaration : declarations) {
            if (!declaration->Accept(visitor)) return false;
        }
    })

    TypeDeclaration* PerformNavigate(std::string navigate) override {
        for (auto& decl : declarations) {
            if (decl->id == navigate) {
                return decl;
            }
        }
        return nullptr;
    }
};

struct InterfaceDeclaration : public TypeDeclaration {
    std::string id;
    std::vector<FunctionDeclaration*> declarations;

    std::vector<SeaType> extendsInterfaces;

    std::string GetId() const override { return id; }

    AST_VISITOR_DEFN({
        for (auto& declaration : declarations) {
            if (!declaration->Accept(visitor)) return false;
        }
    })

    TypeDeclaration* PerformNavigate(std::string navigate) override {
        for (auto& decl : declarations) {
            if (decl->id == navigate) {
                return decl;
            }
        }
        return nullptr;
    }
};

struct ImportDeclaration {
    std::string import;
    
    // Filled out by Import Resolver
    CompilationUnit* importedUnit = nullptr;
};
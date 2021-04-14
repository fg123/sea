#pragma once

#include "ASTVisitor.h"
#include "CompilationContext.h"

#include <ostream>

class ASTPrinter : public ASTVisitor {
    size_t depth = 0;
    std::ostream& out;
public:
    virtual void PreVisitChildren() override {
        depth += 1;
    }

    virtual void PostVisitChildren() override {
        depth -= 1;
    }
    
    void PrintIndentation() {
        if (depth > 0) {
            for (size_t i = 0 ; i < depth - 1; i++) {
                out << "| ";
            }
            out << "`-";
        }
    }

    ASTPrinter(std::ostream& output) : out(output) { }

    virtual bool PreVisit(CompilationUnit* base) override {
        PrintIndentation();
        out << "Compilation unit in file " << base->fileName << std::endl;
        PrintIndentation();
        for (auto& import : base->imports) {
            out << "Import: " << import.import << std::endl;
        }
        return true;
    }
    
    virtual bool PreVisit(ClassDeclaration* base) override {
        PrintIndentation();
        out << "Class Declaration: " << base->id << std::endl;

        depth++;

        PrintIndentation();
        out << (base->isPublic ? "PUBLIC" : "") << std::endl;
        // if (base->extendsType.has_value()) {
        //     PrintIndentation();
        //     out << "Extends: " << (*base->extendsType).ToString() << std::endl;
        // }
        // for (auto& implement : base->implementsInterfaces) {
        //     PrintIndentation();
        //     out << "Implements: " << implement.ToString() << std::endl;
        // }

        depth--;
        return true;
    }

    virtual bool PreVisit(InterfaceDeclaration* base) override {
        PrintIndentation();
        out << "Interface Declaration: " << base->id << std::endl;

        depth++;

        PrintIndentation();
        out << (base->isPublic ? "PUBLIC" : "") << std::endl;
        // for (auto& implement : base->extendsInterfaces) {
        //     PrintIndentation();
        //     out << "Extends: " << implement.ToString() << std::endl;
        // }
        depth--;
        return true;
    }

    virtual bool PreVisit(FunctionDeclaration* base) override {
        PrintIndentation();
        out << "Function Declaration: " << base->id << std::endl;

        depth++;

        PrintIndentation();
        out << (base->isPublic ? "PUBLIC" : "") << std::endl;
        // for (auto& implement : base->extendsInterfaces) {
        //     PrintIndentation();
        //     out << "Extends: " << implement.ToString() << std::endl;
        // }
        depth--;
        return true;
    }


    virtual bool PreVisit(LiteralExpression*) {
        PrintIndentation();
        out << "LiteralExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(BinaryExpression*) {
        PrintIndentation();
        out << "BinaryExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(BinaryTypeExpression*) {
        PrintIndentation();
        out << "BinaryTypeExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(UnaryExpression*) {
        PrintIndentation();
        out << "UnaryExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(IfExpression*) {
        PrintIndentation();
        out << "IfExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(WhenExpression*) {
        PrintIndentation();
        out << "WhenExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(InfixCallExpression*) {
        PrintIndentation();
        out << "InfixCallExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(TryExpression*) {
        PrintIndentation();
        out << "TryExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(ThisExpression*) {
        PrintIndentation();
        out << "ThisExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(SuperExpression*) {
        PrintIndentation();
        out << "SuperExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(NameExpression*) {
        PrintIndentation();
        out << "NameExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(CallExpression*) {
        PrintIndentation();
        out << "CallExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(NavigationExpression*) {
        PrintIndentation();
        out << "NavigationExpression" << std::endl;
        return true;
    }

    virtual bool PreVisit(WhileStatement*) {
        PrintIndentation();
        out << "WhileStatement" << std::endl;
        return true;
    }

    virtual bool PreVisit(ReturnStatement*) {
        PrintIndentation();
        out << "ReturnStatement" << std::endl;
        return true;
    }

    virtual bool PreVisit(ForStatement*) {
        PrintIndentation();
        out << "ForStatement" << std::endl;
        return true;
    }

    virtual bool PreVisit(BlockStatement*) {
        PrintIndentation();
        out << "BlockStatement" << std::endl;
        return true;
    }

    virtual bool PreVisit(LocalVariableDeclarationStatement*) {
        PrintIndentation();
        out << "LocalVariableDeclarationStatement" << std::endl;
        return true;
    }

    virtual bool PreVisit(ExpressionStatement*) {
        PrintIndentation();
        out << "ExpressionStatement" << std::endl;
        return true;
    }

};
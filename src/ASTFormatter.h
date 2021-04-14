#pragma once

#include "AST.h"
#include "ASTVisitor.h"
#include "CompilationContext.h"

class ASTFormatter : public ASTVisitor {
    size_t indentation = 0;
    std::ostream& output;
public:
    ASTFormatter(CompilationContext& context, std::ostream& output) : output(output) {
        for (auto& unit : context.compilationUnits) {
            Start(unit.second.get());
        }
    }
    
    bool PreVisit(CompilationUnit* unit) {
        for (auto& import : unit->imports) {
            output << "import " << import.import << "\n";
        }
        output << "\n";
        return true;
    }
    
    bool PreVisit(ClassDeclaration* decl) {
        return true;
    }
    
    bool PreVisit(InterfaceDeclaration*) {
        return true;
    }
    
    bool PreVisit(FunctionDeclaration* ) {
        return true;
    }
    
};
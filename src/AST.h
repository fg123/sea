#pragma once

#include <memory>
#include <vector>
#include <string>

#include "ASTBase.h"
#include "ASTType.h"
#include "ASTExpressions.h"
#include "ASTStatements.h"
#include "ASTDeclarations.h"

struct CompilationUnit : public ASTNode {
    // Owns Everything
    std::unordered_set<ASTNode*> allNodes;
    std::unordered_map<std::string, Declaration*> declarations;
    std::vector<ImportDeclaration> imports;

    std::unordered_map<std::string, TypeDeclaration*> knownTypes;

    CompilationUnit() {}

    ~CompilationUnit() {        
        for (ASTNode* node : allNodes) {
            delete node;
        }
    }

    AST_VISITOR_DEFN({
        for (auto& declaration : declarations) {
            if (!declaration.second->Accept(visitor)) return false;
        }
    })
};
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
    std::vector<Declaration*> declarations;
    std::vector<ImportDeclaration> imports;

    CompilationUnit() {}

    ~CompilationUnit() {        
        for (ASTNode* node : allNodes) {
            delete node;
        }
    }

    AST_VISITOR_DEFN({
        for (auto& declaration : declarations) {
            if (!declaration->Accept(visitor)) return false;
        }
    })
};
#include "ASTVisitor.h"
#include "AST.h"

void ASTVisitor::Start(CompilationUnit* base) {
    base->Accept(this);
}

SeaError ASTVisitor::VisitorError(const std::string& message) {
    return SeaError(currentNode->fileName,
        currentNode->lineNumber,
        currentNode->colNumber,
        message);
}
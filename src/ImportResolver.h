#pragma once

#include "ASTVisitor.h"
#include "CompilationContext.h"
#include "PathTools.h"

// Resolves Imports
class ImportResolver {
    CompilationContext& context;

public:
    ImportResolver(CompilationContext& context) : context(context) {
        for (auto& unit : context.compilationUnits) {
            std::string directory = PathTools::ContainingDir(unit.second->fileName);
            for (auto& import : unit.second->imports) {
                std::string newPath = PathTools::Join(directory, import.import);
                import.importedUnit = context.GetCompilationUnitByPath(newPath);
            }
            unit.second->knownTypes["Int"] = context.GetPrimitiveInt().type;
            unit.second->knownTypes["Byte"] = context.GetPrimitiveByte().type;
            unit.second->knownTypes["Short"] = context.GetPrimitiveShort().type;
            unit.second->knownTypes["Long"] = context.GetPrimitiveLong().type;
            unit.second->knownTypes["Float"] = context.GetPrimitiveFloat().type;
            unit.second->knownTypes["Double"] = context.GetPrimitiveDouble().type;
            unit.second->knownTypes["Boolean"] = context.GetPrimitiveBoolean().type;
            unit.second->knownTypes["Char"] = context.GetPrimitiveChar().type;
            unit.second->knownTypes["String"] = context.GetPrimitiveString().type;
            unit.second->knownTypes["Any"] = context.GetAnyType().type;
        }
    }
};
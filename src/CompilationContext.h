#pragma once 

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

#include "Parser.h"
#include "ASTPrinter.h"

enum class SeaOption : int {
    PRINT_TOKENS,
    TRACE_DFA_LEXER,
    PRINT_AST,
    PRINT_PARSE_TREE,
    TRACE_SHIFT_REDUCE_PARSER,
    OUTPUT_HIERARCHY_RESOLUTION,
    COUNT 
};

struct FileSource {
    std::vector<std::string> srcLines;
};

class CompilationContext {
    std::unordered_map<SeaOption, bool> options;

    TypeDeclaration* GetPrimitive(const std::string& str) {
        Declaration* decl = compilationUnits[str + ".sea"].get()->declarations[str];
        TypeDeclaration* tdecl = dynamic_cast<TypeDeclaration*>(decl);
        if (!tdecl) {
            throw GeneralError("Primitive " + str + " from standard library is invalid!");
        }
        return tdecl;
    }
public:
    std::unordered_map<std::string, std::unique_ptr<CompilationUnit>> compilationUnits;
    std::map<std::string, FileSource> files;

    CompilationContext() {}

    void AddSourceFile(const std::string& fileName);

    void PrintSourceForError(const std::string& fileName, size_t line, size_t col);

    void SetOption(SeaOption option) {
        options[option] = true;
    }
    
    bool GetOption(SeaOption option) {
        return options[option];
    }

    void DebugPrintAllAst(std::ostream& output) {
        for (auto& unit : compilationUnits) {
            output << "AST For " << unit.second->fileName << "\n";
            ASTPrinter(output).Start(unit.second.get());
        }
    }

    CompilationUnit* GetCompilationUnitByPath(const std::string&);
    // void DebugPrintHierarchyResolution(TypeDeclaration* type);

    SeaType GetPrimitiveInt() {
        return SeaType(GetPrimitive("Int"), true);
    }
    SeaType GetPrimitiveByte() {
        return SeaType(GetPrimitive("Byte"), true);
    }
    SeaType GetPrimitiveShort() {
        return SeaType(GetPrimitive("Short"), true);
    }
    SeaType GetPrimitiveLong() {
        return SeaType(GetPrimitive("Long"), true);
    }
    SeaType GetPrimitiveFloat() {
        return SeaType(GetPrimitive("Float"), true);
    }
    SeaType GetPrimitiveDouble() {
        return SeaType(GetPrimitive("Double"), true);
    }
    SeaType GetPrimitiveBoolean() {
        return SeaType(GetPrimitive("Boolean"), true);
    }
    SeaType GetPrimitiveChar() {
        return SeaType(GetPrimitive("Char"), true);
    }
    SeaType GetPrimitiveString() {
        return SeaType(GetPrimitive("String"), true);
    }
    SeaType GetAnyType() {
        return SeaType(GetPrimitive("Any"));
    }

    SeaType GetNullType() {
        SeaType type;
        type.SetNothing();
        type.nullable = true;
        type.isPrimitive = true;
        return type;
    }

    SeaType GetBaseTypeBetween(const SeaType& typeA, const SeaType& typeB) {
        throw GeneralError("Not Implemented");
        // return SeaType;
    }
};
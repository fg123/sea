#pragma once 

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

// #include "Parser.h"

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

public:
    // std::vector<std::unique_ptr<CompilationUnit>> compilationUnits;
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
    
    // void DebugPrintHierarchyResolution(TypeDeclaration* type);
};
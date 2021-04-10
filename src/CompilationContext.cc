#include "CompilationContext.h"

#include "Error.h"
#include "Util.h"

#include <fstream>
#include <iomanip>
#include <iostream>

void CompilationContext::AddSourceFile(const std::string& fileName) {
    if (files.find(fileName) != files.end()) {
        throw GeneralError("Duplicate source file: " + fileName);
    }
    std::ifstream stream { fileName };
    if (!stream.good()) {
        throw GeneralError("Could not open " + fileName);
    }
    std::string line;
    FileSource& file = files[fileName];

    while (std::getline(stream, line)) {
        file.srcLines.push_back(line + "\n");
    }
    file.srcLines.push_back("\n");
}

void CompilationContext::PrintSourceForError(const std::string& fileName, size_t line, size_t col) {
    if (files.find(fileName) == files.end()) {
        LOG_ERROR("Could not find source file " + fileName + "!");
        return;
    }
    auto& srcLines = files[fileName].srcLines;
    // Show -2 +2 lines of context around erroneous line
    size_t startLine = std::max((int) line - 2, 0);
    size_t endLine = std::min(startLine + 5, srcLines.size());
    std::cerr << fileName << ":" << std::endl;
    for (size_t i = startLine; i < endLine; i++) {
        // SrcLine includes newline
        std::cerr << std::right << std::setw(6) << (i + 1) << " | " << srcLines[i];
        if (i == line) {
            // Mirrored 
            std::string spaceString = "";
            for (size_t j = 0; j < col; j++) {
                if (srcLines[i][j] == '\t') spaceString += "\t";
                else spaceString += " ";
            }
            std::cerr << std::string(6, ' ') << " | " << spaceString << "^--" << std::endl;
        }
    }
}

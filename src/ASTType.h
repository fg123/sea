#pragma once

#include "Util.h"

class TypeDeclaration;

struct Name {
    std::vector<std::string> path;

    Name() {}
    Name(std::string str) {
        path.push_back(str);
    }

    std::string ToString() {
        return ListToString(path, ".");
    }
};

struct SeaType {
    Name name;
    bool nullable = false;
    TypeDeclaration* type = nullptr;
    SeaType() {}
    SeaType(Name name, bool nullable) : name(name), nullable(nullable) {}

    std::string ToString() {
        return name.ToString() + (nullable ? "?" : "");
    }
};

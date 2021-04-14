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
    TypeDeclaration* type = nullptr;
    bool isPrimitive = false;
    bool nullable = false;
    SeaType() {}
    explicit SeaType(TypeDeclaration* type) : SeaType(type, false) {}
    SeaType(TypeDeclaration* type, bool isPrimitive) : type(type), isPrimitive(isPrimitive) {}
    SeaType(Name name, bool nullable) : name(name), nullable(nullable) {}

    std::string ToString() {
        return name.ToString() + (nullable ? "?" : "");
    }

    bool IsNothing() {
        return type == nullptr;
    }

    void SetNothing() {
        type = nullptr;
    }
};

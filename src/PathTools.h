#pragma once
#include <string>

// Tools for Resolving Paths

struct PathTools {
    static std::string Basename(const std::string& path);
    static std::string ContainingDir(const std::string& path);
    static std::string Join(const std::string& p1, const std::string& p2);
};

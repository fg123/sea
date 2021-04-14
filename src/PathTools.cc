#include "PathTools.h"

#include <filesystem>

std::string PathTools::Basename(const std::string& path) {
    return std::filesystem::path(path).filename().string();
}

std::string PathTools::ContainingDir(const std::string& path) {
    return std::filesystem::path(path).parent_path().string();
}

std::string PathTools::Join(const std::string& p1, const std::string& p2) {
    return (std::filesystem::path(p1) / std::filesystem::path(p2)).string();
}


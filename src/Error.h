#pragma once

#include <exception>
#include <sstream>
#include <string>

class SeaError : std::exception {
  std::string fullMessage;

public:
  std::string fileName;
  size_t line;
  size_t col;

  SeaError(std::string file, size_t line, size_t col, std::string message) :
    fileName(file), line(line), col(col) {
    std::ostringstream stream;
    stream << file << ":" << (line + 1) << "L:" << (col + 1) << "C: " << message;
    fullMessage = stream.str();
  }

  virtual const char *what() const noexcept { return fullMessage.c_str(); }
};

using GeneralError = std::runtime_error;
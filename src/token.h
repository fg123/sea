#pragma once

#include "Util.h"

#define FOREACH_TOKEN(OP)               \
  OP(WHITESPACE),                       \
  OP(COMMENT),                          \
  OP(IDENTIFIER),                       \
  /* Keywords */                        \
  OP(BREAK),                            \
  OP(CASE),                             \
  OP(CATCH),                            \
  OP(CLASS),                            \
  OP(CONST),                            \
  OP(CONTINUE),                         \
  OP(DO),                               \
  OP(ELSE),                             \
  OP(EXTENDS),                          \
  OP(FINAL),                            \
  OP(FINALLY),                          \
  OP(FLOAT),                            \
  OP(FOR),                              \
  OP(IF),                               \
  OP(IMPLEMENTS),                       \
  OP(IMPORT),                           \
  OP(IS),                               \
  OP(INTERFACE),                        \
  OP(NEW),                              \
  OP(PUBLIC),                           \
  OP(RETURN),                           \
  OP(STATIC),                           \
  OP(SUPER),                            \
  OP(SWITCH),                           \
  OP(THIS),                             \
  OP(THROW),                            \
  OP(THROWS),                           \
  OP(TRY),                              \
  OP(VOID),                             \
  OP(WHILE),                            \
  /* Literals */                        \
  OP(NUMBER_LITERAL),                   \
  OP(TRUE_LITERAL),                     \
  OP(FALSE_LITERAL),                    \
  OP(CHAR_LITERAL),                     \
  OP(STRING_LITERAL),                   \
  OP(NULL_LITERAL),                     \
  /* Seperator */                       \
  OP(LPAREN),                           \
  OP(RPAREN),                           \
  OP(LBRACE),                           \
  OP(RBRACE),                           \
  OP(LBRACKET),                         \
  OP(RBRACKET),                         \
  OP(SEMICOLON),                        \
  OP(COMMA),                            \
  OP(DOT),                              \
  /* Operators */                       \
  OP(QUESTION),                         \
  OP(ASSIGNMENT),                       \
  OP(GT),                               \
  OP(LT),                               \
  OP(NOT),                              \
  OP(TILDE),                            \
  OP(COLON),                            \
  OP(EQ),                               \
  OP(GE),                               \
  OP(LE),                               \
  OP(NE),                               \
  OP(AND),                              \
  OP(OR),                               \
  OP(INC),                              \
  OP(DEC),                              \
  OP(ADD),                              \
  OP(SUB),                              \
  OP(MULT),                             \
  OP(DIV),                              \
  OP(XOR),                              \
  OP(MOD),                              \
  OP(ADD_EQ),                           \
  OP(SUB_EQ),                           \
  OP(MULT_EQ),                          \
  OP(DIV_EQ),                           \
  OP(MOD_EQ),                           \
  /* This must be at the end */         \
  OP(__COUNT)
// clang-format on

enum class TokenType : int { FOREACH_TOKEN(ENUM) };

extern const char *TokenTypeString[];
TokenType TokenStringToType(const std::string str);

struct Token {
  TokenType type;
  std::string value;

  std::string fileName;
  size_t lineNumber;
  size_t colNumber;

  Token() {}
  Token(TokenType type, std::string value, std::string fileName,
        size_t lineNumber, size_t colNumber)
      : type(type), value(std::move(value)), fileName(std::move(fileName)),
        lineNumber(lineNumber), colNumber(colNumber) {}
    
  std::string ToString() {
    return TokenTypeString[type] + ", " + value;
  }
};
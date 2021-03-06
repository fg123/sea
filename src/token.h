#pragma once

#include "Util.h"

#define FOREACH_TOKEN(OP)               \
  OP(END_OF_FILE),                      \
  OP(IDENTIFIER),                       \
  /* Keywords */                        \
  OP(BREAK),                            \
  OP(CASE),                             \
  OP(CATCH),                            \
  OP(CLASS),                            \
  OP(CONST),                            \
  OP(LET),                              \
  OP(CONTINUE),                         \
  OP(DO),                               \
  OP(ELSE),                             \
  OP(EXTENDS),                          \
  OP(FINAL),                            \
  OP(FINALLY),                          \
  OP(FUNCTION),                         \
  OP(FOR),                              \
  OP(IF),                               \
  OP(IN),                               \
  OP(AS),                               \
  OP(SAFE_AS),                          \
  OP(IMPLEMENTS),                       \
  OP(IMPORT),                           \
  OP(IS),                               \
  OP(INTERFACE),                        \
  OP(NEW),                              \
  OP(PUBLIC),                           \
  OP(RETURN),                           \
  OP(OPERATOR),                         \
  OP(STATIC),                           \
  OP(NATIVE),                           \
  OP(SUPER),                            \
  OP(SWITCH),                           \
  OP(THIS),                             \
  OP(THROW),                            \
  OP(THROWS),                           \
  OP(TRY),                              \
  OP(VOID),                             \
  OP(WHILE),                            \
  OP(WHEN),                             \
  /* Literals */                        \
  OP(INT_LITERAL),                      \
  OP(DOUBLE_LITERAL),                   \
  OP(FLOAT_LITERAL),                    \
  OP(LONG_LITERAL),                     \
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
  OP(SAFE_DOT),                         \
  /* Operators */                       \
  OP(RANGE),                            \
  OP(QUESTION),                         \
  OP(ELVIS),                            \
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

    std::string ToString() const {
        return std::string(TokenTypeString[(int)type]) + ", " + value;
    }
};
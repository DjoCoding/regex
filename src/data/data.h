#ifndef DATA_H
#define DATA_H

#include <src/token/token.h>

extern const char *TokenKindToString[TOKEN_KIND_COUNT];

typedef struct {
    char c;
    TokenKind kind;
} CharToTokenKind;

#define QUANTIFIER_COUNT 4
#define SYMBOL_COUNT 2

extern const CharToTokenKind CharToQuantifierTokenKindMap[QUANTIFIER_COUNT];
extern const CharToTokenKind CharToSymbolTokenKindMap[SYMBOL_COUNT];

#endif
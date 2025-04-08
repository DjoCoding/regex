#ifndef DATA_H
#define DATA_H

#include <src/token/token.h>
#include <src/node/node.h>

#define QUANTIFIER_COUNT 3
#define SYMBOL_COUNT 7
#define PRINTABLE_CHAR_COUNT 69


// Tokens
extern const char *TokenKindToString[TOKEN_KIND_COUNT];

typedef struct {
    char c;
    TokenKind kind;
} CharToTokenKind;


extern const CharToTokenKind CharToQuantifierTokenKindMap[QUANTIFIER_COUNT];
extern const CharToTokenKind CharToSymbolTokenKindMap[SYMBOL_COUNT];

// Nodes
extern const char *NodeToString[NODE_KIND_COUNT];


// Tokens-Nodes
typedef struct {
    TokenKind kind;
    QuantifierKind q;
} TokenKindToQuantifierKind;

extern const TokenKindToQuantifierKind TokenKindToQuantifierKindMap[QUANTIFIER_COUNT];


// Chars
typedef struct {
    char c;
    char *s;
} CharToStringFormat;

extern const CharToStringFormat PrintableCharToStringFormatMap[PRINTABLE_CHAR_COUNT];

#endif
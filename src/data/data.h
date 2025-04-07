#ifndef DATA_H
#define DATA_H

#include <src/token/token.h>
#include <src/node/node.h>

// Tokens
#define QUANTIFIER_COUNT 4
#define SYMBOL_COUNT 3

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
    Quantifier q;
} TokenKindToQuantifier;

extern const TokenKindToQuantifier TokenKindToQuantifierMap[QUANTIFIER_COUNT];

#endif
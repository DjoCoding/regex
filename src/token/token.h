#ifndef TOKEN_H
#define TOKEN_H

#include <src/sv/sv.h>
#include <src/da/da.h>

typedef enum {
    TOKEN_KIND_END = 0,
    TOKEN_KIND_UNIT,
    TOKEN_KIND_OPEN_PAREN,
    TOKEN_KIND_CLOSE_PAREN,
    TOKEN_KIND_OPERATOR_OR,
    TOKEN_KIND_QUANTIFIER_ONE_OR_MORE,
    TOKEN_KIND_QUANTIFIER_ZERO_OR_MORE,
    TOKEN_KIND_QUANTIFIER_ZERO_OR_ONE,
    TOKEN_KIND_QUANTIFIER_ONE,
    TOKEN_KIND_COUNT
} TokenKind;

typedef struct {
    TokenKind kind;
    SV value;
} Token;

DA(Token, TokenList);

Token token_new(TokenKind kind, SV value);
void token_dump(Token this);

#endif
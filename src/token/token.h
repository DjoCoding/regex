#ifndef TOKEN_H
#define TOKEN_H

#include <src/sv/sv.h>
#include <src/list/list.h>

typedef enum {
    TOKEN_KIND_END = 0,
    TOKEN_KIND_UNIT,
    TOKEN_KIND_ANY_UNIT,
    TOKEN_KIND_OPEN_PAREN,
    TOKEN_KIND_CLOSE_PAREN,
    TOKEN_KIND_OPERATOR_OR,
    TOKEN_KIND_OPEN_CURLY,
    TOKEN_KIND_CLOSE_CURLY,
    TOKEN_KIND_COMMA,
    TOKEN_KIND_QUANTIFIER_ONE_OR_MORE,
    TOKEN_KIND_QUANTIFIER_ZERO_OR_MORE,
    TOKEN_KIND_QUANTIFIER_ZERO_OR_ONE,
    TOKEN_KIND_COUNT
} TokenKind;

typedef struct {
    TokenKind kind;
    SV value;
} Token;

TYPE() LIST(Token) TokenList;

#define TOKEN_END ((Token) { .kind = TOKEN_KIND_END, .value = SV_NULL })

Token token_new(TokenKind kind, SV value);
void token_dump(Token this);

#endif
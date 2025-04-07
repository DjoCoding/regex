#ifndef LEXER_H
#define LEXER_H

#include <src/sv/sv.h>
#include <src/token/token.h>

typedef struct {
    const SV source;
    size_t current;
} Lexer;

Lexer lexer_new(const char *content);
TokenList lexer_lex(Lexer *this);

#endif
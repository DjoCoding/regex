#ifndef PARSER_H
#define PARSER_H

#include <src/token/token.h>
#include <src/node/node.h>

typedef struct {
    TokenList tokens;
    size_t current;
} Parser;

Parser parser_new(TokenList tokens);
AST parser_parse(Parser *this);

#endif
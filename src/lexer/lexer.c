#include <src/lexer/lexer.h>

#include <stdbool.h>
#include <ctype.h>

#include <src/error/error.h>
#include <src/data/data.h>
#include <src/shared/shared.h>

Lexer lexer_new(const char *content) {
    return (Lexer) {
        .source = sv_from_cstr(content),
        .current = 0
    };
}

bool lexer_end(Lexer *this) {
    return this->current >= this->source.size;
}

char lexer_peek(Lexer *this) {
    return this->source.content[this->current];
}

void lexer_consume(Lexer *this) {
    this->current++;
}

Token lexer_lex_token(Lexer *this) {
    char c = lexer_peek(this);

    // check if the current token is a quantifier
    for(size_t i = 0; i < LEN(CharToQuantifierTokenKindMap); ++i) {
        CharToTokenKind current = CharToQuantifierTokenKindMap[i];
        if(current.c == c) {
            lexer_consume(this);
            return token_new(
                current.kind,
                SV_NULL
            );
        }
    }

    // check if the current token is another symbol
    for(size_t i = 0; i < LEN(CharToSymbolTokenKindMap); ++i) {
        CharToTokenKind current = CharToSymbolTokenKindMap[i];
        if(current.c == c) {
            lexer_consume(this);
            return token_new(
                current.kind,
                SV_NULL
            );
        }
    }

    // check for a unit
    if(isalnum(c)) {
        lexer_consume(this);
        return token_new(
            TOKEN_KIND_UNIT,
            sv_from_str(&c, 1)
        );
    }

    panic("unknown char %c, invalid regex", c);
}

TokenList lexer_lex(Lexer *this) {
    TokenList tokens = LIST_NEW(TokenList);
    while(!lexer_end(this)) {
        Token token = lexer_lex_token(this);
        LIST_APPEND(&tokens, token);
    }
    LIST_APPEND(&tokens, TOKEN_END);
    return tokens;
}


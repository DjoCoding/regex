#include <src/token/token.h>

#include <src/data/data.h>

Token token_new(TokenKind kind, SV value) {
    return (Token) {
        .kind = kind,
        .value = value
    };
}

void token_dump(Token token) {
    fprintf(stdout, "(%s) -> (" SV_FMT ")\n", TokenKindToString[token.kind], SV_UNWRAP(token.value));
}
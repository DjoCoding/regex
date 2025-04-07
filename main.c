#include <stdio.h>

#include <src/lexer/lexer.h>

int main(void) {
    const char *source = "(abc)+";
    
    Lexer lexer = lexer_new(source);
    
    TokenList tokens = lexer_lex(&lexer);
    for(size_t i = 0; i < tokens.count; ++i) {
        Token tok = tokens.items[i];
        token_dump(tok);
    }

    free(tokens.items);
    return 0;
}

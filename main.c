#include <stdio.h>

#include <src/lexer/lexer.h>
#include <src/parser/parser.h>

int main(void) {
    const char *source = "a|(bc|d+)";
    
    Lexer lexer = lexer_new(source);
    
    TokenList tokens = lexer_lex(&lexer);
    for(size_t i = 0; i < tokens.count; ++i) {
        Token tok = tokens.items[i];
        token_dump(tok);
    }

    Parser parser = parser_new(tokens);
    AST ast = parser_parse(&parser);

    ast_dump(ast);

    free(tokens.items);
    return 0;
}

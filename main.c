#include <stdio.h>

#include <src/lexer/lexer.h>
#include <src/parser/parser.h>
#include <src/compiler/compiler.h>

int main(void) {
    const char *source = "(a.(b|c))?";
    
    Lexer lexer = lexer_new(source);
    
    TokenList tokens = lexer_lex(&lexer);
    for(size_t i = 0; i < tokens.count; ++i) {
        Token tok = tokens.items[i];
        token_dump(tok);
    }

    Parser parser = parser_new(tokens);
    AST ast = parser_parse(&parser);

    {
        fprintf(stdout, "regex: ");
        ast_dump(ast);
    }

    Compiler compiler = compiler_new(ast);
    NFA *nfa = compiler_compile(&compiler);

    NFAGraphGenerator *generator = nfa_graph_gen_new(nfa, "nfa.dot");
    nfa_graph_gen(generator);


    free(tokens.items);
    return 0;
}

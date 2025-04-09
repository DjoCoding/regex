#include <stdio.h>

#define TEST
#undef TEST


#ifndef TEST
#include <src/shared/shared.h>
#include <src/regex/regex.h>

typedef struct {
    char *string;
    bool boolean;
} StringBoolean;

int main(void) {
    char *regex = "^(a|b+c)$";
    StringBoolean tests[] = {
        { "a",              true  },
        { "bbbbbc",             true  },
    };

    NFA *nfa = nfa_from_regex(regex);
    
    NFAGraphGenerator *generator = nfa_graph_gen_new(nfa, "./nfa.dot");
    nfa_graph_gen(generator);

    fprintf(stdout, "regex: %s\n", regex);

    for(size_t i = 0; i < LEN(tests); ++i) {
        char *string = tests[i].string;
        bool expected = tests[i].boolean;
        bool result = regex_test_from_nfa(nfa, string);
        fprintf(stdout, "%s -> match: %s :  passed: %s\n", string, result ? "yes" : "no", result == expected ? "true" : "false");
    }

    nfa_free(nfa);
    return 0;
}

#else

#include <src/lexer/lexer.h>
#include <src/parser/parser.h>
#include <src/compiler/compiler.h>


int main(void) {
    char *regex = "^(a|b+c)$";

    Lexer lexer = lexer_new(regex);
    TokenList tokens = lexer_lex(&lexer);

    Parser parser = parser_new(tokens);
    AST ast = parser_parse(&parser);

    fprintf(stdout, "regex: ");
    ast_dump(ast);

    Compiler compiler = compiler_new(ast);
    NFA *nfa = compiler_compile(&compiler);

    NFAGraphGenerator *generator = nfa_graph_gen_new(nfa, "./nfa.dot");
    nfa_graph_gen(generator);


    free(tokens.items);
    ast_free(ast);
    nfa_free(nfa);

    return 0;
}

#endif

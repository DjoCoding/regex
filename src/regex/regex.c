#include <src/regex/regex.h>

#include <src/shared/shared.h>
#include <src/lexer/lexer.h>
#include <src/parser/parser.h>
#include <src/compiler/compiler.h>
#include <src/matcher/matcher.h>

NFA *nfa_from_regex(char *regex) {
    Lexer lexer = lexer_new(regex);
    TokenList tokens = lexer_lex(&lexer);

    Parser parser = parser_new(tokens);
    AST ast = parser_parse(&parser);

    Compiler compiler = compiler_new(ast);
    NFA *nfa = compiler_compile(&compiler);

    free(tokens.items);
    ast_free(ast);

    return nfa;
}

bool regex_test_from_nfa(NFA *nfa, char *string) {
    RegexMatcher matcher = matcher_new(nfa);
    bool result = matcher_match(&matcher, string);
    return result;
}

bool regex_test(char *regex, char *string) {
    NFA *nfa = nfa_from_regex(regex);
    bool result = regex_test_from_nfa(nfa, string);
    nfa_free(nfa);
    return result;
}
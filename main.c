#include <stdio.h>

#include <src/shared/shared.h>
#include <src/regex/regex.h>

int main(void) {
    char *regex = "(a|b|(cd+))*";
    char *strings[] = {
        "cd",
        "a",
        "b",
        "ab",
        "abcdcd"
    };

    NFA *nfa = nfa_from_regex(regex);
    
    NFAGraphGenerator *generator = nfa_graph_gen_new(nfa, "./nfa.dot");
    nfa_graph_gen(generator);


    fprintf(stdout, "regex: %s\n", regex);

    for(size_t i = 0; i < LEN(strings); ++i) {
        fprintf(stdout, "%s -> %s\n", strings[i], regex_test_from_nfa(nfa, strings[i]) ? "yes" : "no");
    }


    nfa_free(nfa);
    return 0;
}

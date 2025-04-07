#ifndef REGEX_H
#define REGEX_H

#include <stdbool.h>

#include <src/NFA/nfa.h>

// generates NFA data structure from a regex
NFA *nfa_from_regex(char *regex);

// test string from pre-compiled NFA
bool regex_test_from_nfa(NFA *nfa, char *string);

// recieves two c strings, first is regex, second is the string to test
bool regex_test(char *regex, char *string);

#endif
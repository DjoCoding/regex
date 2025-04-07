#ifndef CHECKER_H
#define CHECKER_H

#include <stdbool.h>

#include <src/sv/sv.h>
#include <src/NFA/nfa.h>

typedef struct {
    NFA *nfa;
    SV string;
    size_t current;
} RegexMatcher;

RegexMatcher matcher_new(NFA *nfa);
bool matcher_match(RegexMatcher *regex, char *string);

#endif
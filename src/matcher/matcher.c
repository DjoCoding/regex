#include <src/matcher/matcher.h>

#include <src/error/error.h>

RegexMatcher matcher_new(NFA *nfa) {
    return (RegexMatcher) {
        .nfa = nfa,
        .string = SV_NULL,
        .current = 0,
    };
}

bool matcher_eoi(RegexMatcher *regex) {
    return regex->current >= regex->string.size;
}

void matcher_consume(RegexMatcher *regex) {
    regex->current++;
}

bool matcher_match_state(RegexMatcher *regex, State *state);
bool matcher_match_nfa(RegexMatcher *regex, NFA *nfa);

bool matcher_try_char_transition(RegexMatcher *regex, char c, State *next) {
    if(c == ANY_CHAR) {
        if(matcher_eoi(regex)) return false;
        matcher_consume(regex);
        return matcher_match_state(regex, next);
    }

    if(regex->string.content[regex->current] == c) {
        matcher_consume(regex);
        return matcher_match_state(regex, next);
    }

    return false;
}

bool matcher_try_transition(RegexMatcher *regex, Transition t) {
    switch(t.kind) {
        case TRANSITION_KIND_CHAR:
            return matcher_try_char_transition(regex, t.maker.c, t.state);
        case TRANSITION_KIND_NFA:
            return matcher_match_nfa(regex, t.maker.nfa);
        case TRANSITION_KIND_EPSILON:
            return matcher_match_state(regex, t.state);
        default:
            return false;    
    }
    UNREACHABLE();
}

bool matcher_match_state(RegexMatcher *regex, State *state) {
    if(state->transitions.count == 0) {
        return regex->current >= regex->string.size;
    }

    // save the previous pointer
    size_t current = regex->current;
    
    for(size_t i = 0; i < state->transitions.count; ++i) {
        Transition t = state->transitions.items[i];
        bool does_match = matcher_try_transition(regex, t);
        if(does_match) return true;

        // re-put the pointer
        regex->current = current;
    }
    
    return false;
}

bool matcher_match_nfa(RegexMatcher *regex, NFA *nfa) {
    return matcher_match_state(regex, nfa->start);
}

bool matcher_match(RegexMatcher *regex, char *string) {
    regex->string = sv_from_cstr(string);
    regex->current = 0;
    return matcher_match_nfa(regex, regex->nfa);
}
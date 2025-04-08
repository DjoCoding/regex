#ifndef NFA_H
#define NFA_H

#include <stdbool.h>
#include <stdio.h>

#include <src/list/list.h>

#define ANY_CHAR 0

typedef struct State State;
typedef struct Transition Transition;
typedef struct NFA NFA;

TYPE() LIST(State) States;
TYPE() LIST(Transition) Transitions;
TYPE() LIST(NFA) NFAs;

typedef enum {
    TRANSITION_KIND_CHAR = 0,
    TRANSITION_KIND_NFA,
    TRANSITION_KIND_EPSILON
} TransitionKind;

typedef union {
    NFA *nfa;
    char c;
} TransitionMaker;

struct Transition {
    TransitionKind kind;
    TransitionMaker maker;
    State *state;
};

typedef struct {
    State *copy;
    size_t id;                          // used for graph generation
    bool is_generated;                  // used for graph generation
} StateMetaData;

struct State {
    Transitions transitions;
    StateMetaData data;
};

struct NFA {
    State *start;
    State *accept;
};

typedef struct {
    NFA *nfa;
    const char *filename;
    FILE *file;
    size_t indent;
} NFAGraphGenerator;

NFA *nfa_new(State *start, State *accept);
NFA *nfa_copy(NFA *nfa);
void nfa_free(NFA *nfa);

NFAGraphGenerator *nfa_graph_gen_new(NFA *nfa, const char *filename);
void nfa_graph_gen(NFAGraphGenerator *generator);


State *state_new();
void state_add_epsilon_transition(State *current, State *next);
void state_add_nfa_transition(State *current, NFA *nfa);
void state_add_char_transition(State *current, char c, State *next);

Transition transition_new_char(char c, State *state);
Transition transition_new_nfa(NFA *nfa, State *state);
Transition transition_new_epsilon(State *state);

#endif
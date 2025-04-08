#include <src/NFA/nfa.h>

#include <src/error/error.h>
#include <src/shared/shared.h>
#include <src/data/data.h>

#define NFA_GRAPH_GENERATE_INDENT(g) { fprintf(g->file, "\t"); }
#define NFA_GRAPH_GENERATE_STATEMENT(g, ...) { for(size_t i = 0; i < g->indent; ++i) NFA_GRAPH_GENERATE_INDENT(g); fprintf(g->file, __VA_ARGS__); fprintf(g->file, "\n"); }

size_t _id = 0;

static size_t gen_id() {
    return _id++;
}

State *state_copy(State *state);

NFA *nfa_copy(NFA *nfa);
void nfa_free(NFA *nfa);

NFA *nfa_new(State *start, State *accept) {
    NFA *nfa = malloc(sizeof(NFA));
    assert(nfa != NULL && "malloc failed.");
    nfa->start = start;
    nfa->accept = accept;
    return nfa;
}

Transition transition_copy(Transition t) {
    Transition ct = t;

    if(t.kind == TRANSITION_KIND_NFA) {
        ct.maker.nfa = nfa_copy(t.maker.nfa);
    }

    ct.state = state_copy(t.state);
    return ct;
}

State *state_copy(State *state) {
    if(state->data.copy != NULL) return state->data.copy;

    State *cstate = state_new();

    for(size_t i = 0; i < state->transitions.count; ++i) {
        Transition t = state->transitions.items[i];
        Transition ct = transition_copy(t);
        LIST_APPEND(&cstate->transitions, ct);
    }

    state->data.copy = cstate;
    return cstate;
}

NFA *nfa_copy(NFA *nfa) {
    State *state = state_copy(nfa->start);
    State *accept = NULL;
    return nfa_new(state, accept);
}

void nfa_free(NFA *nfa) {
    free(nfa);
}

Transition transition_new_char(char c, State *state) {
    Transition t = {0};
    t.kind = TRANSITION_KIND_CHAR;
    t.maker.c = c;
    t.state = state;
    return t;
}

Transition transition_new_nfa(NFA *nfa, State *state) {
    Transition t = {0};
    t.kind = TRANSITION_KIND_NFA;
    t.maker.nfa = nfa;
    t.state = state;
    return t;
}

Transition transition_new_epsilon(State *state) {
    Transition t = {0};
    t.kind = TRANSITION_KIND_EPSILON;
    t.state = state;
    return t;
}

State *state_new() {
    State *state = calloc(1, sizeof(State));
    assert(state != NULL && "malloc failed.");
    state->data.id = gen_id();
    state->data.is_generated = false;
    state->data.copy = NULL;
    return state;
}

void state_add_epsilon_transition(State *current, State *next) {
    LIST_APPEND(&current->transitions, transition_new_epsilon(next));
}

void state_add_nfa_transition(State *current, NFA *nfa) {
    LIST_APPEND(&current->transitions, transition_new_nfa(nfa, nfa->start));
}

void state_add_char_transition(State *current, char c, State *next) {
    LIST_APPEND(&current->transitions, transition_new_char(c, next));
}


NFAGraphGenerator *nfa_graph_gen_new(NFA *nfa, const char *filename) {
    FILE *file = fopen(filename, "w");
    if(!file) {
        return NULL;
    }

    NFAGraphGenerator *generator = malloc(sizeof(NFAGraphGenerator));
    if(!generator) {
        fclose(file);
        return NULL;
    }

    generator->nfa = nfa;
    generator->filename = filename;
    generator->file = file;
    generator->indent = 0;

    return generator;
}

void nfa_graph_start_block(NFAGraphGenerator *g) {
    g->indent += 1;
}

void nfa_graph_end_block(NFAGraphGenerator *g) {
    g->indent -= 1;
}

void nfa_graph_gen_state(NFAGraphGenerator *g, State *state);

void nfa_graph_gen_epsilon_transition(NFAGraphGenerator *g, State *from, State *to) {
    NFA_GRAPH_GENERATE_STATEMENT(g, "q%zu -> q%zu [label=\"ε\"];", from->data.id, to->data.id);
    nfa_graph_gen_state(g, to);
}

void nfa_graph_gen_char_transition(NFAGraphGenerator *g, State *from, char c, State *to) {
    char *label = "any";
    
    if(c != 0) {
        for(size_t i = 0; i < LEN(PrintableCharToStringFormatMap); ++i) {
            CharToStringFormat current = PrintableCharToStringFormatMap[i];
            if(current.c == c) { label = current.s; }
        }
    }

    NFA_GRAPH_GENERATE_STATEMENT(g, "q%zu -> q%zu [label=\"%s\"];", from->data.id, to->data.id, label);
    nfa_graph_gen_state(g, to);
}

void nfa_graph_gen_nfa_transition(NFAGraphGenerator *g, State *from, NFA *by, State *to) {
    NFA_GRAPH_GENERATE_STATEMENT(g, "q%zu -> q%zu [label=\"ε\"];", from->data.id, by->start->data.id);
    nfa_graph_gen_state(g, by->start);
}

void nfa_graph_gen_transition(NFAGraphGenerator *g, State *current, Transition t) {
    switch(t.kind) {
        case TRANSITION_KIND_EPSILON:
            return nfa_graph_gen_epsilon_transition(g, current, t.state);
        case TRANSITION_KIND_CHAR:
            return nfa_graph_gen_char_transition(g, current, t.maker.c, t.state);
        case TRANSITION_KIND_NFA:
            return nfa_graph_gen_nfa_transition(g, current, t.maker.nfa, t.state);
        default:
            panic("unregistered transition kind.");
    }

    UNREACHABLE();
}

void nfa_graph_gen_state(NFAGraphGenerator *g, State *state) {
    if(state->data.is_generated) return;

    state->data.is_generated = true;


    if(state->transitions.count == 0) {
        // define the accept state 
        NFA_GRAPH_GENERATE_STATEMENT(g, "q%zu [shape=doublecircle];", state->data.id);
    } else {
        // define current state as normal state
        NFA_GRAPH_GENERATE_STATEMENT(g, "q%zu [label=\"q%zu\"];", state->data.id, state->data.id);
    }
    
    for(size_t i = 0; i < state->transitions.count; ++i) {
        nfa_graph_gen_transition(g, state, state->transitions.items[i]);
    }
}

void nfa_graph_gen(NFAGraphGenerator *g) {
    NFA_GRAPH_GENERATE_STATEMENT(g, "digraph NFA {");

    nfa_graph_start_block(g);
    
    {
        NFA_GRAPH_GENERATE_STATEMENT(g, "rankdir=LR;");
        NFA_GRAPH_GENERATE_STATEMENT(g, "node [shape=circle];");
        
        nfa_graph_gen_state(g, g->nfa->start);
    }
    
    nfa_graph_end_block(g);

    NFA_GRAPH_GENERATE_STATEMENT(g, "}");

    fclose(g->file);
}

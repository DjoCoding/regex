#include <src/compiler/compiler.h>

#include <ctype.h>

#include <src/error/error.h>
#include <src/data/data.h>
#include <src/shared/shared.h>

Compiler compiler_new(AST ast) {
    return (Compiler) {
        .ast = ast
    };
}

NFA *compiler_compile_node(Node *node);

NFA *compiler_compile_quantifier_one_or_more(NFA *nfa) {
    State *start = state_new();
    State *accept = state_new();

    state_add_nfa_transition(start, nfa);
    state_add_epsilon_transition(nfa->accept, accept);
    state_add_epsilon_transition(nfa->accept, nfa->start);

    return nfa_new(start, accept);
}

NFA *compiler_compile_quantifier_zero_or_more(NFA *nfa) {
    State *start = state_new();
    State *accept = state_new();

    state_add_epsilon_transition(start, nfa->start);
    state_add_epsilon_transition(start, accept);

    state_add_epsilon_transition(nfa->accept, accept);
    state_add_epsilon_transition(nfa->accept, nfa->start);

    return nfa_new(start, accept);
}

NFA *compiler_compile_quantifier_zero_or_one(NFA *nfa) {
    State *start = state_new();
    State *accept = state_new();

    state_add_epsilon_transition(start, nfa->start);
    state_add_epsilon_transition(start, accept);
    state_add_epsilon_transition(nfa->accept, accept);

    return nfa_new(start, accept);
}

NFA *compiler_compile_quantifier(NFA *nfa, Quantifier q) {
    if(q == QUANTIFIER_NONE) return nfa;
    
    switch(q) {
        case QUANTIFIER_ONE_OR_MORE:
            return compiler_compile_quantifier_one_or_more(nfa);
        case QUANTIFIER_ZERO_OR_MORE:
            return compiler_compile_quantifier_zero_or_more(nfa);
        case QUANTIFIER_ZERO_OR_ONE:
            return compiler_compile_quantifier_zero_or_one(nfa);
        default:
            panic("unregistered quantifier.");
    }

    UNREACHABLE();
}

NFA *compiler_compile_any_lit() {
    State *start = state_new();
    State *accept = state_new();
    state_add_char_transition(start, ANY_CHAR, accept);
    return nfa_new(start, accept);
}

NFA *compiler_compile_lit(char c) {
    State *start = state_new();
    State *accept = state_new();
    state_add_char_transition(start, c, accept);
    return nfa_new(start, accept);
}

NFA *compiler_compile_primary(Primary pri) {
    switch(pri.kind) {
        case PRIMARY_KIND_ANY_LITREAL:
            return compiler_compile_any_lit();
        case PRIMARY_KIND_LITERAL:
            return compiler_compile_lit(pri.as.lit.c);
        case PRIMARY_KIND_GROUP:
            return compiler_compile_node(pri.as.group);
        case PRIMARY_KIND_CHARACTER_CLASS:
            TODO("implement character class compiling");
        default:
            panic("unregistered primary kind.");
    }

    UNREACHABLE();
}

NFA *compiler_compile_rep(Repitition rep) {
    NFA *pri = compiler_compile_primary(rep.primary);
    return compiler_compile_quantifier(pri, rep.quantifier);
}

NFA *compiler_compile_concat(Concatenation concat) {
    NFA *lhs = compiler_compile_node(concat.lhs);
    NFA *rhs = compiler_compile_node(concat.rhs);

    state_add_epsilon_transition(lhs->accept, rhs->start);

    return nfa_new(lhs->start, rhs->accept);
}

NFA *compiler_compile_alter(Alternation alter) {
    State *start = state_new();
    State *accept = state_new();

    NFA *lhs = compiler_compile_node(alter.lhs);
    NFA *rhs = compiler_compile_node(alter.rhs);

    state_add_nfa_transition(start, lhs);
    state_add_nfa_transition(start, rhs);

    state_add_epsilon_transition(lhs->accept, accept);
    state_add_epsilon_transition(rhs->accept, accept);

    return nfa_new(start, accept);
}


NFA *compiler_compile_node(Node *node) {
    switch(node->kind) {
        case NODE_KIND_ALTERNATION:
            return compiler_compile_alter(node->as.alter);
        case NODE_KIND_CONCATENATION:
            return compiler_compile_concat(node->as.concat);
        case NODE_KIND_REPITITION:
            return compiler_compile_rep(node->as.rep);
        default:
            panic("unregistered node kind.");
    }

    UNREACHABLE();
}


NFA *compiler_compile_root(Node *root) {
    return compiler_compile_node(root);
}

NFA *compiler_compile(Compiler *c) {
    return compiler_compile_root(c->ast.root);
}

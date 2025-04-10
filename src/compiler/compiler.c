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

void mark_char(char c, CharMarker *chars) {
    for(size_t i = 0; i < PRINTABLE_CHAR_COUNT; ++i) {
        if(chars[i].c == c) {
            chars[i].marked = true;
            break;
        }
    }
}

void mark_range(Range range, CharMarker *chars) {
    size_t i = 0;

    while(i < PRINTABLE_CHAR_COUNT) {
        if(chars[i].c == range.from) break;
        ++i;
    }

    while(i < PRINTABLE_CHAR_COUNT && chars[i].c <= range.to) {
        chars[i++].marked = true;
    }
}

NFA *compiler_compile_char_class(CharClass char_class) {
    State *start  = state_new();
    State *accept = state_new();

    ClassItems class = char_class.class;

    CharMarker chars[PRINTABLE_CHAR_COUNT] = {0};

    for(size_t i = 0; i < LEN(PrintableCharToStringFormatMap); ++i) {
        chars[i] = (CharMarker) {
            .c = PrintableCharToStringFormatMap[i].c,
            .marked = false
        };
    }

    for(size_t i = 0; i < class.count; ++i) {
        ClassItem item = class.items[i];

        switch(item.kind) {
            case CLASS_ITEM_KIND_LITERAL:   
                mark_char(item.as.lit.c, chars);
                break;
            case CLASS_ITEM_KIND_RANGE:
                mark_range(item.as.range, chars);
                break;
            default:
                panic("unregistered class item kind.");
        }
    }

    for(size_t i = 0; i < PRINTABLE_CHAR_COUNT; ++i) {
        CharMarker marker = chars[i];

        if(!(marker.marked ^ char_class.negated)) continue;

        NFA *nfa = compiler_compile_lit(marker.c);
        state_add_epsilon_transition(start, nfa->start);
        state_add_epsilon_transition(nfa->accept, accept);
    }

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
            return compiler_compile_char_class(pri.as.char_class);
        default:
            panic("unregistered primary kind.");
    }

    UNREACHABLE();
}

NFA *compiler_compile_quantifier_between_rep(Repitition rep) {
    QuantifierBetween q = rep.quantifier.as_between;

    State *start = state_new();
    State *accept = state_new();
    
    State *last_nfa_accept_state = start;
    NFA *nfa = NULL;

    size_t left = q.left.exists ? q.left.value : 0;
    size_t right = q.right.value;

    if((left == 0) && !q.right.exists) {
        nfa = compiler_compile_primary(rep.primary);
        state_add_epsilon_transition(start, nfa->start);
        state_add_epsilon_transition(nfa->accept, nfa->start);
        state_add_epsilon_transition(nfa->accept, accept);
        state_add_epsilon_transition(start, accept);
        return nfa_new(start, accept);
    }

    if(left != 0) {
        for(size_t i = 0; i < left; ++i) {
            nfa = compiler_compile_primary(rep.primary);
            state_add_nfa_transition(last_nfa_accept_state, nfa);
            last_nfa_accept_state = nfa->accept;
        }
    }

    if(!q.right.exists) {
        state_add_epsilon_transition(nfa->accept, nfa->start);
    } else {
        for(size_t i = left + 1; i <= right; i++) {
            nfa = compiler_compile_primary(rep.primary);
            state_add_epsilon_transition(last_nfa_accept_state, nfa->start);
            state_add_epsilon_transition(last_nfa_accept_state, accept);
            last_nfa_accept_state = nfa->accept;
        }
    }

    state_add_epsilon_transition(nfa->accept, accept);
    return nfa_new(start, accept);
}

NFA *compiler_compile_rep(Repitition rep) {
    Quantifier q = rep.quantifier;
    if(q.kind == QUANTIFIER_BETWEEN) return compiler_compile_quantifier_between_rep(rep);

    NFA *nfa = compiler_compile_primary(rep.primary);
    if(q.kind == QUANTIFIER_NONE) return nfa;
    
    switch(q.kind) {
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

NFA *compiler_compile_ast(AST ast) {
    State *start = state_new();
    State *accept = state_new();

    NFA *nfa = compiler_compile_root(ast.root);

    if(ast.caret) {
        state_add_nfa_transition(start, nfa);
    } else {
        // get any node
        NFA *any = compiler_compile_any_lit();
        
        // connect start state with any
        state_add_epsilon_transition(start, any->start);

        state_add_epsilon_transition(start, nfa->start);
 
        // make a loop back    
        state_add_epsilon_transition(any->accept, any->start);
        
        // connect any node with nfa
        state_add_nfa_transition(any->accept, nfa);
    }

    if(ast.dollar) {
        state_add_epsilon_transition(nfa->accept, accept);
    } else {
        // get any node
        NFA *any = compiler_compile_any_lit();
        
        // make a loop back    
        state_add_epsilon_transition(any->accept, any->start);
        
        state_add_epsilon_transition(nfa->accept, any->start);
        state_add_epsilon_transition(nfa->accept, accept);

        state_add_epsilon_transition(any->accept, accept);
    }

    return nfa_new(start, accept);
}

NFA *compiler_compile(Compiler *c) {
    return compiler_compile_ast(c->ast);
}

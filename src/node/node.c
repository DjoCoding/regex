#include <src/node/node.h>

#include <stdlib.h>
#include <assert.h>

#include <src/error/error.h>

Node *node_new() {
    Node *node = (Node *)calloc(1, sizeof(Node));
    assert(node != NULL && "malloc failed");
    return node;
}

Primary primary_new_any_lit() {
    return (Primary) {
        .kind = PRIMARY_KIND_ANY_LITREAL
    };
}

Primary primary_new_lit(char c) {
    return (Primary) {
        .kind = PRIMARY_KIND_LITERAL,
        .as.lit.c = c
    };
}

Primary primary_new_group(Node *group) {
    return (Primary) {
        .kind = PRIMARY_KIND_GROUP,
        .as.group = group
    };
}


Node *node_new_rep(Primary primary, Quantifier q) {
    Node *node = node_new();
    node->kind = NODE_KIND_REPITITION;
    node->as.rep.primary = primary;
    node->as.rep.quantifier = q;
    return node;
}

Node *node_new_concat(Node *lhs, Node *rhs) {
    Node *node  = node_new();
    node->kind = NODE_KIND_CONCATENATION;
    node->as.alter = (Alternation) {
        .lhs = lhs,
        .rhs = rhs
    };
    return node;
}

Node *node_new_alter(Node *lhs, Node *rhs) {
    Node *node  = node_new();
    node->kind = NODE_KIND_ALTERNATION;
    node->as.alter = (Alternation) {
        .lhs = lhs,
        .rhs = rhs
    };
    return node;
}


AST ast_new(Node *root, bool caret, bool dollar) {
    return (AST) {
        .root = root,
        .caret = caret,
        .dollar = dollar
    };
}

void node_dump(Node *this);
void node_free(Node *node);

void pri_dump(Primary pri) {
    switch(pri.kind) {
        case PRIMARY_KIND_LITERAL:
            fprintf(stdout, "%c", pri.as.lit.c);
            return;
        case PRIMARY_KIND_GROUP:
            fprintf(stdout, "(");
            node_dump(pri.as.group);
            fprintf(stdout, ")");
            return;
        case PRIMARY_KIND_ANY_LITREAL:
            fprintf(stdout, ".");
            return;
        default:
            panic("unregistered primary kind.");
    }
}

void quantifier_dump(Quantifier q) {
    switch(q.kind) {
        case QUANTIFIER_NONE:
            return;
        case QUANTIFIER_ZERO_OR_MORE:
            fprintf(stdout, "*");
            return;
        case QUANTIFIER_ZERO_OR_ONE:
            fprintf(stdout, "?");
            return;
        case QUANTIFIER_ONE_OR_MORE:
            fprintf(stdout, "+");
            return;
        case QUANTIFIER_BETWEEN:
            fprintf(stdout, "{");
            if(q.as_between.left.exists) fprintf(stdout, "%zu", q.as_between.left.value);
            fprintf(stdout, ",");
            if(q.as_between.right.exists) fprintf(stdout, "%zu", q.as_between.right.value);
            fprintf(stdout, "}");
            break;
        default:
            panic("unregistered quantifier.");
    }
}

void rep_dump(Repitition rep) {
    pri_dump(rep.primary);
    quantifier_dump(rep.quantifier);
}

void rep_free(Repitition rep) {
    if(rep.primary.kind == PRIMARY_KIND_GROUP) {
        return node_free(rep.primary.as.group);
    }
}

void concat_dump(Concatenation concat) {
    node_dump(concat.lhs);
    node_dump(concat.rhs);
}

void concat_free(Concatenation concat) {
    node_free(concat.lhs);
    node_free(concat.rhs);
}

void alter_dump(Alternation alter) {
    node_dump(alter.lhs);
    fprintf(stdout, "|");
    node_dump(alter.rhs);
}

void alter_free(Alternation alter) {
    node_free(alter.lhs);
    node_free(alter.rhs);
}

void node_dump(Node *this) {
    switch(this->kind) {
        case NODE_KIND_ALTERNATION:
            return alter_dump(this->as.alter);
        case NODE_KIND_CONCATENATION:
            return concat_dump(this->as.concat);
        case NODE_KIND_REPITITION:
            return rep_dump(this->as.rep);
        default:
            panic("unregistered node kind.");
    }
}

void node_free(Node *node) {
    switch(node->kind) {
        case NODE_KIND_ALTERNATION:
            alter_free(node->as.alter);
            break;
        case NODE_KIND_CONCATENATION:
            concat_free(node->as.concat);
            break;
        case NODE_KIND_REPITITION:
            rep_free(node->as.rep);
            break;
        default:
            panic("unregistered node kind.");
    }

    return free(node);
}

void ast_dump(AST this) {
    if(this.caret) {
        fprintf(stdout, "^");
    }

    node_dump(this.root);

    if(this.dollar) {
        fprintf(stdout, "$");
    }

    fprintf(stdout, "\n");
}

void ast_free(AST this) {
    node_free(this.root);
}
#ifndef NODE_H
#define NODE_H

#include <stdbool.h>

#include <src/sv/sv.h> 

typedef enum {
    NODE_KIND_ALTERNATION = 0,
    NODE_KIND_CONCATENATION,
    NODE_KIND_REPITITION,
    NODE_KIND_COUNT
} NodeKind;

typedef enum {
    QUANTIFIER_NONE = 0,
    QUANTIFIER_ONE,
    QUANTIFIER_ONE_OR_MORE,
    QUANTIFIER_ZERO_OR_MORE,
    QUANTIFIER_ZERO_OR_ONE,
} Quantifier;

typedef struct {
    char c;
} Literal;

typedef enum {
    PRIMARY_KIND_LITERAL = 0,
    PRIMARY_KIND_CHARACTER_CLASS,
    PRIMARY_KIND_GROUP,
} PrimaryKind;

typedef struct Node Node;

typedef union {
    Node *group;
    Literal lit;
} PrimaryAs;

typedef struct {
    PrimaryKind kind;
    PrimaryAs as;
} Primary;

typedef struct {
    Primary primary;
    Quantifier quantifier;
} Repitition;

typedef struct {
    Node *lhs;
    Node *rhs;
} Concatenation;

typedef struct {
    Node *lhs;
    Node *rhs;
} Alternation;

typedef union {
    Alternation alter;
    Concatenation concat;
    Repitition rep;
} NodeAs;

struct Node {
    NodeKind kind;
    NodeAs as;
};

typedef struct {
    Node *root;
} AST;


Primary primary_new_lit(char c);
Primary primary_new_group(Node *group);

Node *node_new_rep(Primary primary, Quantifier q);
Node *node_new_concat(Node *lhs, Node *rhs);
Node *node_new_alter(Node *lhs, Node *rhs);

AST ast_new(Node *root);
void ast_dump(AST this);

#endif
#ifndef NODE_H
#define NODE_H

#include <stdbool.h>

#include <src/sv/sv.h> 
#include <src/list/list.h>

typedef enum {
    NODE_KIND_ALTERNATION = 0,
    NODE_KIND_CONCATENATION,
    NODE_KIND_REPITITION,
    NODE_KIND_COUNT
} NodeKind;

typedef struct {
    char c;
} Literal;

typedef struct {
    char from;
    char to;
} Range;

typedef enum {
    CLASS_ITEM_KIND_LITERAL = 0,
    CLASS_ITEM_KIND_RANGE,
} ClassItemKind;

typedef union {
    Range range;
    Literal lit;
} ClassItemAs;

typedef struct {
    ClassItemKind kind;
    ClassItemAs as;
} ClassItem;

TYPE() LIST(ClassItem) ClassItems;

typedef struct {
    bool negated;
    ClassItems class;
} CharClass;

typedef enum {
    QUANTIFIER_NONE = 0,
    QUANTIFIER_ONE_OR_MORE,
    QUANTIFIER_ZERO_OR_MORE,
    QUANTIFIER_ZERO_OR_ONE,
    QUANTIFIER_BETWEEN,
} QuantifierKind;

typedef struct {
    bool exists;
    size_t value;
} QuantifierBound;

typedef struct {
    QuantifierBound left;
    QuantifierBound right;
} QuantifierBetween;

typedef struct {
    QuantifierKind kind;
    QuantifierBetween as_between;
} Quantifier;

typedef enum {
    PRIMARY_KIND_LITERAL = 0,
    PRIMARY_KIND_ANY_LITREAL,
    PRIMARY_KIND_CHARACTER_CLASS,
    PRIMARY_KIND_GROUP,
} PrimaryKind;

typedef struct Node Node;

typedef union {
    Node *group;
    Literal lit;
    CharClass char_class;
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
    bool caret;
    bool dollar;
    Node *root;
} AST;


bool quantifier_between_validate(QuantifierBetween q);

Range range_new(char from, char to);

ClassItem class_item_from_lit(char c);
ClassItem class_item_from_range(Range range);

CharClass char_class_new(ClassItems class, bool negated);

Primary primary_new_char_class(CharClass char_class);
Primary primary_new_any_lit();
Primary primary_new_lit(char c);
Primary primary_new_group(Node *group);

Node *node_new_rep(Primary primary, Quantifier q);
Node *node_new_concat(Node *lhs, Node *rhs);
Node *node_new_alter(Node *lhs, Node *rhs);

AST ast_new(Node *root, bool caret, bool dollar);
void ast_dump(AST this);
void ast_free(AST this);

#endif
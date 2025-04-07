#ifndef COMPILER_H
#define COMPILER_H

#include <src/node/node.h>
#include <src/NFA/nfa.h>

typedef struct {
    AST ast;
} Compiler;

Compiler compiler_new(AST ast);
NFA *compiler_compile(Compiler *this);

#endif
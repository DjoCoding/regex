#ifndef ERROR_H
#define ERROR_H 

#include <stdio.h>
#include <stdlib.h>

#define panic(...) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

#define UNREACHABLE() panic("%s:%u:unreachable", __FILE__, __LINE__)
#define TODO(...) panic("%s:%u:todo->%s", __FILE__, __LINE__, __VA_ARGS__)

#endif
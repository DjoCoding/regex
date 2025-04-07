#ifndef ERROR_H
#define ERROR_H 

#include <stdio.h>
#include <stdlib.h>

#define panic(...) { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE); }

#endif 
#ifndef SHARED_H
#define SHARED_H

#include <ctype.h>

#define LEN(A) (sizeof(A)/sizeof(A[0]))

#define isnum(c) (!isalpha(c) && isalnum(c))

#endif
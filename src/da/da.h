#ifndef DA_H
#define DA_H

#include <stdlib.h>
#include <assert.h>

// T-type of the array element
// N-name of the new type
#define DA(T, N) \
    typedef struct {\
        T  *items; \
        size_t count; \
        size_t size; \
    } N

// N-name of the array type
#define DA_NEW(N) (N) {0}

#define DA_RESIZE(a) \
    do { \
        if((a)->size == 0) { \
            (a)->size = 10; \
        } \
        (a)->size *= 2; \
        (a)->items = realloc((a)->items, sizeof(*(a)->items) * (a)->size); \
        assert((a)->items != NULL && "realloc failed"); \
    } while(0)

#define DA_APPPEND(a, v) \
    do {\
        if((a)->count >= (a)->size) { DA_RESIZE(a); } \
        (a)->items[(a)->count++] = v; \
    } while(0)

#endif

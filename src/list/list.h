#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <assert.h>

#define TYPE()

// T-type of the array element
#define LIST(T) \
    typedef struct {\
        T  *items; \
        size_t count; \
        size_t size; \
    }

// N-name of the array type
#define LIST_NEW(N) (N) {0}

#define LIST_RESIZE(a) \
    do { \
        if((a)->size == 0) { \
            (a)->size = 10; \
        } \
        (a)->size *= 2; \
        (a)->items = realloc((a)->items, sizeof(*(a)->items) * (a)->size); \
        assert((a)->items != NULL && "realloc failed"); \
    } while(0)

#define LIST_APPEND(a, v) \
    do {\
        if((a)->count >= (a)->size) { LIST_RESIZE(a); } \
        (a)->items[(a)->count++] = v; \
    } while(0)

#endif

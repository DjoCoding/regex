#ifndef SV_H
#define SV_H

#include <stdio.h>

typedef struct {
    char *content;
    size_t size;
} SV;

#define SV_NULL ((SV) {0})

#define SV_FMT "%.*s"
#define SV_UNWRAP(s) (int)s.size, s.content == NULL ? "" : s.content

// string view from a pre-allocated string
SV sv(char *content, size_t size);

// string view from a c string
SV sv_from_cstr(char *content);

// string view from a non-pre-allocated string
SV sv_from_str(const char *content, size_t size);

#endif
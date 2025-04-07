#include <src/sv/sv.h>

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

SV sv(char *content, size_t size) {
    return (SV) {
        .content = content,
        .size = size
    };
}

SV sv_from_cstr(char *content) {
    return sv(content, strlen(content));
}

SV sv_from_str(const char *content, size_t size) {
    char *c = (char *)malloc(sizeof(char) * size);
    assert(c != NULL && "malloc failed");
    memcpy(c, content, size);
    return sv(c, size);
} 

bool sv_cmp(SV a, SV b) {
    if(a.size != b.size) return false;
    return memcmp(a.content, b.content, a.size) == 0;
}
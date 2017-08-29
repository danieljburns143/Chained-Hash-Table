/* fnv.c: fnv hash function */

#include "map.h"

#define FNV_OFFSET_BASIS    (0xcbf29ce484222325ULL)
#define FNV_PRIME           (0x100000001b3ULL)

uint64_t	fnv_hash(const void *d, size_t n) {
    uint64_t hash = FNV_OFFSET_BASIS;
    char* bytePtr = (char*)d;
    for (int i = 0; i < n; i++) {
        hash ^= *bytePtr;
        hash *= FNV_PRIME;
        bytePtr++;
    }
    return hash;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */

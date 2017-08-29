/* entry.c: map entry */

#include "map.h"

Entry *	entry_create(const char *key, const Value value, Entry *next, Type type) {
    Entry* newEntry = malloc(sizeof(Entry));
    if (newEntry == NULL) {
        printf("\n\nmalloc failed\n\n");
        exit(1);
    }
    newEntry->key = strdup(key);
    if (type == STRING) {
        newEntry->value.string = strdup(value.string);
    }
    else {
        newEntry->value.number = value.number;
    }
    newEntry->type = type;
    newEntry->next = next;
    return newEntry;
}

Entry *	entry_delete(Entry *e, bool recursive) {
    if (e == NULL) { return NULL; }
    if (recursive == false || e->next == NULL) {
        free(e->key);
        if (e->type == STRING) {
            free(e->value.string);
        }
        free(e);
        return NULL;
    }
    entry_delete(e->next, true);
    free(e->key);
    if (e->type == STRING) {
        free(e->value.string);
    }
    free(e);
    return NULL;
}

void entry_update(Entry *e, const Value value, Type type) {
    if (e->type == STRING) {
        free(e->value.string);
    }
    if (type == STRING) {
        e->value.string = strdup(value.string);
    }
    else {
        e->value.number = value.number;
    }
    e->type = type;
}

void entry_dump(Entry *e, FILE *stream, const DumpMode mode) {
    switch (mode) {
        case KEY:
            fprintf(stream, "%s\n", e->key);
            break;
        case KEY_VALUE:
            if (e->type == STRING) {
                fprintf(stream, "%s\t%s\n", e->key, e->value.string);
            }
            else {
                fprintf(stream, "%s\t%ld\n", e->key, e->value.number);
            }
            break;
        case VALUE:
            if (e->type == STRING) {
                fprintf(stream, "%s\n", e->value.string);
            }
            else {
                fprintf(stream, "%ld\n", e->value.number);
            }
            break;
        case VALUE_KEY:
            if (e->type == STRING) {
                fprintf(stream, "%s\t%s\n", e->value.string, e->key);
            }
            else {
                fprintf(stream, "%ld\t%s\n", e->value.number, e->key);
            }
            break;
    }
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */

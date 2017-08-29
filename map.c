/* map.c: separate chaining hash table */

#include "map.h"

Map* map_create(size_t capacity, double load_factor) {
    Map* newMap = calloc(1, sizeof(Map));
    newMap->capacity = (capacity == 0) ? DEFAULT_CAPACITY : capacity;
    newMap->load_factor = (load_factor <= 0) ? DEFAULT_LOAD_FACTOR : load_factor;
    newMap->buckets = calloc(newMap->capacity, sizeof(Entry));
    newMap->size = 0;
    return newMap;
}

Map* map_delete(Map *m) {
    for (size_t i = 0; i < m->capacity; i++) {
        entry_delete(m->buckets[i].next, true);
    }
    free(m->buckets);
    free(m);
    return NULL;
}

void map_insert(Map *m, const char *key, const Value value, Type type) {
    if (((double)m->size / m->capacity) > m->load_factor) {
        map_resize(m, m->capacity * 2);
    }
    uint64_t i = fnv_hash(key, strlen(key)) % m->capacity;
    Entry* entryIter = m->buckets[i].next;
    while (entryIter != NULL) {
        if (strcmp(entryIter->key, key) == 0) {
           entry_update(entryIter, value, type);
           return;
        }
        entryIter = entryIter->next;
    }
    m->buckets[i].next = entry_create(key, value, m->buckets[i].next, type);
    m->size++;
    return;
}


Entry* map_search(Map *m, const char *key) {
    uint64_t i = fnv_hash(key, strlen(key)) % m->capacity;
    Entry* entryIter = m->buckets[i].next;
    while (entryIter != NULL) {
        if (strcmp(entryIter->key, key) == 0) {
            return entryIter;
        }
        entryIter = entryIter->next;
    }
    return NULL;
}

bool map_remove(Map *m, const char *key) {
    Entry* removeEntryPtr = map_search(m, key);
    if (removeEntryPtr != NULL) {
        Entry* entryIter = &(m->buckets[fnv_hash(key, strlen(key)) % m->capacity]);
        while (entryIter->next != removeEntryPtr) { entryIter = entryIter->next; }
        entryIter->next = removeEntryPtr->next;
        entry_delete(removeEntryPtr, false);
        m->size--;
        return true;
    }
    return false;
}

void map_dump(Map *m, FILE *stream, const DumpMode mode) {
    for (int i = 0; i < m->capacity; i++) {
        Entry* entryIter = m->buckets[i].next;
        while (entryIter != NULL) {
            entry_dump(entryIter, stream, mode);
            entryIter = entryIter->next;
        }
    }
}

void map_resize(Map *m, size_t new_capacity) {
    Entry* newBuckets = calloc(new_capacity, sizeof(Entry));
    for (int i = 0; i < m->capacity; i++) {
        Entry* entryIter = m->buckets[i].next;
        while (entryIter != NULL) {
            uint64_t newI = fnv_hash(entryIter->key, strlen(entryIter->key)) % new_capacity;
            Entry* tempIter = entryIter->next;
            Entry* head = newBuckets[newI].next;
            newBuckets[newI].next = entryIter;
            entryIter->next = head;
            entryIter = tempIter;
        }
    }
    free(m->buckets);
    m->capacity = new_capacity;
    m->buckets = newBuckets;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */

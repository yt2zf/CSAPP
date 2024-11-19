/* 
 * csim.h - Prototypes for Cache struct and functions
 */

#include <stddef.h>

#ifndef CSIM_MAIN_H
#define CSIM_MAIN_H

// use singly-linked list to represent cahcelines
typedef struct cacheLine{
    unsigned int tag;
    struct cacheLine *next;
} cacheline_t;

typedef struct cacheLineQueue{
    cacheline_t *head;
    cacheline_t *tail;
    size_t size;
    size_t capacity;
} cacheLine_queue_t;

typedef struct cache{
    size_t size;
    // set array, sets[i] is the address of cacheLine queue in i-th set
    cacheLine_queue_t **sets; 
} cache_t;

cache_t *initCache();
void freeCache(cache_t *cache);

void insertLast(cacheLine_queue_t *queue, cacheline_t *cacheline);
cacheline_t *removeByTag(cacheLine_queue_t *queue, unsigned int tag);

void load(cache_t *cache, int address);
void store(cache_t *cache, int address);
void modify(cache_t *cache, int address);
void evict(cache_t *cache, int address);

#endif /* CSIM_MAIN_H */

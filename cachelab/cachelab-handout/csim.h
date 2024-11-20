/* 
 * csim.h - Prototypes for Cache struct and functions
 */

#include <stddef.h>
#include <stdbool.h>

#ifndef CSIM_MAIN_H
#define CSIM_MAIN_H

typedef int cache_access_status_t; // hit, miss, miss+evict
#define CACHE_HIT  1
#define CACHE_MISS 2
#define CACHE_MISS_EVICT 3

struct cacheAccessSummary{
  unsigned int num_hits;
  unsigned int num_misses;
  unsigned int num_evictions;
};

// use singly-linked list to represent cahcelines
typedef struct cacheLine{
    unsigned long tag;
    struct cacheLine *next;
} cacheline_t;

typedef struct cacheLineQueue{
    cacheline_t *head;
    cacheline_t *tail;
    size_t size;
    size_t capacity;
} cacheLine_queue_t;

typedef struct cache{
    int sBits;
    int bBits;
    size_t size;
    // set array, sets[i] is the address of cacheLine queue in i-th set
    cacheLine_queue_t **sets; 
} cache_t;

/* functions for Cache type*/
cache_t *initCache();
void freeCache(cache_t *cache);
cache_access_status_t accessCache(cache_t *cache, unsigned long address);
cache_access_status_t load(cache_t *cache, unsigned long address);
cache_access_status_t store(cache_t *cache, unsigned long address);
cache_access_status_t *modify(cache_t *cache, unsigned long address);

/* functions for CacheLine queue type*/
bool insertAtTail(cacheLine_queue_t *queue, unsigned long tag);
bool removeFromHead(cacheLine_queue_t *queue);
bool accessRefresh(cacheLine_queue_t *queue, unsigned long tag);

#endif /* CSIM_MAIN_H */

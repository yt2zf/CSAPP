#include "cachelab.h"
#include "csim.h"
#include <stdlib.h>
#include <stdio.h>

struct cacheAccessSummary globalSummary;

cache_t *initCache(int sBits, size_t E, int bBits){
    cache_t *cache = malloc(sizeof(cache_t));
    if (cache == NULL){
        printf("ERROR: cache == NULL\n");
        return NULL;
    }
    cache->sBits = sBits;
    cache->bBits = bBits;
    cache->size = 1 << sBits;
    cache->sets = malloc(cache->size * sizeof(cacheLine_queue_t *));
    if (cache->sets == NULL){
        printf("ERROR: cache->sets == NULL\n");
        free(cache);
        return NULL;
    }

    for (size_t i = 0; i<cache->size; i++){
        cache->sets[i] = malloc(sizeof(cacheLine_queue_t));
        if (cache->sets[i] == NULL){
            printf("ERROR: cache->sets[i] == NULL\n");
            for (size_t j=0; j<i; j++){
                free(cache->sets[j]);
            }
            free(cache->sets);
            free(cache);
            return NULL;
        }
        cache->sets[i]->capacity = E;
        cache->sets[i]->size = 0;
        cache->sets[i]->head = NULL;
        cache->sets[i]->tail = NULL;
    }
    return cache;
}

void freeCache(cache_t *cache){
    if (cache == NULL){
        return;
    }
    size_t numOfSets = cache->size;
    for (size_t i = 0; i < numOfSets; i++){
        size_t numOfLines = cache->sets[i]->size;
        cacheline_t *cur = cache->sets[i]->head;
        for (size_t j=0; j < numOfLines; j++){
            cacheline_t *next = cur->next;
            free(cur);
            cur = next;
        }
       
        free(cache->sets[i]);
    }
    free(cache->sets);
    free(cache);
}

void insertLastByTag(cacheLine_queue_t *queue, unsigned long tag){
    if (queue == NULL || queue->size >= queue->capacity){
        return;
    }
    cacheline_t *newLine = malloc(sizeof(cacheline_t));
    if (newLine == NULL){
        printf("ERROR: malloc(sizeof(cacheline_t)) == NULL\n");
        return;
    }
    newLine->tag = tag;
    newLine->next = NULL;
    if (queue->tail == NULL){
        queue->head = newLine;
        queue->tail = newLine;
    } else{
        queue->tail->next = newLine;
        queue->tail = newLine;
    }
    queue->size++;
}

void insertLast(cacheLine_queue_t *queue, cacheline_t *cacheline){
    if (cacheline == NULL || queue == NULL || queue->size >= queue->capacity){
        return;
    }
    cacheline->next = NULL;
    if (queue->tail == NULL){
        queue->head = cacheline;
        queue->tail = cacheline;
    } else{
        queue->tail->next = cacheline;
        queue->tail = cacheline;
    }
    queue->size++;
}

cacheline_t *popByTag(cacheLine_queue_t *queue, unsigned long tag){
    if (queue == NULL || queue->size == 0){
        return NULL;
    }
    if (queue->head->tag == tag){
        cacheline_t *oldHead = queue->head;
        queue->head = queue->head->next;
        queue->size--;

        if (queue->size <= 1){
            queue->tail = queue->head;
        }
        oldHead->next = NULL;
        return oldHead;
    }

    cacheline_t *prev = queue->head;
    cacheline_t *cur = queue->head->next;
    while (cur != NULL){
        if (cur->tag == tag){
            prev->next = cur->next;
            cur->next = NULL;
            queue->size--;
            if (queue->size <= 1){
                queue->tail = queue->head;
            }
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }
    return NULL;
}

bool removeHead(cacheLine_queue_t *queue){
    if (queue==NULL || queue->size == 0){
        return false;
    }
    cacheline_t *oldHead = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    if (queue->size <= 1){
        queue->tail = queue->head;
    }
    free(oldHead);
    return true;
}

cache_access_status_t accessCache(cache_t *cache, unsigned long address){
    unsigned long setIndex = ((1 << (cache->sBits)) - 1) & (address >> (cache->bBits));
    unsigned long tag = address >> (cache->sBits + cache->bBits);
    
    // access后的cache line放入队尾
    cacheline_t *hitLine = popByTag(cache->sets[setIndex], tag);
    if (hitLine != NULL){
        // hit
        insertLast(cache->sets[setIndex], hitLine);
        return CACHE_HIT;
    } else{
        // miss
        if (cache->sets[setIndex]->size == cache->sets[setIndex]->capacity){
            // miss + evict
            removeHead(cache->sets[setIndex]);
            insertLastByTag(cache->sets[setIndex], tag);
            return CACHE_MISS_EVICT;
        }
        insertLastByTag(cache->sets[setIndex], tag);
        return CACHE_MISS;
    }
}

cache_access_status_t load(cache_t *cache, unsigned long address){
    cache_access_status_t status =  accessCache(cache, address);
    switch (status){
        case CACHE_HIT:
            globalSummary.num_hits += 1;
            break;
        case CACHE_MISS:
            globalSummary.num_misses += 1;
            break;
        case CACHE_MISS_EVICT:
            globalSummary.num_misses += 1;
            globalSummary.num_evictions += 1;
            break;
    }
    return status;
}

cache_access_status_t store(cache_t *cache, unsigned long address){
    cache_access_status_t status =  accessCache(cache, address);
    switch (status){
        case CACHE_HIT:
            globalSummary.num_hits += 1;
            break;
        case CACHE_MISS:
            globalSummary.num_misses += 1;
            break;
        case CACHE_MISS_EVICT:
            globalSummary.num_misses += 1;
            globalSummary.num_evictions += 1;
            break;
    }
    return status;
}

cache_access_status_t *modify(cache_t *cache, unsigned long address){
    static cache_access_status_t states[2];
    states[0] = load(cache, address);
    states[1] = store(cache, address);
    return states;
}


int main() { 
    globalSummary.num_hits = 0;
    globalSummary.num_misses = 0;
    globalSummary.num_evictions = 0;

    cache_t *cache = initCache(4, 1, 4);
    load(cache, 0x10);
    modify(cache, 0x20);
    load(cache, 0x22);
    store(cache, 0x18);
    load(cache, 0x110);
    load(cache, 0x210);
    modify(cache, 0x12);

    printSummary(globalSummary.num_hits, globalSummary.num_misses, globalSummary.num_evictions);
    freeCache(cache);
    return 0;
}

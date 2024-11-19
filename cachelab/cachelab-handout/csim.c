#include "cachelab.h"
#include "csim.h"
#include <stdlib.h>
#include <stdio.h>

cache_t *initCache(size_t size, size_t E){
    cache_t *cache = malloc(sizeof(cache_t));
    if (cache == NULL){
        printf("ERROR: cache == NULL\n");
        return NULL;
    }
    cache->size = size;
    cache->sets = malloc(size * sizeof(cacheLine_queue_t *));
    if (cache->sets == NULL){
        printf("ERROR: cache->sets == NULL\n");
        free(cache);
        return NULL;
    }

    for (size_t i = 0; i<size; i++){
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
    for (size_t i = 0; i<numOfSets; i++){
        size_t numOfLines = cache->sets[i]->size;
        cacheline_t *cur = cache->sets[i]->head;
        for (size_t j=0; j<numOfLines; j++){
            cacheline_t *next = cur->next;
            free(cur);
            cur = next;
        }
        free(cache->sets[i]);
    }
    free(cache);
}



void insertLast(cacheLine_queue_t *queue, cacheline_t *cacheline){
    if (queue == NULL || queue->size >= queue->capacity){
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

cacheline_t* removeByTag(cacheLine_queue_t *queue, unsigned int tag){
    if (queue == NULL || queue->size == 0){
        return NULL;
    }
    cacheline_t *prev = queue->head;
    cacheline_t *cur = queue->head->next;
    while (cur != NULL){
        if (cur->tag == tag){
            prev->next = cur->next;
            cur->next = NULL;
            return cur;
        }
        prev = cur;
        cur = cur->next;
    }
    return NULL;
}

int main()
{   
    cache_t *cache = initCache(8, 4);
    printSummary(0, 0, 0);
    
    free(cache);
    return 0;
}

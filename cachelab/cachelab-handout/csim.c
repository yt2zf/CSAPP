#include "cachelab.h"
#include "csim.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>

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

bool insertAtTail(cacheLine_queue_t *queue, unsigned long tag){
    if (queue == NULL || queue->size >= queue->capacity){
        return false;
    }
    cacheline_t *newLine = malloc(sizeof(cacheline_t));
    if (newLine == NULL){
        printf("ERROR: malloc(sizeof(cacheline_t)) == NULL\n");
        return false;
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
    return true;
}

/* access 后的cache line放入队尾，表示使用时间最新; 
return false说明队列里没有找到对应tag
*/
bool accessRefresh(cacheLine_queue_t *queue, unsigned long tag){
     if (queue == NULL || queue->size == 0){
        return false;
    }

    /* 检查对应的cahceline是否本来就在队尾 */
    if (queue->tail->tag == tag){
        return true;
    } else{
        if (queue->size == 1){
            return false;
        }
    }

    /* pop the cacheline by given tag START*/
    cacheline_t *hitLine = NULL;
    if (queue->head->tag == tag){
        cacheline_t *oldHead = queue->head;
        queue->head = queue->head->next;
        queue->size--;
        if (queue->size <= 1){
            queue->tail = queue->head;
        }
        oldHead->next = NULL;
        hitLine = oldHead;
    } else{
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
                hitLine =  cur;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
    }
    /* pop the cacheline by given tag  END*/

    if (hitLine != NULL){
        // hit and insert hitLine at tail of queue;
        queue->tail->next = hitLine; // queue->tail cant be null here, since pop only executes when size > 1
        queue->tail = hitLine;
        queue->size++; 
        return true;
    }
    return false;
}

bool removeFromHead(cacheLine_queue_t *queue){
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
    if (accessRefresh(cache->sets[setIndex], tag)){
        // hit
        return CACHE_HIT;
    } else{
        // miss
        if (cache->sets[setIndex]->size == cache->sets[setIndex]->capacity){
            // miss + evict
            removeFromHead(cache->sets[setIndex]);
            insertAtTail(cache->sets[setIndex], tag);
            return CACHE_MISS_EVICT;
        }
        insertAtTail(cache->sets[setIndex], tag);
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

/*
 * usage - Print usage info
 */
void usage(char *argv[]){
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h          Print this help message.\n");
    printf("  -v          Optional verbose flag\n");
    printf("  -s <num>    Number of set index bits.\n");
    printf("  -E <num>    Number of lines per set.\n");       
    printf("  -b <num>    Number of block offset bits.\n");
    printf("  -t <file>   Trace file.\n");
}

char *getAccessStatusStr(cache_access_status_t state){
    switch (state){
        case CACHE_HIT:
            return "hit";
            break;
        case CACHE_MISS:
            return "miss";
            break;
        case CACHE_MISS_EVICT:
            return "miss eviction";
            break;
        case CACHE_UNDEFINED:
        default:
            return "";
            break;
    }
}


int main(int argc, char *argv[]) {
    /* init cache summary */
    globalSummary.num_hits = 0;
    globalSummary.num_misses = 0;
    globalSummary.num_evictions = 0;
    
    /* parse commond-line arguments */
    int sBits;
    int E;
    int bBits;
    char *filename;
    bool verbose = false;

    char cmdC;
    char *optstring = "s:E:b:t:hv";
    while ((cmdC = getopt(argc, argv, optstring)) != -1){
        switch(cmdC){
            case 's':
                sBits = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                bBits = atoi(optarg);
                break;
            case 't':
                filename = optarg;
                break;
            case 'h':
                usage(argv);
                break;
            case 'v':
                verbose = true;
                break;
            default:
                verbose = false;
                break;
        }
    }

    /* simulating */
    cache_t *cache = initCache(sBits, E, bBits);
    char cmd[3];
    unsigned long address = 0;
    int readSize = 0;
    FILE* trace_fp = fopen(filename, "r");
    assert(trace_fp);

    cache_access_status_t accessStates[2];
    while (fscanf(trace_fp, "%s %lx,%d\n", cmd, &address, &readSize) != EOF){
        accessStates[0] = CACHE_UNDEFINED;
        accessStates[1] = CACHE_UNDEFINED;
        switch (cmd[0]){
            case 'L':
                accessStates[0] = load(cache, address);
                break;
            case 'M':
                cache_access_status_t *tmp = modify(cache, address);
                accessStates[0] = tmp[0];
                accessStates[1] = tmp[1];
                break;
            case 'S':
                accessStates[0] = store(cache, address);
                break;
        }
        if (verbose){
            printf("%s %lx,%d %s %s\n", cmd, address, readSize, getAccessStatusStr(accessStates[0]), getAccessStatusStr(accessStates[1]));
        }
    }

    // output result
    fclose(trace_fp);
    printSummary(globalSummary.num_hits, globalSummary.num_misses, globalSummary.num_evictions);
    freeCache(cache);
    return 0;
}

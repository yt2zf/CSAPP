#ifndef __SEMBUF_H__
#define __SEMBUF_H__

#include <semaphore.h>

typedef struct{
    int *buf;  /* Buffer array*/
    int n;     /* capacity */
    int front; /* buf[(front+1)%n] is first item */
    int rear;  /* buf[rear%n] is last item */
    sem_t mutex; /* Protects access to buf */
    sem_t slots; /* Counts available slots */
    sem_t items; /* Counts available items */ 
} sembuf_t;

void sembuf_init(sembuf_t *sp, int n);
void sembuf_free(sembuf_t *sp);
void sembuf_insert(sembuf_t *sp, int item);
int sembuf_remove(sembuf_t *sp);  

#endif
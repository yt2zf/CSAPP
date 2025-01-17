#include "csapp.h"
#include "sembuf.h"

void sembuf_init(sembuf_t *sp, int n){
    sp->buf = Calloc(n, sizeof(int));
    sp->n = n;
    sp->front = sp->rear = 0;
    Sem_init(&sp->mutex, 0, 1);
    Sem_init(&sp->slots, 0, n);
    Sem_init(&sp->items, 0, 0);
}

void sembuf_free(sembuf_t *sp){
    Free(sp->buf);
}

void sembuf_insert(sembuf_t *sp, int item){
    P(&sp->slots); /* wait for available slot */
    P(&sp->mutex); /* lock the buffer */
    sp->buf[(++sp->rear)%(sp->n)] = item;
    V(&sp->mutex);
    V(&sp->items);
}

int sembuf_remove(sembuf_t *sp){
    int item;
    P(&sp->items); /* Wait for available item*/
    P(&sp->mutex); /* lock the buffer */
    item = sp->buf[(++sp->front)%(sp->n)];
    V(&sp->mutex);
    V(&sp->slots);
    return item;
}
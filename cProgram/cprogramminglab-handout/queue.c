/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL){
      return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{   
    if (q == NULL){
      return;
    }
    if (q->head == NULL){
      free(q);
      return;
    }
    /* Free queue structure */
    list_ele_t *cur = q->head;
    list_ele_t *next = cur->next;
    free(cur->value);
    free(cur);
    free(q);

    while (next != NULL){
        cur = next;
        next = cur->next;

        /* free strings*/
        free(cur->value);
        /* free elements*/
        free(cur);
    }
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{   
    if (q == NULL){
      return false;
    }
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL){
      return false;
    }

    /* allocate space for string */
    char *newv;
    newv = malloc((strlen(s) + 1)*sizeof(char));
    if (newv == NULL){
      free(newh);
      return false;
    }
    strcpy(newv, s);

    /* set list_ele_t and queue head*/
    newh->value = newv;
    newh->next = q->head;
    q->head = newh;
    if (q->size == 0){
      q->tail = newh;
    }
    q->size++;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{   
    if (q == NULL){
      return false;
    }
    // O(1) time

    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    if (newt == NULL){
      return false;
    }

    /* allocate space for string */
    char *newv;
    newv = malloc((strlen(s) + 1)*sizeof(char));
    if (newv == NULL){
      free(newt);
      return false;
    }
    strcpy(newv, s);

    /* set list_ele_t and queue tail;*/
    newt->value = newv;
    newt->next = NULL;
    if (q->tail == NULL){
      q->head = newt;
      q->tail = newt;
    } else{
      q->tail->next = newt;
      q->tail = newt;
    }
   
    q->size++;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{   
    if (q==NULL || q->size == 0){
      return false;
    }

    list_ele_t *temp = q->head;
    q->head = q->head->next;
    q->size--;
    if (q->size == 1){
      q->tail = q->head;
    } else if (q->size == 0){
      q->tail = NULL;
    }

    if (sp != NULL){
      bufsize = bufsize < strlen(temp->value) + 1? bufsize:strlen(temp->value) + 1;
      strncpy(sp, temp->value, bufsize);
      sp[bufsize-1] = '\0';
    }
    free(temp->value);
    free(temp);
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{   
    if (q == NULL){
      return 0;
    }
    return q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{   
    if (q == NULL || q->size <= 1){
      return;
    }

    list_ele_t *prev = NULL;
    list_ele_t *cur = q->head;
    list_ele_t *tmp;

    while (cur != NULL){
       tmp = cur->next;
       cur->next = prev;
       prev = cur;
       cur = tmp;
    }

    /* change head and tail of queue */
    tmp = q->head;
    q->head = q->tail;
    q->tail = tmp;
}


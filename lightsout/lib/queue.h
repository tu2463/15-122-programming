/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
/*
 * Queues
 *
 * 15-122 Principles of Imperative Computation */

#include <stdbool.h>

#ifndef QUEUE_H
#define QUEUE_H

/*********************/
/* Client interface */
/*********************/

// typedef ______ queue_elem;
typedef void *queue_elem;

typedef void print_data_fn(void *x);
typedef void queue_elem_free_fn(queue_elem x);

/*********************/
/* Library interface */
/*********************/

// typedef ______* queue_t;
typedef struct queue_header *queue_t;

bool queue_empty(queue_t Q)
  /*@requires Q != NULL; @*/ ;

queue_t queue_new()
  /*@ensures \result != NULL; @*/ ;

void enq(queue_t Q, queue_elem x)
  /*@requires Q != NULL; @*/ ;

queue_elem deq(queue_t Q)
  /*@requires Q != NULL; @*/ ;

void queue_print(queue_t Q, print_data_fn* print_data)
  /*@requires Q != NULL && print_data != NULL; @*/ ;

void queue_free(queue_t Q, queue_elem_free_fn* elem_free)
  /*@requires Q != NULL; @*/
  /* if elem_free is NULL, then elements will not be freed */ ;

#endif

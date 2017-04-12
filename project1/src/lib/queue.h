#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef void (*freeFunc)(void *);

//typedef enum { FALSE, TRUE } bool;

typedef struct {
  void *a_elements[];
  int current;
  int queue_max_size;
  int logical_size;
  int elementSize;
  int aval_pos;
  freeFunc freeFn;
} queue;

void queue_new(queue *queue, int elementSize, int maxsize ,freeFunc freeFn);
void queue_destroy(queue *queue);

void queue_push(queue *queue, void *element);
void * queue_peak(queue *queue);
void * queue_pop(queue *queue);

#endif //_QUEUE_H_

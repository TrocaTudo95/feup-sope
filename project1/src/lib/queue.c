#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "minunit.h" // unit testing

#include "queue.h"

void queue_new(queue *queue, int elementSize, int maxsize, freeFunc freeFn){

        assert(elementSize > 0);
        queue->elementSize = elementSize;
        queue->freeFn = freeFn;
        queue->queue_max_size = maxsize;
        queue->logical_size = 0;
        queue->current = 0;
        queue->aval_pos = -1;
}

void queue_destroy(queue *queue){
        int i = 0;
        for (i = 0; i < queue->queue_max_size; i++) {
                if(queue->a_elements[i]!= NULL)
                        queue->freeFn(queue->a_elements[i]);
        }
        free(queue->a_elements);
        free(queue);
}

void queue_push(queue *queue, void *element){
        assert(element != NULL);

        if(queue->aval_pos == -1)
                queue->a_elements[queue->logical_size] = element;
        else {
                queue->a_elements[queue->aval_pos] = element;
                queue->aval_pos = queue->aval_pos+ 1;
                if(queue->aval_pos == queue->current)
                        queue->aval_pos = 0;
        }

        queue->logical_size = queue->logical_size +1;
        assert(queue->logical_size < queue->queue_max_size);
}

void * queue_peak(queue *queue){
  return queue->a_elements[queue->current];
}

void * queue_pop(queue *queue){

}

#ifndef CODA_THREADSAFE_LIBRARY_H
#define CODA_THREADSAFE_LIBRARY_H

#include <sys/semaphore.h>

typedef struct node {
    int value;
    struct node *next;
} node;

typedef struct queue_t {
    node *front;
    node *prev;
    sem_t mutex;
    sem_t popsem;
} Queue;

Queue* qcreate();

void qpush(Queue* queue, int value, int nthread);

void qpop(Queue* queue, int *value, int nthread);

void qdestroy(Queue* queue);

void nanopause();

#endif //CODA_THREADSAFE_LIBRARY_H

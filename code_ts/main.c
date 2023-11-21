#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "library.h"

#define N 3
/*
// Struttura di un nodo della coda thread-safe
typedef struct node {
    int value;
    struct node *next;
} node;

// Struttura della coda thread-safe (usa una linked list)
typedef struct queue_t {
    node *front;
    node *prev;
    //dispatch_semaphore_t mutex;
    //dispatch_semaphore_t popsem;
    sem_t mutex;
    sem_t popsem;
} Queue;

Queue *queue;

// crea una coda vuota
Queue* qcreate()
{
    // crea la coda
    Queue *queue = malloc(sizeof(Queue));

    // inizializza la coda
    queue->front = NULL;
    queue->prev  = NULL;
    sem_init(&queue->mutex, 0, 1);
    //dispatch_semaphore_t *mut = &queue->mutex;
    // *mut = dispatch_semaphore_create(1);
    sem_init(&queue->mutex, 0, 0);
    //dispatch_semaphore_t *sem = &queue->popsem;
    // *sem = dispatch_semaphore_create(0);
    return queue;
}

// Inserisce un elemento nella coda
void push(Queue* queue, int value, int npers)
{
    //dispatch_semaphore_wait(queue->mutex, DISPATCH_TIME_FOREVER);
    sem_wait(&queue->mutex);
    printf("Sono %d e ho il mutex\n", npers);

    // creazione di un nuovo nodo
    node *temp = malloc(sizeof(struct node));
    temp->value = value;
    temp->next  = NULL;

    // verifico se la coda è vuota
    if (queue->front == NULL) {
        //in questo caso front e prev coincidono
        queue->front = temp;
        queue->prev  = temp;
        printf("Inserisco nella coda il valore: %d, e faccio post su popsem\n", value);
        sem_post(&queue->popsem);
        //dispatch_semaphore_signal(queue->popsem);
    }
    else {
        //aggiunta di un elemento
        node *old_prev = queue->prev;
        old_prev->next = temp;
        queue->prev = temp;
        printf("Inserisco nella coda il valore: %d\n", value);
    }

    sem_post(&queue->mutex);
    printf("Sono %d e ho lasciato il mutex\n", npers);
    //dispatch_semaphore_signal(queue->mutex);
}

// Estrae un elemento dalla coda
void pop(Queue* queue, int *value, int npers)
{
    sem_wait(&queue->mutex);
    printf("Sono %d e ho il mutex\n", npers);
    //dispatch_semaphore_wait(queue->mutex, DISPATCH_TIME_FOREVER);

    // verifico se la coda è vuota
    node *front = queue->front;

    if (front != NULL) {
    // lettura del valore ed cancellazione dell'elemento dalla coda
        *value = front->value;
        queue->front = front->next;
        free(front);
        printf("Estraggo dalla coda il valore: %d\n", *value);
        //sem_post(&queue->popsem);
        //dispatch_semaphore_signal(queue->popsem);
    } else {
        printf("la coda è vuota, mi blocco:\n");
    }
    sem_post(&queue->mutex);
    printf("Sono %d e ho lasciato mutex\n", npers);
    //dispatch_semaphore_signal(queue->mutex);
    sem_wait(&queue->popsem);
    //dispatch_semaphore_wait(queue->popsem, DISPATCH_TIME_FOREVER);
}
*/
Queue *queue;

void *thread(void *arg){
    int nthread = *(int *)arg;
    int info;
    int infoo;
    for (int i = 0; i < N; i++)
    {
        int fval = rand()%10 + 1;
        int sval = rand()%10 + 1;

        qpush(queue, fval,nthread);
        qpop(queue, &info, nthread);

        qpop(queue, &infoo, nthread);

        qpush(queue, sval, nthread);
    }
    pthread_exit(0);
}

int main(){
    pthread_attr_t a;
    pthread_t threads[N];
    int nthread[N];

    queue = qcreate();
    printf("Coda creata\n");

    pthread_attr_init(&a);

    for(int i=0; i < N; i++){
        nthread[i] = i;
        printf("Sto per creare il thread %d-esimo\n", nthread[i]);
        pthread_create(&threads[i], &a, thread, &nthread[i]);
    }

    for(int i=0; i < N; i++){
        void *ret;
        pthread_join(threads[i], &ret);
    }
    qdestroy(queue);

    return 0;
}


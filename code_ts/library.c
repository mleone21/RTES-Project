#include "library.h"

#include <stdio.h>
#include <stdlib.h>


// crea una coda vuota
Queue* qcreate()
{
    // crea la coda
    Queue *queue = malloc(sizeof(Queue));

    // inizializza la coda
    queue->front = NULL;
    queue->prev  = NULL;
    sem_init(&queue->mutex, 0, 1);
    sem_init(&queue->mutex, 0, 0);
    return queue;
}

// Inserisce un elemento nella coda
void qpush(Queue* queue, int value, int nthread)
{
    sem_wait(&queue->mutex);
    printf("Sono il thread numero %d e ho il mutex\n", nthread);

    // creazione di un nuovo nodo
    node *temp = malloc(sizeof(struct node));
    temp->value = value;
    temp->next  = NULL;

    // verifico se la coda è vuota
    if (queue->front == NULL) {
        //in questo caso front e prev coincidono
        queue->front = temp;
        queue->prev  = temp;
        printf("Sono il thread numero %d e inserisco nella coda il valore: %d, e faccio post su popsem\n", nthread, value);
        sem_post(&queue->popsem);
    }
    else {
        //aggiunta di un elemento
        node *old_prev = queue->prev;
        old_prev->next = temp;
        queue->prev = temp;
        printf("Sono il thread numero %d e inserisco nella coda il valore: %d\n", nthread, value);
    }

    sem_post(&queue->mutex);
    printf("Sono il thread numero %d e ho lasciato il mutex\n", nthread);
}

// Estrae un elemento dalla coda
void qpop(Queue* queue, int *value, int nthread)
{
    sem_wait(&queue->mutex);
    printf("Sono il thread numero %d e ho il mutex\n", nthread);

    // verifico se la coda è vuota
    node *front = queue->front;

    if (front != NULL) {
        // lettura del valore ed cancellazione dell'elemento dalla coda
        *value = front->value;
        queue->front = front->next;
        free(front);
        printf("Sono il thread numero %d ed estraggo dalla coda il valore: %d\n", nthread, *value);
    } else {
        printf("Sono il thread numero %d, la coda è vuota, mi blocco:\n", nthread);
    }
    sem_post(&queue->mutex);
    printf("Sono il thread numero %d e ho lasciato mutex\n", nthread);
    sem_wait(&queue->popsem);
}

void qdestroy (Queue* queue){
    sem_destroy(&queue->mutex);
    sem_destroy(&queue->popsem);
    free(queue);
}
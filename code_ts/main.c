#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "library.h"

#define N 7

Queue *queue;

void *thread(void *arg){
    int nthread = *(int *)arg;
    int popval, testpopval, testpushval;
    double pushtime, poptime;

    testpushval = rand()%10 + 1;

    clock_t beginpush = clock();
    qpush(queue, testpushval, nthread);
    clock_t endpush = clock();
    pushtime = (double)(endpush - beginpush) / CLOCKS_PER_SEC;

    clock_t beginpop = clock();
    qpop(queue, &testpopval, nthread);
    clock_t endpop = clock();
    poptime = (double)(endpop - beginpop) / CLOCKS_PER_SEC;
    printf("Sono il thread numero %d e il timeTest vale: push %lf, pop %lf\n", nthread + 1, pushtime, poptime);
    nanopause();

    for (int i = 0; i < N; i++)
    {
        int fval = rand()%10 + 1;
        int sval = rand()%10 + 1;

        qpush(queue, fval, nthread);
        qpush(queue, sval, nthread);
        qpop(queue, &popval, nthread);
        qpop(queue, &popval, nthread);
        qpop(queue, &popval, nthread);
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

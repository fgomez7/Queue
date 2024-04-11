#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>
#include "queue.h"

typedef struct queue
{
    int head;
    int tail;
    int size;
    // int counter;
    void ** buffer;
    // pthread_mutex_t lock;
    // pthread_cond_t fullBuffer;
    // pthread_cond_t emptyBuffer;
    sem_t mutex;
    sem_t fullsem;
    sem_t emptysem;
}queue;

queue_t *queue_new(int size){
    queue_t * newQueue = malloc(sizeof(queue));
    newQueue -> head = 0;
    newQueue -> tail = 0;
    newQueue -> size = size;
    // newQueue -> counter = 0;
    newQueue -> buffer = malloc(sizeof(void*) * size);
    // pthread_mutex_init(&(newQueue->lock), NULL);
    // pthread_cond_init(&(newQueue ->emptyBuffer), NULL);
    // pthread_cond_init(&(newQueue -> fullBuffer), NULL);
    sem_init(&(newQueue->mutex), 0, 1);
    sem_init(&(newQueue->fullsem), 0, 0);
    sem_init(&(newQueue->emptysem), 0, size);
    return newQueue;
}

void queue_delete(queue_t **q){
    // pthread_mutex_destroy(&(*q)->lock);
    // pthread_cond_destroy(&(*q)->fullBuffer);
    // pthread_cond_destroy(&(*q)->emptyBuffer);
    free((*q)->buffer);
    sem_destroy(&(*q)->mutex);
    sem_destroy(&(*q)->fullsem);
    sem_destroy(&(*q)->emptysem);
    free(*q);
    *q = NULL;
}

bool queue_push(queue_t *q, void *elem){
    if (q == NULL){
        return false;
    }
    // pthread_mutex_lock(&(q->lock));
    // while (q->counter == q->size){pthread_cond_wait(&(q->fullBuffer), &(q->lock));}
    // q->buffer[q->tail] = elem;
    // q->tail = (q->tail += 1) % q->size;
    // q->counter += 1;
    // pthread_cond_signal(&(q->emptyBuffer));
    // pthread_mutex_unlock(&(q->lock));
    // return true;
    sem_wait(&(q->emptysem));
    sem_wait(&(q->mutex));
    q->buffer[q->tail] = elem;
    q->tail = (q->tail + 1) % q->size;
    sem_post(&(q->mutex));
    sem_post(&(q->fullsem));
    return true;
}

bool queue_pop(queue_t *q, void **elem){
    if (!q){
        return false;
    }
    if (&(*elem) == NULL){
        return false;
    }
    // pthread_mutex_lock(&(q->lock));
    // while (q->counter == 0){ pthread_cond_wait(&(q->emptyBuffer), &(q->lock));}
    // *elem = q->buffer[q->head];
    // q->head = (q->head += 1) % q->size;
    // q->counter -= 1;
    // pthread_cond_signal(&(q->fullBuffer));
    // pthread_mutex_unlock(&(q->lock));
    // return true;
    sem_wait(&(q->fullsem));
    sem_wait(&(q->mutex));
    *elem = q->buffer[q->head];
    q->head = (q->head + 1) % q->size;
    sem_post(&(q->mutex));
    sem_post(&(q->emptysem));
    return true;
}

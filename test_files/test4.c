#include <pthread.h>
#include <unistd.h>
#include "queue.h"

void *thread1(void *args){

    queue_t *q = ((void **)args)[0];
    pthread_cond_t *c = ((void **)args)[1];
    int *v = ((void **)args)[2];
    // push a 1
    queue_push(q, NULL);
    bool bF = queue_push(NULL, (void*)'I');
    *v = 1;

    pthread_cond_signal(c);
    if (bF == true){
        return (void*)1;
    }
    return NULL;

}
void *thread2(void *args){
    queue_t *q = ((void **)args)[0];
    pthread_cond_t *c = ((void **)args)[1];
    int *v = ((void **)args)[2];
    if (!*v) {
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(c, &mutex);
        pthread_mutex_unlock(&mutex);
    }

    // expect to pop 'H'
    void *r;
    queue_pop(q, &r);
    bool popF = queue_pop(q, NULL);
    if (r != NULL){
        return (void*)1;
    }
    // if (((char)r != 'H')){
    //     // if not, then we failed
    //     return (void *)1;
    // }
    if (popF == true){
        return (void*)1;
    }
    return NULL;
}

int main(int argc, char**argv){
    (void)argc;
    (void)argv;
    queue_t *q = queue_new(10);
    if (q == NULL){
        return 1;
    }
    pthread_t t1, t2;
    pthread_cond_t c;
    pthread_cond_init(&c, NULL);
    int v = 0;

    void *args[3] = {q, &c, &v};

    pthread_create(&t1, NULL, thread1, args);
    pthread_create(&t2, NULL, thread2, args);

    void *rc;
    pthread_join(t1, &rc);
    if (rc != NULL)
        return 1;
    pthread_join(t2, &rc);
    if (rc != NULL)
        return 1;

    queue_delete(&q);
    return 0;

}

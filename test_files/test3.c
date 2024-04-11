#include "queue.h"

#include <pthread.h>
#include <unistd.h>

void *thread1(void *args) {
  queue_t *q = ((void **)args)[0];
  pthread_cond_t *c = ((void **)args)[1];
  int *v = ((void **)args)[2];
  // push a 1
  queue_push(q, (void *)'H');

  *v = 1;

  pthread_cond_signal(c);
  return NULL;
}

void *thread2(void *args) {
    queue_t *q = ((void **)args)[0];
    pthread_cond_t *c = ((void **)args)[1];
    int *v = ((void **)args)[2];
    queue_push(q, (void *)'I');
    *v = 1;
    pthread_cond_signal(c);
    return NULL;
}

void *thread3(void *args){
    queue_t *q = ((void **)args)[0];
    pthread_cond_t *c = ((void**)args)[1];
    int *v = ((void **)args)[2];
    if (!*v){
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(c, &mutex);
        pthread_mutex_unlock(&mutex);
    }
    void *r;
    void *s;
    queue_pop(q, &r);
    queue_pop(q, &s);
    if (((char)r != 'H') && ((char)s != 'I')){
        return (void *)1;
    }
    return NULL;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  queue_t *q = queue_new(1);
  if (q == NULL) {
    return 1;
  }
  pthread_t t1, t2, t3;

  pthread_cond_t c;
  pthread_cond_init(&c, NULL);
  int v = 0;

  void *args[3] = {q, &c, &v};

  pthread_create(&t1, NULL, thread1, args);
  pthread_create(&t2, NULL, thread2, args);
  pthread_create(&t3, NULL, thread3, args);

  void *rc;
  pthread_join(t1, &rc);
  if (rc != NULL)
    return 1;
  pthread_join(t2, &rc);
  if (rc != NULL)
    return 1;

  pthread_join(t3, &rc);
  if (rc != NULL){
    return 1;
  }

  queue_delete(&q);
  return 0;
}

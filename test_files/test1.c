#include <unistd.h>
#include "queue.h"

int main(int argc, char **argv){
    (void)argc;
    (void)argv;
    queue_t *q = queue_new(10);
    if (q == NULL){
        return 1;
    }

    queue_push(q, (void*)'h');
    queue_push(q, (void*)'i');

    void*r;
    void*s;
    queue_pop(q, &r);
    queue_pop(q, &s);

    if (((char)r != 'h') && ((char)s != 'i')){
        return 1;
    }

    queue_delete(&q);

    return 0;
}

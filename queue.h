#ifndef _QUEUE_H
#define _QUEUE_H

struct queue {
    void *data;
    struct queue *prev;
    struct queue *next;
};

typedef struct queue queue_t;

void queue_init(queue_t **);
queue_t *queue_append(queue_t *, void *);
queue_t *queue_prepend(queue_t *, void *);
queue_t *queue_remove(queue_t *, const void *);
unsigned int queue_length(queue_t *);
queue_t *queue_sort(queue_t *, int (*func) (const void *, const void *));
void queue_foreach(queue_t *, void (*func) (void *, void *), void *);
void queue_free(queue_t *);

#endif

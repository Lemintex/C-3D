#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdlib.h>
#include "mesh.h"

typedef struct node {
	triangle_t triangle;
	struct node *next;
} node_t;

typedef struct queue {
	node_t *front;
	node_t *rear;
	size_t len;
} queue_t;

queue_t *create_queue();

int is_empty(queue_t *queue);

void enqueue(queue_t *queue, triangle_t triangle);

triangle_t dequeue(queue_t *queue);

#endif

#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdlib.h>
#include "mesh.h"

typedef struct node {
	triangle_t triangle;
	struct node* next;
} node_t;


void enqueue(node_t **head, triangle_t triangle);

triangle_t dequeue(node_t **head);
#endif

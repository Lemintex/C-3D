#include "queue.h"

typedef struct node {
    int val;
    struct node *next;
} node_t;

void enqueue(node_t **head, triangle_t triangle) {
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node) return;

    new_node->triangle = triangle;
    new_node->next = *head;

    *head = new_node;
}

triangle_t dequeue(node_t **head) {
    node_t *current, *prev = NULL;
    int retval = -1;

    if (*head == NULL) return -1;

    current = *head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

	triangle_t triangle =current->triangle;
    free(current);
    
    if (prev)
        prev->next = NULL;
    else
        *head = NULL;

    return triangle;
}

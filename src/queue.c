#include "queue.h"

queue_t *create_queue() {
  queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
  queue->front = queue->rear = NULL;
  queue->len = 0;
  return queue;
}

int is_empty(queue_t *queue) { return (queue->len == 0); }

void enqueue(queue_t *queue, triangle_t triangle) {
  node_t *new_node = malloc(sizeof(node_t));
  if (!new_node)
    return;

  new_node->triangle = triangle;
  new_node->next = NULL;

  if (is_empty(queue)) {
    queue->front = queue->rear = new_node;
  } else {
    queue->rear->next = new_node;
    queue->rear = new_node;
  }
  queue->len = queue->len + 1;
}

triangle_t dequeue(queue_t *queue) {
  queue->len = queue->len - 1;

  node_t *front = queue->front;
  triangle_t triangle = front->triangle;

  queue->front = front->next;

  free(front);
  return triangle;
}

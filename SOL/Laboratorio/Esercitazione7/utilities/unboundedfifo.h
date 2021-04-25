#ifndef _UNBOUNDED_QUEUE_
#define _UNBOUNDED_QUEUE_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wrappers.h>

struct node;
struct queue;

struct queue* create_queue(size_t);
bool queue_is_empty(struct queue*);
void push(struct queue**, void*);
void* pop(struct queue*);
void free_queue(struct queue*);

#endif
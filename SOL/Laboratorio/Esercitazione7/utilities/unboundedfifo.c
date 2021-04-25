#include <unboundedfifo.h>

struct node
{
	void* data;
	struct node* next;
};

struct queue
{
	size_t data_size;
	struct node* first_node;
	struct node* last_node;
};

struct queue* create_queue(size_t dim)
{
	struct node* head;
	EXIT_IF_NULL(head, (struct node*) malloc(sizeof(struct node)), "malloc");

	head->data = NULL;
	head->next = NULL;

	struct queue* q;
	EXIT_IF_NULL(q, (struct queue*) malloc(sizeof(struct queue)), "malloc");
	q->data_size = dim;
	q->first_node = head;
	q->last_node = head;

	return q;
}

bool queue_is_empty(struct queue* q)
{
	return q->first_node == q->last_node;
}

void push(struct queue** q, void* data)
{
	struct node* new_node;
	EXIT_IF_NULL(new_node, (struct node*) malloc(sizeof(struct node)), "malloc");
	EXIT_IF_NULL(new_node->data, (void*) malloc((*q)->data_size), "malloc");
	memcpy(new_node->data, data, (*q)->data_size);
	new_node->next = NULL;

	if ((*q)->last_node == (*q)->first_node) // queue is empty
	{
		(*q)->first_node->next = new_node;
		(*q)->last_node = new_node;
	}
	else
	{
		(*q)->last_node->next = new_node;
		(*q)->last_node = new_node;
	}
}

void* pop(struct queue* q)
{
	if (q->first_node == q->last_node) // queue is empty
		return NULL;

	struct node* tmp = q->first_node->next;
	if (tmp->next != NULL)
		q->first_node->next = tmp->next;
	else
	{
		q->first_node->next = NULL;
		q->last_node = q->first_node;
	}

	void* data;
	EXIT_IF_NULL(data, (void*) malloc(q->data_size), "malloc");
	memcpy(data, tmp->data, q->data_size);

	free(tmp->data);
	free(tmp); // freeing popped node

	return data;
}

void free_queue(struct queue* q)
{
	if (!q)
		return;

	while (q->first_node->next != NULL)
	{
		struct node* tmp = q->first_node->next;
		q->first_node->next = (q->first_node->next)->next;

		free(tmp->data);
		free(tmp);
	}

	free(q->first_node);
	free(q);
}
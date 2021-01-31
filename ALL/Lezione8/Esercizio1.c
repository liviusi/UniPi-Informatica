#include <stdio.h>
#include <stdlib.h>

typedef struct _elem
{
    int key;
    struct _elem *next;
} elem;

void stampaContrario(elem *head)
{
    if ( head != NULL )
    {
        stampaContrario(head->next);
        printf("%d\n", head->key);
    }
}

void liberaLista(elem **head)
{
    elem *dummy;
    while ( (*head) != NULL )
    {
        dummy = *head;
        *head = (*head)->next;
        free(dummy);
    }
}

int main()
{
    int N, i, newval;
    elem *head = NULL, *curr = NULL, *new_node = NULL;;
    scanf("%d", &N);
    for ( i = 0; i < N; i++ )
    {
        new_node = (elem*) malloc(sizeof(elem));
        scanf("%d", &newval);
        new_node->key = newval;
        new_node->next = NULL;
        if ( head == NULL )
        {
            head = new_node;
            curr = new_node;
        }
        else
        {
            curr->next = new_node;
            curr = curr->next;
        }
    }
    stampaContrario(head);
    liberaLista(&head);
    return 0;
}
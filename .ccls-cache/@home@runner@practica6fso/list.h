#ifndef LIST_H
#define LIST_H

typedef struct _node {
    int value;
    struct _node *next;
} Node;

void insert_ordered(Node **head, int value);
void free_list(Node *head);

#endif // LIST_H

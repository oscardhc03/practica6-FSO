#include <stdlib.h>
#include "list.h"

void insert_ordered(Node **head, int value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->value = value;
    new_node->next = NULL;

    if (*head == NULL || (*head)->value >= value) {
        new_node->next = *head;
        *head = new_node;
    } else {
        Node *current = *head;
        while (current->next != NULL && current->next->value < value) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void free_list(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}


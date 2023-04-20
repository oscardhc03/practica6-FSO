#include "primos.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

bool es_primo(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

Node* insert(Node* head, int val) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->val = val;
    new_node->next = NULL;

    if (head == NULL) {
        return new_node;
    }

    Node* curr = head;
    Node* prev = NULL;
    while (curr != NULL && curr->val < val) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        new_node->next = head;
        return new_node;
    }

    prev->next = new_node;
    new_node->next = curr;
    return head;
}

void imprimir_lista(Node* head) {
    while (head != NULL) {
        printf("%d\n", head->val);
        head = head->next;
    }
}

void encontrar_primos(int start, int end, int pipe_fd[]) {
    close(pipe_fd[0]);
    for (int i = start; i <= end; ++i) {
        if (es_primo(i)) {
            write(pipe_fd[1], &i, sizeof(int));
        }
    }
    close(pipe_fd[1]);
    exit(0);
}

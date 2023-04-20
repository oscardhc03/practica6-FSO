#ifndef PRIMES_H
#define PRIMES_H

#include <stdbool.h>

typedef struct Node {
    int val;
    struct Node* next;
} Node;

bool es_primo(int n);

Node* insertar(Node* head, int val);

void imprimir_lista(Node* head);

void encontrar_primos(int start, int end, int pipe_fd[]);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "primos.h"

#define NUM_PROCES 4
int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Uso: %s start end\n", argv[0]);
        return 1;
    }
    int start = atoi(argv[1]);
    int end = atoi(argv[2]);
    // Dividir el rango de números en sub-rangos para cada procesador
    int range = end - start + 1;
    int sub_range = range / NUM_PROCES;
    int sub_ranges[NUM_PROCES][2];

    for (int i = 0; i < NUM_PROCES; ++i) {
        int sub_start = start + i * sub_range;
        int sub_end = i == NUM_PROCES - 1 ? end : sub_start + sub_range - 1;
        sub_ranges[i][0] = sub_start;
        sub_ranges[i][1] = sub_end;
    }

    // Crear tuberías para comunicarse entre procesos
    int pipe_fds[NUM_PROCES][2];
    for (int i = 0; i < NUM_PROCES; ++i) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Crear procesos para buscar números primos en cada sub-rango
    int pid;
    for (int i = 0; i < NUM_PROCES; ++i) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            encontrar_primos(sub_ranges[i][0], sub_ranges[i][1], pipe_fds[i]);
        }
    }

    // Cerrar el extremo de escritura de las tuberías en el proceso principal
    for (int i = 0; i < NUM_PROCES; ++i) {
        close(pipe_fds[i][1]);
    }

    // Leer los números primos encontrados por cada proceso e insertarlos en una lista enlazada
    Node* head = NULL;
    int num;
    for (int i = 0; i < NUM_PROCES; ++i) {
        while (read(pipe_fds[i][0], &num, sizeof(int)) > 0) {
            head = insert(head, num);
        }
        if (wait(NULL) == -1) {
            perror("wait");
            exit(1);
        }
        close(pipe_fds[i][0]);
    }

    // Imprimir la lista de números primos encontrados
    imprimir_lista(head);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "primos.h"

#define NUM_PROCESADORES 4

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Uso: %s inicio fin\n", argv[0]);
        return 1;
    }

    // Convertir los argumentos en números enteros
    int inicio = atoi(argv[1]);
    int fin = atoi(argv[2]);

    // Dividir el rango de números en sub-rangos para cada procesador
    int rango = fin - inicio + 1;
    int sub_rango = rango / NUM_PROCESADORES;
    int subrangos[NUM_PROCESADORES][2];

    for (int i = 0; i < NUM_PROCESADORES; ++i) {
        int sub_inicio = inicio + i * sub_rango;
        int sub_fin = i == NUM_PROCESADORES - 1 ? fin : sub_inicio + sub_rango - 1;
        subrangos[i][0] = sub_inicio;
        subrangos[i][1] = sub_fin;
    }

    // Crear tuberías para comunicarse entre procesos
    int pipe_fds[NUM_PROCESADORES][2];
    for (int i = 0; i < NUM_PROCESADORES; ++i) {
        if (pipe(pipe_fds[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Crear procesos para buscar números primos en cada sub-rango
    int pid;
    for (int i = 0; i < NUM_PROCESADORES; ++i) {
        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            buscar_primos(subrangos[i][0], subrangos[i][1], pipe_fds[i]);
        }
    }

    // Cerrar el extremo de escritura de las tuberías en el proceso principal
    for (int i = 0; i < NUM_PROCESADORES; ++i) {
        close(pipe_fds[i][1]);
    }

    // Leer los números primos encontrados por cada proceso e insertarlos en una lista enlazada
    Nodo* cabeza = NULL;
    int num;
    for (int i = 0; i < NUM_PROCESADORES; ++i) {
        while (read(pipe_fds[i][0], &num, sizeof(int)) > 0) {
            cabeza = insertar(cabeza, num);
        }
        if (wait(NULL) == -1) {
            perror("wait");
            exit(1);
        }
        close(pipe_fds[i][0]);
    }

    // Imprimir la lista de números primos encontrados
    imprimir_lista(cabeza);

    return 0;
}

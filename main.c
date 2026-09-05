#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Erro: Numero incorreto de argumentos\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0) {
        fprintf(stderr, "Erro: algoritmo invalido\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int total_time;
    char name[64];
    int p, d, c;
    if (argc != 3){
        fprintf(stderr, "Argumentos invalidos\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0){
        fprintf(stderr, "Algoritmo invalido\n");
        return EXIT_FAILURE;
    }
    FILE *arq = fopen(argv[2], "r");
    if (!arq) {
        fprintf(stderr, "Erro na abertura do arquivo\n");
        return EXIT_FAILURE;
    }
    if (fscanf(arq, "%d", &total_time) != 1 || total_time <= 0) {
        fprintf(stderr, "Tempo total nao numerico ou negativo\n");
        fclose(arq);
        return EXIT_FAILURE;
    }
    while (fscanf(arq, "%63s %d %d %d", name, &p, &d, &c) == 4) {
        if (p <= 0 || d <= 0 || c <= 0) {
            fprintf(stderr, "Valores devem ser positivos\n");
            fclose(arq);
            return EXIT_FAILURE;
        }
        if (c > d || d > p) {
            fprintf(stderr, "Violacao de C <= D <= P\n");
            fclose(arq);
            return EXIT_FAILURE;
        }
    }
    if (!feof(arq)) {
        fprintf(stderr, "Campos incompletos ou nao numericos\n");
        fclose(arq);
        return EXIT_FAILURE;
    }
    fclose(arq);
    return EXIT_SUCCESS;
}

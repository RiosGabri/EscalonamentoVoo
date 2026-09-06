#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TASKS 100

typedef struct {
    char name[64];
    int p;
    int d;
    int c;
    int remaining_c;
    int absolute_deadline;
    int id;
} Task;

int main(int argc, char *argv[]) {
    int total_time;
    Task tasks[MAX_TASKS];
    int num_tasks = 0;

    if (argc != 3) {
        fprintf(stderr, "Argumentos invalidos\n");
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "rate") != 0 && strcmp(argv[1], "edf") != 0) {
        fprintf(stderr, "Algoritmo invalido\n");
        return EXIT_FAILURE;
    }

    FILE *arq = fopen(argv[2], "r");
    if (!arq) {
        fprintf(stderr, "Erro na abertura do arquivo\n");
        return EXIT_FAILURE;
    }

    if (fscanf(arq, "%d", &total_time) != 1 || total_time <= 0) {
        fprintf(stderr, "Tempo total invalido\n");
        fclose(arq);
        return EXIT_FAILURE;
    }

    while (fscanf(arq, "%63s %d %d %d", tasks[num_tasks].name, &tasks[num_tasks].p, &tasks[num_tasks].d, &tasks[num_tasks].c) == 4) {
        if (tasks[num_tasks].p <= 0 || tasks[num_tasks].d <= 0 || tasks[num_tasks].c <= 0) {
            fprintf(stderr, "Valores devem ser positivos\n");
            fclose(arq);
            return EXIT_FAILURE;
        }
        if (tasks[num_tasks].c > tasks[num_tasks].d || tasks[num_tasks].d > tasks[num_tasks].p) {
            fprintf(stderr, "Violacao de C <= D <= P\n");
            fclose(arq);
            return EXIT_FAILURE;
        }
        tasks[num_tasks].id = num_tasks;
        tasks[num_tasks].remaining_c = 0;
        tasks[num_tasks].absolute_deadline = 0;
        num_tasks++;
    }

    if (!feof(arq)) {
        fprintf(stderr, "Campos incompletos ou nao numericos\n");
        fclose(arq);
        return EXIT_FAILURE;
    }
    fclose(arq);
    for (int t = 0; t < total_time; t++) {
        for (int i = 0; i < num_tasks; i++) {
            if (t % tasks[i].p == 0) {
                tasks[i].remaining_c = tasks[i].c;
                tasks[i].absolute_deadline = t + tasks[i].d;
            }
        }
        for (int i = 0; i < num_tasks; i++) {
            if (t == tasks[i].absolute_deadline && tasks[i].remaining_c > 0) {
                tasks[i].remaining_c = 0;
            }
        }
    }
    return EXIT_SUCCESS;
}

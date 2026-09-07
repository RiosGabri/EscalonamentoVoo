#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TAREFAS 100
#define LOGIN "grp"

typedef struct {
    char nome[64];
    int p;
    int d;
    int c;
    int tempo_restante;
    int prazo_absoluto;
    int id;
    int prazos_perdidos;
    int concluidas;
    int canceladas;
} Tarefa;

int selecionar_tarefa_rate(Tarefa tarefas[], int num_tarefas) {
    int melhor_idx = -1;
    for (int i = 0; i < num_tarefas; i++) {
        if (tarefas[i].tempo_restante > 0) {
            if (melhor_idx == -1 || tarefas[i].p < tarefas[melhor_idx].p) {
                melhor_idx = i;
            }
        }
    }
    return melhor_idx;
}

int main(int argc, char *argv[]) {
    int tempo_total;
    Tarefa tarefas[MAX_TAREFAS];
    int num_tarefas = 0;

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
        fprintf(stderr, "Erro ao abrir arquivo\n");
        return EXIT_FAILURE;
    }

    if (fscanf(arq, "%d", &tempo_total) != 1 || tempo_total <= 0) {
        fprintf(stderr, "Tempo total invalido\n");
        fclose(arq);
        return EXIT_FAILURE;
    }

    while (fscanf(arq, "%63s %d %d %d", tarefas[num_tarefas].nome, &tarefas[num_tarefas].p, &tarefas[num_tarefas].d, &tarefas[num_tarefas].c) == 4) {
        if (tarefas[num_tarefas].p <= 0 || tarefas[num_tarefas].d <= 0 || tarefas[num_tarefas].c <= 0) {
            fprintf(stderr, "Valores devem ser positivos\n");
            fclose(arq);
            return EXIT_FAILURE;
        }
        if (tarefas[num_tarefas].c > tarefas[num_tarefas].d || tarefas[num_tarefas].d > tarefas[num_tarefas].p) {
            fprintf(stderr, "Violacao da regra C <= D <= P\n");
            fclose(arq);
            return EXIT_FAILURE;
        }
        tarefas[num_tarefas].id = num_tarefas;
        tarefas[num_tarefas].tempo_restante = 0;
        tarefas[num_tarefas].prazo_absoluto = 0;
        tarefas[num_tarefas].prazos_perdidos = 0;
        tarefas[num_tarefas].concluidas = 0;
        tarefas[num_tarefas].canceladas = 0;
        num_tarefas++;
    }

    if (!feof(arq)) {
        fprintf(stderr, "Campos incompletos ou invalidos\n");
        fclose(arq);
        return EXIT_FAILURE;
    }
    fclose(arq);

    char nome_arquivo_saida[128];
    snprintf(nome_arquivo_saida, sizeof(nome_arquivo_saida), "%s_%s.out", argv[1], LOGIN);
    FILE *saida = fopen(nome_arquivo_saida, "w");
    if (!saida) {
        fprintf(stderr, "Erro ao criar arquivo de saida\n");
        return EXIT_FAILURE;
    }
    fprintf(saida, "EXECUTION BY RATE\n");

    int tarefa_atual = -2;
    int duracao_bloco = 0;

    for (int t = 0; t < tempo_total; t++) {
        for (int i = 0; i < num_tarefas; i++) {
            if (t == tarefas[i].prazo_absoluto && tarefas[i].tempo_restante > 0) {
                tarefas[i].prazos_perdidos++;
                tarefas[i].tempo_restante = 0;
            }
        }
        for (int i = 0; i < num_tarefas; i++) {
            if (t % tarefas[i].p == 0) {
                tarefas[i].tempo_restante = tarefas[i].c;
                tarefas[i].prazo_absoluto = t + tarefas[i].d;
            }
        }
        int selecionada = selecionar_tarefa_rate(tarefas, num_tarefas);
        if (selecionada != tarefa_atual) {
            if (duracao_bloco > 0) {
                if (tarefa_atual == -1) {
                    fprintf(saida, "idle for %d units\n", duracao_bloco);
                } else {
                    fprintf(saida, "[%s] for %d units\n", tarefas[tarefa_atual].nome, duracao_bloco);
                }
            }
            tarefa_atual = selecionada;
            duracao_bloco = 1;
        } else {
            duracao_bloco++;
        }
        if (selecionada != -1) {
            tarefas[selecionada].tempo_restante--;
            if (tarefas[selecionada].tempo_restante == 0) {
                tarefas[selecionada].concluidas++;
            }
        }
    }
    if (duracao_bloco > 0) {
        if (tarefa_atual == -1) {
            fprintf(saida, "idle for %d units\n", duracao_bloco);
        } else {
            fprintf(saida, "[%s] for %d units\n", tarefas[tarefa_atual].nome, duracao_bloco);
        }
    }
    for (int i = 0; i < num_tarefas; i++) {
        if (tarefas[i].tempo_restante > 0) {
            tarefas[i].canceladas++;
        }
    }
    fprintf(saida, "LOST DEADLINES\n");
    for (int i = 0; i < num_tarefas; i++) {
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].prazos_perdidos);
    }

    fprintf(saida, "COMPLETE EXECUTION\n");
    for (int i = 0; i < num_tarefas; i++) {
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].concluidas);
    }

    fprintf(saida, "KILLED\n");
    for (int i = 0; i < num_tarefas; i++) {
        fprintf(saida, "[%s] %d\n", tarefas[i].nome, tarefas[i].canceladas);
    }

    fclose(saida);
    return EXIT_SUCCESS;
}

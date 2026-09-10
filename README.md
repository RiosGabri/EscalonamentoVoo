# Escalonamento de Tarefas Críticas de Voo

Implementação 3 — Infraestrutura de Software

## Sobre o projeto

Este projeto implementa, em C, um escalonador de tarefas periódicas de voo.
O programa recebe por linha de comando o algoritmo de escalonamento (`rate` ou `edf`) e um arquivo contendo as tarefas a serem simuladas.
A aplicação valida os argumentos e os dados do arquivo de entrada e, em seguida, simula a execução das tarefas ao longo do tempo.
São implementadas duas estratégias de prioridade: **Rate-Monotonic**, que prioriza tarefas pelo menor período,
 e **Earliest Deadline First (EDF)**, que prioriza a tarefa com o deadline absoluto mais próximo.
Ao final da simulação, o programa registra a sequência de execução, períodos de ociosidade, deadlines perdidos, execuções completas e tarefas canceladas.

## Sistema operacional

O projeto foi desenvolvido e testado em **Linux Mint**, executado em uma máquina virtual pelo VirtualBox.

## Arquivos

- `main.c` — implementação completa: parsing e validação do arquivo de
  entrada, motor de simulação instante a instante, seleção de tarefa por
  **rate-monotonic** e por **earliest-deadline-first (EDF)**, geração do log
  de execução e das contagens finais (deadlines perdidos, execuções
  completas, tarefas canceladas ao fim da simulação).
- `Makefile` — compila o executável `scheduler` e limpa artefatos gerados
  (`make clean`).
- `.gitignore` — define arquivos e diretórios que não devem ser versionados pelo Git, evitando o envio de artefatos gerados durante a compilação ou execução.
- `agenda.txt` — arquivo utilizado como recurso de trabalho durante o desenvolvimento do projeto.
- `README.md` — documentação do projeto, com descrição, instruções de compilação, execução e testes. 

## Compilação

```
make
make clean
```

## Execução

```
./scheduler rate <arquivo_de_entrada>
./scheduler edf <arquivo_de_entrada>
```

O programa grava o resultado em `rate_<grp>.out` ou `edf_<grp>.out`
e não imprime nada em stdout durante a execução normal. 
Erros de entrada são reportados em stderr com código de
saída diferente de zero, e nenhum arquivo de saída é criado nesse caso.

## Entrada

O programa recebe dois argumentos pela linha de comando:

```bash
./scheduler <algoritmo> <arquivo_de_entrada>
```

O primeiro argumento define a estratégia utilizada (`rate` ou `edf`). O segundo indica o arquivo que contém os dados da simulação.

O arquivo de entrada segue o formato:

```text
[TEMPO TOTAL]
[NOME] [PERÍODO] [DEADLINE] [BURST]
...
```

A primeira linha informa o tempo total da simulação. Cada linha seguinte representa uma tarefa, contendo seu nome, período (`P`), deadline (`D`) e custo de execução (`C`). Todas as tarefas chegam pela primeira vez no instante 0, os valores devem ser inteiros positivos e deve ser respeitada a relação `C <= D <= P`.

Exemplo:

```text
50
ATT 20 12 8
NAV 50 30 15
```

## Saída

Após uma execução válida, o programa gera um arquivo de saída específico para o algoritmo utilizado:

```text
rate_grp.out
edf_grp.out
```

O arquivo registra a execução da simulação, indicando os períodos em que cada tarefa utiliza a CPU e os momentos em que o processador permanece ocioso. Ao final, são apresentadas as tarefas que perderam deadlines, as execuções concluídas e as tarefas classificadas como `KILLED`.

Em uma execução normal, o resultado da simulação não é impresso no `stdout`. Quando ocorre um erro de entrada, a mensagem é enviada para `stderr` e o programa termina com código diferente de zero, sem gerar o arquivo de saída.

## Testes

Os testes utilizam o arquivo de exemplo e as entradas inválidas presentes na pasta `Testes`.

Exemplo:

```bash
./scheduler rate voo.txt
./scheduler edf voo.txt
```

Para testar uma entrada inválida:

```bash
./scheduler rate Testes/<arquivo_de_teste>.txt
```

Foram testados argumentos inválidos, algoritmo inválido, arquivo inexistente, valores inválidos de `P`, `D` e `C`, campos incompletos e excesso de tarefas.

## O que foi testado

Validação manual feita durante o desenvolvimento, comparando a saída do
programa com o exemplo dado no enunciado (`voo.txt`, tarefas ATT e NAV) e
com casos de entrada inválida construídos à mão:

- **Caso de exemplo do enunciado**: saída de `rate` conferida linha a linha
  contra o log de referência do enunciado (blocos de execução, letras de
  status F/H/L, contagens de `LOST DEADLINES`, `COMPLETE EXECUTION` e
  `KILLED`) — bateram exatamente.
- **EDF no mesmo conjunto de tarefas**: usado para observar o caso
  comparativo exigido no relatório (rate perde deadline, EDF não).
- **Entradas inválidas**, cada uma verificando stderr não vazio, código de
  saída ≠ 0 e ausência de arquivo `.out`:
  - número incorreto de argumentos;
  - primeiro argumento diferente de `rate`/`edf`;
  - arquivo de entrada inexistente;
  - violação de `C ≤ D ≤ P` (`C > D` e `D > P` testados separadamente);
  - valor não positivo em um campo;
  - campo faltando — incluindo o caso da linha malformada ser a **última**
    do arquivo (esse caso tinha um bug específico, corrigido: a leitura
    parcial era confundida com fim de arquivo válido).
- **Excesso de tarefas**: confirma que
  o programa recusa a entrada em vez de estourar o array internamente.

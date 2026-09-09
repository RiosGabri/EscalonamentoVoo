# Escalonamento de Tarefas Críticas de Voo

Implementação 3 — Infraestrutura de Software

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

### Formato do arquivo de entrada

```
[TEMPO TOTAL]
[NOME] [PERÍODO] [DEADLINE] [BURST]
...
```

Todas as tarefas chegam pela primeira vez no instante 0, valores inteiros
positivos, e vale sempre `C ≤ D ≤ P`.

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

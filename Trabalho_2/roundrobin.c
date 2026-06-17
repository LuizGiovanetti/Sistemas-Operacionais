#include <stdio.h>
#include <stdlib.h>

#define MAX_TEMPO 500

int N;
int QUANTUM;

typedef struct{
    int ingresso;
    int duracao;
    int prioridade;
    int espera;
    int completo;
    int estado;
}Processo;

Processo *processos;
int *empilhador;
int aux = 0;

int **gantt;
int tempo_total = 0;

void alocar_memoria(){
    processos  = calloc(N, sizeof(Processo));
    empilhador = calloc(N, sizeof(int));

    gantt = malloc(N * sizeof(int *));
    for(int i = 0; i < N; i++)
        gantt[i] = calloc(MAX_TEMPO, sizeof(int));
}

void liberar_memoria(){
    free(processos);
    free(empilhador);

    for(int i = 0; i < N; i++)
        free(gantt[i]);
    free(gantt);
}

void ler_dados(){
    printf("Numero de processos: ");
    scanf("%d", &N);

    printf("Quantum: ");
    scanf("%d", &QUANTUM);

    alocar_memoria();

    printf("\n");
    for(int i = 0; i < N; i++){
        printf("Processo P%d:\n", i);
        printf("  Tempo de ingresso: ");
        scanf("%d", &processos[i].ingresso);
        printf("  Duracao (burst):   ");
        scanf("%d", &processos[i].duracao);
        printf("  Prioridade:        ");
        scanf("%d", &processos[i].prioridade);
        printf("\n");
    }
}

void inicializar_fila(){
    for(int i = 0; i < N; i++)
        empilhador[i] = -1;
}

void imprimir_fila(){
    for(int j = 0; j < N; j++)
        printf("%2d ", empilhador[j]);
    printf("\n");
}

void admitir_processos(int tempo_global){
    for(int tarefa = 0; tarefa < N; tarefa++){
        if((processos[tarefa].ingresso == tempo_global) && (!processos[tarefa].completo)){
            if(aux < N){
                empilhador[aux] = tarefa;
                aux++;
            }
        }
    }
}

void rotacionar_fila(){
    int primeiro = empilhador[0];

    for(int j = 0; j < N - 1; j++)
        empilhador[j] = empilhador[j + 1];

    empilhador[aux - 1] = primeiro;
}

void remover_processo_atual(){
    for(int j = 0; j < N - 1; j++)
        empilhador[j] = empilhador[j + 1];

    empilhador[N - 1] = -1;
    aux--;
}

int executar_tick(int tick){
    int proc = empilhador[0];
    gantt[proc][tick] = 1;
    processos[proc].estado++;

    for(int j = 1; j < N; j++)
        if((empilhador[j] != -1) && (!processos[empilhador[j]].completo))
            processos[empilhador[j]].espera++;

    if(processos[proc].estado == processos[proc].duracao){
        processos[proc].completo = 1;
        processos[proc].estado = 0;
        return 1;
    }

    return 0;
}

void exibir_resultados(){
    float soma = 0;
    printf("\nTempo de espera por processo:\n");

    for(int j = 0; j < N; j++){
        printf("  P%d: %d\n", j, processos[j].espera);
        soma += processos[j].espera;
    }

    printf("Tempo medio de espera: %.2f\n", soma / N);
}

void imprimir_gantt(){
    printf("\n>>> DIAGRAMA DE GANTT (Round Robin - Quantum=%d) <<<\n", QUANTUM);

    for(int p = 0; p < N; p++){
        printf("P%d | ", p);

        for(int t = 0; t < tempo_total; t++){
            if(gantt[p][t])
                printf("|||");
            else
                printf("   ");
        }

        printf("\n");
    }

    printf("     ");
    for(int t = 0; t < tempo_total; t++)
        printf("%d  ", t);

    printf("\n     ");

    for(int t = 0; t < tempo_total; t++)
        printf("-");

    printf(" (tempo t)\n");
}

void round_robin(){
    int terminado     = 0;
    int tempo_global  = 0;
    int tempo_quantum = 0;

    inicializar_fila();
    admitir_processos(0);

    while(terminado < N){
        if(empilhador[0] != -1){
            int concluido = executar_tick(tempo_global);
            tempo_quantum++;
            tempo_global++;

            if(concluido){
                remover_processo_atual();
                terminado++;
                tempo_quantum = 0;
            }else if(tempo_quantum == QUANTUM){
                rotacionar_fila();
                tempo_quantum = 0;
            }

            admitir_processos(tempo_global);
        }else{
            tempo_global++;
            admitir_processos(tempo_global);
        }
    }

    tempo_total = tempo_global;
}

int main(){
    ler_dados();
    round_robin();
    exibir_resultados();
    imprimir_gantt();
    liberar_memoria();

    return 0;
}
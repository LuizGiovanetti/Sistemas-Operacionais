// SHORTEST JOB FIRT SJF 

#include <stdio.h>
#include <string.h>

#define N 5
#define MAX_TEMPO 100

// Dados da Tabela 6.1
int chegada[] = {0, 0, 1, 3, 5};
int duracao[]  = {5, 2, 4, 1, 2};
int id[]       = {1, 2, 3, 4, 5};

int espera[N];
int completo[N];
int executado[N]; // Quanto o processo já rodou
int fila[N];      // O "empilhador" ou Fila de Prontos
int aux = 0;      // Contador de processos na fila

int timeline[MAX_TEMPO];
int tempoTotal = 0;



void inicializar_sistema() {
    aux = 0;
    tempoTotal = 0;
    memset(espera, 0, sizeof(espera));
    memset(completo, 0, sizeof(completo));
    memset(executado, 0, sizeof(executado));
    memset(timeline, -1, sizeof(timeline));
    for(int i = 0; i < N; i++) fila[i] = -1;
}

// Admite quem chega no exato segundo atual
void admitir_processos(int t) {
    for(int i = 0; i < N; i++) {
        if(chegada[i] == t && !completo[i]) {
            fila[aux++] = i;
        }
    }
}

// Remove o processo que terminou e reorganiza a fila
void remover_da_fila() {
    for(int i = 0; i < aux - 1; i++) fila[i] = fila[i+1];
    fila[aux-1] = -1;
    aux--;
}


void SJF_Completo() {
    inicializar_sistema();
    int terminados = 0;

    while(terminados < N) {
        admitir_processos(tempoTotal);

        if(aux > 0) {
            // Lógica SJF: No momento de escolher, busca o menor na fila
            int menorProcesso = 0;
            for(int i = 1; i < aux; i++) {
                if(duracao[fila[i]] < duracao[fila[menorProcesso]]) 
                menorProcesso = i;
            }
            
            // No SJF Não-Preemptivo, o SO "trava" no escolhido até acabar
            int p = fila[menorProcesso];
            
            while(executado[p] < duracao[p]) {
                if(executado[p] == 0) 
                espera[p] = tempoTotal - chegada[p];
                
                timeline[tempoTotal] = id[p];
                executado[p]++;
                tempoTotal++;
                
               
                admitir_processos(tempoTotal); 
                
                
                for(int i = 0; i < aux; i++) {
                    if(fila[i] != p) 
                    espera[fila[i]]++;
                }
            }
            completo[p] = 1;
            // Remove especificamente o processo p da fila
            for(int i = menorProcesso; i < aux - 1; i++) 
            fila[i] = fila[i+1];
            aux--;
            terminados++;
        } else {
            tempoTotal++;
        }
    }
}



void imprimirGantt(char* nome) {
   
    int soma_duracoes = 0;
    for(int i = 0; i < N; i++) soma_duracoes += duracao[i];

    printf("\n>>> %s <<<\n", nome);
    
    for(int i = 0; i < N; i++) {
        printf("P%d | ", id[i]);
        
        
        int fim = -1;
        for(int t = 0; t < tempoTotal; t++) {
            if(timeline[t] == id[i]) fim = t;
        }

        for(int t = 0; t < tempoTotal; t++) {
            if(timeline[t] == id[i]) 
                printf("###"); // Executando
            else if(t >= chegada[i] && t <= fim) 
                printf(":::"); // Esperando na fila
            else 
                printf("   "); // Fora do sistema
        }
        printf("\n");
    }
    
   
    printf("     ");
    for(int t = 0; t <= tempoTotal; t++) {
        printf("%-3d", t);
    }
    printf(" (t)");

    
    float soma_espera = 0;
    for(int i = 0; i < N; i++) soma_espera += espera[i];
    printf("\nMedia de Espera: %.2f ms\n", soma_espera / N);
}

int main() {
    for(int i = 0; i < N; i++) {
        id[i] = i + 1;
        printf("P%d - Chegada e Duracao: ", id[i]);
        scanf("%d %d", &chegada[i], &duracao[i]);
    }
   SJF_Completo();
    imprimirGantt("SJF COMPLETO");

    return 0;
}
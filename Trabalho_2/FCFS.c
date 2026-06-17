

#include <stdio.h>
#include <string.h>

#define N 5
#define MAX_TEMPO 100

// Dados da Tabela 6.1
int chegada[N], duracao[N], id[N], prioridade[N];
int espera[N], completo[N], executado[N], fila[N];
int timeline[MAX_TEMPO];
int aux = 0, tempoTotal = 0;

// --- FUNÇÕES DE INFRAESTRUTURA (IGUAL AO DO SEU AMIGO) ---

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



void FCFS_Completo() {
    inicializar_sistema();
    int terminados = 0;

    while(terminados < N) {
        admitir_processos(tempoTotal);

        if(aux > 0) {
            int p = fila[0]; // Sempre o primeiro que chegou (First-Come)
            
            // Se ele está começando agora, calcula a espera
            if(executado[p] == 0)
             espera[p] = tempoTotal - chegada[p];

            timeline[tempoTotal] = id[p];
            executado[p]++;
            
           
            for(int i = 1; i < aux; i++)
             espera[fila[i]]++;

            if(executado[p] == duracao[p]) {
                completo[p] = 1;
                remover_da_fila();
                terminados++;
            }
        }
        tempoTotal++;
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
     FCFS_Completo();

    imprimirGantt("FCFS COMPLETO");
    

    return 0;
}
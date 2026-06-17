#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdatomic.h>   
#include <omp.h>

#define DEFAULT_GAULESES   5
#define DEFAULT_JAVALIS    3
#define DEFAULT_REFEICOES  3

#define TEMPO_COMER        2
#define TEMPO_COZINHAR     2

static const char *nomes[] = {
    "Asterix", "Obelix", "Panoramix", "Abracurcix",
    "Cacafolix", "Ideiafix", "Fulliautomatix", "Assurancetourix",
    "Ordenalfabetix", "Cetautomatix"
};

static int N;
static int M;
static int R;
static int numjavalis;
static int refeicoes_feitas[10];

static atomic_int gauleses_prontos;
static int cozinheiro_chamado;
static int gauleses_esperando;

static sem_t hajavali;    
static sem_t naohajavali;  
static sem_t mutex;      


static void log_evento(const char *quem, const char *acao, const char *extra)
{
    #pragma omp critical(saida)
    {
        if (extra && extra[0] != '\0')
            printf("[%-20s] %s %s\n", quem, acao, extra);
        else
            printf("[%-20s] %s\n", quem, acao);
        fflush(stdout);
    }
}


static void RetiraJavali(int id)
{
    char buf[64];

    sem_wait(&mutex);

    while (numjavalis == 0) {
        if (!cozinheiro_chamado) {
            cozinheiro_chamado = 1;
            log_evento(nomes[id], "Mesa VAZIA — acordando cozinheiro...", "");
            sem_post(&naohajavali);
        } else {
            log_evento(nomes[id], "Mesa VAZIA — aguardando reabastecimento...", "");
        }

        gauleses_esperando++;

        sem_post(&mutex);      
        sem_wait(&hajavali);   
        sem_wait(&mutex);
    }

    numjavalis--;
    snprintf(buf, sizeof(buf), "(restam %d javalis na mesa)", numjavalis);
    log_evento(nomes[id], "PEGOU um javali", buf);

    sem_post(&mutex);
}


static void ColocaJavalis(int n)
{
    char buf[64];
    int acordar;

    sem_wait(&naohajavali);    

    if (atomic_load(&gauleses_prontos) >= N) {
        return;
    }

    sleep(TEMPO_COZINHAR);

    sem_wait(&mutex);

    numjavalis = n;
    cozinheiro_chamado = 0;

    snprintf(buf, sizeof(buf), "(%d javalis na mesa)", numjavalis);
    log_evento("~~~ Cozinheiro ~~~", "REPOS os javalis", buf);

    acordar = gauleses_esperando;
    gauleses_esperando = 0;

    sem_post(&mutex);

    for (int i = 0; i < acordar; i++)
        sem_post(&hajavali);
}


static void thread_gaules(int id)
{
    log_evento(nomes[id], "CHEGOU a fogueira", "");

    for (int i = 0; i < R; i++) {
        RetiraJavali(id);

        char buf[32];
        snprintf(buf, sizeof(buf), "(refeicao %d/%d)", i + 1, R);
        log_evento(nomes[id], "COMENDO [javali!]", buf);
        sleep(TEMPO_COMER);

        log_evento(nomes[id], "TERMINOU de comer", "");

        #pragma omp atomic
        refeicoes_feitas[id]++;
    }

    log_evento(nomes[id], "FOI EMBORA (satisfeito!)", "");

    sem_wait(&mutex);
    int prontos = atomic_fetch_add(&gauleses_prontos, 1) + 1;
    if (prontos == N) {
        sem_post(&naohajavali);
    }
    sem_post(&mutex);
}


static void thread_cozinheiro(void)
{
    log_evento("~~~ Cozinheiro ~~~", "Pronto para cozinhar!", "");
    
    while (atomic_load(&gauleses_prontos) < N) {
        ColocaJavalis(M);
    }

    log_evento("~~~ Cozinheiro ~~~", "Encerrou o servico.", "");
}


int main(int argc, char *argv[])
{
    N = (argc > 1) ? atoi(argv[1]) : DEFAULT_GAULESES;
    M = (argc > 2) ? atoi(argv[2]) : DEFAULT_JAVALIS;
    R = (argc > 3) ? atoi(argv[3]) : DEFAULT_REFEICOES;

    if (N < 2 || N > 10) {
        fprintf(stderr, "Numero de gauleses deve ser entre 2 e 10.\n");
        return EXIT_FAILURE;
    }

    printf("\n");
    printf("+==============================================+\n");
    printf("|             JANTAR DOS GAULESES              |\n");
    printf("|==============================================|\n");
    printf("|  Gauleses        : %-4d                      |\n", N);
    printf("|  Javalis na mesa : %-4d                      |\n", M);
    printf("|  Refeicoes       : %-4d                      |\n", R);
    printf("+==============================================+\n\n");

    sem_init(&hajavali,    0, 0);
    sem_init(&naohajavali, 0, 0);
    sem_init(&mutex,       0, 1);

    numjavalis          = M;
    atomic_init(&gauleses_prontos, 0);
    cozinheiro_chamado  = 0;   
    gauleses_esperando  = 0;   

    printf("[Mesa] Iniciando com %d javalis grelhados.\n\n", numjavalis);

    #pragma omp parallel num_threads(N + 1)
    {
        int id = omp_get_thread_num();

        #pragma omp barrier

        if (id < N)
            thread_gaules(id);
        else
            thread_cozinheiro();
    }

    printf("\n+==============================================+\n");
    printf("|                :) DADOS FINAL               |\n");
    printf("|==============================================|\n");
    int total = 0;
    for (int i = 0; i < N; i++) {
        printf("|  %-18s -> %d refeicao(oes)        |\n",
               nomes[i], refeicoes_feitas[i]);
        total += refeicoes_feitas[i];
    }
    printf("|----------------------------------------------|\n");
    printf("|  Total de refeicoes: %-4d                    |\n", total);
    printf("+==============================================+\n\n");

    sem_destroy(&hajavali);
    sem_destroy(&naohajavali);
    sem_destroy(&mutex);

    return EXIT_SUCCESS;
}
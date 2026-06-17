# Trabalho 3 - Jantar dos Gauleses (Sincronização com Threads)

## Sobre o Projeto

Este repositório contém o material desenvolvido para a disciplina de **Sistemas Operacionais**, com o objetivo de implementar o clássico problema de sincronização conhecido como **"Jantar dos Gauleses"**, uma variação do problema do Jantar dos Filósofos e do Produtor-Consumidor.

O projeto simula uma tribo gaulesa que janta em comunidade a partir de uma mesa com capacidade para um número limitado de javalis grelhados. A implementação utiliza threads em C com a biblioteca OpenMP e semáforos para gerenciar a concorrência entre os gauleses (consumidores) e o cozinheiro (produtor).

## Problema Proposto

Uma tribo gaulesa janta em comunidade a partir de uma mesa com espaço para **M** javalis grelhados. Quando um gaulês quer comer, ele serve-se e retira um javali da mesa, a menos que a mesa esteja vazia. Nesse caso, o gaulês acorda o cozinheiro e aguarda que este reponha os javalis na mesa.

### Regras de Funcionamento

- **N** gauleses (entre 2 e 10) sentam-se à mesa
- **M** javalis são colocados inicialmente na mesa (padrão: 3)
- Cada gaulês deve comer **R** refeições (padrão: 3)
- Quando a mesa fica vazia, o primeiro gaulês que percebe acorda o cozinheiro
- Os demais gauleses aguardam o reabastecimento
- O cozinheiro coloca M javalis na mesa e acorda os gauleses que estavam esperando
- Cada gaulês leva 2 segundos para comer um javali
- O cozinheiro leva 2 segundos para preparar uma nova fornada

## Características da Implementação

- **Linguagem:** C
- **Paradigma:** Programação concorrente com threads
- **API de paralelismo:** OpenMP (`#pragma omp parallel`)
- **Sincronização:** Semáforos POSIX (`sem_wait`, `sem_post`)

### Estruturas de sincronização

- `sem_t mutex` — controle de acesso à região crítica
- `sem_t hajavali` — sinaliza que há javalis disponíveis
- `sem_t naohajavali` — sinaliza que a mesa está vazia
- **Atomicidade:** operações atômicas para contador de gauleses prontos (`atomic_int`)

## Funcionamento do Código
### Fluxo de Execução

1. Inicialização dos semáforos e variáveis globais
2. Criação de N + 1 threads (N gauleses + 1 cozinheiro)
3. Cada gaulês executa um loop de R refeições:
   - Tenta retirar um javali da mesa (`RetiraJavali`)
   - Se a mesa estiver vazia, acorda o cozinheiro e aguarda
   - Come o javali por 2 segundos
   - Registra a refeição realizada
4. O cozinheiro executa em loop:
   - Aguarda ser chamado (`sem_wait(&naohajavali)`)
   - Prepara os javalis por 2 segundos
   - Coloca M javalis na mesa
   - Acorda todos os gauleses que estavam esperando
5. Quando todos os gauleses completam suas refeições, o programa finaliza

### Principais Funções

| Função | Descrição |
|---|---|
| `RetiraJavali(int id)` | Gaulês tenta retirar um javali da mesa. Se vazia, acorda o cozinheiro e aguarda |
| `ColocaJavalis(int n)` | Cozinheiro coloca n javalis na mesa e acorda os gauleses |
| `thread_gaules(int id)` | Comportamento de cada gaulês (chega, come R vezes, vai embora) |
| `thread_cozinheiro(void)` | Comportamento do cozinheiro (prepara javalis enquanto houver gauleses) |

## Logs de Eventos

O sistema gera logs detalhados das ações executadas, como:
[Asterix] CHEGOU a fogueira
[Asterix] PEGOU um javali (restam 2 javalis na mesa)
[Asterix] COMENDO [javali!] (refeicao 1/3)
[~~~ Cozinheiro ~~~] REPOS os javalis (3 javalis na mesa)

## Como Compilar e Executar

### Pré-requisitos

- Compilador GCC com suporte a OpenMP
- Sistema Linux/Unix (testado no Ubuntu 26.04)

### Compilação

```bash
gcc Monike_Luiz.c -o Monike_Luiz -fopenmp
```

### Execução

```bash
./Monike_Luiz [N] [M] [R]
```

**Parâmetros:**

- `N`: Número de gauleses (2 a 10) — padrão: 5
- `M`: Javalis colocados na mesa por vez — padrão: 3
- `R`: Refeições que cada gaulês deve comer — padrão: 3

**Exemplos:**

```bash
# Execução padrão (5 gauleses, 3 javalis, 3 refeições)
./Monike_Luiz

# 4 gauleses, 2 javalis por vez, 2 refeições cada
./Monike_Luiz 4 2 2

# 10 gauleses, 5 javalis por vez, 4 refeições cada
./Monike_Luiz 10 5 4
```

## Exemplo de Saída

+==============================================+
|             JANTAR DOS GAULESES              |
|==============================================|
|  Gauleses        : 5                         |
|  Javalis na mesa : 3                         |
|  Refeicoes       : 3                         |
+==============================================+

[Mesa] Iniciando com 3 javalis grelhados.

[Asterix             ] CHEGOU a fogueira
[Obelix              ] CHEGOU a fogueira
[Asterix             ] PEGOU um javali (restam 2 javalis na mesa)
[Asterix             ] COMENDO [javali!] (refeicao 1/3)
...
+==============================================+
|                :) DADOS FINAL                |
|==============================================|
|  Asterix           -> 3 refeicao(oes)        |
|  Obelix            -> 3 refeicao(oes)        |
|  ...                                         |
|----------------------------------------------|
|  Total de refeicoes: 15                      |
+==============================================+

## Autores

* Luiz Eduardo Albano Giovanetti
* Monike Dala Costa Pagno

## Observações

Este repositório possui finalidade exclusivamente acadêmica e foi desenvolvido como parte das atividades da disciplina de Sistemas Operacionais da UTFPR.

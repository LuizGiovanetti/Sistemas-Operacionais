# Trabalho 2 - Algoritmos de Escalonamento de Processos

## Sobre o Projeto

Este repositório contém o material desenvolvido para a disciplina de **Sistemas Operacionais**, com o objetivo de implementar e analisar diferentes algoritmos de escalonamento de processos utilizando a linguagem C.

O projeto realiza o cálculo do tempo médio de espera na fila de execução e apresenta um diagrama de Gantt para visualizar a ordem de execução dos processos em cada algoritmo estudado.

## Algoritmos Implementados

* FCFS (First Come, First Served)
* SJF (Shortest Job First) - Não Preemptivo
* Escalonamento por Prioridades - Não Preemptivo
* Round Robin (RR) - Preemptivo
* Escalonamento por Prioridades Preemptivo (Desafio)

## Objetivos

* Compreender o funcionamento dos principais algoritmos de escalonamento de CPU;
* Comparar o tempo médio de espera entre diferentes estratégias de escalonamento;
* Representar visualmente a execução dos processos por meio de diagramas de Gantt;
* Aplicar conceitos de concorrência e gerenciamento de processos estudados na disciplina.

## Tecnologias Utilizadas

* Linguagem C
* Compilador GCC
* Terminal/Console para execução dos programas
* Diagramas de Gantt gerados em ASCII

## Desenvolvimento

Cada algoritmo foi implementado separadamente para facilitar a análise de seu comportamento e dos resultados obtidos. Os programas recebem informações dos processos, executam o escalonamento conforme a estratégia escolhida e exibem:

* Ordem de execução dos processos;
* Diagrama de Gantt;
* Tempo de espera individual;
* Tempo médio de espera da fila.

Além da implementação dos algoritmos obrigatórios, também foi desenvolvido o algoritmo de Escalonamento por Prioridades Preemptivo como atividade complementar proposta no desafio da disciplina.

## Resultados

A execução dos programas permite comparar o desempenho dos algoritmos de escalonamento e observar como diferentes critérios de escolha dos processos impactam diretamente o tempo médio de espera e a utilização da CPU.

## Autores

* Luiz Eduardo Albano Giovanetti
* Monike Dala Costa Pagno

## Observações

Este repositório possui finalidade exclusivamente acadêmica e foi desenvolvido como parte das atividades da disciplina de Sistemas Operacionais da UTFPR.

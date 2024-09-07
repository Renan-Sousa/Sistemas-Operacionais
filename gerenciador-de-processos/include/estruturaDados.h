#ifndef ESTRUTURADADOS_H_INCLUDED
#define ESTRUTURADADOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_BLOCKED 100
#define MAX_FINISHED 100

// Estrutura para representar um processo simulado
typedef struct {
    int id;
    int var; // Valor da variável inteira
    int contP;  // Contador de programa
    char **programa; // Vetor de strings representando o programa
    int tamProg; // Tamanho do programa
    int bloqueio; // Indicador de bloqueio
    int completo; // Indica se o processo foi finalizado
    int tempo_inicio; //Indica o tempo que o processo inicia 
} ProcessoSimulado;

typedef struct {
    int tempo_gasto;
    int contador_processo;
    int *processos_finalizados;
    int *processos_bloqueados;
    ProcessoSimulado **processos;
    ProcessoSimulado **processos_espera;
    ProcessoSimulado *processo_execucao;
} Gerenciador;

#endif
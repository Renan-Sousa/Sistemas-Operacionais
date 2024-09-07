#include "reporter.h"

// Função para imprimir o estado atual do sistema
void reporter(Gerenciador *pm) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Código do processo filho
        printf("\n****************************************************************\n");
        printf("Estado do sistema:\n");
        printf("****************************************************************\n");
        printf("TEMPO ATUAL: %d\n", pm->tempo_gasto);

        // Imprimir processo executando
        printf("\nPROCESSO EXECUTANDO:\n");
        if (pm->processo_execucao != NULL) {
            ProcessoSimulado *p = pm->processo_execucao;
            printf("pid: %d, ppid: %d, valor: %d, tempo inicio: %d, CPU usada ate agora: %d\n",
                   p->id, getppid(), p->var, p->tempo_inicio, p->contP); // Prioridade e tempos fixos como exemplo
        } else {
            printf("Nenhum processo executando.\n");
        }

        // Imprimir processos bloqueados
        printf("\nPROCESSOS BLOQUEADO:\n");
        for (int i = 0; i < MAX_BLOCKED; i++) {
            if (pm->processos_bloqueados[i] != -1) {
                ProcessoSimulado *p = pm->processos[pm->processos_bloqueados[i]];
                printf("pid: %d, ppid: %d, valor: %d, tempo inicio: %d, CPU usada ate agora: %d\n",
                       p->id, getppid(), p->var, p->tempo_inicio, p->contP); // Prioridade e tempos fixos como exemplo
            }
        }

        // Imprimir processos bloqueados
        printf("\nPROCESSOS FINALIZADOS:\n");
        for (int i = 0; i < MAX_BLOCKED; i++) {
            if (pm->processos_finalizados[i] != -1) {
                ProcessoSimulado *p = pm->processos[pm->processos_finalizados[i]];
                printf("pid: %d, ppid: %d, valor: %d, tempo inicio: %d, CPU usada ate agora: %d\n",
                       p->id, getppid(), p->var, p->tempo_inicio, p->contP); // Prioridade e tempos fixos como exemplo
            }
        }

        // Imprimir processos prontos
        printf("\nPROCESSOS ESPERANDO:\n");
        for (int i = 0; i < MAX_FINISHED; i++) {
            if (pm->processos_espera[i] != NULL) {
                ProcessoSimulado *p = pm->processos_espera[i];
                printf("pid: %d, ppid: %d, valor: %d, tempo inicio: %d, CPU usada ate agora: %d\n",
                       p->id, getppid(), p->var, p->tempo_inicio, p->contP); // Prioridade e tempos fixos como exemplo
            }
        }

        printf("****************************************************************\n\n");
        exit(EXIT_SUCCESS);
    } else {
        // Código do processo pai
        wait(NULL); // Aguarda o processo filho terminar
    }
}

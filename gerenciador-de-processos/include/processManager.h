#ifndef PROCESSMANAGER_H_INCLUDED
#define PROCESSMANAGER_H_INCLUDED
#include "estruturaDados.h"
#include "reporter.h"

ProcessoSimulado* novoProcesso(char **programa, int tamProg);
Gerenciador* novoGerenciador();
void adicionarEspera(Gerenciador *pm, int id_processo);
void removerEspera(Gerenciador *pm);
void bloquear(Gerenciador *pm, int id_processo);
void desbloquear(Gerenciador *pm);
void adicionarFinalizados(Gerenciador *pm, int id_processo);
ProcessoSimulado* copiarProcesso(ProcessoSimulado *pai, int desvio);
void adicionaNovoExecutando(Gerenciador *pm);
int executarInstrucao(ProcessoSimulado *p, Gerenciador *pm);
void simular(Gerenciador *pm);
void processManager(int read_pipe, const char *nome_arquivo);


#endif
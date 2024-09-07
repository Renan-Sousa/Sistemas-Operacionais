#ifndef __MEMORIA_H__
#define __MEMORIA_H__

#include "tabela.h"

void acessoMemoria(TabelaHash *tabela, unsigned addr, char rw, char *algoritmo, int *lidas, int *escritas, int *tempo, int *falta_pag, int *memoria, int *pag_sujas);

void lru(TabelaHash *tabela, unsigned addr, char rw, int tempo, int *pag_sujas, int ins);
void nru(TabelaHash *tabela, unsigned addr, char rw, int tempo, int *pag_sujas);
void segunda_chance(TabelaHash *tabela, unsigned addr, char rw, int tempo, int *pag_sujas);

#endif
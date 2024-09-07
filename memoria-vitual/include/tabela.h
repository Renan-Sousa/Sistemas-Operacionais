#ifndef __TABELA_H__
#define __TABELA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct quadro{
    int modificado;
    int presente;
    int referenciado;
    int ultimoAcesso;
    int endereco;
} Quadro;

typedef struct No {
    Quadro* pagina;
    struct No* proximo;
} No;

typedef struct lista {
  int tam;
  No* inicio;
  No* fim;
} Lista;

typedef struct TabelaHash {
    Lista** tab;
    int tamanho;
    int s; //deslocamento
} TabelaHash;

Lista* criar_lista();
unsigned table_hash(TabelaHash* tabela, unsigned addr);
TabelaHash* criar_tabela_hash(int tam_Pagina, int tam_Memoria);
void inserir(TabelaHash* tabela, unsigned addr, char rw, int tempo);
Quadro* buscar(TabelaHash* tabela, unsigned addr);
void liberar_tabela_hash(TabelaHash* tabela);
void imprimir_lista(Lista* lista);
void imprimir_tabela(TabelaHash* tabela);
Quadro* criar_quadro(unsigned addr, char rw, int tempo);

#endif
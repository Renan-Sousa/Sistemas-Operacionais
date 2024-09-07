#include "tabela.h"

// Funcao que cria uma nova lista
Lista* criar_lista() {
    Lista* nova_lista = (Lista*) malloc(sizeof(Lista));
    nova_lista->tam = 0;
    nova_lista->inicio = NULL;
    nova_lista->fim = NULL;
    return nova_lista;
}

// Funcao de hash dinamica
unsigned table_hash(TabelaHash* tabela, unsigned addr) {
    return (addr >> tabela->s) % tabela->tamanho;
}

// Funcao que cria uma nova tabela hash
TabelaHash* criar_tabela_hash(int tam_Pagina, int tam_Memoria) {
    TabelaHash* tabela = (TabelaHash*) malloc(sizeof(TabelaHash));
    int tam = tam_Memoria / tam_Pagina;
    tabela->tamanho = tam;
    unsigned tmp = tam_Pagina * 1024;
    unsigned s = 0;
     while (tmp>1) {
        tmp = tmp>>1;
        s++;
    }
    tabela->s = s;
    tabela->tab = (Lista**) malloc(tam * sizeof(Lista*));
    for (int i = 0; i < tam; i++) {
        tabela->tab[i] = criar_lista();
    }
    return tabela;
}

// Funcao que insere uma pagina na tabela hash
void inserir(TabelaHash* tabela, unsigned addr, char rw, int tempo) {
    Quadro *pagina = criar_quadro(addr, rw, tempo);
    unsigned pos = table_hash(tabela, pagina->endereco);
    No* novo_no = (No*) malloc(sizeof(No));
    novo_no->pagina = pagina;
    novo_no->proximo = NULL;

    Lista* lista = tabela->tab[pos];
    
    if (lista->inicio == NULL) {
        lista->inicio = novo_no;
    } else {
        lista->fim->proximo = novo_no;
    }
    
    lista->fim = novo_no;
    lista->tam++;
}

// Funcao que busca uma pagina na tabela hash
Quadro* buscar(TabelaHash* tabela, unsigned addr) {
    unsigned pos = table_hash(tabela, addr);
    No* atual = tabela->tab[pos]->inicio;
    
    while (atual != NULL) {
        if (atual->pagina->endereco == addr) {
            return atual->pagina;
        }
        atual = atual->proximo;
    }
    
    return NULL; // Pagina nao encontrada
}

// Funcao para liberar a memoria da tabela hash
void liberar_tabela_hash(TabelaHash* tabela) {
    for (int i = 0; i < tabela->tamanho; i++) {
        No* atual = tabela->tab[i]->inicio;
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->proximo;
            free(temp->pagina);
            free(temp);
        }
        free(tabela->tab[i]);
    }
    free(tabela->tab);
    free(tabela);
}

Quadro* criar_quadro(unsigned addr, char rw, int tempo) {
    Quadro* quad = (Quadro*) malloc(sizeof(Quadro));
    
    quad->endereco = addr;
    
    if(rw == 'W') 
        quad->modificado = 1;
    else 
        quad->modificado = 0;
    
    quad->presente = 1;
    quad->referenciado = 0;
    quad->ultimoAcesso = tempo;
    
    return quad;
}

void imprimir_lista(Lista* lista) {
    No* atual = lista->inicio;
    while (atual != NULL) {
        if (atual->pagina->presente)
            printf("%x ", atual->pagina->endereco);
        atual = atual->proximo;
    }
}

// Função que imprime a tabela hash
void imprimir_tabela(TabelaHash* tabela) {
    printf("Tabela Hash:\n");
    for (int i = 0; i < tabela->tamanho; i++) {
        printf("Índice %d: ", i);
        imprimir_lista(tabela->tab[i]);
        printf("\n");
    }
}
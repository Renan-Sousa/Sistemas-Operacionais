#include "memoria.h"

void acessoMemoria(TabelaHash *tabela, unsigned addr, char rw, char *algoritmo, int *lidas, int *escritas, int *tempo, int *falta_pag, int *memoria, int *pag_sujas) {
    if(rw == 'W') 
        (*escritas)++;
    else 
        (*lidas)++;

    Quadro *aux_pag = NULL;
    aux_pag = buscar(tabela, addr);

    if(aux_pag != NULL){
        // Verifica se a página está presente na memória
        if(aux_pag->presente){
            // A página está na memória, então atualiza o estado dela
            if(rw == 'W') aux_pag->modificado = 1; 
            aux_pag->ultimoAcesso = *tempo;
            aux_pag->referenciado = 1;
        } 
        else {
            // Página está na tabela, mas não na memória, trata como page fault
            (*falta_pag)++;

            // Atualiza o estado da página para indicar que ela está agora presente na memória
            aux_pag->presente = 1;
            aux_pag->ultimoAcesso = *tempo;
            aux_pag->referenciado = 1;
            if(rw == 'W') aux_pag->modificado = 1;

            // Se não houver espaço, realiza substituição de página
            if(strcmp(algoritmo, "lru") == 0){
                lru(tabela, addr, rw, *tempo, pag_sujas, 1);
            }
            if(strcmp(algoritmo, "nru") == 0){
                nru(tabela, addr, rw, *tempo, pag_sujas);
            }
            if(strcmp(algoritmo, "segunda_chance") == 0){
                segunda_chance(tabela, addr, rw, *tempo, pag_sujas);      
            }
        
        }
    } 
    else{
        // Página não encontrada na tabela, trata como page fault
        (*falta_pag)++;

        // Verifica se há espaço na memória física
        if(*memoria < tabela->tamanho){
            inserir(tabela, addr, rw, *tempo);
            (*memoria)++;
        }
        else{
            // Se não houver espaço, realiza substituição de página
            if(strcmp(algoritmo, "lru") == 0){
                lru(tabela, addr, rw, *tempo, pag_sujas, 0);
            }
            if(strcmp(algoritmo, "nru") == 0){
                nru(tabela, addr, rw, *tempo, pag_sujas);
            }
            if(strcmp(algoritmo, "segunda_chance") == 0){
                segunda_chance(tabela, addr, rw, *tempo, pag_sujas);      
            }
        }
    }

    (*tempo)++;
}

void lru(TabelaHash *tabela, unsigned addr, char rw, int tempo, int *pag_sujas, int ins) {
    Quadro* quad_subs = NULL;

    int menor_tempo = 2000000; // Defina um valor grande inicial para comparação
    // Itera sobre cada lista na tabela hash
    for (int i = 0; i < tabela->tamanho; i++) {
        No* atual = tabela->tab[i]->inicio;

        while (atual != NULL) {
            Quadro *pagina_atual = atual->pagina;
            if (pagina_atual->presente == 1 && pagina_atual->ultimoAcesso < menor_tempo) {
                menor_tempo = pagina_atual->ultimoAcesso;
                quad_subs = pagina_atual;
            }
            atual = atual->proximo;
        }
    }
    // Marca a página menos recentemente usada como não presente
    if (quad_subs != NULL) {
        quad_subs->presente = 0;
        if(quad_subs->modificado) (*pag_sujas)++;
    }
    
    if (ins == 0) inserir(tabela, addr, rw, tempo);  
}

void nru(TabelaHash *tabela, unsigned addr, char rw, int tempo, int *pag_sujas) {
    Quadro* quad_subs = NULL;
    int classe_encontrada = 4; // Começa com a classe mais alta (0 a 3)
    // Itera sobre cada lista na tabela hash
    for (int i = 0; i < tabela->tamanho; i++) {
        No* atual = tabela->tab[i]->inicio;

        while (atual != NULL) {
            Quadro *pagina_atual = atual->pagina;
            if (pagina_atual->presente == 1) {
                int classe_atual = (pagina_atual->referenciado << 1) | pagina_atual->modificado;

                // Se a página pertence a uma classe mais baixa (menor valor), substituí-la
                if (classe_atual < classe_encontrada) {
                    classe_encontrada = classe_atual;
                    quad_subs = pagina_atual;
                }

                // Se encontrarmos uma página da classe 0, podemos parar de procurar
                if (classe_encontrada == 0) break;
            }
            atual = atual->proximo;
        }
        if (classe_encontrada == 0) break; // Sai do loop externo se encontrar uma página de classe 0
    }
    // Marca a página selecionada como não presente e, se estiver modificada, atualiza o contador de páginas sujas
    if (quad_subs != NULL) {
        quad_subs->presente = 0;
        if (quad_subs->modificado) (*pag_sujas)++;
    }
    // Insere a nova página na tabela
    inserir(tabela, addr, rw, tempo);
}

void segunda_chance(TabelaHash *tabela, unsigned addr, char rw, int tempo, int *pag_sujas) {
    Quadro* quad_subs = NULL;
    int menor_tempo = 2000000; // Defina um valor grande inicial para comparação
    while (1) {
        // Itera sobre cada lista na tabela hash
        for (int i = 0; i < tabela->tamanho; i++) {
            No* atual = tabela->tab[i]->inicio;

            while (atual != NULL) {
                Quadro *pagina_atual = atual->pagina;
                if (pagina_atual->presente == 1 && pagina_atual->ultimoAcesso < menor_tempo) {
                    menor_tempo = pagina_atual->ultimoAcesso;
                    quad_subs = pagina_atual;
                }
                atual = atual->proximo;
            }
        }
        //Segunda chance
        if(quad_subs->referenciado == 1){
            quad_subs->referenciado = 0;
            quad_subs->ultimoAcesso = tempo;
        } 
        else{
            break;
        }
    }
    if (quad_subs != NULL) {
        quad_subs->presente = 0;
        if(quad_subs->modificado) (*pag_sujas)++;
    }
    // Insere a nova página na tabela
    inserir(tabela, addr, rw, tempo);
}

#include "tabela.h"
#include "memoria.h"


int main(int argc, char *argv[]) {
    // Atribuindo os argumentos passados para as variáveis
    char *algoritmo = argv[1];
    char *arquivo_entrada = argv[2];
    int tam_Pagina = atoi(argv[3]);
    int tam_Memoria = atoi(argv[4]);

    if(tam_Pagina < 2 || tam_Pagina > 64){	
		printf("ERRO: O tamanho de cada pagina deve estar entre 2 e 64. \n");
		return 0;
	}
		
	if(tam_Memoria < 128 || tam_Memoria > 16384){
		printf("ERRO: O tamanho da memoria deve estar entre 128 e 16384. \n");
		return 0;
	}	
	
	if(strcmp(algoritmo, "lru") && strcmp(algoritmo, "nru") && strcmp(algoritmo,"segunda_chance")){
		printf("ERRO: O algoritmo deve ser lru, nru ou segunda_chance. \n");
		return 0;	
	}

    FILE *arquivo = fopen(arquivo_entrada,"r");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    }

    printf("\nExecutando o simulador...\n");
   
    unsigned addr;
    int tempo=0, memoria=0, pag_lidas=0, pag_escritas=0, falta_pag=0, pag_sujas=0;
    char rw;

    TabelaHash *tabela = criar_tabela_hash(tam_Pagina, tam_Memoria);

    // Lê cada linha do arquivo até o final
    while (fscanf(arquivo, "%x %c", &addr, &rw) != EOF) {
        acessoMemoria(tabela, addr, rw, algoritmo, &pag_lidas, &pag_escritas, &tempo, &falta_pag, &memoria, &pag_sujas);
    }

    fclose(arquivo);

    // imprimir_tabela(tabela);
    liberar_tabela_hash(tabela);

    printf("Arquivo de entrada: %s\n", arquivo_entrada);
    printf("Tamanho da memoria: %d KB\n", tam_Memoria);
    printf("Tamanho da pagina: %d KB\n", tam_Pagina);
    printf("Tecnica de reposicao: %s\n", algoritmo);
    printf("Paginas lidas: %d\n", pag_lidas);
    printf("Paginas escritas: %d\n", pag_escritas);
    printf("Falta de paginas: %d\n", falta_pag);
    printf("Paginas Sujas: %d\n", pag_sujas);

    return 0;
}
 
#include "processManager.h"

// Função para criar um novo processo simulado
ProcessoSimulado* novoProcesso(char **programa, int tamProg) {
    ProcessoSimulado *p = (ProcessoSimulado*)malloc(sizeof(ProcessoSimulado));
    p->id = 0;
    p->var = 0;
    p->contP = 0;
    p->programa = programa;
    p->tamProg = tamProg;
    p->bloqueio = 0;
    p->completo = 0;
    p->tempo_inicio = -1;
    return p;
}

// Função para criar um novo gerenciador de processos
Gerenciador* novoGerenciador() {
    Gerenciador *pm = (Gerenciador*)malloc(sizeof(Gerenciador));

    pm->processos = (ProcessoSimulado**)malloc(100 * sizeof(ProcessoSimulado*));
    pm->processos_espera = (ProcessoSimulado**)malloc(100 * sizeof(ProcessoSimulado*));
    pm->processos_finalizados = (int*)malloc(MAX_FINISHED * sizeof(int));
    pm->processos_bloqueados = (int*)malloc(MAX_BLOCKED * sizeof(int));

    for (int i = 0; i < MAX_BLOCKED; i++) {
        pm->processos_bloqueados[i] = -1;
    }

    for (int i = 0; i < MAX_FINISHED; i++) {
        pm->processos_finalizados[i] = -1;
    }

    for (int i = 0; i < 100; i++) {
        pm->processos_espera[i] = NULL;
    }

    pm->contador_processo = 0;
    pm->tempo_gasto = 0;
    pm->processo_execucao = NULL;

    return pm;
}

// Função para adicionar um processo à lista de esperando
void adicionarEspera(Gerenciador *pm, int id_processo) {
    // Encontra a menor posição disponível com -1
    for (int i = 0; i < MAX_FINISHED; i++) {
        if (pm->processos_espera[i] == NULL) {
            pm->processos_espera[i] = pm->processos[id_processo];
            return;
        }
    }
    fprintf(stderr, "Erro: Lista de processos esperando cheia.\n");
}


void removerEspera(Gerenciador *pm) {
    // Verifica se a lista não está vazia
    if (pm->processos_espera[0] != NULL) {
        // Remove o primeiro processo
        pm->processos_espera[0] = NULL;

        // Move todos os processos para a esquerda
        for (int i = 0; i < MAX_BLOCKED - 1; i++) {
            pm->processos_espera[i] = pm->processos_espera[i + 1];
        }
        pm->processos_espera[MAX_BLOCKED - 1] = NULL; // Define o último como NULL após a movimentação
    } else {
        fprintf(stderr, "Erro: Lista de processos esperando vazia.\n");
    }
}

// Função para adicionar um processo à lista de bloqueados
void bloquear(Gerenciador *pm, int id_processo) {
    // Encontra a menor posição disponível com -1
    for (int i = 0; i < MAX_BLOCKED; i++) {
        if (pm->processos_bloqueados[i] == -1) {
            pm->processos_bloqueados[i] = id_processo;
            return;
        }
    }
    fprintf(stderr, "Erro: Lista de processos bloqueados cheia.\n");
}

void desbloquear(Gerenciador *pm) {
    // Encontra a maior posição com um ID de processo
    for (int i = MAX_BLOCKED - 1; i >= 0; i--) {
        if (pm->processos_bloqueados[i] != -1) {
            int id_removido = pm->processos_bloqueados[i];
            pm->processos[id_removido]->bloqueio = 0;
            pm->processos_bloqueados[i] = -1;

            // Move todos os processos para a esquerda
            for (int j = i; j < MAX_BLOCKED - 1; j++) {
                pm->processos_bloqueados[j] = pm->processos_bloqueados[j + 1];
            }
            pm->processos_bloqueados[MAX_BLOCKED - 1] = -1; // Define o último como -1 após a movimentação

            adicionarEspera(pm, id_removido);
            fprintf(stderr, "Aviso: Processo %d desbloqueado.\n", id_removido);
            return;
            // return id_removido;
        }
    }
    fprintf(stderr, "Erro: Lista de processos bloqueados vazia.\n");
    // return -1; // Retorna um valor inválido
}

// Função para adicionar um processo à lista de terminados
void adicionarFinalizados(Gerenciador *pm, int id_processo) {
    // Verifica se o processo já está na lista
    for (int i = 0; i < MAX_FINISHED; i++) {
        if (pm->processos_finalizados[i] == id_processo) {
            return;
        }
    }

    // Encontra a menor posição disponível com -1
    for (int i = 0; i < MAX_FINISHED; i++) {
        if (pm->processos_finalizados[i] == -1) {
            pm->processos_finalizados[i] = id_processo;
            return;
        }
    }
    fprintf(stderr, "Erro: Lista de processos terminados cheia.\n");
}


// Função para copiar um processo simulado
ProcessoSimulado* copiarProcesso(ProcessoSimulado *pai, int desvio) {
    ProcessoSimulado *p = (ProcessoSimulado*)malloc(sizeof(ProcessoSimulado));
    p->id = pai->id;
    p->var = pai->var;
    p->contP = pai->contP + desvio;
    p->programa = pai->programa;
    p->tamProg = pai->tamProg;
    p->bloqueio = 0;
    p->completo = 0;
    p->tempo_inicio = -1;
    return p;
}

void adicionaNovoExecutando(Gerenciador *pm){
    pm->processo_execucao = pm->processos_espera[0];
    removerEspera(pm);
}

// Função para executar uma instrução
int executarInstrucao(ProcessoSimulado *p, Gerenciador *pm) {
    char *instrucao = p->programa[p->contP];
    int valor;
    char nome_arquivo[100];

    //printf("Executando a instrução %s\n",instrucao);

    // Parse a instrução
    if (sscanf(instrucao, "S %d", &valor) == 1) {
        p->var = valor;
    } else if (sscanf(instrucao, "A %d", &valor) == 1) {
        p->var += valor;
    } else if (sscanf(instrucao, "D %d", &valor) == 1) {
        p->var -= valor;
    } else if (strcmp(instrucao, "B") == 0) {
        bloquear(pm, p->id);
        p->contP++;
        p->bloqueio = 1;
        return 2;
    } else if (strcmp(instrucao, "E") == 0) {
        p->completo = 1;
        return 1;
    } else if (sscanf(instrucao, "F %d", &valor) == 1) {
        // Cria um novo processo
        ProcessoSimulado *novo_processo = copiarProcesso(p, 1);
        p->contP += valor; // Atualiza o contP do processo pai
        novo_processo->id = pm->contador_processo; 
        pm->processos[pm->contador_processo] = novo_processo;
        adicionarEspera(pm,novo_processo->id);
        pm->contador_processo++;
        p->contP++;
        return 3;
    } else if (sscanf(instrucao, "R %s", nome_arquivo) == 1) {
        // Substitui o programa do processo
        FILE *arquivo = fopen(nome_arquivo, "r");
        if (arquivo) {
            char *novo_programa[100];
            int tam_novo_programa = 0;
            char linha[100];
            while (fgets(linha, sizeof(linha), arquivo)) {
                // Remove o caractere de nova linha, se existir
                linha[strcspn(linha, "\r\n")] = 0;
                novo_programa[tam_novo_programa] = strdup(linha);
                tam_novo_programa++;
            }
            fclose(arquivo);

            // Atualiza o programa do processo
            p->programa = malloc(tam_novo_programa * sizeof(char *));
            for (int i = 0; i < tam_novo_programa; i++) {
                p->programa[i] = novo_programa[i];
            }

            p->var = 0;
            p->tamProg = tam_novo_programa;
            p->contP = 0; // Reinicia o contador de programa
            
            return 0;
        } else {
            fprintf(stderr, "Erro ao abrir o nome_arquivo %s\n", nome_arquivo);
        }
    }

    p->contP++;
    return 0;
}

void simular(Gerenciador *pm) {
    int estado = 0;
    if(pm->processo_execucao == NULL){
        if(pm->processos_espera[0] != NULL)
            adicionaNovoExecutando(pm);
    }
    else{
        ProcessoSimulado *p = pm->processo_execucao;

        if(p->tempo_inicio == -1)
            p->tempo_inicio = pm->tempo_gasto;

        if (p->bloqueio != 1 && !p->completo) {
            estado = executarInstrucao(p, pm);

        }

        // ENTROU PQ ACABOU
        if (estado == 1 && p->bloqueio != 1) {
            adicionarFinalizados(pm, p->id);
            if(pm->processos_espera[0] != NULL){
                adicionaNovoExecutando(pm); 
            }
            else
                pm->processo_execucao = NULL;
            printf("Sem processos na fila\n");

        }

        // ENTROU PQ BLOQUEOU
        if (estado == 2) {
            if(pm->processos_espera[0] != NULL){
                adicionaNovoExecutando(pm); 
            }
            else
                printf("Sem processos na fila\n");
        }

        // ENTROU PQ TROCOU
        if (estado == 3) {
            adicionaNovoExecutando(pm); 
            adicionarEspera(pm, p->id);
            //printf("\nAtualizado\nProcesso p com id = %d\nProcesso aux com id = %d\n", p->id, aux->id);
        }
    }
}

void processManager(int read_pipe, const char *nome_arquivo) {
    char comando;
    Gerenciador *pm = novoGerenciador();
    char *programa_inicial[100];
    int tam_programa_inicial = 0;

    // Lê o programa inicial a partir de um nome_arquivo
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo) {
        char linha[100];
        while (fgets(linha, sizeof(linha), arquivo)) {
            // Remove o caractere de nova linha, se existir
            linha[strcspn(linha, "\r\n")] = 0;
            programa_inicial[tam_programa_inicial] = strdup(linha);
            tam_programa_inicial++;
        }
        fclose(arquivo);
    } else {
        fprintf(stderr, "Erro ao abrir o nome_arquivo %s\n", nome_arquivo);
        exit(EXIT_FAILURE);
    }

    pm->processos[pm->contador_processo] = novoProcesso(programa_inicial, tam_programa_inicial);
    pm->processo_execucao = pm->processos[pm->contador_processo];
    pm->contador_processo++;

    while (read(read_pipe, &comando, 1) > 0) {
        switch (comando) {
            case 'Q':
                // Executa uma única linha do programa
                simular(pm);
                pm->tempo_gasto++;
                break;
            case 'U':
                // Desbloqueia o primeiro processo bloqueado
                desbloquear(pm);
                break;
            case 'P':
                // Imprime o estado atual do sistema
                reporter(pm);
                break;
            case 'T':
                // Imprime o tempo de retorno médio e finaliza
                printf("Reportando os dados.\n");
                reporter(pm);
                printf("Finalizando simulador.\n");
                exit(0);
                break;
            default:
                fprintf(stderr, "Comando desconhecido: %c\n", comando);
        }
    }
}
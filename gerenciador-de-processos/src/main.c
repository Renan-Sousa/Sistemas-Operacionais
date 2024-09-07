#include "estruturaDados.h"
#include "processManager.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Favor usar: ./teste <arquivo_programa>\n");
        exit(EXIT_FAILURE);
    }

    const char *nome_arquivo = argv[1];
    int pipefd[2];
    pid_t pid;

    // Cria um pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Cria um processo do tipo process manager
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Código do processo process manager
        close(pipefd[1]); // Fecha o lado de escrita do pipe
        processManager(pipefd[0], nome_arquivo);
        close(pipefd[0]); // Fecha o lado de leitura do pipe
        exit(EXIT_SUCCESS);
    } else {
        // Código do processo commander
        close(pipefd[0]); // Fecha o lado de leitura do pipe
        char comando;
        while (1) {
            printf("Digite um comando (Q, U, P, T): ");
            scanf(" %c", &comando);
            write(pipefd[1], &comando, 1);
            if (comando == 'T') {
                break;
            }
            sleep(1); // Aguarda 1 segundo
        }
        close(pipefd[1]); // Fecha o lado de escrita do pipe
        wait(NULL); // Aguarda o término do process manager
    }

    return 0;
}

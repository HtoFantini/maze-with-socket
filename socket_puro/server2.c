#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[3], "-i") != 0) {
        fprintf(stderr, "Uso: %s <v4|v6> <porta> -i <arquivo_labirinto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Configuração de protocolo e porta
    int server_fd;
    struct sockaddr_in6 addr6;
    struct sockaddr_in addr4;
    int porta = atoi(argv[2]);

    if (strcmp(argv[1], "v4") == 0) {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("Erro ao criar socket IPv4");
            exit(EXIT_FAILURE);
        }
        
        memset(&addr4, 0, sizeof(addr4));
        addr4.sin_family = AF_INET;
        addr4.sin_port = htons(porta);
        addr4.sin_addr.s_addr = INADDR_ANY;

        if (bind(server_fd, (struct sockaddr*)&addr4, sizeof(addr4)) < 0) {
            perror("Erro no bind IPv4");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
    } else if (strcmp(argv[1], "v6") == 0) {
        server_fd = socket(AF_INET6, SOCK_STREAM, 0);
        if (server_fd == -1) {
            perror("Erro ao criar socket IPv6");
            exit(EXIT_FAILURE);
        }

        memset(&addr6, 0, sizeof(addr6));
        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons(porta);
        addr6.sin6_addr = in6addr_any;

        if (bind(server_fd, (struct sockaddr*)&addr6, sizeof(addr6)) < 0) {
            perror("Erro no bind IPv6");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Protocolo inválido. Use 'v4' para IPv4 ou 'v6' para IPv6.\n");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Erro ao escutar");
        exit(EXIT_FAILURE);
    }

    printf("Servidor aguardando conexão na porta %d...\n", porta);

    int new_socket;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);

    // Aceitar conexão
    new_socket = accept(server_fd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_socket == -1) {
        perror("Erro ao aceitar conexão");
        exit(EXIT_FAILURE);
    }

    printf("Conexão estabelecida com o cliente\n");

     char buffer[1024];
    int bytes_lidos;

    while (1) {
        // Limpa o buffer antes de cada leitura
        memset(buffer, 0, sizeof(buffer));

        // Lê os dados do cliente
        bytes_lidos = read(new_socket, buffer, sizeof(buffer) - 1);
        if (bytes_lidos == -1) {
            perror("Erro ao ler dados do cliente");
            break;
        }

        // Adiciona um terminador nulo para evitar leitura de "lixo"
        buffer[bytes_lidos] = '\0';

        printf("Comando recebido do cliente: %s\n", buffer);

        // Verifica se o cliente deseja encerrar a conexão
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Cliente pediu para sair do jogo.\n");
            break;
        }

        // Envia uma resposta ao cliente
        send(new_socket, "Movimento processado", 20, 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

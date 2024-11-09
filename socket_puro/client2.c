#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <endereco IP> <porta>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct addrinfo hints, *res;
    char buffer[1024];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;       // Aceitar IPv4 ou IPv6
    hints.ai_socktype = SOCK_STREAM;   // Socket TCP

    int status = getaddrinfo(argv[1], argv[2], &hints, &res);
    if (status != 0) {
        fprintf(stderr, "Erro no getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Erro ao criar socket");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("Erro ao conectar ao servidor");
        freeaddrinfo(res);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);
    printf("Conectado ao servidor\n");

    while (1) {
        printf("Digite um comando (cima=1, direita=2, esquerda=3, baixo=4, finalizar=0): ");
        memset(buffer, 0, sizeof(buffer));  // Limpa o buffer antes da entrada
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Remove o newline

        // Envia o comando para o servidor
        send(sockfd, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
            printf("Encerrando a conexão.\n");
            break;
        }

        // Limpa o buffer antes de receber a resposta do servidor
        memset(buffer, 0, sizeof(buffer));
        int len = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (len > 0) {
            buffer[len] = '\0';
            printf("Resposta do servidor: %s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}

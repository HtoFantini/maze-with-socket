#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct action {
    int type;
    int moves[100];
    int board[10][10];
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <endereco IP> <porta>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct addrinfo hints, *res;
    struct action msg;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

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
    
    bool game_started = false;
    
    while (1) {
        int command;
        printf("Digite um comando:\n(iniciar=0, movimentar=1, mapa=2, dica=3, recomecar=6, sair=7): ");
        scanf("%d", &command);

        memset(&msg, 0, sizeof(msg));
        msg.type = command;

        // Envia a estrutura 'action' para o servidor
        send(sockfd, &msg, sizeof(msg), 0);

        if ((msg.type == 0) && (game_started == false)){
            printf("Imprime possible moves, recebidos do servidor\n");
            //flag que inicia o jogo
            game_started = true;
        } else if ((msg.type == 0) && (game_started == true)){
            printf("Jogo ja foi inicializado\n");
        }

        if (msg.type == 7) {  // Se o comando for 'exit'
            printf("Encerrando a conexão.\n");  
            break;
        }

        // Recebe a resposta do servidor
        memset(&msg, 0, sizeof(msg));
        int len = recv(sockfd, &msg, sizeof(msg), 0);
        if (len > 0) {
            printf("Resposta do servidor: Tipo de ação recebida = %d\n", msg.type);
        }
    }

    close(sockfd);
    return 0;
}

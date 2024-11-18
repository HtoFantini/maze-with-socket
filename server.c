#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utils.c"



struct action {
    int type;
    int moves[100];
    int board[10][10];
};

int main(int argc, char *argv[]) {
    if (argc != 5 || strcmp(argv[3], "-i") != 0) {
        fprintf(stderr, "Uso: %s <v4|v6> <porta> -i <arquivo_labirinto>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

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

    new_socket = accept(server_fd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_socket == -1) {
        perror("Erro ao aceitar conexão");
        exit(EXIT_FAILURE);
    }

    printf("Conexão estabelecida com o cliente\n");

    struct action msg;
    struct action msg_received;
    bool game_started = false;
    bool game_ended = false;

    while (1) {
        memset(&msg, 0, sizeof(msg));

        int msg_from_client = recv(new_socket, &msg, sizeof(msg), 0);
        
        if (msg_from_client <= 0) {
            perror("Erro ao receber dados do cliente");
            break;
        }

        printf("Comando recebido do cliente: Tipo de ação = %d\n", msg.type);

        if ((msg.type != 0) && (game_started == false)){
            perror("error: start the game first ");
            continue;
        }
        
        //Inicia o jogo
        if ((msg.type == 0) && (game_started == false)){
            printf("Starting new game\n");
            printf("<Envia para o cliente os movimentos possiveis>\n");
            game_started = true;
        }

        // Movimentação
        if(msg.type == 1){
            for(unsigned i=0;i<4;i++){
                msg.moves[0] = 1;  // Possível movimento: "up"
                msg.moves[1] = 3;  // Possível movimento: "right"
                msg.moves[2] = 0;  // Marca o fim dos movimentos válidos
            }
        }

        // Mapa do tabuleiro
        if(msg.type == 2){
            for(unsigned i=0;i< 4;i++){
                // msg.board = matrix;
            }
        }

        // Sair do Jogo
        if (msg.type == 7) {  // Se o comando for 'exit'
            printf("<reseta o estado do jogo>\n");
            printf("client disconnected.\n");
            break;
        }

        // Envia a resposta ao cliente
        send(new_socket, &msg, sizeof(msg), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

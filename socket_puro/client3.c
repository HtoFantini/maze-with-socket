#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <string.h>
#include "utils.c"

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
    
    struct action msg;
    struct action msg_received;
    bool game_started = false;
    bool game_ended = false;
    
    while (1) {
        int command;
        char move;
        
        printf("Digite um comando:\n(iniciar=0, movimentar=1, mapa=2, dica=3, recomecar=6, sair=7): ");
        scanf("%d", &command);

        memset(&msg, 0, sizeof(msg));
        msg.type = command;
        // Envia a estrutura 'action' para o servidor
        send(sockfd, &msg, sizeof(msg), 0);

        // TRATAR CASO NAO SEJA ESCRITO UM NUMERO E SIM UMA LETRA, qualquer letra buga o programa

        if ((msg.type != 0) && (game_started == false)){
            perror("error: start the game first");
            continue;
        }

        if (msg.type == 7) {  // Se o comando for 'exit'
            printf("Encerrando a conexão.\n");
            game_started = false;  
            break;
        }

        if ((msg.type == 0) && (game_started == false)){
            print_possible_moves(msg.moves);
            //flag que inicia o jogo
            game_started = true;
        } else if ((msg.type == 0) && (game_started == true)){
            printf("Game is already running\n");
        }

        // Movimentação
        if (msg.type == 1) {
            int msg_received = recv(sockfd, &msg, sizeof(msg), 0);
            print_possible_moves(msg.moves);
            if(msg.type == 4){
                printf("\nDigite o movimento (1=Cima, 2=Direita, 3=Baixo, 4=Esquerda): ");
                 if (scanf("%s", command) != 1) {
                    fprintf(stderr, "Entrada inválida. Saindo...\n");
                    send(sockfd, &msg, sizeof(msg), 0);
                    break;
                }
            }
        }

        // Mapa do tabuleiro
        if(msg.type == 2){
            for(unsigned i=0;i< 10;i++){
                // print_matrix(msg.board);
            }
        }

        if(msg.type == 3){
            for(unsigned i=0;i< 10;i++){
                // print_matrix(msg.board);
            }
        }


    }

    close(sockfd);
    return 0;
}

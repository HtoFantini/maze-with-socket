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
#include "common.h"

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
    
    
    struct action msg;
    
    while (1) {
        char command[10];

        if (game_end == false){
            //printf("Digite um comando:\n(start, up, right, down, left, map, hint, reset, exit): ");
            scanf("%s", command);

            memset(&msg, 0, sizeof(msg));

            if (strcmp(command, "exit") == 0){
                msg.type = 7;
                send(sockfd, &msg, sizeof(msg), 0);
                //printf("Exiting... \n");
                break;
            }

            if ((game_started == false) && (strcmp(command, "start") != 0)){
                perror("error: start the game first");
                printf("\n");
                continue;
            }

            if((game_started == true) && (strcmp(command, "start") == 0)){
                perror("error: the game is already running");
                printf("\n");
                continue;
            }
            else if ((strcmp(command, "start") == 0) && (game_started == false)){
                game_started = true;
                msg.type = 0;
                send(sockfd, &msg, sizeof(msg), 0);
                recv(sockfd, &msg, sizeof(msg), 0);

                read_matrix_size(msg.board,&rows,&cols);
                filter = create_filter_matrix(rows, cols);

                if (msg.type == 4){
                    print_possible_moves(msg.moves);
                    printf("\n");
                }
            }
            else if ((strcmp(command, "start") == 0) && (game_started == true)){
                printf("The game has been already started\n");
                continue;
            }
            else if ((strcmp(command, "up") == 0) || (strcmp(command, "right") == 0) || (strcmp(command, "down") == 0) || (strcmp(command, "left") == 0)){
                msg.type = 1;
                msg.moves[0] = command_to_int(command);
                send(sockfd, &msg, sizeof(msg), 0);
                recv(sockfd, &msg, sizeof(msg), 0);

                    if (msg.type == 4){
                        if (msg.moves[0] == 9){
                        perror("error: you cannot go this way");
                        printf("\n");
                        } else {
                            print_possible_moves(msg.moves);
                            printf("\n");
                        }
                    }

                    if(msg.type == 5){
                        printf("You escaped!\n");
                        int **dynamic_buffer = create_filter_matrix(rows,cols);
                        assign_board_to_dynamic(msg.board,dynamic_buffer,rows,cols);
                        char_matrix = int_to_char_matrix(dynamic_buffer,rows,cols);
                        print_char_matrix(char_matrix,rows,cols);
                        game_end = true;
                        free_matrix(dynamic_buffer,rows);
                        continue;
                    }
            }
            else if (strcmp(command, "map") == 0){
                msg.type = 2;
                send(sockfd, &msg, sizeof(msg), 0);
                recv(sockfd, &msg, sizeof(msg), 0);
                int **dynamic_buffer = create_filter_matrix(rows,cols);
                assign_board_to_dynamic(msg.board,dynamic_buffer,rows,cols);
                char_matrix = int_to_char_matrix(dynamic_buffer,rows,cols);
                print_char_matrix(char_matrix,rows,cols);
                printf("\n");
                free_matrix(dynamic_buffer,rows);
            }
            else if (strcmp(command, "hint") == 0){
                msg.type = 3;
                send(sockfd, &msg, sizeof(msg), 0);
                printf("Printing hint... \n");
                printf("\n");
            }
            else if (strcmp(command, "reset") == 0){
                msg.type = 6;
                send(sockfd, &msg, sizeof(msg), 0);
                recv(sockfd, &msg, sizeof(msg), 0);
                print_possible_moves(msg.moves);
                printf("\n");
                continue;
            } else {
                perror("error: command not found");
                printf("\n");
                continue;
            }
        }
        else if (game_end == true){
            //printf("\nDigite reset ou exit:\n");
            scanf("%s", command);
            if (strcmp(command, "exit") == 0){
                break;
            }
            if (strcmp(command, "reset") == 0){
                msg.type = 6;
                send(sockfd, &msg, sizeof(msg), 0);
                recv(sockfd, &msg, sizeof(msg), 0);
                if (msg.type == 4){
                    print_possible_moves(msg.moves);
                    printf("\n");
                }
                game_end = false;
                continue;;
            } else {
                perror("error: command not found");
            }
        }
        else {
            perror("error: Conection corrupted, closing socket\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}

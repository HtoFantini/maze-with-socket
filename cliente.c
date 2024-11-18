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

int command_to_int(const char *command) {
    if (strcmp(command, "up") == 0) return 1;
    if (strcmp(command, "right") == 0) return 2;
    if (strcmp(command, "down") == 0) return 3;
    if (strcmp(command, "left") == 0) return 4;
    return 0;  // Comando inválido
}

void print_board(int board[10][10], int rows, int cols) {
    printf("board:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

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
    
    const char *file = "input/maze.txt";
    int rows, cols;
    char **char_matrix;
    int **filter = create_filter_matrix(rows, cols);

    while (1) {
        char command[10];

        if (game_ended == false){
            printf("Digite um comando:\n(start, up, right, down, left, map, hint, reset, exit): ");
            scanf("%s", command);

            memset(&msg, 0, sizeof(msg));

            if ((game_started = false) && (strcmp(command, "start") != 0)){
                perror("error: start the game first");
                continue;
            }
            

            if (strcmp(command, "exit") == 0){
                msg.type = 7;
                send(sockfd, &msg, sizeof(msg), 0);
                printf("Exiting... \n");
                break;
            }
            else if ((strcmp(command, "start") == 0) && game_started == false){
                game_started = true;
                msg.type = 0;
                send(sockfd, &msg, sizeof(msg), 0);
                recv(sockfd, &msg, sizeof(msg), 0);

                if (msg.type == 4){
                    print_possible_moves(msg.moves);
                }
            }
            else if ((strcmp(command, "start") == 0) && game_started == true){
                printf("The game has been already started\n");
                continue;
            }
            else if ((strcmp(command, "up") == 0) || (strcmp(command, "right") == 0) || (strcmp(command, "down") == 0) || (strcmp(command, "left") == 0)){
                msg.type = 1;
                msg.moves[0] = command_to_int(command);
                send(sockfd, &msg, sizeof(msg), 0);
                recv(sockfd, &msg, sizeof(msg), 0);

                if (msg.type == 4){
                    print_possible_moves(msg.moves);
                }

                if(msg.type == 5){
                    printf("You escaped!\n");
                    print_board(msg.board,rows,cols);
                    // int **dynamic_board = malloc(rows * sizeof(int *));
                    // if (!dynamic_board) {
                    // perror("Erro ao alocar memória para dynamic_board");
                    // exit(EXIT_FAILURE);
                    // }
                    // for (int i = 0; i < rows; i++) {
                    //     dynamic_board[i] = malloc(cols * sizeof(int));
                    //     if (!dynamic_board[i]) {
                    //         perror("Erro ao alocar memória para linha do dynamic_board");
                    //         for (int j = 0; j < i; j++) {
                    //             free(dynamic_board[j]);
                    //         }
                    //         free(dynamic_board);
                    //         exit(EXIT_FAILURE);
                    //     }
                    // }
                    // assign_board_to_dynamic(msg.board,dynamic_board,rows,cols);
                    // char_matrix = int_to_char_matrix(dynamic_board,rows,cols);
                    // print_char_matrix(char_matrix,rows,cols);
                    game_ended = true;
                    //free(dynamic_board);
                    continue;
                }
            }
            else if (strcmp(command, "map") == 0){
                msg.type = 2;
                send(sockfd, &msg, sizeof(msg), 0);
                printf("Printing maze... \n");
            }
            else if (strcmp(command, "hint") == 0){
                msg.type = 3;
                send(sockfd, &msg, sizeof(msg), 0);
                printf("Printing hint... \n");
            }
            else if (strcmp(command, "reset") == 0){
                msg.type = 6;
                send(sockfd, &msg, sizeof(msg), 0);
                printf("Reseting game... \n");
            } else {
                perror("error: command not found");
                continue;
            }
        }
        else if (game_ended == true){
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
                }
                game_ended = false;
                continue;
            }
        }
        else {
            perror("error: Conection corrupted, closing socket\n");
            break;
        }
    }

    close(sockfd);
    free_char_matrix(char_matrix, rows);
    free_matrix(filter, rows);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "common.h"

void int_to_command(int move, char *command) {
    if (move == 1) {
        strcpy(command, "up");
    } else if (move == 2) {
        strcpy(command, "right");
    } else if (move == 3) {
        strcpy(command, "down");
    } else if (move == 4) {
        strcpy(command, "left");
    }
}

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
    bool game_started = false;
    //bool game_end = false;

    const char *file = argv[4];
    int rows, cols;

    load_rows_and_cols(file, &rows, &cols);
    printf("Dimensões carregadas: %dx%d\n", rows, cols);

    int **maze = load_maze(file, rows, cols);
    int **root_maze = load_maze(file, rows, cols);
    int **filter = create_filter_matrix(rows, cols);
    char **char_matrix;

    if (!maze || !root_maze || !filter) {
        fprintf(stderr, "Erro ao carregar as matrizes.\n");
        return EXIT_FAILURE;
    }


    while (1) {
        char command[10];
        if (recv(new_socket, &msg, sizeof(msg), 0) <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        if ((msg.type == 0) && (game_started == true)) {
            perror("error: the game is already running");
            continue;
        }
        if ((msg.type == 0) && (game_started == false)) {
            printf("Client started the game.\n");
            game_started = true;
            msg.type = 4;
            start_game(maze,rows,cols);

            update_known_places(maze, filter, rows, cols);

            printf("\nMatriz Atual:\n");
            char_matrix = int_to_char_matrix(apply_filter(maze, filter, rows, cols), rows, cols);
            print_char_matrix(char_matrix,rows, cols);

            printf("\nMovimentos possíveis: \n");
            int *possib_moves = possible_moves(maze, rows, cols);
            if (!possib_moves) {
                fprintf(stderr, "Erro ao calcular movimentos possíveis.\n");
                break;
            }
            print_possible_moves(possib_moves);
            printf("%d %d %d %d\n", possib_moves[0], possib_moves[1], possib_moves[2], possib_moves[3]);

            msg.moves[0] = possib_moves[0];
            msg.moves[1] = possib_moves[1];
            msg.moves[2] = possib_moves[2];
            msg.moves[3] = possib_moves[3];

            send(new_socket, &msg, sizeof(msg), 0);
            
            free(possib_moves);
        } else if (msg.type == 1) {

            //printf("game_ended? %d \n",game_ended(maze,rows,cols,root_maze));
            int_to_command(msg.moves[0], command);
            printf("%s\n", command);

            int *possib_moves = possible_moves(maze, rows, cols);

            if (check_move(command, possib_moves)) {
                move_player(maze, rows, cols, root_maze, command);
                update_known_places(maze, filter, rows, cols);
            

                print_maze(maze,rows,cols);
                possib_moves = possible_moves(maze, rows, cols);
                adjust_array(possib_moves);
                printf("%d %d %d %d\n", possib_moves[0], possib_moves[1], possib_moves[2], possib_moves[3]);

                memset(msg.moves, 0, sizeof(msg.moves));
                msg.moves[0] = possib_moves[0];
                msg.moves[1] = possib_moves[1];
                msg.moves[2] = possib_moves[2];
                msg.moves[3] = possib_moves[3];

                if (game_ended(maze, rows, cols, root_maze)){
                    msg.type = 5;
                    //game_end = true;
                    copy_to_board(msg.board,root_maze,rows,cols);
                    print_board(msg.board,rows,cols);
                } else {
                    msg.type=4;
                }
            } else {
                msg.type = 4;
                msg.moves[0] = 9;
            }

            send(new_socket, &msg, sizeof(msg), 0);

            printf("Player moved. Updated possible moves sent to client.\n");

            free(possib_moves);
        } else if (msg.type == 2) {
            msg.type = 4;
            printf("Client requested maze map\n");
            copy_to_board(msg.board,apply_filter(maze,filter,rows,cols),rows,cols);
            print_board(msg.board,rows,cols);
            send(new_socket, &msg, sizeof(msg), 0);
        } else if (msg.type == 6){
            msg.type = 4;
            //game_end = false;
            
            for (int i = 0; i < rows; i++) {
                memcpy(maze[i], root_maze[i], cols * sizeof(int));
            }
            printf("maze = root_maze\n");
            print_maze(maze,rows,cols);
            
            printf("starting new game\n");
            start_game(maze,rows,cols);
            print_maze(maze,rows,cols);

            clear_matrix(filter,rows,cols);
            update_known_places(maze,filter,rows,cols);
            int *possib_moves = possible_moves(maze, rows, cols);
            adjust_array(possib_moves);

            msg.moves[0] = possib_moves[0];
            msg.moves[1] = possib_moves[1];
            msg.moves[2] = possib_moves[2];
            msg.moves[3] = possib_moves[3];

            send(new_socket, &msg, sizeof(msg), 0);

            free(possib_moves);
        } else if (msg.type == 7){
            printf("Client disconnected\n");
            break;
        }
        else {
            printf("Unknown message type received.\n");
        }
    }

    close(new_socket);
    close(server_fd);
    free_char_matrix(char_matrix,rows);
    free_matrix(maze,rows);
    free_matrix(filter,rows);
    free_matrix(root_maze,rows);
    return 0;
}

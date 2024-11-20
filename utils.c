#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>  

#define MIN_ROWS 5
#define MIN_COLS 5
#define MAX_ROWS 10
#define MAX_COLS 10

///////////////////////////////// MOVIMENTACAO DO USUARIO ///////////////////////

int* locate_player(int **matrix, int rows, int cols){
    int *location = (int*)malloc(2 * sizeof(int));
    if (location == NULL) {
        perror("Erro na alocação de memória para localizar o jogador");
        exit(1);
    }

    location[0] = -1;
    location[1] = -1;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 5) {
                location[0] = i;
                location[1] = j;
                return location;
            }
        }
    }

    perror("Player not found");
    return location;
}

int* locate_exit(int **matrix, int rows, int cols){
    int *location = (int*)malloc(2 * sizeof(int));

    location[0] = -1;
    location[1] = -1;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 3) {
                location[0] = i;
                location[1] = j;
                return location; 
            }
        }
    }

    perror("Exit not found");
}

int* locate_entry(int **matrix, int rows, int cols){
    int *location = (int*)malloc(2 * sizeof(int));

    location[0] = -1;
    location[1] = -1;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 2) {
                location[0] = i;
                location[1] = j;
                return location; 
            }
        }
    }

    perror("Entry not found");
}

int* next_neighbours(int **matrix, int rows, int cols) {
    
    int* player_loc = locate_player(matrix, rows, cols);
    int* neighbours = (int*)malloc(4 * sizeof(int));
    int player_row = player_loc[0];
    int player_col = player_loc[1];
    
    if (neighbours == NULL) {
        perror("Erro na alocação de memória para os vizinhos");
        exit(1); // ou retornar NULL
    }
    
    if ((player_row == -1) || (player_col == -1)){
        perror("no player found");
        free(neighbours); // Liberar memória caso não encontre o jogador
        return NULL;
    }

      if (player_row > 0) {
        neighbours[0] = matrix[player_row - 1][player_col];
    } else {
        neighbours[0] = -1;
    }

    // Direita
    if (player_col < cols - 1) {
        neighbours[1] = matrix[player_row][player_col + 1];
    } else {
        neighbours[1] = -1;
    }

    // Baixo
    if (player_row < rows - 1) {
        neighbours[2] = matrix[player_row + 1][player_col];
    } else {
        neighbours[2] = -1;
    }

    // Esquerda
    if (player_col > 0) {
        neighbours[3] = matrix[player_row][player_col - 1];
    } else {
        neighbours[3] = -1;
    }

    return neighbours;
}

int can_move_up(int **matrix, int rows, int cols){
    int *neighbours = next_neighbours(matrix, rows, cols);
    
    if ((neighbours[0] == 1) || (neighbours[0] == 2) || (neighbours[0] == 3)){
        free(neighbours);
        return 1;
    } else {
        free(neighbours);
        return 0;
    }
}

int can_move_right(int **matrix, int rows, int cols){
    int *neighbours = next_neighbours(matrix, rows, cols);
    
    if ((neighbours[1] == 1) || (neighbours[1] == 2) || (neighbours[1] == 3)){
        free(neighbours);
        return 2;
    } else {
        free(neighbours);
        return 0;
    } 
}

int can_move_down(int **matrix, int rows, int cols){
    int *neighbours = next_neighbours(matrix, rows, cols);
    
    if ((neighbours[2] == 1) || (neighbours[2] == 2) || (neighbours[2] == 3)){
        free(neighbours);
        return 3;
    } else {
        free(neighbours);
        return 0;
    }
}

int can_move_left(int **matrix, int rows, int cols){
    int *neighbours = next_neighbours(matrix, rows, cols);
    
    if ((neighbours[3] == 1) || (neighbours[3] == 2) || (neighbours[3] == 3)){
        free(neighbours);
        return 4;
    } else {
        free(neighbours);
        return 0;
    }
}



int* can_move(int **matrix, int rows, int cols){
    int *moves = malloc(4 * sizeof(int));

    moves[0] = can_move_up(matrix, rows, cols);
    moves[1] = can_move_right(matrix, rows, cols);
    moves[2] = can_move_down(matrix, rows, cols);
    moves[3] = can_move_left(matrix, rows, cols);

    return moves;

}

int* possible_moves(int **matrix, int rows, int cols){
    int *poss_moves = can_move(matrix,rows,cols);
    printf("can_move result: %d %d %d %d\n", poss_moves[0], poss_moves[1], poss_moves[2], poss_moves[3]);
    unsigned i,j = 0;

    while(i < 4) {
        if (poss_moves[i] == 0){
            j = i+1;
            while(j < 4 && poss_moves[j] == 0) {
                j++;
            }
        }
        if(j < 4){
            int aux = poss_moves[i];
            poss_moves[i] = poss_moves[j];
            poss_moves[j] = aux;
        } else{
            break;
        }
        i++;
    }

    // for(i=4; i < 100; i++){
    //     poss_moves[i] = 0;
    // }

    return poss_moves;
}

void print_possible_moves(int *moves) {
    printf("Possible moves: ");
    for (int i = 0; i < 4 && moves[i] != 0; i++) { 
        if(moves[i] == 1){
            printf("up");
            if(moves[i+1] != 0){
                printf(", ");
            } else {
                printf(".");
            }
        }

        if(moves[i] == 2){
            printf("right");
            if(moves[i+1] != 0){
                printf(", ");
            } else {
                printf(".");
            }
        }

        if(moves[i] == 3){
            printf("down");
            if(moves[i+1] != 0){
                printf(", ");
            } else {
                printf(".");
            }
        }

        if(moves[i] == 4){
            printf("left");
            if(moves[i+1] != 0){
                printf(", ");
            } else {
                printf(".");
            }
        }
    }
    printf("\n");
}

void move_player(int **matrix, int rows, int cols, int **root_matrix, const char *command){
    int *player_loc = locate_player(matrix, rows, cols);

    if(strcmp(command, "up") == 0){
        matrix[player_loc[0]-1][player_loc[1]] = 5;
        matrix[player_loc[0]][player_loc[1]] = root_matrix[player_loc[0]][player_loc[1]];
    } else if (strcmp(command, "right") == 0){
        matrix[player_loc[0]][player_loc[1]+1] = 5;
        matrix[player_loc[0]][player_loc[1]] = root_matrix[player_loc[0]][player_loc[1]];
    } else if (strcmp(command, "down") == 0){
        matrix[player_loc[0]+1][player_loc[1]] = 5;
        matrix[player_loc[0]][player_loc[1]] = root_matrix[player_loc[0]][player_loc[1]];
    } else if (strcmp(command, "left") == 0){
        matrix[player_loc[0]][player_loc[1]-1] = 5;
        matrix[player_loc[0]][player_loc[1]] = root_matrix[player_loc[0]][player_loc[1]];
    }
    else{
        perror("error: command not found");
    }
    free(player_loc);
}

int check_move(const char *command, int *possib_moves) {
    int aux = 0;

    if (strcmp(command, "up") == 0) {
        aux = 1;
    } else if (strcmp(command, "right") == 0) {
        aux = 2;
    } else if (strcmp(command, "down") == 0) {
        aux = 3;
    } else if (strcmp(command, "left") == 0) {
        aux = 4;
    } else {
        printf("error: command not found\n");
        return 0; 
    }

    for (int i = 0; i < 4; i++) {
        if (possib_moves[i] == aux) {
            return 1; 
        }
    }

    printf("error: you cannot go this way\n");
    return 0;  
}

void adjust_array(int *vec) {
    int aux[4] = {0};
    int index = 0;

    for (int i = 0; i < 4; i++) {
        if (vec[i] != 0) {
            aux[index++] = vec[i];
        }
    }

    for (int i = 0; i < 4; i++) {
        vec[i] = aux[i];
    }
}

////////////////////////// MANIPULACAO DE MATRIZES /////////////////////////////

void load_rows_and_cols(const char *file_name, int *rows, int *cols) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("error: file could not be opened");
        exit(EXIT_FAILURE);
    }

    *rows = 0;
    *cols = 0;
    int aux_cols = 0;
    char ch;

    // Contagem de linhas e colunas
    while ((ch = fgetc(file)) != EOF) {
        if (ch == ' ') {
            aux_cols++;
        } else if (ch == '\n') {
            (*rows)++;
            if (*cols == 0) {
                *cols = aux_cols + 1;
            }
            aux_cols = 0;
        }
    }

    fclose(file);

    if(*rows < MIN_ROWS || *cols < MIN_COLS || *rows > MAX_ROWS|| *cols > MAX_COLS){
        fprintf(stderr, "error: Labirinto de tamanho inadequado! Seu tamanho deve ser entre %dx%d e %dx%d.\n",
        MIN_ROWS, MIN_COLS, MAX_ROWS, MAX_COLS);
        exit(EXIT_FAILURE);
    }
}

int** load_maze(const char *file_name, int rows, int cols) {
    FILE *arquivo = fopen(file_name, "r");
    if (arquivo == NULL){
        perror("error: could not open the file");
        exit(EXIT_FAILURE);
    }

    // Alocando a matriz dinamicamente
    int **matriz = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matriz[i] = (int *)malloc(cols * sizeof(int));
    }

    // Preenchendo a matriz com os valores do arquivo
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(arquivo, "%d", &matriz[i][j]);
        }
    }

    fclose(arquivo);
    return matriz;
}

void print_maze(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void free_matrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int** create_filter_matrix(int rows, int cols){
    int **matrix = (int**)malloc(rows * sizeof(int*));
    for (unsigned i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }

    for (unsigned i = 0; i < rows; i++){
        for (unsigned j = 0; j < cols; j++){
            matrix[i][j] = 0;
        }
    }

    return matrix;
}

void update_known_places(int **matrix, int **filter_matrix, int rows, int cols){
    int * player_loc = locate_player(matrix,rows,cols);

    // current location
    filter_matrix[player_loc[0]][player_loc[1]] = 1;
    // up
    if(player_loc[0]-1 > 0){
        filter_matrix[player_loc[0]-1][player_loc[1]] = 1;
    }
    // right
    if(player_loc[1]+1 <= cols-1){
        filter_matrix[player_loc[0]][player_loc[1]+1] = 1;
    }
    // down
    if(player_loc[0]+1 <= rows-1){
        filter_matrix[player_loc[0]+1][player_loc[1]] = 1;
    }
    // left
    if(player_loc[1]-1 > 0){
        filter_matrix[player_loc[0]][player_loc[1]-1] = 1;
    }
}

int** apply_filter(int** current_matrix, int** filter_matrix, int rows, int cols){
    int **updated_matrix = malloc(rows * sizeof(int *));
    for (unsigned i = 0; i < rows; i++) {
        updated_matrix[i] = malloc(cols * sizeof(int));
    }

    for (unsigned i = 0; i < rows; i++) {
        for (unsigned j = 0; j < cols; j++) {
            if(filter_matrix[i][j] == 0){
                updated_matrix[i][j] = 4;
            } else {
               updated_matrix[i][j] = current_matrix[i][j]; 
            }
        }
    }
    return updated_matrix;
}

char** int_to_char_matrix(int** matrix, int rows, int cols){
    char **char_matrix = (char**)malloc(rows * sizeof(char*));
    for (unsigned i = 0; i < rows; i++) {
        char_matrix[i] = (char*)malloc(cols * sizeof(char));
    }

    for (unsigned i = 0; i < rows; i++) {
        for (unsigned j = 0; j < cols; j++) {
            if(matrix[i][j] == 0){
                char_matrix[i][j] = '#';
            }
            else if(matrix[i][j] == 1){
                char_matrix[i][j] = '_';
            }
            else if(matrix[i][j] == 2){
                char_matrix[i][j] = '>';
            }
            else if(matrix[i][j] == 3){
                char_matrix[i][j] = 'X';
            }
            else if(matrix[i][j] == 4){
                char_matrix[i][j] = '?';
            }
            else if(matrix[i][j] == 5){
                char_matrix[i][j] = '+';
            } 
            else {
                char_matrix[i][j] = ' ';
            }
        }
    }
    return char_matrix;
}

void print_char_matrix(char** char_matrix, int rows, int cols){
    for (unsigned i = 0; i < rows; i++) {
        for (unsigned j = 0; j < cols; j++) {
            printf("%c ", char_matrix[i][j]);
        }
        printf("\n");
    }
}

void free_char_matrix(char** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void copy_to_board(int target[10][10], int **matrix, int rows, int cols) {

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            target[i][j] = 0;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            target[i][j] = matrix[i][j];
        }
    }
}

void assign_board_to_dynamic(int static_board[10][10], int **dynamic_board, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dynamic_board[i][j] = static_board[i][j]; // Atribui os valores
        }
    }
}

void assign_moves_to_dynamic(int static_moves[4], int *dynamic_moves) {
    for (int i = 0; i < 4; i++) {
        dynamic_moves[i] = static_moves[i];
    }
}

void print_board(int board[10][10], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

void clear_matrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 0;
        }
    }
}
/////////////////////////////// COMANDOS DO JOGO ////////////////////////////

void start_game(int **matrix, int rows, int cols){
    int *entry_loc = locate_entry(matrix, rows, cols);

    matrix[entry_loc[0]][entry_loc[1]] = 5;
}

int game_ended (int **matrix, int rows, int cols, int **root_matrix){
    int *player_loc = locate_player(matrix, rows, cols);
    int *exit_loc = locate_exit(root_matrix, rows, cols);

    printf("Printing root_maze \n");
    print_maze(root_matrix,rows,cols);
    printf("printing maze\n");
    print_maze(matrix,rows,cols);
    printf("\nplayer_loc: %d %d", player_loc[0],player_loc[1]);
    printf("exit_loc: %d %d", exit_loc[0],exit_loc[1]);

    if((player_loc[0] == exit_loc[0]) && (player_loc[1] == exit_loc[1])){
        free(player_loc);
        free(exit_loc);
        return 1;
    }
    free(player_loc);
    free(exit_loc);
    return 0;
}


int main2() {
    const char *file = "input/maze.txt";
    int rows, cols;

    printf("Carregando dimensões da matriz...\n");
    load_rows_and_cols(file, &rows, &cols);
    printf("Dimensões carregadas: %dx%d\n", rows, cols);

    printf("Carregando labirinto...\n");
    int **maze = load_maze(file, rows, cols);
    int **root_maze = load_maze(file, rows, cols);
    int **filter = create_filter_matrix(rows, cols);
    char **char_matrix;

    if (!maze || !root_maze || !filter) {
        fprintf(stderr, "Erro ao carregar as matrizes.\n");
        return EXIT_FAILURE;
    }

    start_game(maze, rows, cols);

    char command[10];
    while(true) {
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

        printf("\nDigite o movimento (1=Cima, 2=Direita, 3=Baixo, 4=Esquerda, 0=Sair): ");
        if (scanf("%s", command) != 1) {
            fprintf(stderr, "Entrada inválida. Saindo...\n");
            break;
        }

       if (check_move(command, possib_moves) == 0){
        continue;
       } else {
            move_player(maze, rows, cols, root_maze, command);
       }

        free(possib_moves);

        if (game_ended(maze, rows, cols, root_maze)) {
            printf("Win!\n");
            break;
        }

    }

    // Liberando a memória alocada
    free_char_matrix(char_matrix, rows);
    // free(vec);
    // free(neighbours);
    // free(moves);
    // free(poss_moves);
    free_matrix(maze, rows);
    free_matrix(root_maze, rows);
    free_matrix(filter, rows);

    return 0;
}
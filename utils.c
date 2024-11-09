#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>  

#define MIN_ROWS 5
#define MIN_COLS 5
#define MAX_ROWS 10
#define MAX_COLS 10

void load_rows_and_cols(const char *file_name, int *rows, int *cols) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
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
        fprintf(stderr, "Erro: Labirinto de tamanho inadequado! Seu tamanho deve ser entre %dx%d e %dx%d.\n",
        MIN_ROWS, MIN_COLS, MAX_ROWS, MAX_COLS);
        exit(EXIT_FAILURE);
    }
}

int** load_matrix(const char *file_name, int rows, int cols) {
    FILE *arquivo = fopen(file_name, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
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

void print_matrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    const char *file = "input/in.txt";
    int rows, cols;


    // Conta as linhas e colunas do arquivo
    load_rows_and_cols(file, &rows, &cols);
    // Carrega o labirinto
    int **matrix = load_matrix(file, rows, cols);
    // Imprime a matriz
    print_matrix(matrix, rows, cols);
    
    // Liberando a memória alocada
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
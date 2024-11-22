#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MIN_ROWS 5
#define MIN_COLS 5
#define MAX_ROWS 10
#define MAX_COLS 10

int* locate_player(int **matrix, int rows, int cols);
int* locate_exit(int **matrix, int rows, int cols);
int* locate_entry(int **matrix, int rows, int cols);
int* next_neighbours(int **matrix, int rows, int cols);
int can_move_up(int **matrix, int rows, int cols);
int can_move_right(int **matrix, int rows, int cols);
int can_move_down(int **matrix, int rows, int cols);
int can_move_left(int **matrix, int rows, int cols);
int* can_move(int **matrix, int rows, int cols);
int* possible_moves(int **matrix, int rows, int cols);
void print_possible_moves(int *moves);
void move_player(int **matrix, int rows, int cols, int **root_matrix, const char *command);
int check_move(const char *command, int *possib_moves);
void adjust_array(int *vec);

void load_rows_and_cols(const char *file_name, int *rows, int *cols);
int** load_maze(const char *file_name, int rows, int cols);
void print_maze(int **matrix, int rows, int cols);
void free_matrix(int **matrix, int rows);
int** create_filter_matrix(int rows, int cols);
void update_known_places(int **matrix, int **filter_matrix, int rows, int cols);
int** apply_filter(int** current_matrix, int** filter_matrix, int rows, int cols);
char** int_to_char_matrix(int** matrix, int rows, int cols);
void print_char_matrix(char** char_matrix, int rows, int cols);
void free_char_matrix(char** matrix, int rows);
void copy_to_board(int target[10][10], int **matrix, int rows, int cols);
void assign_board_to_dynamic(int static_board[10][10], int **dynamic_board, int rows, int cols);
void assign_moves_to_dynamic(int static_moves[4], int *dynamic_moves);
void print_board(int board[10][10], int rows, int cols);
void clear_matrix(int **matrix, int rows, int cols);

// Declaração das funções relacionadas aos comandos do jogo
void start_game(int **matrix, int rows, int cols);
int game_ended (int **matrix, int rows, int cols, int **root_matrix);

#endif // COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9

// 检查是否可以在给定位置放置 num，包含对角线规则
int is_valid(int board[SIZE][SIZE], int row, int col, int num) {
    for (int x = 0; x < SIZE; x++) {
        if (board[row][x] == num || board[x][col] == num)
            return 0;
    }

    // 检查 3x3 宫
    int startRow = row / 3 * 3;
    int startCol = col / 3 * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == num)
                return 0;
        }
    }

    // 检查对角线（仅当 row == col 或 row + col == 8 时）
    if (row == col) {  // 主对角线
        for (int i = 0; i < SIZE; i++) {
            if (board[i][i] == num)
                return 0;
        }
    }

    if (row + col == SIZE - 1) {  // 副对角线
        for (int i = 0; i < SIZE; i++) {
            if (board[i][SIZE - 1 - i] == num)
                return 0;
        }
    }

    return 1;
}


// 递归回溯法生成完整数独解
int solve_sudoku(int board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (is_valid(board, row, col, num)) {
                        board[row][col] = num;
                        if (solve_sudoku(board))
                            return 1;
                        board[row][col] = 0;
                    }
                }
                return 0; //1-9都不行,无解,回溯
            }
        }
    }
    return 1;
}


// 填充主对角线上的 3 个 3x3 的宫
void fill_diagonal_blocks(int board[SIZE][SIZE]) {
    for (int k = 0; k < SIZE; k += 3) {
        int num;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                do {
                    num = rand() % 9 + 1;
                } while (!is_valid(board, k + i, k + j, num));
                board[k + i][k + j] = num;
            }
        }
    }
}


// 生成完整的对角线数独
void generate_complete_sudoku(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }

    // 填充主对角线上的 3x3 宫
    fill_diagonal_blocks(board);

    // 递归填充剩余的空格
    solve_sudoku(board);
}


// 递归回溯法解数独并计数
int solve_and_count(int board[SIZE][SIZE], int* solutions) {
    if (*solutions > 1)
        return 0;
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (is_valid(board, row, col, num)) {
                        board[row][col] = num;
                        solve_and_count(board, solutions);
                        board[row][col] = 0;
                    }
                }
                return 0;
            }
        }
    }
    (*solutions)++;
    return 1;
}


// 检查是否有唯一解
int has_unique_solution(int board[SIZE][SIZE]) {
    int solutions = 0;
    solve_and_count(board, &solutions);
    return solutions == 1;
}


// 挖洞
void dig_holes(int board[SIZE][SIZE], int holes_count) {
    srand(time(NULL));
    int attempts = holes_count;

    while (attempts > 0) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;

        if (board[row][col] != 0) {
            int backup = board[row][col];
            board[row][col] = 0;

            int board_copy[SIZE][SIZE];
            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++)
                    board_copy[i][j] = board[i][j];

            if (!has_unique_solution(board_copy)) {
                board[row][col] = backup;
            } else {
                attempts--;
            }
        }
    }
}


// 打印数独棋盘
void print_board(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        // 打印行分隔线
        if (i % 3 == 0 && i != 0) {
            printf("---------------------\n");
        }
        
        for (int j = 0; j < SIZE; j++) {
            // 打印列分隔线
            if (j % 3 == 0 && j != 0) {
                printf("| ");
            }
            
            if (board[i][j] == 0) {
                printf(". ");
            } else {
                printf("%d ", board[i][j]);
            }
        }
        printf("\n");
    }
}


int main() {
    int board[SIZE][SIZE];

    // 生成完整的对角线数独解
    generate_complete_sudoku(board);

    // 挖洞，生成有 40 个空格的数独谜题
    dig_holes(board, 40);

    // 打印生成的数独谜题
    print_board(board);

    return 0;
}

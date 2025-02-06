#ifndef DRAW_H
#define DRAW_H

#include "common.h"


#define CLEAR              printf("\033H\033J")
#define SET_TEXT_COLOR(X)  printf("\033b%c", 0x20|X)
#define MOVE_CURSOR(R, C)  printf("\033Y%c%c", R+' '-1, C+' '-1)
#define RESET_FORMATING    printf("\033b\007")

typedef enum {
    BLACK,
    BLUE,
    RED,
    MAGENTA,
    GREEN,
    CYAN,
    YELLOW,
    WHITE
} Color;

void setBufferedInput(bool enable);

void print_score(u32 score);
void print_board(u32 board[SIZE][SIZE]);
void diff_board(u32 old[SIZE][SIZE], u32 new[SIZE][SIZE]);
void print_indicators(void);
void print_win(void);
void print_game_over(void);


#endif

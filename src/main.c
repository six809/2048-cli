#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>	  
#include <unistd.h>	  // defines: STDIN_FILENO, usleep
#include <time.h>

#include "../include/common.h"
#include "../include/board.h"
#include "../include/draw.h"


void signal_callback_handler()
{
    SET_TEXT_COLOR(RED);
    printf("%*s\n", 26, "TERMINATED");
    RESET_FORMATING;

    setBufferedInput(true);

    exit(EXIT_SUCCESS);
}

void setup() {
    CLEAR;
    // make cursor invisible, erase entire screen
    printf("\033f\033J");
    // init rng
    srand(time(NULL));
    // disable input buffering, input are processed immediately
    setBufferedInput(false);
    // register signal handler for when ctrl-c is pressed
	signal(SIGINT, signal_callback_handler);
}

int main(/* int argc, char *argv[] */) {

    setup();

    int c;
    u32 score = 0;
    u32 last_score = 0;
    bool run = true;
    u32 board[SIZE][SIZE];
    u32 tmp_board[SIZE][SIZE];
    u32 last_board[SIZE][SIZE];
    board_init(board);
    memcpy(last_board, board, sizeof(board));
    
    print_score(score);
    print_board(board);
    print_indicators();

    while (run) {
        fflush(stdout);
        c = getchar();
        bool has_move = false;

        if (c == -1) {
            puts("\nError! Cannot read keyboard input!");
            break;
        } else if (c == 27 || c == 91) {
            continue;
        } else if (c == 'q' || c == 'Q') {
            break;
        } else if (c == 'r' || c == 'R') {
            score = 0;
            board_init(board);
            print_score(score);
            print_board(board);
            print_indicators();
            continue;
        } else if (c == 'b') {
            // undo last move
            memcpy(board, last_board, sizeof(board));
            score = last_score;
            print_score(score);
            print_board(board);
            print_indicators();
            continue;
        } else if (c == 12) {
            CLEAR;
            print_score(score);
            print_board(board);
            print_indicators();
            continue;
        }

        // save current board tp a tmp buffer
        memcpy(tmp_board, board, sizeof(board));
        last_score = score;

        switch (c) {
            case 'k': case 'w': case '^':
            case 65:  // up arrow
                has_move = board_move_up(board, &score);
                break;
            case 'j': case 's': case '|':
            case 66:  // down arrow
                has_move = board_move_down(board, &score);
                break;
            case 'l': case 'd': case 9:
            case 67:  // right arrow
                has_move = board_move_right(board, &score);
                break;
            case 'h': case 'a': case 8:
            case 68:  // left arrow
                has_move = board_move_left(board, &score);
                break;
        }

        if (has_move) {
            // save the current board
            memcpy(last_board, tmp_board, sizeof(board));

            board_add_piece(board);

            print_score(score);
            diff_board(tmp_board, board);
            print_indicators();

            if (!board_can_move(board)) {
                print_game_over();
                print_board(board);
                print_indicators();
                
                run = false;
                break;
            } else if (board_win(board)) {
                print_win();
                print_board(board);
                print_indicators();

                run = false;
                break;
            }
        } 
    }

    // restore terminal settings
    setBufferedInput(true);
    putchar('\n');

    return EXIT_SUCCESS;
}

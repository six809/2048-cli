#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>	  
#include <unistd.h>	  // defines: STDIN_FILENO, usleep
#include <time.h>

#include "../include/commun.h"
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
    printf("\033[?25l\033[2J");
    // init rng
    srand(time(NULL));
    // disable input buffering, input are processed immediately
    setBufferedInput(false);
    // register signal handler for when ctrl-c is pressed
	signal(SIGINT, signal_callback_handler);
}

int main(/* int argc, char *argv[] */) {

    setup();

    char c;
    u32 score = 0;
    bool run = true;
    u32 board[SIZE][SIZE];
    board_init(board);

    print_score(score);
    print_board(board);
    print_indicators();

    while (run) {
        c = getchar();
        bool has_move;

        if (c == -1) {
            puts("\nError! Cannot read keyboard input!");
            break;
        } else if (c == 27 || c == 91) {
            continue;
        } else if (c == 'q') {
            break;
        }
        
        switch (c) {
            case 65:  // up arrow
                has_move = board_move_up(board, &score);
                break;
            case 66:  // down arrow
                has_move = board_move_down(board, &score);
                break;
            case 67:  // right arrow
                has_move = board_move_right(board, &score);
                break;
            case 68:  // left arrow
                has_move = board_move_left(board, &score);
                break;
        
            default:
                has_move = false;
                break;
        }

        if (has_move) {
            board_add_piece(board);

            print_score(score);
            print_board(board);
            print_indicators();

            if (!board_can_move(board)) {
                run = false;
                break;
            }
        }
        
    }

    // restore terminal settings
    setBufferedInput(true);
    
    return EXIT_SUCCESS;
}
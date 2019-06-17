#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

#include "minesweeper.hpp"

//update the board in terminal
void update_display(WINDOW *win, Minesweeper game){
    char character_map[9] = {'_', '1', '2', '3', '4', '5', '6', '7', '8'};
    int current_row = 1;
    for(int i = 0; i < game.height; i++){
        wmove(win, current_row, 1);
        for(int j = 0; j < game.width; j++){
            char character = ' ';

            Node *cell = &game.board[i * game.width + j];
            
            if(cell->visited){
                character = character_map[cell->value];
            }

            if(cell->is_mine && !cell->is_flag && game.hit_mine){
                character = '*';
            }

            if(cell->is_flag){
                character = 'F';
            }

            waddch(win, character);
        }
        current_row++;
    }
}

int main(){
    int input;

    int max_x = 10;
    int max_y = 10;
    int mines = 10;

    int cur_x = 1;
    int cur_y = 1;

    int win_start_x = 0;
    int win_start_y = 0;
    
    Minesweeper game = Minesweeper(max_y, max_x, mines);

    initscr();
    noecho();
    clear();
    nodelay(stdscr, true);
    keypad(stdscr, true);

    WINDOW *win = newwin(max_y + 2, max_x + 2, win_start_y, win_start_x);
    box(win, 0, 0);
    refresh();
    wmove(win, 1, 1);
    update_display(win, game);

    for(;;)
    {
        if((input = getch()) != ERR){
            switch (input)
            {
                case KEY_DOWN:
                    if(cur_y < max_y){
                        cur_y++;
                    }
                    break;

                case KEY_UP:
                    if(cur_y > 1){
                        cur_y--;
                    }
                    break;

                case KEY_RIGHT:
                    if(cur_x < max_x){
                        cur_x++;
                    }
                    break;

                case KEY_LEFT:
                    if(cur_x > 1){
                        cur_x--;
                    }
                    break;

                case 10: //enter key
                    if(!game.board[(cur_y - 1) * game.height + cur_x - 1].is_mine){
                        game.open_cell(cur_y - 1, cur_x - 1);
                    } else {
                        game.hit_mine = true;
                    }
                    break;

                case 102:
                    game.toggle_flag(cur_y - 1, cur_x - 1);
                    break;

                default:
                    break;
            }

            update_display(win, game);
            wmove(win, cur_y, cur_x);
            wrefresh(win);
        }
    }
    
    delwin(win);
    endwin();
    printf("test\n");
}

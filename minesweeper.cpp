#include <iostream>
#include <curses.h>
#include <stdlib.h>
#include <cstring>

#include "minesweeper.hpp"
#include "cxxopts.hpp"

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
            } else if (cell->is_mine && game.game_finished && !game.hit_mine){
                character = 'F';
            }

            if(cell->is_flag){
                character = 'F';
            }

            waddch(win, character);
        }
        current_row++;
    }
}

int main(int argc, char *argv[]){
    int input;
    bool restart = true;
    
    int cur_x;
    int cur_y;

    int width = 8;
    int height = 8;
    int mines = 10;

    int win_start_x = 0;
    int win_start_y = 0;

    // Parse arguments
    cxxopts::Options args("Minespweeper", "");

    args.add_options()
        ("b,beginner", "Beginner")
        ("i,intermediate", "Intermediate")
        ("e,expert", "Expert")
        ("c,custom", "Custom")
        ("h,height", "Height", cxxopts::value<int>()->default_value("8"))
        ("w,width", "Width", cxxopts::value<int>()->default_value("8"))
        ("m,mines", "Number of mines", cxxopts::value<int>()->default_value("10"))
        ("help", "Show help");

    auto result = args.parse(argc, argv);
    
    if(result.count("help")){
        std::cout << args.help({""}) << std::endl;
        return 0;
    } else if(result.count("b")){
        height = 8;
        width = 8;
        mines = 10;
    } else if(result.count("i")){
        height = 16;
        width = 16;
        mines = 40;
    } else if(result.count("e")){
        height = 16;
        width = 30;
        mines = 99;
    } else if(result.count("c")){
        if(result.count("h") && result.count("w") && result.count("m")){
            try
            {
                height = result["h"].as<int>();
                width = result["w"].as<int>();
                mines = result["m"].as<int>();
                if(mines >= height * width){
                    mines = height * width - 1;
                }
            }
            catch(const cxxopts::OptionException& e)
            {
                std::cerr << e.what() << '\n';
                return 0;
            }
        }
    }

    Minesweeper game = Minesweeper(height, width, mines);

    initscr();
    noecho();
    clear();
    nodelay(stdscr, true);
    keypad(stdscr, true);

    WINDOW *win = newwin(height + 2, width + 2, win_start_y, win_start_x);

    while(restart){
        // set cursor's initial position
        cur_x = 1;
        cur_y = 1;

        // draw window
        refresh();
        box(win, 0, 0);
        update_display(win, game);
        wmove(win, 1, 1);
        wrefresh(win);
        
        restart = false;

        while(!game.game_finished)
        {
            if((input = getch()) != ERR){
                switch (input)
                {
                    case KEY_DOWN:
                        if(cur_y < height){
                            cur_y++;
                        }
                        break;

                    case KEY_UP:
                        if(cur_y > 1){
                            cur_y--;
                        }
                        break;

                    case KEY_RIGHT:
                        if(cur_x < width){
                            cur_x++;
                        }
                        break;

                    case KEY_LEFT:
                        if(cur_x > 1){
                            cur_x--;
                        }
                        break;

                    case 10: //enter key
                        game.open_cell(cur_y - 1, cur_x - 1);
                        break;

                    case 102: //F key
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

        // End game statistics
        move(height + 3, 0);

        if(game.hit_mine){
            printw("Hit Mine!\n");    
        } else {
            printw("You win!\n");
            printw("You did it in %.2f seconds.\n", game.get_playing_time());
        }

        printw("Press 'R' to restart, 'ENTER' to quit...");
        refresh();

        while(1){
            input = getch();
            if(input != ERR){
                if(input == 114){     // R key
                    game.restart();
                    restart = true;
                    clear();
                    break;
                } else if(input == 10){
                    break;
                }
            }
        }
    }

    delwin(win);
    endwin();

    return 0;
}

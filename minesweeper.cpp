#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <cstring>

struct Node
{
    //coordinates
    int x;
    int y;

    //value of the cell
    int value;
    bool visited;
    bool is_mine;
    bool is_flag;

    //neighbours
    int num_neighbours;
    Node **neighbours;
};

class Minesweeper{
private:
    //compute neighbours of every cell
    void compute_neighbours(){
        Node *current_node;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                current_node = &board[i * height + j];
                //printf("%i", current_node.x);
                if(i == 0){         //top row of the board
                    if(j == 0){     //top left corner
                        current_node->num_neighbours = 3;
                        current_node->neighbours = new Node *[3];
                        current_node->neighbours[0] = &board[1];
                        current_node->neighbours[1] = &board[width];
                        current_node->neighbours[2] = &board[width + 1];
                    } else if(j == width - 1){  //top right corner
                        current_node->num_neighbours = 3;
                        current_node->neighbours = new Node *[3];
                        current_node->neighbours[0] = &board[width - 2];
                        current_node->neighbours[1] = &board[width * 2 - 2];
                        current_node->neighbours[2] = &board[width * 2 - 1];
                    } else {
                        current_node->num_neighbours = 5;
                        current_node->neighbours = new Node *[5];
                        current_node->neighbours[0] = &board[j - 1];
                        current_node->neighbours[1] = &board[(i + 1) * height + j - 1];
                        current_node->neighbours[2] = &board[(i + 1) * height + j];
                        current_node->neighbours[3] = &board[(i + 1) * height + j + 1];
                        current_node->neighbours[4] = &board[j + 1];
                    }
                } else if(i == height - 1){     //bottom row
                    if(j == 0){                 //bottom left corner
                        current_node->num_neighbours = 3;
                        current_node->neighbours = new Node *[3];
                        current_node->neighbours[0] = &board[(i - 1) * height];
                        current_node->neighbours[1] = &board[(i - 1) * height + 1];
                        current_node->neighbours[2] = &board[i * height + 1];
                    } else if(j == width - 1){  //bottom right corner
                        current_node->num_neighbours = 3;
                        current_node->neighbours = new Node *[3];
                        current_node->neighbours[0] = &board[(i - 1) * height + j];
                        current_node->neighbours[1] = &board[(i - 1) * height + j - 1];
                        current_node->neighbours[2] = &board[i * height + j - 1];
                    } else {
                        current_node->num_neighbours = 5;
                        current_node->neighbours = new Node *[5];
                        current_node->neighbours[0] = &board[i * height + j - 1];
                        current_node->neighbours[1] = &board[(i - 1) * height + j - 1];
                        current_node->neighbours[2] = &board[(i - 1) * height + j];
                        current_node->neighbours[3] = &board[(i - 1) * height + j + 1];
                        current_node->neighbours[4] = &board[i * height + j + 1];
                    }
                } else {
                    if(j == 0){     //left wall
                        current_node->num_neighbours = 5;
                        current_node->neighbours = new Node *[5];
                        current_node->neighbours[0] = &board[(i - 1) * height];
                        current_node->neighbours[1] = &board[(i - 1) * height + 1];
                        current_node->neighbours[2] = &board[i * height + 1];
                        current_node->neighbours[3] = &board[(i + 1) * height + 1];
                        current_node->neighbours[4] = &board[(i + 1) * height];
                    } else if(j == width - 1){      //right wall
                        current_node->num_neighbours = 5;
                        current_node->neighbours = new Node *[5];
                        current_node->neighbours[0] = &board[(i - 1) * height + j];
                        current_node->neighbours[1] = &board[(i - 1) * height + j - 1];
                        current_node->neighbours[2] = &board[i * height + j - 1];
                        current_node->neighbours[3] = &board[(i + 1) * height + j - 1];
                        current_node->neighbours[4] = &board[(i + 1) * height + j];
                    } else {        //middle
                        current_node->num_neighbours = 8;
                        current_node->neighbours = new Node *[8];
                        current_node->neighbours[0] = &board[(i - 1) * height + j - 1];
                        current_node->neighbours[1] = &board[(i - 1) * height + j];
                        current_node->neighbours[2] = &board[(i - 1) * height + j + 1];
                        current_node->neighbours[3] = &board[i * height + j - 1];
                        current_node->neighbours[4] = &board[i * height + j + 1];
                        current_node->neighbours[5] = &board[(i + 1) * height + j - 1];
                        current_node->neighbours[6] = &board[(i + 1) * height + j];
                        current_node->neighbours[7] = &board[(i + 1) * height + j + 1];
                    }
                }
                
            }
        }
    }

    //compute number of mines around the cell
    void compute_digit_cells(){
        Node *current_node;
        for(int i = 0; i < height * width; i++){
            current_node = &board[i];
            for(int n = 0; n < current_node->num_neighbours; n++){
                if((*current_node->neighbours[n]).is_mine){
                    current_node->value++;
                }
            }
        }
    }

    void set_up(){
        srand(time(0));
        int position;

        //lay out mines
        for(int i = 0; i < mines; i++){
            bool found = false;
            while(!found){
                position = rand() % (height * width);
                if(board[position].value == 0){
                    found = true;
                    board[position].is_mine = true; //set mine
                }
            }
        }

        //obtain a list of neighbours for each cell
        compute_neighbours();
        
        //calculate number of mines surrounding each cell
        compute_digit_cells();
    }

public:
    Node* board;
    int mines;
    int height;
    int width;

    int mines_left;
    
    Minesweeper(int h, int w, int m){
        height = h;
        width = w;
        mines = m;
        mines_left = m;

        board = new Node[height * width];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                
                Node *cell = &board[i * height + j];
                cell->is_mine = false;
                cell->visited = false;
                cell->is_flag = false;
                cell->value = 0;
                cell->x = j;
                cell->y = i;
            }
        }
        
        set_up();
    }
    
    Node* getBoard(){
        return board;
    }

    //breadth first search when opening empty area
    //just open the cell otherwise
    void open_cell(int y, int x){
        Node *cell = &board[y * height + x];
        if(!cell->visited){
            if(!cell->is_mine){
                if(cell->value != 0){
                    cell->visited = true;
                } else {
                    cell->visited = true;
                    for(int n = 0; n < cell->num_neighbours; n++){
                        Node *neighbour = cell->neighbours[n];
                        open_cell(neighbour->y, neighbour->x);
                    }
                }
            }
        }
    }
};

//update the board in terminal
void update_display(WINDOW *win, Minesweeper game){
    char character_map[9] = {'_', '1', '2', '3', '4', '5', '6', '7', '8'};
    int current_row = 1;
    int current_column = 1;
    for(int i = 0; i < game.height; i++){
        wmove(win, current_row, 1);
        for(int j = 0; j < game.width; j++){
            char character = ' ';

            Node *cell = &game.board[i * game.height + j];
            
            if(cell->visited){
                character = character_map[cell->value];

                if(cell->is_mine){
                    character = '*';
                }
            }

            waddch(win, character);
            current_column++;
        }
        current_column = 1;
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
                    game.open_cell(cur_y - 1, cur_x - 1);
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
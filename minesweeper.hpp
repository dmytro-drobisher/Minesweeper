#include <cstdlib>
#include <chrono>

struct Node{
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
    // number of cells opened that are not mines
    int opened_cells;
    bool first_click;

    // time statistics
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::chrono::time_point<std::chrono::system_clock> end_time;

    void compute_neighbours();

    void compute_digit_cells();

    void set_up_minefield();

    void reset_minefield();

    void swap_mine(int y, int x);

public:
    Node* board;
    int mines;
    int height;
    int width;

    bool game_finished;
    bool hit_mine;

    Minesweeper(int h, int w, int m){
        first_click = true;
        game_finished = false;
        hit_mine = false;
        opened_cells = 0;
        
        height = h;
        width = w;
        mines = m;

        // initialise board
        board = new Node[height * width];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                
                Node *cell = &board[i * width + j];
                cell->is_mine = false;
                cell->visited = false;
                cell->is_flag = false;
                cell->value = 0;
                cell->x = j;
                cell->y = i;
            }
        }
        
        // lay out mines
        set_up_minefield();

        // obtain a list of neighbours for each cell
        compute_neighbours();
    }

    // depth first search when opening empty area
    // just open the cell otherwise
    void open_cell(int y, int x, bool external);

    void toggle_flag(int y, int x);

    double get_playing_time();

    // restart game
    void restart();
};

void Minesweeper::compute_neighbours(){
    //compute neighbours of every cell
    //constructs a square lattice
    //surely a better method must exist

    Node *current_node;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            current_node = &board[i * width + j];
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
                    current_node->neighbours[1] = &board[(i + 1) * width + j - 1];
                    current_node->neighbours[2] = &board[(i + 1) * width + j];
                    current_node->neighbours[3] = &board[(i + 1) * width + j + 1];
                    current_node->neighbours[4] = &board[j + 1];
                }
            } else if(i == height - 1){     //bottom row
                if(j == 0){                 //bottom left corner
                    current_node->num_neighbours = 3;
                    current_node->neighbours = new Node *[3];
                    current_node->neighbours[0] = &board[(i - 1) * width];
                    current_node->neighbours[1] = &board[(i - 1) * width + 1];
                    current_node->neighbours[2] = &board[i * width + 1];
                } else if(j == width - 1){  //bottom right corner
                    current_node->num_neighbours = 3;
                    current_node->neighbours = new Node *[3];
                    current_node->neighbours[0] = &board[(i - 1) * width + j];
                    current_node->neighbours[1] = &board[(i - 1) * width + j - 1];
                    current_node->neighbours[2] = &board[i * width + j - 1];
                } else {
                    current_node->num_neighbours = 5;
                    current_node->neighbours = new Node *[5];
                    current_node->neighbours[0] = &board[i * width + j - 1];
                    current_node->neighbours[1] = &board[(i - 1) * width + j - 1];
                    current_node->neighbours[2] = &board[(i - 1) * width + j];
                    current_node->neighbours[3] = &board[(i - 1) * width + j + 1];
                    current_node->neighbours[4] = &board[i * width + j + 1];
                }
            } else {
                if(j == 0){     //left wall
                    current_node->num_neighbours = 5;
                    current_node->neighbours = new Node *[5];
                    current_node->neighbours[0] = &board[(i - 1) * width];
                    current_node->neighbours[1] = &board[(i - 1) * width + 1];
                    current_node->neighbours[2] = &board[i * width + 1];
                    current_node->neighbours[3] = &board[(i + 1) * width + 1];
                    current_node->neighbours[4] = &board[(i + 1) * width];
                } else if(j == width - 1){      //right wall
                    current_node->num_neighbours = 5;
                    current_node->neighbours = new Node *[5];
                    current_node->neighbours[0] = &board[(i - 1) * width + j];
                    current_node->neighbours[1] = &board[(i - 1) * width + j - 1];
                    current_node->neighbours[2] = &board[i * width + j - 1];
                    current_node->neighbours[3] = &board[(i + 1) * width + j - 1];
                    current_node->neighbours[4] = &board[(i + 1) * width + j];
                } else {        //middle
                    current_node->num_neighbours = 8;
                    current_node->neighbours = new Node *[8];
                    current_node->neighbours[0] = &board[(i - 1) * width + j - 1];
                    current_node->neighbours[1] = &board[(i - 1) * width + j];
                    current_node->neighbours[2] = &board[(i - 1) * width + j + 1];
                    current_node->neighbours[3] = &board[i * width + j - 1];
                    current_node->neighbours[4] = &board[i * width + j + 1];
                    current_node->neighbours[5] = &board[(i + 1) * width + j - 1];
                    current_node->neighbours[6] = &board[(i + 1) * width + j];
                    current_node->neighbours[7] = &board[(i + 1) * width + j + 1];
                }
            }
            
        }
    }
}

void Minesweeper::compute_digit_cells(){
    //compute number of mines around the cell
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

void Minesweeper::set_up_minefield(){
    srand(time(0));
    int position;

    //lay out mines
    for(int i = 0; i < mines; i++){
        bool found = false;
        while(!found){
            position = rand() % (height * width);
            if(!board[position].is_mine){
                found = true;
                board[position].is_mine = true; //set mine
            }
        }
    }
}

void Minesweeper::reset_minefield(){
    Node *cell;
    
    // reset each cell to default values
    for (int i = 0; i < height * width; i++){
        cell = &board[i];

        cell->is_mine = false;
        cell->is_flag = false;
        cell->visited = false;
        cell->value = 0;
    }
}

void Minesweeper::swap_mine(int y, int x){
    // swap mine to top left corner if initial click is a mine
    Node *cell;
    
    // iterate from top left corner until a free cell is found
    for (int i = 0; i < width * height; i++){
        cell = &board[i];
        if(!cell->is_mine){
            cell->is_mine = true;
            break;
        }
    }
    
    // remove mine from current cell
    cell = &board[y * width + x];
    cell->is_mine = false;
}

void Minesweeper::open_cell(int y, int x, bool external = true){
    Node *cell = &board[y * width + x];
    
    if(first_click){
        // swap mine to top left corner if first click is a mine
        // same behaviour as in windows minesweeper
        if(cell->is_mine){
            swap_mine(y, x);
        }

        // calculate number of mines surrounding each cell
        compute_digit_cells();

        // set start time
        start_time = std::chrono::system_clock::now();
        first_click = false;
    }

    if(!hit_mine){
        // open previously unvisited cell
        if(!cell->visited){
            if(!cell->is_mine){
                cell->is_flag = false;
                
                opened_cells++;

                // run DFS to open an empty region
                if(cell->value != 0){
                    cell->visited = true;
                } else {
                    cell->visited = true;
                    for(int n = 0; n < cell->num_neighbours; n++){
                        Node *neighbour = cell->neighbours[n];
                        open_cell(neighbour->y, neighbour->x, false);
                    }
                }
            } else if(external) {
                // unvisited cell from external trigger
                hit_mine = true;
                game_finished = true;

                // set end time
                end_time = std::chrono::system_clock::now();
            }
        } else if(external) {
            // chord: open nearby cells if possible and all flags are correct
            int num_flags = 0;
            bool all_flags_on_mines = true;
            Node *neighbour;

            // compute number of flags surrounding current cell and determine if they are in mines
            for(int i = 0; i < cell->num_neighbours; i++){
                neighbour = cell->neighbours[i];
                if(neighbour->is_flag){
                    num_flags++;
                }

                if((!neighbour->is_mine && neighbour->is_flag) || (neighbour->is_mine && !neighbour->is_flag)){
                    // misplaced flag
                    all_flags_on_mines = false;
                }
            }

            // chord if correct number of flags
            if(num_flags == cell->value){
                if(all_flags_on_mines){
                    for(int i = 0; i < cell->num_neighbours; i++){
                        neighbour = cell->neighbours[i];
                        open_cell(neighbour->y, neighbour->x, false);
                    }
                } else {
                    // correct number of flags around the cell but not all mnes are covered
                    hit_mine = true;
                    game_finished = true;

                    // set finish time
                    end_time = std::chrono::system_clock::now();
                }
            }
        }

        // if all non-mine cells have been opened
        if(opened_cells == height * width - mines){
            end_time = std::chrono::system_clock::now();
            game_finished = true;
        }
    }
}

void Minesweeper::toggle_flag(int y, int x){
    Node *cell = &board[y * width + x];
    if(!cell->visited && !hit_mine){
        cell->is_flag = !cell->is_flag;
    }
}

double Minesweeper::get_playing_time(){
    std::chrono::duration<double> duration = end_time - start_time;
    return duration.count();
}

void Minesweeper::restart(){
    // reset game controls
    first_click = true;
    game_finished = false;
    hit_mine = false;
    opened_cells = 0;

    // reset the board
    reset_minefield();
    set_up_minefield();
}
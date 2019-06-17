#include <ctime>
#include <cstdlib>

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
    void compute_neighbours();

    void compute_digit_cells();

    void set_up();

public:
    Node* board;
    int mines;
    int height;
    int width;

    int flags_set;
    int mines_left;

    bool hit_mine;

    Minesweeper(int h, int w, int m){
        height = h;
        width = w;
        mines = m;
        mines_left = m;
        hit_mine = false;

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
        
        set_up();
    }

    //breadth first search when opening empty area
    //just open the cell otherwise
    void open_cell(int y, int x, bool external);

    void toggle_flag(int y, int x);
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

void Minesweeper::set_up(){
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

void Minesweeper::open_cell(int y, int x, bool external = true){
    Node *cell = &board[y * width + x];
    
    if(!hit_mine){
        //open previously unvisited cell
        if(!cell->visited){
            if(!cell->is_mine){
                cell->is_flag = false;
                
                //run BFS to open an empty region
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
        } else if(external) {
            //chord: open nearby cells if possible and all flags are correct
            int num_flags = 0;
            bool all_flags_on_mines = true;
            Node *neighbour;
            for(int i = 0; i < cell->num_neighbours; i++){
                neighbour = cell->neighbours[i];
                if(neighbour->is_flag){
                    num_flags++;
                }

                if((!neighbour->is_mine && neighbour->is_flag) || (neighbour->is_mine && !neighbour->is_flag)){
                    //misplaced flag
                    all_flags_on_mines = false;
                }
            }

            //chord if correct number of flags
            if(num_flags == cell->value){
                if(all_flags_on_mines){
                    for(int i = 0; i < cell->num_neighbours; i++){
                        neighbour = cell->neighbours[i];
                        open_cell(neighbour->y, neighbour->x, false);
                    }
                } else {
                    hit_mine = true;
                }
            }
        }
    }
}

void Minesweeper::toggle_flag(int y, int x){
    Node *cell = &board[y * width + x];
    if(!cell->visited && !hit_mine){
        cell->is_flag = !cell->is_flag;
    }
}

# Minesweeper

A simple implementation of minesweeper in C++ using ncurses. Currently work in progress

##### Requirements

* ncurses
* cxxopts

To compile run: 

g++ minesweeper.cpp -o minesweeper -lncurses -O3 -std=c++1

##### Controls:

* arrow keys - move the cursor
* Enter - open unvisited cell/chord visited cell
* F - toggle flag on unvisited cell

##### TODO:

- [x] Fix bug when win is detected but not all cells are opened
- [ ] Implement mine swapping on first click
- [ ] Implement game restart
- [ ] Fix blank UI unless a button is pressed
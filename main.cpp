#include <conio.h>
#include <stdlib.h> /* srand, rand */
#include <windows.h>

#include <iostream>
#include <vector>

#define STARTING_SNAKE_SIZE 1
#define DEBUG_PRINT true

struct Cell {
  int state;
};

struct gameState {
  std::vector<std::vector<Cell>> gameBoard;
  int rows;
  int cols;
  int snake_size;
};

/**
 * Gets: number of seconds to wait for user input
 * Returns: '_' if there was no input, otherwise returns the char inputed
 *
 * N.B. : _kbhit() and _getch() are non-deprecated C++ versions of these conio.h
 * functions
 **/
char waitForCharInput(float seconds) {
  char c = '_';  // default return
  float interval = seconds / 10.0;
  while (seconds > 0) {
    if (_kbhit()) {  // if there is a key in keyboard buffer
      c = _getch();  // get the char
      break;         // we got char! No need to wait anymore...
    }

    Sleep(interval);      // one second sleep
    seconds -= interval;  // countdown a second
  }
  Sleep(seconds);
  return c;
}

gameState *createGame(int rows, int cols) {
  gameState *g = new gameState;
  g->rows = rows;
  g->cols = cols;
  g->snake_size = STARTING_SNAKE_SIZE;
  int food_i = rand() % g->rows;
  int food_j = rand() % g->cols;
  for (int i = 0; i < rows; i++) {
    std::vector<Cell> temp;
    for (int j = 0; j < cols; j++) {
      Cell tempCell;
      tempCell.state = 0;
      if (i == (rows / 2) && j == (cols / 2))
        tempCell.state = 1;            // square of head of snake
      if (i == food_i && j == food_j)  // food has not been spawned
        tempCell.state = -1;           // food state

      temp.push_back(tempCell);
    }
    g->gameBoard.push_back(temp);
  }
  return g;
}

void makeMove(gameState *g, int dir) {
  // if/else for processing the input
  int head_i = -1, head_j = -1;
  int food_i = -1, food_j = -1;
  for (int i = 0; i < (g->gameBoard).size(); i++) {
    for (int j = 0; j < (g->gameBoard)[i].size(); j++) {
      Cell *elem = &(g->gameBoard)[i][j];
      if (elem->state == 1) {
        head_i = i;
        head_j = j;
      }
      if (elem->state == -1) {  // food coordinate
        food_i = i;
        food_j = j;
      } else if (elem->state != 0)
        elem->state += 1;  // increase age of snake pieces
      if (elem->state > g->snake_size) {
        elem->state = 0;
      }
    }
  }
  switch (dir) {
    case 0:
      head_j = (head_j + 1) % g->cols;
      break;
    case 1:
      head_i = (head_i + 1) % g->rows;
      break;
    case 2:
      head_j = (head_j + g->cols - 1) %
               g->cols;  // (head_j + g->cols - 1) avoids negative result
      break;
    case 3:
      head_i = (head_i + g->rows - 1) %
               g->rows;  // (head_j + g->cols - 1) avoids negative result
      break;
    default:
      std::cout << "this should not happen";
      break;
  }
  (g->gameBoard)[head_i][head_j].state = 1;  // head has lowest age (1)

  int has_eaten = 0;
  // find an unoccupied spot to place food
  while ((g->gameBoard)[food_i][food_j].state != 0 &&
         (g->gameBoard)[food_i][food_j].state != -1) {
    has_eaten = 1;
    food_i = rand() % g->rows;
    food_j = rand() % g->cols;
  }
  g->snake_size += has_eaten;
  (g->gameBoard)[food_i][food_j].state = -1;
}

void printGame(gameState g) {
  system("cls");
  for (auto line : g.gameBoard) {
    for (auto elem : line) {
      switch (elem.state) {
        case 0:
          std::cout << '.';
          break;
        case -1:
          std::cout << '*';
          break;
        default:
          if (DEBUG_PRINT) {
            std::cout << elem.state;
          } else {
            std::cout << '#';
          }
          break;
      }
      std::cout << ' ';
    }
    std::cout << std::endl;
  }
}

int main(int argc, char *argv[]) {
  gameState *g = createGame(12, 12);
  printGame(*g);
  float gameSpeed = 160.0;  // delay between inputs, in ms
  char input;
  input = waitForCharInput(gameSpeed);
  int dir = 0;

  while (input != 'q') {  // while we have not quit the game
    // direction only changes at 90 deg incremets, instant 180 turn not possible
    if (input == 'd' && dir != 2) dir = 0;
    if (input == 's' && dir != 3) dir = 1;
    if (input == 'a' && dir != 0) dir = 2;
    if (input == 'w' && dir != 1) dir = 3;
    makeMove(g, dir);
    printGame(*g);
    input = waitForCharInput(gameSpeed);

    // TODO: increase gameSpeed gradually: NOTE refactor variable name, it is
    // confusing
  }
  free(g);
  return 0;
}
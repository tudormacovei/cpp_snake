#include <iostream>
#include <vector>

#define STARTING_SNAKE_SIZE 1
#define STARTING_ROW 2
#define STARTING_COL 2

struct Cell {
  int state;
};

struct gameState {
  std::vector<std::vector<Cell>> gameBoard;
  int rows;
  int cols;
  int snake_size;
};

gameState *createGame(int rows, int cols) {
  gameState *g = new gameState;
  g->rows = rows;
  g->cols = cols;
  g->snake_size = STARTING_SNAKE_SIZE;
  for (int i = 0; i < rows; i++) {
    std::vector<Cell> temp;
    for (int j = 0; j < cols; j++) {
      Cell tempCell;
      tempCell.state = 0;
      if (i == STARTING_ROW && j == STARTING_COL)
        tempCell.state = 1;  // square of head of snake
      temp.push_back(tempCell);
    }
    g->gameBoard.push_back(temp);
  }
  return g;
}

void makeMove(gameState *g, int dir) {
  // if/else for processing the input
  int head_i, head_j;
  for (int i = 0; i < (g->gameBoard).size(); i++) {
    for (int j = 0; j < (g->gameBoard)[i].size(); j++) {
      Cell *elem = &(g->gameBoard)[i][j];
      if (elem->state == 1) {
        head_i = i;
        head_j = j;
      }
      if (elem->state != 0) elem->state += 1;  // increase age of snake pieces
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
}

void printGame(gameState g) {
  system("cls");
  for (auto line : g.gameBoard) {
    for (auto elem : line) {
      //   std::cout << (elem.state == 0 ? '.' : '#');
      std::cout << elem.state;
    }
    std::cout << std::endl;
  }
}

int main(int argc, char *argv[]) {
  gameState *g = createGame(5, 20);
  printGame(*g);
  std::string input;
  std::cin >> input;
  int dir = 0;

  while (input != ":q") {  // while we have not quit the game
    if (input == "d") dir = 0;
    if (input == "s") dir = 1;
    if (input == "a") dir = 2;
    if (input == "w") dir = 3;
    makeMove(g, dir);
    printGame(*g);
    std::cin >> input;
  }
  free(g);
  return 0;
}
#include <conio.h>
#include <stdlib.h> /* srand, rand */
#include <unistd.h>
#include <windows.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// the number of characters on a 'full' line, used for text centering
#define LINE_CHAR_SIZE 69
#define LINE_CHAR_SIZE_HALF 34

// game variables
#define STARTING_SNAKE_SIZE 1
#define DEBUG_PRINT false

// TODO: change Cell to use an enum to make things more readable

struct Cell {
  int state;
  int direction;
};

struct gameState {
  std::vector<std::vector<Cell>> gameBoard{};
  unsigned int rows{};
  unsigned int cols{};
  unsigned int snake_size{};
};

char cellDirectionToChar(Cell c) {
  switch (c.direction) {
    case 0:
      return '>';
      break;

    case 1:
      return 'v';
      break;

    case 2:
      return '<';
      break;

    case 3:
      return '^';
      break;

    default:
      break;
  }
  return '_';
}

/**
 * Gets: number of miliseconds to wait for user input
 * Returns: '_' if there was no input, otherwise returns the char inputed
 *
 * N.B. : _kbhit() and _getch() are non-deprecated C++ versions of these conio.h
 * functions
 **/
char waitForCharInput(float seconds) {
  char c{'_'};     // Player input, default value is 'no input'
  int interval{};  // Interval between keyboard polls
  int total_ms{};  // total time (in ms) to pause execution for in this function

  interval = int((seconds * 1000.f) / 50.0f);
  total_ms = int(seconds *
                 1000.f);  // convert given seconds to miliseconds, needed since
                           // Sleep() takes an unsigned int, not a float value
  // TODO: switch to ncurses instead of _kbhit at some point
  while (total_ms > 0) {
    total_ms -= interval;
    Sleep(DWORD(interval));  // 'interval' miliseconds of sleep

    if (_kbhit()) {        // if there is a key in keyboard buffer
      c = char(_getch());  // get the char
      break;               // we got char! No need to wait anymore...
    }
  }
  // we got the keystroke but still wait for remaining time
  // so that refresh rate (perceived game speed) is constant
  // if (total_ms > 0) Sleep(DWORD(total_ms));
  return c;
}

gameState *createGame(unsigned int rows, unsigned int cols) {
  gameState *g = new gameState;
  g->rows = rows;
  g->cols = cols;
  g->snake_size = STARTING_SNAKE_SIZE;

  // set a random position for the 'food' Cell
  unsigned int food_i{};
  food_i = (unsigned int)(rand()) % g->rows;
  unsigned int food_j{};
  food_j = (unsigned int)(rand()) % g->cols;

  for (unsigned int i = 0; i < rows; i++) {
    std::vector<Cell> temp;
    for (unsigned int j = 0; j < cols; j++) {
      Cell tempCell;
      tempCell.state = 0;
      temp.push_back(tempCell);
    }
    g->gameBoard.push_back(temp);
  }
  g->gameBoard[rows / 2][cols / 2].state = 1;  // Head of snake
  g->gameBoard[food_i][food_j].state = -1;     // Food 'state'

  return g;
}

void makeMove(gameState *g, int dir) {
  // if/else for processing the input
  unsigned head_i{};
  unsigned head_j{};
  unsigned food_i{};
  unsigned food_j{};
  for (unsigned int i = 0; i < (g->gameBoard).size(); i++) {
    for (unsigned int j = 0; j < (g->gameBoard)[i].size(); j++) {
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
      if (elem->state > int(g->snake_size)) {
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
  (g->gameBoard)[head_i][head_j].direction = dir;

  bool has_eaten = false;
  // find an unoccupied spot to place food
  while ((g->gameBoard)[food_i][food_j].state != 0 &&
         (g->gameBoard)[food_i][food_j].state != -1) {
    has_eaten = true;
    food_i = (unsigned int)(rand()) % g->rows;
    food_j = (unsigned int)(rand()) % g->cols;
  }
  g->snake_size += has_eaten ? 1 : 0;
  (g->gameBoard)[food_i][food_j].state = -1;
}

void printGame(gameState g);  // fw declaration

// executes the game, returns the score obtained
int mainGameLoop() {
  gameState *g = createGame(12, 12);
  printGame(*g);
  float gameSpeed = 0.5f;  // delay between inputs, in ms
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

  return int(g->snake_size);
}

struct Button {
  std::string text{};
  bool selected{false};
  int resultingState{};  // what state the game is put into if button is
                         // selected
};

void printTitle() {
  std::cout
      << "      ___           ___           ___           ___           ___    "
         " \n     /\\  \\         /\\__\\         /\\  \\         /\\__\\      "
         "   /\\  \\    \n    /::\\  \\       /::|  |       /::\\  \\       "
         "/:/  /        /::\\  \\   \n   /:/\\ \\  \\     /:|:|  |      "
         "/:/\\:\\  \\     /:/__/        /:/\\:\\  \\  \n  _\\:\\~\\ \\  \\   "
         "/:/|:|  |__   /::\\~\\:\\  \\   /::\\__\\____   /::\\~\\:\\  \\ \n "
         "/\\ \\:\\ \\ \\__\\ /:/ |:| /\\__\\ /:/\\:\\ \\:\\__\\ "
         "/:/\\:::::\\__\\ /:/\\:\\ \\:\\__\\\n \\:\\ \\:\\ \\/__/ \\/__|:|/:/ "
         " / \\/__\\:\\/:/  / \\/_|:|~~|~    \\:\\~\\:\\ \\/__/\n  \\:\\ "
         "\\:\\__\\       |:/:/  /       \\::/  /     |:|  |      \\:\\ "
         "\\:\\__\\  \n   \\:\\/:/  /       |::/  /        /:/  /      |:|  |  "
         "     \\:\\ \\/__/  \n    \\::/  /        /:/  /        /:/  /       "
         "|:|  |        \\:\\__\\    \n     \\/__/         \\/__/         "
         "\\/__/         \\|__|         \\/__/";
}

void printBottom(int highScore, bool isPaused,
                 int currScore) {  // should probably make a gameState struct
  std::string separatorString(LINE_CHAR_SIZE, char(196));
  std::cout << separatorString << '\n';
  if (isPaused)
    std::cout << "Press 'W' and 'S' to navigate. Press SPACE to "
                 "select current option. HIGH SCORE: "
              << highScore;
  else
    std::cout
        << "Controls: W, A, S, D. Press Q to exit to menu. CURRENT SCORE: "
        << currScore;
  std::cout << std::endl;
}

void printGame(gameState g) {
  int indentAmount{};
  char gameOutlineChar = char(177);

  system("cls");

  indentAmount = LINE_CHAR_SIZE_HALF - int(g.cols);
  std::string indentString((unsigned int)(indentAmount), ' ');
  std::string horizontalOutlineString((2 * (g.cols + 1)), gameOutlineChar);

  std::cout << '\n' << indentString << horizontalOutlineString << '\n';
  for (auto line : g.gameBoard) {
    std::cout << indentString << gameOutlineChar;
    for (auto elem : line) {
      switch (elem.state) {
        case 0:
          std::cout << ' ';
          break;
        case -1:
          std::cout << '*';
          break;
        default:
          if (DEBUG_PRINT) {
            std::cout << elem.state;
          } else {
            std::cout << cellDirectionToChar(elem);
          }
          break;
      }
      std::cout << ' ';
    }
    std::cout << gameOutlineChar << '\n';
  }
  std::cout << indentString << horizontalOutlineString << std::endl;
  printBottom(0, false, int(g.snake_size));
}

void printButton(Button button) {
  int indentAmount{};
  char outlinechar{button.selected ? char(177) : ' '};

  indentAmount = LINE_CHAR_SIZE_HALF - int(button.text.size() / 2) + 1;
  std::string indentString((unsigned int)(indentAmount), ' ');

  std::string horizontalOutlineString((button.text.size() + 2), outlinechar);
  std::cout << '\n'
            << indentString << horizontalOutlineString << '\n'
            << indentString << outlinechar << button.text << outlinechar << '\n'
            << indentString << horizontalOutlineString << '\n';
}

void printPause(std::vector<Button> buttons, int highScore) {
  std::cout << std::endl;
  system("cls");

  std::string indentString = "            ";  // 12 spaces
  printTitle();
  std::cout << "\n\n\n";

  for (auto button : buttons) {
    printButton(button);
  }
  printBottom(highScore, true, 0);
}

int pauseScreenLoop(int highScore) {
  char input{'_'};
  unsigned int currentlySelected{0};  // currently selected button

  std::vector<Button> buttons;
  Button playButton;
  Button quitButton;
  playButton.text = "Play";
  playButton.resultingState = 1;

  quitButton.text = "Quit";
  quitButton.resultingState = 0;

  buttons.push_back(playButton);
  buttons.push_back(quitButton);

  while (input != 'q') {  // while we have not quit the game
    buttons[currentlySelected].selected = false;

    if (input == 's') currentlySelected++;
    if (input == 'w' && currentlySelected != 0) currentlySelected--;
    if (input == ' ') return (buttons[currentlySelected]).resultingState;

    // make sure it is well-defined
    currentlySelected =
        std::min((unsigned int)(std::max((unsigned int)0, currentlySelected)),
                 (unsigned int)(buttons.size() - 1));

    buttons[currentlySelected].selected = true;
    printPause(buttons, highScore);

    input = waitForCharInput(0.2f);
  }
  return 0;  // quit game
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  int highScore{0};
  int gameState{0};  // 0 = quit, 1 = start screen, 2 = playing
  while ((gameState = pauseScreenLoop(highScore))) {
    if (gameState == 1) {
      highScore = std::max(mainGameLoop(), highScore);
    }
  }
  // show quit screen
  return 0;
}
#include "main.h"

int main() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  nodelay(stdscr, TRUE);
  printOverlay();
  srand(time(NULL));
  game_loop();
  endwin();
}

void game_loop() {
  GameInfo_t *game = getGame();
  Add *check = getAdd();
  while (check->exit != 1) {
    *game = updateCurrentState();
    if (game->field != NULL)
      printGame(game);
    inputCheck(getch());
  }
}

void inputCheck(int input) {
  if (input == ENTER)
    userInput(Start, 0);
  else if (input == Q || input == q)
    userInput(Terminate, 0);
  else if (input == UP)
    userInput(Up, 0);
  else if (input == DOWN)
    userInput(Down, 0);
  else if (input == LEFT)
    userInput(Left, 0);
  else if (input == RIGHT)
    userInput(Right, 0);
  else if (input == SPACE)
    userInput(Action, 0);
  else if (input == P || input == p)
    userInput(Pause, 0);
}

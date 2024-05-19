#include "tetris.h"

void userInput(UserAction_t user_input, bool hold) {
  (void)hold;
  Add *input = getAdd();
  input->input = user_input;
  input->check = 1;
}

GameInfo_t updateCurrentState() {
  GameInfo_t *game = getGame();
  changeState(game);
  return *game;
}
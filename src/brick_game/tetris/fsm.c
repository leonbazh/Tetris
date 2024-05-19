#include "tetris.h"

void changeState(GameInfo_t *game) {
  Add *check = getAdd();
  Figure *figure = getFigure();
  if (check->state == BEFORE)
    prepare_game(game, figure, check);
  if (check->state == START)
    spawn_game(game, figure, check);
  if (check->state == PAUSE)
    pause_game(game, check);
  if (check->state == MOVING)
    moving_game(game, check);
  if (check->state == SHIFTING)
    shifting_game(figure, check);
  if (check->state == ATTACHING)
    attaching_game(game, figure, check);
  if (check->state == END)
    end_game(check);
  if (check->state == STOP)
    exit_game(game, figure, check);
  copyField(game, figure);
}

void prepare_game(GameInfo_t *game, Figure *figure, Add *input) {
  if (input->input == Start && input->check) {
    if (input->end == 1) {
      restartGame(game, figure);
      input->end = 0;
    } else {
      figure = getFigure();
      game = getGame();
    }
    randomFigure(game, figure);
    figure->id = figure->nextid;
    input->state = START;
  } else if (input->input == Terminate && input->check) {
    input->exit = 1;
  }
  input->check = 0;
}

void spawn_game(GameInfo_t *game, Figure *figure, Add *input) {
  spawnFigure(game, figure);
  figure->id = figure->nextid;
  randomFigure(game, figure);
  if (input->state != END)
    input->state = MOVING;
}

void pause_game(GameInfo_t *game, Add *input) {
  if (input->check) {
    if (input->input == Pause) {
      game->pause = 0;
      input->state = MOVING;
    } else if (input->input == Terminate) {
      input->state = STOP;
    }
  }
}

void moving_game(GameInfo_t *game, Add *input) {
  if (input->check) {
    if (input->input == Terminate)
      input->state = STOP;
    else if (input->input == Pause) {
      game->pause = 1;
      input->state = PAUSE;
    } else if (input->input == Up || input->input == Left ||
               input->input == Right || input->input == Action ||
               input->input == Down)
      moveFigure(input->input);
  }
  if (readyShift(game, input)) {
    input->state = SHIFTING;
  }
  input->check = 0;
}

void shifting_game(Figure *figure, Add *state) {
  if (collision(figure)) {
    state->state = ATTACHING;
  } else {
    figure->y++;
    moveField(figure);
    state->state = MOVING;
  }
}

void attaching_game(GameInfo_t *game, Figure *figure, Add *state) {
  moveStatic(figure);
  cleanLines(game, figure);
  state->state = START;
}

void end_game(Add *state) {
  state->state = BEFORE;
  state->end = 1;
}

void exit_game(GameInfo_t *game, Figure *figure, Add *state) {
  freeGame(game, figure);
  state->exit = 1;
}
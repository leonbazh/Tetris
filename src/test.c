#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "brick_game/tetris/tetris.h"

START_TEST(test1) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  restartGame(game, figure);
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      ck_assert_int_eq(0, figure->exfield[i][j]);
  ck_assert_int_eq(0, game->score);
  ck_assert_int_eq(0, game->pause);
  ck_assert_int_eq(1, game->level);
}
END_TEST

START_TEST(test2) {
  GameInfo_t game = initGame();
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      ck_assert_int_eq(0, game.field[i][j]);
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      ck_assert_int_eq(0, game.next[i][j]);
  freeField(&(game.field), 20);
  freeField(&(game.next), 4);
  ck_assert_ptr_null(game.field);
  ck_assert_ptr_null(game.next);
}
END_TEST

START_TEST(test3) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  for (int i = 0; i < 19; i++)
    for (int j = 0; j < 10; j++)
      figure->exfield[i][j] = 1;
  copyField(game, figure);
  for (int i = 0; i < 19; i++)
    for (int j = 0; j < 10; j++)
      ck_assert_int_eq(1, game->field[i][j]);
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      ck_assert_int_eq(0, game->next[i][j]);
  freeGame(game, figure);
}
END_TEST

START_TEST(test4) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      game->field[i][j] = 1;

  int check = 0;
  for (int j = 0; j < 10; j++) {
    if (game->field[0][j])
      check++;
    if (check == 10)
      check = 1;
  }
  ck_assert_int_eq(1, check);
  freeGame(game, figure);
}
END_TEST

START_TEST(test5) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  randomFigure(game, figure);
  figure->id = figure->nextid;
  spawnFigure(game, figure);
  ck_assert_int_eq(3, figure->x);
  ck_assert_int_eq(1, figure->y);
  ck_assert_int_ne(0, figure->id);
  ck_assert_int_eq(1, figure->nextid);
  freeGame(game, figure);
}

START_TEST(test6) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      game->next[i][j] = 1;
  figure->figure = copyFigure(figure->figure, game->next);
  figure->y = 0;
  figure->x = 3;
  figure->exfield =
      placeFigure(figure->exfield, figure->figure, figure->y, figure->x);
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      ck_assert_int_ne(0, figure->exfield[i][j]);
  freeGame(game, figure);
}

START_TEST(test7) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  for (int i = -1; i < 100; i++) {
    randomFigure(game, figure);
    spawnFigure(game, figure);
    moveFigure(Action);
  }
  freeGame(game, figure);
}
START_TEST(test8) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  randomFigure(game, figure);
  spawnFigure(game, figure);
  moveFigure(Right);
  ck_assert_int_eq(4, figure->x);
  freeGame(game, figure);
}
START_TEST(test9) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  randomFigure(game, figure);
  spawnFigure(game, figure);
  moveFigure(Left);
  ck_assert_int_eq(3, figure->x);
  freeGame(game, figure);
}
START_TEST(test10) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  Add *check = getAdd();
  int res = readyShift(game, check);
  ck_assert_int_eq(1, res);
  freeGame(game, figure);
}

START_TEST(test11) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  int res = collision(figure);
  ck_assert_int_eq(0, res);
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      figure->staticfield[i][j] = 1;
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      figure->exfield[i][j] = 1;
  res = collision(figure);
  ck_assert_int_eq(0, res);
  for (int i = 0; i < 20; i++)
    for (int j = 0; j < 10; j++)
      figure->exfield[i][j] = 0;
  res = collision(figure);
  ck_assert_int_eq(0, res);
  freeGame(game, figure);
}
START_TEST(test12) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  randomFigure(game, figure);
  spawnFigure(game, figure);
  figure->y++;
  moveField(figure);
  ck_assert_int_eq(2, figure->y);
  freeGame(game, figure);
}
START_TEST(test13) {
  Figure *figure = getFigure();
  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 10; j++)
      figure->exfield[i][j] = 1;

  for (int i = 0; i < 2; i++)
    for (int j = 0; j < 10; j++)
      ck_assert_int_eq(1, figure->exfield[i][j]);

  freeField(&figure->exfield, 10);
  freeField(&figure->staticfield, 10);
}
START_TEST(test14) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  for (int i = 19; i < 20; i++)
    for (int j = 0; j < 10; j++)
      figure->staticfield[i][j] = 1;
  cleanLines(game, figure);
  ck_assert_int_eq(100, game->score);
}
START_TEST(test15) {
  GameInfo_t *game = getGame();
  Add *state = getAdd();
  state->check = 1;
  state->input = Start;
  changeState(game);
  ck_assert_ptr_nonnull(game->field);
  ck_assert_ptr_nonnull(game->next);
  ck_assert_int_eq(MOVING, state->state);
  state->state = START;
  state->check = 1;
  state->input = Terminate;
  changeState(game);
  ck_assert_int_eq(BEFORE, state->state);
  changeState(game);
  ck_assert_int_eq(1, state->exit);
}

START_TEST(test16) {
  GameInfo_t *game = getGame();
  Figure *figure = getFigure();
  Add *state = getAdd();
  state->state = START;
  state->check = 1;
  state->input = Start;
  changeState(game);
  changeState(game);
  ck_assert_int_eq(MOVING, state->state);
  ck_assert_int_eq(2, figure->y);
  ck_assert_int_eq(3, figure->x);

  freeGame(game, figure);
}

START_TEST(test17) {
  GameInfo_t *game = getGame();
  Add *state = getAdd();
  state->state = START;
  state->check = 1;
  state->input = Start;
  changeState(game);
  changeState(game);
  state->check = 1;
  state->input = Action;
  changeState(game);
  ck_assert_int_eq(MOVING, state->state);
}
START_TEST(test18) {
  GameInfo_t *game = getGame();
  Add *state = getAdd();
  state->state = START;
  state->check = 1;
  state->input = Start;
  changeState(game);
  changeState(game);
  state->check = 1;
  state->input = Pause;
  changeState(game);
  ck_assert_int_eq(PAUSE, state->state);
}
START_TEST(test19) {
  GameInfo_t *game = getGame();
  Add *state = getAdd();
  state->state = START;
  state->check = 1;
  state->input = Start;
  changeState(game);
  changeState(game);
  state->check = 1;
  state->input = Right;
  changeState(game);
  ck_assert_int_eq(MOVING, state->state);
}
START_TEST(test20) {
  GameInfo_t *game = getGame();
  Add *state = getAdd();
  state->state = START;
  state->check = 1;
  state->input = Start;
  changeState(game);
  changeState(game);
  state->check = 1;
  state->input = Left;
  changeState(game);
  ck_assert_int_eq(MOVING, state->state);
}
START_TEST(test21) {
  GameInfo_t *game = getGame();
  Add *state = getAdd();
  state->state = START;
  state->check = 1;
  state->input = Start;
  changeState(game);
  changeState(game);
  state->check = 1;
  state->input = Terminate;
  changeState(game);
  ck_assert_int_eq(STOP, state->state);
}
START_TEST(test22) {
  GameInfo_t *game = getGame();
  Add *state = getAdd();
  state->state = START;
  state->check = 1;
  state->input = Start;
  changeState(game);
  changeState(game);
  state->check = 1;
  state->input = Pause;
  changeState(game);
  ck_assert_int_eq(PAUSE, state->state);
  ck_assert_int_eq(1, game->pause);
}
START_TEST(test23) {
  GameInfo_t *game = getGame();
  Add *state = getAdd();
  state->state = PAUSE;
  state->check = 1;
  state->input = Pause;
  changeState(game);
  ck_assert_int_eq(MOVING, state->state);
  ck_assert_int_eq(1, game->pause);
  game->pause = 1;
  state->state = PAUSE;
  state->input = 1;
  state->input = Terminate;
  changeState(game);
  ck_assert_int_eq(PAUSE, state->state);
  ck_assert_int_eq(1, game->pause);
}

START_TEST(test24) {
  GameInfo_t *game = getGame();
  *game = updateCurrentState();
  ck_assert_ptr_nonnull(game);
}

START_TEST(test25) {
  Add *state = getAdd();
  GameInfo_t game_inf = {0};
  game_inf = updateCurrentState();
  ck_assert_ptr_nonnull(game_inf.field);
  ck_assert_ptr_nonnull(game_inf.next);
  state->check = 1;
  state->input = Start;
  game_inf = updateCurrentState();
  ck_assert_ptr_nonnull(game_inf.field);
  ck_assert_ptr_nonnull(game_inf.next);
}
START_TEST(test26) {
  Add *input = getAdd();
  userInput(Action, false);
  ck_assert_int_eq(Action, input->input);
  ck_assert_int_eq(1, input->check);
}
int main() {
  Suite *suite = suite_create("UNITS");
  TCase *tcase = tcase_create("TESTS");
  SRunner *srunner = srunner_create(suite);
  int val = 0;

  suite_add_tcase(suite, tcase);

  tcase_add_test(tcase, test1);
  tcase_add_test(tcase, test2);
  tcase_add_test(tcase, test3);
  tcase_add_test(tcase, test4);
  tcase_add_test(tcase, test5);
  tcase_add_test(tcase, test6);
  tcase_add_test(tcase, test7);
  tcase_add_test(tcase, test8);
  tcase_add_test(tcase, test9);
  tcase_add_test(tcase, test10);
  tcase_add_test(tcase, test11);
  tcase_add_test(tcase, test12);
  tcase_add_test(tcase, test13);
  tcase_add_test(tcase, test14);
  tcase_add_test(tcase, test15);
  tcase_add_test(tcase, test16);
  tcase_add_test(tcase, test17);
  tcase_add_test(tcase, test18);
  tcase_add_test(tcase, test19);
  tcase_add_test(tcase, test20);
  tcase_add_test(tcase, test21);
  tcase_add_test(tcase, test22);
  tcase_add_test(tcase, test23);
  tcase_add_test(tcase, test24);
  tcase_add_test(tcase, test25);
  tcase_add_test(tcase, test26);

  srunner_run_all(srunner, CK_NORMAL);
  val = srunner_ntests_failed(srunner);
  srunner_free(srunner);
  return val == 0 ? 0 : 1;
}

#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define figure_x 4
#define figure_y 4
#define field_x 10
#define field_y 20
#define ENTER 10
#define UP 65
#define DOWN 66
#define RIGHT 67
#define LEFT 68
#define SPACE 32
#define P 80
#define p 112
#define Q 81
#define q 113

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  BEFORE,
  START,
  MOVING,
  SHIFTING,
  ATTACHING,
  PAUSE,
  END,
  STOP
} GameState_t;

typedef struct {
  int gamealloc;
  int figurealloc;
  int input;
  int check;
  int end;
  int exit;
  time_t time;
  GameState_t state;
} Add;

typedef struct {
  int x;
  int y;
  int **figure;
  int **exfield;
  int **staticfield;
  int id;
  int nextid;
} Figure;

GameInfo_t initGame();
GameInfo_t *getGame();
UserAction_t *getAction();
Figure initFigure();
Figure *getFigure();
Add *getAdd();

void printGame(GameInfo_t *game);
void createField(int ***field, int height, int width);
void getNextFigure(GameInfo_t *game, int id);
int **copyFigure(int **field, int **figure);
int **placeFigure(int **field, int **figure, int y, int x);
void spawnFigure(GameInfo_t *game, Figure *figure);
void moveFigure(UserAction_t action);
void randomFigure(GameInfo_t *game, Figure *figure);
void printFigure(int **field);
void moveField(Figure *figure);
void copyField(GameInfo_t *game, Figure *figure);
void resetField(int **field);
int collision(Figure *figure);
int rightside_collision(Figure *figure);
int leftside_collision(Figure *figure);
int figure_left(Figure *figure);
int figure_right(Figure *figure);
int figure_bottom(Figure *figure);
void moveStatic(Figure *figure);
void rotateFigure(Figure *figure);
int rotate_collision(Figure *figure);

void changeState(GameInfo_t *game);
void prepare_game(GameInfo_t *game, Figure *figure, Add *input);
void spawn_game(GameInfo_t *game, Figure *figure, Add *input);
void pause_game(GameInfo_t *game, Add *input);
void moving_game(GameInfo_t *game, Add *input);
void shifting_game(Figure *figure, Add *state);
void attaching_game(GameInfo_t *game, Figure *figure, Add *state);
void end_game(Add *state);
void exit_game(GameInfo_t *game, Figure *figure, Add *state);

void inputCheck(int input);
void game_loop();

int readyShift(GameInfo_t *game, Add *add);

void restartGame(GameInfo_t *game, Figure *figure);

void deleteLine(Figure *figure, int i);
void cleanLines(GameInfo_t *game, Figure *figure);

void freeGame(GameInfo_t *game, Figure *figure);
void freeField(int ***field, int size);

int loadScore();

void downFigure(Figure *figure);

GameInfo_t updateCurrentState();
void userInput(UserAction_t action, bool hold);

void changeState(GameInfo_t *game);
void prepare_game(GameInfo_t *game, Figure *figure, Add *input);
void spawn_game(GameInfo_t *game, Figure *figure, Add *input);
void pause_game(GameInfo_t *game, Add *input);
void moving_game(GameInfo_t *game, Add *input);
void shifting_game(Figure *figure, Add *state);
void attaching_game(GameInfo_t *game, Figure *figure, Add *state);
void end_game(Add *state);
void exit_game(GameInfo_t *game, Figure *figure, Add *state);

void resetFigure(int **figure);

#endif
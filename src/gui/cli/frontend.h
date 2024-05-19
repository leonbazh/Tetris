#ifndef FRONTEND_H
#define FRONTEND_H

#include "../../brick_game/tetris/tetris.h"
#include <ncurses.h>

void printGame(GameInfo_t *game);
void printOverlay();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

#endif
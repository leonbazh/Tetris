#include "frontend.h"

void printGame(GameInfo_t *game) {
  for (int i = 0; i < field_y; i++) {
    for (int j = 0; j < field_x; j++) {
      if (game->field[i][j] == 1)
        mvprintw(i + 1, j * 2 + 1, "[]");
      else
        mvprintw(i + 1, j * 2 + 1, "  ");
    }
  }
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      if (game->next[i][j] == 1)
        mvprintw(4 + i + 1, 25 + j * 2 + 1, "[]");
      else
        mvprintw(4 + i + 1, 25 + j * 2 + 1, "  ");
    }
  }
  printOverlay();
}

void printOverlay() {
  GameInfo_t *game = getGame();
  print_rectangle(0, 21, 0, 21);
  print_rectangle(0, 21, 22, 42);
  mvprintw(1, 27, "GAME STATS");
  mvprintw(3, 24, "NEXT FIGURE:");
  mvprintw(10, 24, "HIGH SCORE: %d", game->high_score);
  mvprintw(12, 24, "SCORE: %d", game->score);
  mvprintw(14, 24, "SPEED: %d", game->speed);
  mvprintw(16, 24, "LEVEL: %d", game->level);
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++)
    mvaddch(top_y, i, ACS_HLINE);
  mvaddch(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }

  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++)
    mvaddch(bottom_y, i, ACS_HLINE);
  mvaddch(bottom_y, i, ACS_LRCORNER);
}
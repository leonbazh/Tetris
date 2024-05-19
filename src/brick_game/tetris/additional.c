#include "tetris.h"

void getNextFigure(GameInfo_t *game, int id) {
  int templates[7][4][4] = {
      {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
      {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
      {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
      {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      game->next[i][j] = templates[id][i][j];
    }
  }
}

Add *getAdd() {
  static Add add;
  return &add;
}

GameInfo_t *getGame() {
  static GameInfo_t game;
  Add *check = getAdd();
  if (!check->gamealloc) {
    game = initGame();
    check->gamealloc = 1;
  }
  return &game;
}

Figure *getFigure() {
  static Figure figure;
  Add *check = getAdd();
  if (!check->figurealloc) {
    figure = initFigure();
    check->figurealloc = 1;
  }
  return &figure;
}

void restartGame(GameInfo_t *game, Figure *figure) {
  resetField(game->field);
  resetField(figure->exfield);
  resetField(figure->staticfield);
  game->speed = 1;
  game->score = 0;
  game->high_score = 0;
  game->level = 1;
  game->pause = 0;
}

int loadScore() {
  FILE *fp = fopen("score.conf", "r");
  int score = 0;
  if (fp) {
    fscanf(fp, "%d", &score);
  }
  fclose(fp);
  return score;
}

void saveScore(GameInfo_t *game) {
  FILE *fp = fopen("score.conf", "w");
  if (fp) {
    fprintf(fp, "%d", game->high_score);
  }
  fclose(fp);
}

void cleanLines(GameInfo_t *game, Figure *figure) {
  int col = 0;
  int count = 0;
  for (int i = 20 - 1; i >= 0; i--) {
    for (int j = 0; j < 10; j++) {
      if (figure->staticfield[i][j])
        col++;
      if (col == 10) {
        deleteLine(figure, i);
        count++;
        i++;
      }
    }
    col = 0;
  }
  if (count) {
    if (count == 1)
      game->score += 100;
    if (count == 2)
      game->score += 300;
    if (count == 3)
      game->score += 700;
    if (count == 4)
      game->score += 1500;
  }
  if (game->score > game->high_score) {
    game->high_score = game->score;
    saveScore(game);
  }
  if (game->level < 11 && game->score > (game->level * 600)) {
    game->level++;
    game->speed++;
  }
}

void deleteLine(Figure *figure, int i) {
  for (int j = 0; j < 10; j++)
    figure->staticfield[i][j] = 0;

  for (int k = i; k > 0; k--)
    for (int j = 0; j < 10; j++)
      figure->staticfield[k][j] = figure->staticfield[k - 1][j];
}

int readyShift(GameInfo_t *game, Add *add) {
  time_t current_time = time(NULL);
  int flag = 0;
  double speed;
  double speeds[6] = {0.7, 0.6, 0.5, 0.4, 0.3, 0.2};
  if (game->speed == 1)
    speed = speeds[0];
  if (game->speed == 2)
    speed = speeds[1];
  if (game->speed == 3)
    speed = speeds[2];
  if (game->speed == 4)
    speed = speeds[3];
  if (game->speed == 5)
    speed = speeds[4];
  if (game->speed == 6)
    speed = speeds[5];
  if (add->time == 0 || difftime(current_time, add->time) >= speed) {
    add->time = current_time;
    flag = 1;
  }
  return flag;
}

GameInfo_t initGame() {
  GameInfo_t *game = (GameInfo_t *)malloc(sizeof(GameInfo_t));
  createField(&game->field, field_y, field_x);
  createField(&game->next, figure_y, figure_x);
  resetField(game->field);
  resetFigure(game->next);
  game->speed = 1;
  game->score = 0;
  game->high_score = loadScore();
  game->level = 1;
  game->pause = 0;
  return *game;
}

Figure initFigure() {
  Figure *figure = (Figure *)malloc(sizeof(Figure));
  createField(&figure->figure, figure_y, figure_x);
  createField(&figure->exfield, field_y, field_x);
  createField(&figure->staticfield, field_y, field_x);
  return *figure;
}

void createField(int ***field, int height, int width) {
  *field = (int **)malloc(height * sizeof(int *));
  for (int i = 0; i < height; i++) {
    (*field)[i] = (int *)malloc((width) * sizeof(int));
  }
}

int collision(Figure *figure) {
  int flag = 0;
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      if (figure->figure[i][j] == 1) {
        if (i + figure->y > 18 ||
            figure->staticfield[i + figure->y + 1][j + figure->x]) {
          flag = 1;
        }
      }
    }
  }
  return flag;
}

void moveStatic(Figure *figure) {
  for (int i = 0; i < field_y; i++) {
    for (int j = 0; j < field_x; j++) {
      if (figure->exfield[i][j] || figure->staticfield[i][j]) {
        figure->staticfield[i][j] = 1;
      }
    }
  }
  resetField(figure->exfield);
}

void copyField(GameInfo_t *game, Figure *figure) {
  if (game->field != NULL && figure->exfield != NULL) {
    for (int i = 0; i < field_y; i++) {
      for (int j = 0; j < field_x; j++) {
        game->field[i][j] = 0;
      }
    }
    for (int i = 0; i < field_y; i++) {
      for (int j = 0; j < field_x; j++) {
        if (figure->exfield[i][j] || figure->staticfield[i][j])
          game->field[i][j] = 1;
      }
    }
  }
}

void resetField(int **field) {
  for (int i = 0; i < field_y; i++) {
    for (int j = 0; j < field_x; j++) {
      field[i][j] = 0;
    }
  }
}

void resetFigure(int **figure) {
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      figure[i][j] = 0;
    }
  }
}

void randomFigure(GameInfo_t *game, Figure *figure) {
  figure->nextid = rand() % 7;
  getNextFigure(game, figure->nextid);
}

void spawnFigure(GameInfo_t *game, Figure *figure) {
  figure->x = 3;
  figure->y = 1;
  figure->figure = copyFigure(figure->figure, game->next);
  figure->exfield =
      placeFigure(figure->exfield, figure->figure, figure->y, figure->x);
}

int **copyFigure(int **field, int **figure) {
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      field[i][j] = figure[i][j];
    }
  }
  return field;
}
int **placeFigure(int **field, int **figure, int y, int x) {
  GameInfo_t *game = getGame();
  Add *check = getAdd();
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      if (figure[i][j] == 1)
        field[i + y][j + x] = figure[i][j];
      if (field[i + y][j + x] == 1 && game->field[i + y][j + x] == 1) {
        check->state = END;
      }
    }
  }
  return field;
}

void moveFigure(UserAction_t action) {
  Figure *figure = getFigure();
  if (action == Right) {
    if (!rightside_collision(figure)) {
      figure->x++;
    }
  } else if (action == Left) {
    if (!leftside_collision(figure)) {
      figure->x--;
    }
  } else if (action == Action) {
    rotateFigure(figure);
  } else if (action == Down) {
    downFigure(figure);
  }
  moveField(figure);
}

void downFigure(Figure *figure) {
  while (!collision(figure)) {
    figure->y++;
  }
}

void rotateFigure(Figure *figure) {
  int rotated_figure[4][4] = {0};
  int temp_figure[4][4] = {0};
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      temp_figure[i][j] = figure->figure[i][j];
    }
  }
  if (figure->id == 0 || figure->id == 1 || figure->id == 2) {
    for (int i = 0; i < figure_y; i++) {
      for (int j = 0; j < figure_x; j++) {
        rotated_figure[j][i] = figure->figure[i][j];
      }
    }
  } else {
    for (int i = 0; i < figure_y; i++) {
      for (int j = 0; j < figure_x; j++) {
        rotated_figure[j][figure_y - i - 1] = figure->figure[i][j];
      }
    }
  }

  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      figure->figure[i][j] = rotated_figure[i][j];
    }
  }
  if (rotate_collision(figure) || figure_left(figure) < 0 ||
      figure_right(figure) > 9) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        figure->figure[i][j] = temp_figure[i][j];
      }
    }
  }
}

int rotate_collision(Figure *figure) {
  int flag = 0;
  resetField(figure->exfield);
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      if (figure->figure[i][j] == 1)
        figure->exfield[i + figure->y][j + figure->x] = figure->figure[i][j];
    }
  }
  for (int i = 0; i < field_y; i++) {
    for (int j = 0; j < field_x; j++) {
      if (figure->exfield[i][j] == 1 && figure->staticfield[i][j] == 1) {
        flag = 1;
      }
    }
  }
  return flag;
}

int leftside_collision(Figure *figure) {
  int flag = 0;
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      if ((figure->figure[i][j] &&
           figure->staticfield[i + figure->y][j + figure->x - 1]) ||
          figure_left(figure) == 0) {
        flag = 1;
      }
    }
  }
  return flag;
}

int figure_left(Figure *figure) {
  int flag = 0, check = 0;
  for (int i = 0; i < figure_x && check == 0; i++) {
    for (int j = 0; j < figure_y && check == 0; j++) {
      if (figure->figure[j][i] == 1)
        flag = i, check = 1;
    }
  }
  flag += figure->x;
  return flag;
}

int rightside_collision(Figure *figure) {
  int flag = 0;
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      if ((figure->figure[i][j] &&
           figure->staticfield[i + figure->y][j + figure->x + 1]) ||
          figure_right(figure) == 9) {
        flag = 1;
      }
    }
  }
  return flag;
}

int figure_right(Figure *figure) {
  int flag = 0, check = 0;
  for (int i = 3; i >= 0 && check == 0; i--) {
    for (int j = 0; j < figure_y && check == 0; j++) {
      if (figure->figure[j][i] == 1)
        flag = i, check = 1;
    }
  }
  flag += figure->x;
  return flag;
}

void moveField(Figure *figure) {
  resetField(figure->exfield);
  for (int i = 0; i < figure_y; i++) {
    for (int j = 0; j < figure_x; j++) {
      if (figure->figure[i][j] == 1)
        figure->exfield[i + figure->y][j + figure->x] = figure->figure[i][j];
    }
  }
}

void freeGame(GameInfo_t *game, Figure *figure) {
  freeField(&game->field, 20);
  freeField(&game->next, 4);
  freeField(&figure->exfield, 20);
  freeField(&figure->staticfield, 20);
  freeField(&figure->figure, 4);
  game = NULL;
  figure = NULL;
  free(game);
  free(figure);
}

void freeField(int ***field, int size) {
  for (int i = 0; i < size; i++) {
    free((*field)[i]);
  }
  free(*field);
  *field = NULL;
}
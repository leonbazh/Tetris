#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define field_x 12;
#define field_y 22;

typedef enum {
    Start, //ENTER_KEY
    Pause, //P
    Terminate, //Q
    Left, //KEY_LEFT
    Right, //KEY_RIGHT
    Up, //KEY_UP
    Down, //KEY_DOWN
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

typedef struct{
    int x;
    int y;
    int size;
    Block *blocks;
}Figure;

typedef struct{
    int b;
}Block;

void userInput(UserAction_t action, bool hold); //hold - удержание клавиши

GameInfo_t updateCurrentState();

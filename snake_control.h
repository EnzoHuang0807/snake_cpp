#include <iostream>
#include <raylib.h>
#include <deque>
#include <vector>
#include <raymath.h>

enum Direction 
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

class Snake;
class Food;
class Game;

Direction setSnake1(
    int foodPos[][2], int foodSize, 
    int snake1[][2], int snakeSize1, int snakeDirection1[2],
    int snake2[][2], int snakeSize2, int snakeDirection2[2]);

Direction setSnake2(
    int foodPos[][2], int foodSize, 
    int snake1[][2], int snakeSize1, int snakeDirection1[2],
    int snake2[][2], int snakeSize2, int snakeDirection2[2]);


void processScript(Game& game, int snake);

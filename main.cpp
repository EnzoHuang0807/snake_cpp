#include <iostream>
#include <raylib.h>
#include <deque>
#include <vector>
#include <raymath.h>
#include <snake_control.h>

#define MAX 100 
using namespace std;

static bool allowMove1 = false, allowMove2 = false;
Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};
Color white = {255, 255, 255, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
public:
    deque<Vector2> body;
    deque<Vector2> initBody;
    Color snakeColor;

    bool addSegment = false;
    bool isDead = false;
    Vector2 direction = {1, 0};

    Snake(deque<Vector2> _body, Color _snakeColor)
    {
        body = _body;
        initBody = _body;
        snakeColor = _snakeColor;
    }

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
            DrawRectangleRounded(segment, 0.5, 6, snakeColor);
        }
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if (addSegment == true)
        {
            addSegment = false;
        }
        else
        {
            body.pop_back();
        }
    }

    void Reset()
    {
        body = initBody;
        direction = {1, 0};
        isDead = false;
    }
};

class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody1, deque<Vector2> snakeBody2)
    {
        Image image = LoadImage("Graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody1, snakeBody2);
    }

    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2> snakeBody1, deque<Vector2> snakeBody2)
    {
        Vector2 position = GenerateRandomCell();
        while (ElementInDeque(position, snakeBody1) || ElementInDeque(position, snakeBody2))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Game
{
public:
    Snake snake1 = Snake({Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}}, darkGreen);
    Snake snake2 = Snake({Vector2{6, 20}, Vector2{5, 20}, Vector2{4, 20}}, white);
    vector<Food> food;
    int foodSize = 4;
    
    bool running = true;
    int turn = 1;
    int score1 = 0;
    int score2 = 0;
    Sound eatSound;
    Sound wallSound;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");

        for(int i = 0; i < foodSize; i++){
            food.emplace_back(snake1.body, snake2.body);
        }
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw()
    {
        for (int i = 0; i < foodSize; i++)
            food[i].Draw();

        snake1.Draw();
        snake2.Draw();
    }

    void Update()
    {
        if (running && turn == 1){
            snake1.Update();
            CheckSnake(snake1, snake2);
        }
        if (running && turn == 2){
            snake2.Update();
            CheckSnake(snake2, snake1);
        }
    }

    void CheckSnake(Snake& snake, Snake& enemy)
    {
        CheckCollisionWithFood(snake);
        CheckCollisionWithEdges(snake);
        CheckCollisionWithTail(snake);
        CheckCollisionWithSnake(snake, enemy);
    }

    void CheckCollisionWithFood(Snake& snake)
    {
        for (int i = 0; i < foodSize; i++){
            if (Vector2Equals(snake.body[0], food[i].position))
            {
                food[i].position = food[i].GenerateRandomPos(snake1.body, snake2.body);
                snake.addSegment = true;
                PlaySound(eatSound);
            }
        }
    }

    void CheckCollisionWithEdges(Snake& snake)
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            snake.isDead = true;
            GameOver();
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            snake.isDead = true;
            GameOver();
        }
    }

    void CheckCollisionWithTail(Snake &snake)
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody))
        {
            snake.isDead = true;
            GameOver();
        }
    }

    void CheckCollisionWithSnake(Snake& snake, Snake& enemy)
    {

        if (ElementInDeque(snake.body[0], enemy.body))
        {
            snake.isDead = true;
            GameOver();
        }
    }

    void GameOver()
    {
        score1 = (snake1.isDead)? score1 : score1 + 1;
        score2 = (snake2.isDead)? score2 : score2 + 1;
        snake1.Reset();
        snake2.Reset();

        for (int i = 0; i < foodSize; i++)
            food[i].position = food[i].GenerateRandomPos(snake1.body, snake2.body);
        running = false;
        PlaySound(wallSound);
    }
};

int main()
{
    cout << "Starting the game..." << endl;
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        if (EventTriggered(0.2))
        {   

#ifdef SCRIPT_1
            if (allowMove1 == true){
                processScript(game, 1);
            }
#endif
            game.Update();
            game.turn = (game.turn == 1)? 2 : 1;

#ifdef SCRIPT_2
            if (allowMove2 == true){
                processScript(game, 2);
            }
#endif
            game.Update();
            game.turn = (game.turn == 1)? 2 : 1;

            allowMove1 = true;
            allowMove2 = true;
        }

#ifdef MANUAL_1

        if (IsKeyPressed(KEY_UP) && game.snake1.direction.y != 1 && allowMove1)
        {
            game.snake1.direction = {0, -1};
            game.running = true;
            allowMove1 = false;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake1.direction.y != -1 && allowMove1)
        {
            game.snake1.direction = {0, 1};
            game.running = true;
            allowMove1 = false;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake1.direction.x != 1 && allowMove1)
        {
            game.snake1.direction = {-1, 0};
            game.running = true;
            allowMove1 = false;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake1.direction.x != -1 && allowMove1)
        {
            game.snake1.direction = {1, 0};
            game.running = true;
            allowMove1 = false;
        }
#endif

#ifdef MANUAL_2

        if (IsKeyPressed(KEY_W) && game.snake2.direction.y != 1 && allowMove2)
        {
            game.snake2.direction = {0, -1};
            game.running = true;
            allowMove2 = false;
        }
        if (IsKeyPressed(KEY_S) && game.snake2.direction.y != -1 && allowMove2)
        {
            game.snake2.direction = {0, 1};
            game.running = true;
            allowMove2 = false;
        }
        if (IsKeyPressed(KEY_A) && game.snake2.direction.x != 1 && allowMove2)
        {
            game.snake2.direction = {-1, 0};
            game.running = true;
            allowMove2= false;
        }
        if (IsKeyPressed(KEY_D) && game.snake2.direction.x != -1 && allowMove2)
        {
            game.snake2.direction = {1, 0};
            game.running = true;
            allowMove2 = false;
        }
#endif

        // Drawing
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);
        DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
        DrawText(TextFormat("%i", game.score1), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
        DrawText(TextFormat("%i", game.score2), offset + cellSize * cellCount - 5, offset + cellSize * cellCount + 10, 40, white);
        game.Draw();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}


void processScript(Game& game, int snake)
{
    int foodSize = game.foodSize;

    //construvt food 
    int foodPos[MAX][2];
    for (int i = 0; i < foodSize; i++)
    {
        foodPos[i][0] = game.food[i].position.x;
        foodPos[i][1] = game.food[i].position.y;
    }

    //construct snake body 
    int snake1[MAX][2];
    int snake2[MAX][2];
    int snakeSize1 = game.snake1.body.size();
    int snakeSize2 = game.snake2.body.size();

    for (int i = 0; i < snakeSize1; i++)
    {
        snake1[i][0] = game.snake1.body[i].x;
        snake1[i][1] = game.snake1.body[i].y;
    }
    for (int i = 0; i < snakeSize2; i++)
    {
        snake2[i][0] = game.snake2.body[i].x;
        snake2[i][1] = game.snake2.body[i].y;
    }

    //construct snake direction
    int snakeDirection1[2] = {(int)game.snake1.direction.x, (int)game.snake1.direction.y};
    int snakeDirection2[2] = {(int)game.snake2.direction.x, (int)game.snake2.direction.y};

    Direction nextMove;
    if (snake == 1)
    {
        nextMove = setSnake1(
            foodPos, foodSize, 
            snake1, snakeSize1, snakeDirection1, 
            snake2, snakeSize2, snakeDirection2);

        if (nextMove == UP && game.snake1.direction.y != 1)
        {
            game.snake1.direction = {0, -1};
            game.running = true;
        }
        if (nextMove == DOWN && game.snake1.direction.y != -1)
        {
            game.snake1.direction = {0, 1};
            game.running = true;
        }
        if (nextMove == LEFT && game.snake1.direction.x != 1)
        {
            game.snake1.direction = {-1, 0};
            game.running = true;
        }
        if (nextMove == RIGHT && game.snake1.direction.x != -1)
        {
            game.snake1.direction = {1, 0};
            game.running = true;
        }
    }
    else
    {
        nextMove = setSnake2(
            foodPos, foodSize, 
            snake1, snakeSize1, snakeDirection1, 
            snake2, snakeSize2, snakeDirection2);
        
        if (nextMove == UP && game.snake1.direction.y != 1)
        {
            game.snake2.direction = {0, -1};
            game.running = true;
        }
        if (nextMove == DOWN && game.snake2.direction.y != -1)
        {
            game.snake2.direction = {0, 1};
            game.running = true;
        }
        if (nextMove == LEFT && game.snake2.direction.x != 1)
        {
            game.snake2.direction = {-1, 0};
            game.running = true;
        }
        if (nextMove == RIGHT && game.snake2.direction.x != -1)
        {
            game.snake2.direction = {1, 0};
            game.running = true;
        }
    }
}
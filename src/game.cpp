#include <iostream>
#include <graphics.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>

// W A S D
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

const int x_max = 500, y_max = 500;
const int max_length = (x_max - 10) * (y_max - 10); // max len of snake

static int food_x, food_y;
static std::vector<int> snake_x(max_length, 0), snake_y(max_length,0);
static int snake_len = 1;
static int direction_x = 0, direction_y = -10;
static int score = 0;

void gameOver()
{
    ;
    ;
}
void drawBorder(int x_max, int y_max)
{
    setcolor(GREEN);
    rectangle(0, 0, x_max, y_max);
    rectangle(10, 10, x_max - 10, y_max - 10);
    setfillstyle(SOLID_FILL, GREEN);
    floodfill(5, 5, GREEN);
}

boolean onFood()
{
    return snake_x[0] == food_x && snake_y[0] == food_y;
}

boolean inPlayArea() {
    int snake_head_x = snake_x.front();
    int snake_head_y = snake_y.front();

    if (snake_head_x >= x_max-10 or snake_head_x < 10 or snake_head_y >= y_max-10 or snake_head_y < 10) {
        return false;
    }
    for (int i = 1; i < snake_len; ++i) {
        if ((snake_head_x == snake_x[i]) && (snake_head_y == snake_y[i]))
        {
            return false;
        }
    }
    return true;
}

void makeFood()
{
    bool on_snake = false;
    do
    {
        food_x = (1 + rand() % (x_max - 10));
        food_y = (1 + rand() % (y_max - 10));

        // make sure the food is on same grid as the snake
        food_x = food_x / 10;
        food_x = food_x * 10;
        if (food_x == 0)
            food_x += 10;
        if (food_x == x_max - 10)
            food_x -= 10;

        food_y = food_y / 10;
        food_y = food_y * 10;
        if (food_y == 0)
            food_y += 10;
        if (food_y == y_max - 10)
            food_y -= 10;

        // Check if the food is on the snake's body
        for (int i = 0; i < snake_len; i++)
        {
            if (snake_x[i] == food_x && snake_y[i] == food_y)
            {
                on_snake = true;
                break;
            }
        }
    } while (on_snake);
    setfillstyle(SOLID_FILL, YELLOW);
    bar(food_x, food_y, food_x + 10, food_y + 10); // draw the food
}


void drawSnake()
{
    setfillstyle(SOLID_FILL, WHITE);
    for (int i = 0; i < snake_len; i++)
    {
        bar(snake_x[i], snake_y[i], snake_x[i] + 10, snake_y[i] + 10);
    }
}

void updateSnake()
{
    for (int i = snake_len - 1; i > 0; i--)
    {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }
}

int main()
{
    snake_x.front() = 200;
    snake_y.front() = 200;
    initwindow(x_max, y_max);

    makeFood();

    srand(time(0)); // Seed the random number generator with the current time

    drawBorder(x_max, y_max);

    int frame_time = 100;

    for (;;)
    {
        if (!inPlayArea()) {
            gameOver();
            closegraph();
            return 0;
        } else if (onFood()) {
            snake_len++;
            score++;
            std::cout << "Score : " << score << std::endl;
            makeFood();
        } else {
            setfillstyle(SOLID_FILL, BLACK);
            int snake_end_x = snake_x[snake_len-1];
            int snake_end_y = snake_y[snake_len-1];
            bar(snake_end_x, snake_end_y, snake_end_x+10, snake_end_y+10);
        }

        // movement logic
        if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_W))
        {
            if (!(direction_x == 0 and direction_y == 10))
                direction_x = 0, direction_y = -10;
        }
        else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_D))
        {
            if (!(direction_x == -10 and direction_y == 0))
                direction_x = 10, direction_y = 0;
        }
        else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(VK_S))
        {
            if (!(direction_x == 0 and direction_y == -10))
                direction_x = 0, direction_y = 10;
        }
        else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_A))
        {
            if (!(direction_x == 10 and direction_y == 0))
                direction_x = -10, direction_y = 0;
        }
        updateSnake();
        snake_x[0] += direction_x;
        snake_y[0] += direction_y;

        drawSnake();

        delay(frame_time);
    }

    getch();
    closegraph();
}

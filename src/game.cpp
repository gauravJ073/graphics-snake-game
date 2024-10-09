#include <iostream>
#include <graphics.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <utility>

// W A S D
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

// Menu
using MenuOption = std::pair<std::string, void (*)()>;

// Window Size
const int x_max = 500;
const int y_max = 500;

// Gameplay
static int food_x, food_y; // Current food item coordinates
static int score = 0;

// Snake
const int max_length = (x_max - 10) * (y_max - 10); // max len of snake
static std::vector<int> snake_x(max_length, 0);
static std::vector<int> snake_y(max_length, 0);
static int snake_len = 1;

// ------ Function Prototypes ------
// Game State
void resetSnake();

// Utility
bool onFood();
bool inPlayArea(int item_x, int item_y);
void drawButton(int menu_x, int menu_y, std::string text, bool focus);
void drawMenu(int menu_x, int menu_y, int menu_size, std::vector<MenuOption> options, int selected = 0);

// Gameplay
void makeFood();
void drawSnake();
void updateSnake();

// User Interaction
void mainMenu();
void gameOver();
void game();
void exitGame();
// --------- xxx ---------

void resetSnake()
{
    snake_x.front() = 200;
    snake_y.front() = 200;
    snake_len = 1;
}

void drawButton(int x, int y, std::string text, bool focus)
{
    if (focus)
    {
        setcolor(BLACK);
        setbkcolor(WHITE);
        outtextxy(x, y, (char *)text.c_str());
    }
    else
    {
        setcolor(WHITE);
        setbkcolor(DARKGRAY);
          outtextxy(x, y, (char *)text.c_str());
    }

    setcolor(WHITE);
    setbkcolor(BLACK);
}

void drawMenu(int x, int y, int menu_size, std::vector<std::string> headers, std::vector<MenuOption> options, int selected = 0)
{
    // Gives time if user reaches menu from another Keypress.
    // If not given then the keypress (mostly ENTER) from previous menu is registered.
    delay(200);


    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(x, y, x + menu_size, y + menu_size);

    // Draw Headers
    setcolor(BLACK);
    setbkcolor(LIGHTGRAY);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    int title_y = 150;
    for (auto text : headers) {
        int title_x = x + (menu_size/2) - (textwidth((char*)text.c_str())/2);
        outtextxy(title_x, title_y, (char *)text.c_str());
        title_y += 40;
    }

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    int x_offset = x + (menu_size/2) - (textwidth((char*)options[selected].first.c_str())/2);
    int y_offset = title_y;
    int button_height = 40;

    // Initial Menu Options Drawn
    for (int i = 0; i < options.size(); ++i)
    {
        int button_y = y_offset + (i * button_height); // calculating Y-coordinate for current option
        x_offset = x + (menu_size/2) - (textwidth((char*)options[i].first.c_str())/2);
        drawButton(x_offset, button_y, options[i].first, selected == i);
    }

    // Menu Loop
    for (;;)
    {
        // Change Selection on UP or 'W' Keypress
        if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_W))
        {
            // Painting the previous button as "Normal"
            int button_y = y_offset + (selected * button_height);
            x_offset = x + (menu_size/2) - (textwidth((char*)options[selected].first.c_str())/2);
            drawButton(x_offset, button_y, options[selected].first, false);

            // reducing selection and cycling selection value.
            if (--selected < 0)
                selected = options.size() - 1;

            // Painting the new selection as "Active"
            button_y = y_offset + (selected * button_height);
            x_offset = x + (menu_size/2) - (textwidth((char*)options[selected].first.c_str())/2);
            drawButton(x_offset, button_y, options[selected].first, true);
        }

        // Change Selection on DOWN or 'S' Keypress
        else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(VK_S))
        {
            // Painting the previous button as "Normal"

            int button_y = y_offset + (selected * button_height);
            x_offset = x + (menu_size/2) - (textwidth((char*)options[selected].first.c_str())/2);
            drawButton(x_offset, button_y, options[selected].first, false);

            // increasing selection value and cycling selection value.
            if (++selected >= options.size())
                selected = 0;

            // Painting the new selection as "Active"
            button_y = y_offset + (selected * button_height);
            x_offset = x + (menu_size/2) - (textwidth((char*)options[selected].first.c_str())/2);
            drawButton(x_offset, button_y, options[selected].first, true);
        }

        // Execution function associated with current selection on ENTER Keypress
        else if (GetAsyncKeyState(VK_RETURN))
        {
            (options[selected].second)();
        }

        delay(100); // this gives time betwen each keypress register
    }
}

void drawBorder(int x_max, int y_max)
{
    setfillstyle(SOLID_FILL, GREEN);
    bar(0, 0, 10, y_max);
    bar(0, 0, x_max, 10);
    bar(0, y_max, x_max, y_max - 10);
    bar(x_max, 0, x_max - 10, y_max);
}

bool onFood()
{
    return snake_x[0] == food_x && snake_y[0] == food_y;
}

// TODO : Use it while making food item, maybe.
bool inPlayArea(int x, int y)
{
    // Play Area includes all points inside `Boundary` and not on `Snake`
    // Checking if point is inside boundary or not.
    if (x >= x_max - 10 or x < 10 or y >= y_max - 10 or y < 10)
    {
        return false;
    }

    // Checking if point is on snake body.
    for (int i = 1; i < snake_len; ++i)
    {
        if ((x == snake_x[i]) && (y == snake_y[i]))
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
        on_snake = false;
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
    // Drawing only the new head since the previous body points have already been painted. So no need to redraw them.
    bar(snake_x.front(), snake_y.front(), snake_x.front() + 10, snake_y.front() + 10);
}

void updateSnake()
{
    for (int i = snake_len - 1; i > 0; i--)
    {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }
}

void exitGame()
{
    closegraph();
    exit(0);
}

void mainMenu()
{
    std::vector<MenuOption> main_menu_options;

    main_menu_options.push_back(MenuOption("START GAME", *game));
    main_menu_options.push_back(MenuOption("EXIT GAME", *exitGame));

    drawMenu(80, 100, 340, {"Snake Game", "Main Menu"}, main_menu_options);
}

void gameOver()
{
    std::vector<MenuOption> game_over_options;

    game_over_options.push_back(MenuOption("MAIN MENU", *mainMenu));
    game_over_options.push_back(MenuOption("EXIT GAME", *exitGame));
    std::string score_string = "Your Score : " + std::to_string(score);
    drawMenu(80, 100, 340, {"Game Over", score_string}, game_over_options);
}

void game()
{
    static int direction_x = 0, direction_y = -10; // Direction of movement (Default : UP)
    int frame_time = 100;

    cleardevice();
    resetSnake();

    drawBorder(x_max, y_max);

    srand(time(0)); // Seed the random number generator with the current time
    makeFood();

    for (;;)
    {
        if (!inPlayArea(snake_x[0], snake_y[0])) // Exit if Snake Head is outside Play Area.
        {
            gameOver();
            // return score;
        }
        else if (onFood()) // If Snake Head is on food, increase lenght and score and generate a new food item.
        {
            snake_len++;
            score++;
            makeFood();
            if (score % 2 == 0)
                frame_time -= 1;
        }
        else // If it is a normal movement with no event, repaint the tail as black to hide it.
        {
            setfillstyle(SOLID_FILL, BLACK);
            int snake_end_x = snake_x[snake_len - 1];
            int snake_end_y = snake_y[snake_len - 1];
            bar(snake_end_x, snake_end_y, snake_end_x + 10, snake_end_y + 10);
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

        // Shift all body points to right by 1.
        updateSnake();

        // Set new head location.
        snake_x[0] += direction_x;
        snake_y[0] += direction_y;

        drawSnake();
        delay(frame_time);
    }
}

int main()
{
    initwindow(x_max, y_max);
    mainMenu();
    getch();
    return 0;
}

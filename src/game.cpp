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
// const int max_length = (x_max - 10) * (y_max - 10); // max len of snake
const int max_length = 1000;

static int food_x, food_y;
static std::vector<int> snake_x(max_length, 0), snake_y(max_length, 0);
static int snake_len = 1;
static int direction_x = 0, direction_y = -10;
static int score = 0;

void gameOver()
{
    /*
    bar- area
    display : GAME  OVER
    a white button to move up and down to select:-
    main menu -if selected set score =0, exit game- close graph
    */
    int menu_size=340, menu_x = 80, menu_y=100;
    int gameover_size=4;
    int menu_selection=0;//0=mainmenu, 1=exitgame

    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(menu_x, menu_y, menu_x+menu_size, menu_y+menu_size);

    //display gameover
    setcolor(WHITE);
    setbkcolor(DARKGRAY);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, gameover_size);
    int gameover_x=x_max/2-(gameover_size*8*10/2), gameover_y=150;
    outtextxy(gameover_x, gameover_y, "GAME  OVER");

    //menu selection
    int end_menu_size=3;

    int const end_menu_items=2;
    char* end_menu[end_menu_items];
    end_menu[0]="Main  Menu";
    end_menu[1]="Exit  Game";

    int selection=0;
    int end_menu_x=x_max/2-(end_menu_size*8*10/2), end_menu_y=300;
    printf("%d", end_menu_x);
    
    for(;;){
        if(GetAsyncKeyState(VK_UP)){
            if(selection==1) selection--;
        }
        else if(GetAsyncKeyState(VK_DOWN)){
            if(selection==0)selection++;
        }
        else if(GetAsyncKeyState(VK_RETURN)){
            if(selection==0){
                //mainmenu();
            }
            else if(selection==1){
                closegraph();
                exit(0);
            }
        }
        if(selection==0){
            setcolor(BLACK);
            setbkcolor(WHITE);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, end_menu_size);
            outtextxy(end_menu_x+10, end_menu_y, end_menu[0]);
            setcolor(WHITE);
            setbkcolor(DARKGRAY);
            outtextxy(end_menu_x+10, end_menu_y+20, end_menu[1]);
        }
        else{
            setcolor(WHITE);
            setbkcolor(DARKGRAY); 
            settextstyle(DEFAULT_FONT, HORIZ_DIR, end_menu_size);
            outtextxy(end_menu_x+10, end_menu_y, end_menu[0]);
            setcolor(BLACK);
            setbkcolor(WHITE);
            outtextxy(end_menu_x+10, end_menu_y+20, end_menu[1]);
        }
        delay(100);
        
    }
}

void drawBorder(int x_max, int y_max)
{
    setfillstyle(SOLID_FILL, GREEN);
    bar (0, 0, 10, y_max);
    bar (0, 0, x_max, 10);
    bar (0, y_max, x_max, y_max-10);
    bar (x_max, 0, x_max-10, y_max);
}

boolean onFood()
{
    return snake_x[0] == food_x && snake_y[0] == food_y;
}

// TODO : Use it while making food item, maybe.
boolean inPlayArea(int x, int y)
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
    bar(snake_x.front(), snake_y.front(), snake_x.front()+10, snake_y.front()+10);
}

void updateSnake()
{
    for (int i = snake_len - 1; i > 0; i--)
    {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }
}

int game(){
    makeFood();

    srand(time(0)); // Seed the random number generator with the current time

    drawBorder(x_max, y_max);

    int frame_time = 100;

    for (;;)
    {
        if (!inPlayArea(snake_x[0], snake_y[0])) // Exit if Snake Head is outside Play Area.
        {
            gameOver();
            return score;
        }
        else if (onFood()) // If Snake Head is on food, increase lenght and score and generate a new food item.
        {
            snake_len++;
            score++;
            makeFood();
            if(score%2==0) frame_time-=1;
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
    snake_x.front() = 200;
    snake_y.front() = 200;
    initwindow(x_max, y_max);
    int sc=game();
    
    getch();
    closegraph();
    return sc;
    
}

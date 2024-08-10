#include <graphics.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>

const int x_max=500,y_max=500;
const int max_length=(x_max-10)*(y_max-10);//max len of snake

//drawing green border to set the play area
void drawBorder(int x_max, int y_max){
    setcolor(GREEN);
    rectangle(0, 0, x_max, y_max);
    rectangle(10, 10, x_max-10, y_max-10);
    setfillstyle(SOLID_FILL, GREEN);
    floodfill(5, 5, GREEN);
}

boolean foodTaken(int foodx, int foody, int snakex, int snakey){
    return snakex==foodx && snakey==foody;
}

void eatFood(int foodx, int foody, int &food_no, int &snake_len){
    setfillstyle(SOLID_FILL, BLACK);
    bar(foodx, foody, foodx+10, foody+10);
    food_no+=1;
    snake_len+=1;
    //implement end game logic if snake_len>=max_length
}

void makeFood(int &foodx, int &foody){
    do{
	    foodx = (rand()%(x_max-10));
	    foody = (rand()%(y_max-10));
    }while(getpixel(foodx,foody)!=0 && foodx > 10 && foody > 10);

    //make sure the food is on same grid as the snake
    foodx=foodx/10;
    foodx=foodx*10;
    if(foodx==0)foodx+=10;
    if(foodx==x_max-10)foodx-=10;

    foody=foody/10;
    foody=foody*10;
    if(foody==0)foody+=10;
    if(foody==y_max-10)foody-=10;
}

void changeDirection(int const direction_change, int &snake_direction, std::vector<int> &snake_x, std::vector<int> &snake_y){
    switch(direction_change){
        case 0://no change
            if(snake_direction==1){snake_y[0]=snake_y[0]-10;}
            else if(snake_direction==2){snake_x[0]=snake_x[0]+10;}
            else if(snake_direction==3){snake_y[0]=snake_y[0]+10;}
            else if(snake_direction==4) {snake_x[0]=snake_x[0]-10;}
            break;
        case 1://change to up
            if(snake_direction!=3){//making sure current direction is not down
                snake_y[0]=snake_y[0]-10;
                snake_direction=1;
            }
            break;
        case 2://change to right
            if(snake_direction!=4){//making sure current direction is not left
                snake_x[0]=snake_x[0]+10;
                snake_direction=2;
            }
            break;
        case 3://change to down
            if(snake_direction!=1){//making sure current direction is not up
                snake_y[0]=snake_y[0]+10;
                snake_direction=3;
            }
            break;
        case 4://change to down
            if(snake_direction!=2){//making sure current direction is not right
                snake_x[0]=snake_x[0]-10;
                snake_direction=4;
            }
            break;
    }
}
void drawSnake(std::vector<int> &snake_x, std::vector<int> &snake_y, int const snake_len){
    for(int i =0 ; i<snake_len;i++){
        bar(snake_x[i],snake_y[i],snake_x[i]+10,snake_y[i]+10);
    }

}
void updateSnake(std::vector<int> &snake_x, std::vector<int> &snake_y, int const snake_len){
    for(int i= snake_len-1; i >0;i--){
        snake_x[i] = snake_x[i-1];
        snake_y[i] = snake_y[i -1];
    }
}
int main(){
    srand(time(0)); // Seed the random number generator with the current time
    // int gm, gd=DETECT;
    // char *data="";
    // initgraph(&gm, &gd, data);
    
    initwindow(x_max, y_max);
    
    drawBorder(x_max, y_max);


    //SNAKE
    std::vector<int> snake_x(max_length, 0), snake_y(max_length, 0);//to store the coordinates of the snake
    int snake_len=1;//to store the lenght of snake

    //FOOD
    int foodx=x_max/2, foody=y_max/2;//food coordinates
    int food_no=1;//food count
    
    //direction
    int snake_direction=1;
    /*
    UP=1
    RIGHT=2
    DOWN=3
    LEFT=4
    */
    snake_x[0]=100;
    snake_y[0]=100;
    for(;;){
        cleardevice();
        // drawBorder(x_max, y_max);
        //food logic
        if(foodTaken(foodx, foody, snake_x[0], snake_y[0])){
            eatFood(foodx, foody, food_no, snake_len);
            makeFood(foodx, foody);
            setfillstyle(SOLID_FILL, GREEN);
        }
        bar(foodx, foody, foodx+10, foody+10); //draw the food

        //movement logic
        int direction_change=0;
        /*
        NO CHANGE = 0
        UP=1
        RIGHT=2
        DOWN=3
        LEFT=4
        */
        if (GetAsyncKeyState(VK_UP) & 0x8000) direction_change = 1;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) direction_change = 2;
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) direction_change = 3;
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) direction_change = 4;
        
        changeDirection(direction_change, snake_direction, snake_x, snake_y);
        drawSnake(snake_x, snake_y, snake_len);
        updateSnake(snake_x, snake_y, snake_len);
        delay(100);
    }
    getch();
    closegraph();
}

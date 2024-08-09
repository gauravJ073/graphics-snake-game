#include <graphics.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

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

boolean food_taken(int foodx, int foody, int snakex, int snakey){
    return snakex==foodx && snakey==foody;
}

void eat_food(int foodx, int foody, int &food_no, int &snake_len){
    setfillstyle(SOLID_FILL, BLACK);
    bar(foodx, foody, foodx+10, foody+10);
    food_no+=1;
    snake_len+=1;
}

void make_food(int &foodx, int &foody){
    do{
	    foodx = (1+rand()%(x_max-10));
	    foody = (1+rand()%(y_max-10));
    }while(getpixel(foodx,foody)!=0 && foodx > 10 && foody > 10);

    //make sure the food is on same grid as the snake
    foodx=foodx/10;
    foodx=foodx*10;

    foody=foody/10;
    foody=foody*10;
}

int main(){
    
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
    int direction=1;
    /*
    UP=1
    RIGHT=2
    DOWN=3
    LEFT=4
    */

    for(;;){
        //food logic
        if(food_taken(foodx, foody, snake_x[0], snake_y[0])){
            eat_food(foodx, foody, food_no, snake_len);
            make_food(foodx, foody);
            setfillstyle(SOLID_FILL, GREEN);
        }
        bar(foodx, foody, foodx+10, foody+10);
    }
    getch();
    closegraph();
}

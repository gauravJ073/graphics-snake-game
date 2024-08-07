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
    rectangle(10-1, 10-1, x_max-10+1, y_max-10+1);
    setfillstyle(SOLID_FILL, GREEN);
    floodfill(5, 5, GREEN);
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

    getch();
    closegraph();
}

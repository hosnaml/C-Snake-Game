/*
    This file contains original code written by Hosna Molavi and Weronika Danilczuk
    for the project in IS1200.
 
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic32mx.h>

#include "gamefunctions.h"

/* Global variable to store current status of the buttons */
int BUTTONS;

/* Array to stora all x-coordinates of the apples */
int apple_xpos[] = {
    87,
    12,
    32,
    100,
    47,
    110,
    38,
    75,
    27,
    10,
    76,
    98
};

/* Array to stora all y-coordinates of the apples */
int apple_ypos[] = {
    10,
    15,
    20,
    23,
    13,
    22,
    5,
    17,
    8,
    19,
    30,
    6
};

/* This was based on code from lab 3 */
/* This function returns the current status of all buttons 1-4 */
int getbtns(void){
    BUTTONS = (PORTD >> 5) & 0x07;                                // Mask buttons 2-4
    BUTTONS = (BUTTONS << 1 );                                    // Shift one step to the left
    BUTTONS = BUTTONS | ((PORTF >> 1) & 0x01);                    // "add" result from btn1 (stored in RF1)
    return BUTTONS;
}

/* Function to generate random apples - did not work */
/*
void showapple(){
    // We used the rand function to generate a random coordination adress for the apple.
    // We check if the apple is not on the snake. before displaying it.
    int i;
    for( i = 0; i < snakelength; i++){
        if(apple.x == snake.x[i]){
            apple.x = rand()%128;
            i = 0;
        }
        if(apple.y == snake.y[i]){
            apple.y = rand()%32;
            i = 0;
        }
    }
    humanarray[apple.x][apple.y] = 1;
}
 */


/* This function cheks game conditions and updates the snake movements */
void snakegame(){
    movingsnake();                  // Set the current direction of the snake/update coordinates
    
    if(found_apple()){
        eatingapple();
    }
    
    if(hit_border()){
        gameover();
    }
    
    if(hit_self()){
        gameover();
    }
    
    showingsnake();
    
    update_apple();
    delay(100000);                  // Delay snake movements on the display
}


/* Function to check the condition if the snake found an apple */
int found_apple(){
    if(snake.x[0] == apple_xpos[score] && snake.y[0] == apple_ypos[score]){
        return 1;
    } else {
        return 0;
    }
}

/* This function checks the condition if the snake hit a display border */
int hit_border(){
    if(snake.x[0] == 128 || snake.x[0] == 0 || snake.y[0] == 32 || snake.y[0] == 0) {
        return 1;
    } else {
        return 0;
    }
}

/* This function checks the condition if the snake hit itself */
int hit_self(){
    int i;
    for( i = 1 ; i < snake.length - 1 ; i++){
        if(snake.x[0] == snake.x[i]){
            if (snake.y[0] == snake.y[i]){
                return 1;
            }
        }
    }
    return 0;
}


/* This function performs the routine when the game is over */
void gameover(){
    char score_str[] = "";
    score_str[0] = score + '0';             //Make an int to a string;
    clear_display();
    
    while(1){                               //Infinite loop - stay here until new game starts
        display_string(0,"GAME OVER");
        display_string(1,"SCORE:");
        display_string(2,score_str);
        display_update();
        mapping();
        display_image(0, computerarray);
    }
}

/* This function updates the new position of the snake */
void showingsnake(){
    int i;
    for(i = 0; i < snake.length; i++){
        humanarray[snake.y[i]][snake.x[i]] = 1;
    }
}

/* Function to change / set current direction of the snake */
void movingsnake(){
    int n;
    for(n = snake.length - 1; n > 0; n--){
        snake.x[n] = snake.x[n-1];
        snake.y[n] = snake.y[n-1];
    }
    
    if(snake.direction == 'r' ){
        snake.x[0]++;
    }
    if(snake.direction == 'l' ){
        snake.x[0]--;
    }
    if(snake.direction == 'd' ){
        snake.y[0]++;
    }
    if(snake.direction == 'u' ){
        snake.y[0]--;
    }
    changedirection();
}

/* This function checks inputs from buttons and changes the direction of the snake */
void changedirection(){
    BUTTONS = getbtns();
    
    /* Set new direction + check so that it's not 180 degrees from current direction*/
    if(snake.direction != 'r'){
        if(BUTTONS & 0x8){
            snake.direction = 'l';
        }
    }
    if(snake.direction != 'l'){
        if(BUTTONS & 0x1){
            snake.direction = 'r';
        }
    }
    if(snake.direction != 'd'){
        if(BUTTONS & 0x4){
            snake.direction = 'u';
        }
    }
    if(snake.direction != 'u'){
        if(BUTTONS & 0x2){
            snake.direction = 'd';
        }
    }
}


/* This function updates apple position. */
void update_apple(){
        int xpos = apple_xpos[score];
        int ypos = apple_ypos[score];
        humanarray[ypos][xpos] = 1;
}

/* This function updates information when snake eats an apple */
void eatingapple(){
        score++;
        snake.length++;
}

/* This function initializes the snake for a new game */
void snake_init(){
    /* Initialize snake before game starts */
    
    snake.y = snake_ypos;                   // Set y position to point to first element in y-coord array
    snake.x = snake_xpos;                   // Set x position to point to first element in x-coord array
    
    snake.length = 8;                       //Starting length of snake set to 8
    snake.direction = 'r';                  //At start, snake will move to the right
    score = 0;                              //Setting score to 0
    
    /* Set starting position for the snake*/
    int start_row = 10;
    int head_position = 20;
    int i;
    
    for(i=0; i<snake.length;i++){
        snake.x[i] = head_position-i;
        snake.y[i] = start_row;
    }
}




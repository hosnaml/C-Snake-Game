/*
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson
   Latest update 2015-08-28 by F Lundevall
   For copyright and licensing, see file COPYING
 
    Some parts are original code written by Hosna Molavi and Weronika Danilczuk.
 
 */

#include <stdint.h>

/* Declare display-related functions from mipslabfunc.c */
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from mipslabfunc.c */
char * itoaconv( int num );
void labwork(void);
int nextprime( int inval );
void quicksleep(int cyc);
void tick( unsigned int * timep );

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr );

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */
/* Written as part of asm lab: delay, time2string */
void delay(int);
void time2string( char *, int );
/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */
int getbtns(void);
int getsw(void);
void enable_interrupt(void);





/*    The following part was compeletly written by Hosna Molavi and Weronika Danilczuk    */

/* Object snake contains infotmation about the snake. */
typedef struct Snake{
    uint8_t *y;
    uint8_t *x;
    int length;
    char direction;
} Snake;

/* Create instance of the object */
Snake snake;


/*              DATA              */

uint8_t humanarray[32][128];
uint8_t computerarray[512];

uint8_t snake_xpos[200];
uint8_t snake_ypos[200];

int score;                             


/*             FUNCTIONS             */

/* This function returns the current status of all buttons 1-4*/
int getbtns(void);

/* This function contains checks game conditions and updates the snake movements */
void snakegame();

/* This function updates the new position of the snake */
void showingsnake();

/* This function performs the routine when the game is over */
void gameover();

/* Function to change / set current direction of the snake */
void movingsnake();

/* This function checks inputs from buttons and changes the direction of the snake */
void changedirection();

/* This function updates information when snake eats an apple */
void eatingapple();

/* This function updates apple position on display */
void update_apple();

/* This function initializes the snake for a new game */
void snake_init();

/* Function to check the condition if the snake found an apple */
int found_apple();

/* This function checks the condition if the snake hit a display border */
int hit_border();

/* This function checks the condition if the snake hit itself */
int hit_self();









/*
    This file contains functions created for lab3 in IS1200.
 
    Some parts are original code written by Hosna Molavi and Weronika Danilczuk.
 */

#include <pic32mx.h>
#include <stdint.h>

/* Authors: Hosna Molavi and Weronika Danilczuk */
/* Including header file for display functions and game funcitons*/
#include "gamefunctions.h"


/*    INITIALIZATION    */
   
/* Code from lab */
#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9


#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

/* Authors: Hosna Molavi and Weronika Danilczuk */
/* Defenition of function data */
uint8_t humanarray[32][128];
uint8_t computerarray[512];
char textbuffer[4][16];



/* Code from lab */
void display_init() {
	DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
	delay(10);
	DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
	delay(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
	delay(10);
	DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
	delay(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
	delay(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

/* Code from lab */
void delay(int cyc) {
    int i;
    for(i = cyc; i > 0; i--);
}

/* Code from lab */
uint8_t spi_send_recv(uint8_t data) {
    while(!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while(!(SPI2STAT & 0x01));
    return SPI2BUF;
}

/* Code from lab */
void display_string(int line, char *s) {
    int i;
    if(line < 0 || line >= 4)
        return;
    if(!s)
        return;
    
    for(i = 0; i < 16; i++){
        if(*s) {
            textbuffer[line][i] = *s;
            s++;
        } else
            textbuffer[line][i] = ' ';
    }
}

/* Code based on lab 3 function with an alternation on the limit for the final for loop */
/* value 32 was changed to 128 */
void display_image(int x, const uint8_t *data) {
    int i, j;
    
    for(i = 0; i < 4; i++) {    //Iterate through each page
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        spi_send_recv(i);
        
        spi_send_recv(x & 0xF);
        spi_send_recv(0x10 | ((x >> 4) & 0xF));
        
        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
        
        for(j = 0; j < 128; j++) //Iterate through each column on current page
            spi_send_recv(data[i*128 + j]);
    }
}

/* Code from lab */
void display_update() {
    int i, j, k;
    int c;
    for(i = 0; i < 4; i++) {
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        spi_send_recv(i);
        
        spi_send_recv(0x0);
        spi_send_recv(0x10);
        
        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;
        
        for(j = 0; j < 16; j++) {
            c = textbuffer[i][j];
            if(c & 0x80)
                continue;
            
            for(k = 0; k < 8; k++)
                spi_send_recv(font[c*8 + k]);
        }
    }
}



/* The following functions are original code written by Hosna Molavi and Weronika Danilczuk */

/* Function to set all pixels to 0 */
void clear_display(){
    //int score = 0;
    int allrows, allcolumns, i;
    
    for(allrows = 0; allrows < 32; allrows++){
        for(allcolumns = 0 ; allcolumns < 128; allcolumns++){
            humanarray[allrows][allcolumns] = 0;
        }
    }
    
    for(i = 0; i < 512 ; i++){
        computerarray[i] = 0;
    }
}

/* Function to transform data from human-readable array to computer readable array.
 To transfer data over the SPI */
void mapping(){
    int page, column, row;
    uint8_t binaryposition;
    uint8_t pixelnumber;
    
    for(page=0; page<4; page++){
        
        for(column=0; column < 128 ; column++){
            
            pixelnumber = 0;
            binaryposition=1;
            
            for(row=0 ; row < 8 ; row++){
                
                if(humanarray[8*page + row][column]){
                    pixelnumber |= binaryposition;
                }
                binaryposition <<= 1;
            }
            computerarray[column + page*128] = pixelnumber;
        }
    }
}

/* This function is called repeadetly from the main program */
void display(){
    clear_display();
    snakegame();
    mapping();
    display_image(0,computerarray);
}







  

/*
    This file contains code from the file mipslabmain.c from Lab3 in IS1200.
 
    This file written 2015 by Axel Isaksson,
    modified 2015, 2017 by F Lundevall
    Latest update 2017-04-21 by F Lundevall
    For copyright and licensing, see file COPYING
 
    Some parts are original code authored by Hosna Molavi and Weronika Danilczuk.

 */



#include <pic32mx.h>
#include <stdint.h>

/* Authors: Hosna Molavi and Weronika Danilczuk */
#include "gamefunctions.h"

int main(void) {
    
    /*  Code from lab 3  */
    
    SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
    SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
    while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
    OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
    while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
    SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;
    /* SPI2STAT bit SPIROV = 0; */
    SPI2STATCLR = 0x40;
    /* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
    /* SPI2CON bit MSTEN = 1; */
    SPI2CONSET = 0x20;
    /* SPI2CON bit ON = 1; */
    SPI2CONSET = 0x8000;
	
    
    /* The following code was written by Hosna Molavi and Weronika Danilczuk */
    
    /* Initialize OLED display */
    display_init();
    
    /* Initialize snake data */
    snake_init();
    
    while( 1 ) {
      display();
    }
    return 0;
}


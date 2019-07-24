/*
 *--------------------------------------
 * Program Name: Paint for CE
 * Author: Malcolm Anderson
 * License: uuhhhhh idk
 * Description: It's Paint. For the CE.
 *--------------------------------------
*/

/* Keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Standard headers (recommended) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <debug.h>
#include <keypadc.h>

#define SCREEN_MAX_X 320
#define SCREEN_MAX_Y 240

/* Put your function prototypes here */
void generateFruit(void);
void drawGame(void);
void shiftSegments(void);
/* Put all your globals here */

int xPos = 100;
int yPos = 100;
int squareSize = 10;
int speed = 6;
kb_key_t key;

int fruitX = 0;
int fruitY = 0;

int segments[2][500] = 0;
unsigned int noSegments = 1;
unsigned int maxSegments = 500;
char scoreBuf[20];
char resBuf[20];

int i = 0; // for iterations


unsigned long int frames = 0;

enum Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3
};

enum GameStatus {
    Playing = 0,
    Paused = 1,
    Lost = 2
};

enum GameStatus currentGameStatus = Playing;

enum Direction currentDir;

int buttonBuf = 0;

void main(void) {
    currentDir = Right;

    
    /* Fill in the body of the main function here */

    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetColor(gfx_white);
    gfx_SetTextFGColor(gfx_white);

    generateFruit();




    
    kb_Scan();

    do {
        /* Update kb_Data */
        kb_Scan();

        frames++;
        gfx_FillScreen(gfx_white);
        
        boot_WaitShort();
        

    } while (currentGameStatus != Lost);
    

    gfx_End();
}

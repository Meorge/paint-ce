/*
 *--------------------------------------
 * Program Name: Snake for CE
 * Author: Malcolm Anderson
 * License: uuhhhhh idk
 * Description: It's Snake. For the CE.
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
        gfx_FillScreen(gfx_black);

        if (kb_Data[1] == kb_Del) {
            currentGameStatus = Lost;
        }

        if (kb_Data[1] == kb_2nd && currentGameStatus == Playing && buttonBuf == 0) {
            currentGameStatus = Paused;
            for (i = 0; i < 50; i++) {
                boot_WaitShort();
            }

            buttonBuf = 40;
            
        } else if (kb_Data[1] == kb_2nd && currentGameStatus == Paused && buttonBuf == 0) {
            currentGameStatus = Playing;
            buttonBuf = 40;
        }

        if (buttonBuf - 1 >= 0) {
            buttonBuf--;
        }

        if (currentGameStatus == Paused) {
            gfx_SetTextFGColor(127);

            sprintf(scoreBuf, "PAUSED - %d", noSegments);
            gfx_PrintStringXY(scoreBuf, 0, 0);

            drawGame();
            gfx_SwapDraw();
            boot_WaitShort();
            boot_WaitShort();
            boot_WaitShort();
 
            continue;
        }

        drawGame();



        /* Load group 7 registers */
        key = kb_Data[7];

        switch (key) {
            case kb_Down:
                if (currentDir != Up) currentDir = Down;
                break;
            case kb_Right:
                if (currentDir != Left) currentDir = Right;
                break;
            case kb_Up:
                if (currentDir != Down) currentDir = Up;
                break;
            case kb_Left:
                if (currentDir != Right) currentDir = Left;
                break;
            default:
                break;
        }

        switch ((int)currentDir)
        {
        case 0: // up
            yPos-= speed;
            break;
        case 1: // Right
            xPos+= speed;
            break;
        case 2: // Down
            yPos+= speed;
            break;
        case 3: // Left
            xPos-= speed;
            break;
        default:
            break;
        }

        if (yPos > SCREEN_MAX_Y) {
            yPos = 0;
        } else if (yPos < 0) {
            yPos = SCREEN_MAX_Y;
        }

        if (xPos > SCREEN_MAX_X) {
            xPos = 0;
        } else if (xPos < 0) {
            xPos = SCREEN_MAX_X;
        }

        shiftSegments();
        segments[0][0] = xPos;
        segments[1][0] = yPos;

        for (i = 1; i < noSegments; i++) {
            if (segments[0][0] == segments[0][i] && segments[1][0] == segments[1][i]) {
                currentGameStatus = Lost;
                break;
            }
        }

        if (abs(segments[0][0] - fruitX) < 10 && abs(segments[1][0] - fruitY) < 10) {
            // speed++;
            noSegments += 3;
            if (noSegments >= maxSegments) {

            } else {
                shiftSegments();
                segments[0][noSegments - 1] = segments[0][noSegments - 2];
                segments[1][noSegments - 1] = segments[0][noSegments - 2];
                switch ((int)currentDir)
                {
                case 0: // up
                    segments[1][noSegments - 1] += squareSize * 3;
                    // yPos--;
                    break;
                case 1: // Right
                    segments[0][noSegments - 1] -= squareSize * 3;
                    // xPos++;
                    break;
                case 2: // Down
                    segments[1][noSegments - 1] -= squareSize * 3;
                    // yPos++;
                    break;
                case 3: // Left
                    segments[0][noSegments - 1] += squareSize * 3;
                    // xPos--;
                    break;
                default:
                    break;
                }
                generateFruit();
            }
            
        }



        gfx_SetTextFGColor(127);

        sprintf(scoreBuf, "%d", noSegments);
        gfx_PrintStringXY(scoreBuf, 0, 0);

        gfx_SwapDraw();


        boot_WaitShort();
        boot_WaitShort();
        boot_WaitShort();
        

    } while (currentGameStatus != Lost);

    gfx_FillScreen(gfx_black);
    gfx_SetTextFGColor(gfx_yellow);

    sprintf(resBuf, "Your Score Was %d", noSegments);
    gfx_PrintStringXY(resBuf,(LCD_WIDTH - gfx_GetStringWidth(resBuf)) / 2, (LCD_HEIGHT - 8) / 2);
    gfx_SwapDraw();
    boot_WaitShort();
    boot_WaitShort();
    boot_WaitShort();
    boot_WaitShort();
    while (!os_GetCSC());
    

    gfx_End();
}

void drawGame(void) {
    gfx_SetColor(gfx_white);

    for (i = 0; i < noSegments; i++) {
        gfx_FillRectangle(segments[0][i], segments[1][i], squareSize, squareSize);   
    }

    gfx_SetColor(gfx_yellow);
    gfx_FillCircle(fruitX, fruitY, squareSize / 2);
}

void generateFruit(void) {
    srand(rtc_Time());
    fruitX = rand() % (SCREEN_MAX_X);
    fruitY = rand() % (SCREEN_MAX_Y);
}

void shiftSegments(void) {
    for (i = noSegments; i > 0; i--) {
        segments[0][i] = segments[0][i - 1];
        segments[1][i] = segments[1][i - 1];
    }
    // segments[0][0] = xPos;
    // segments[1][0] = yPos;
}

/* Put other functions here */

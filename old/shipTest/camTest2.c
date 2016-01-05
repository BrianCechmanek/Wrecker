/*
    Camera Test
    Smooth Scroll (Hopefully!)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "BearLibTerminal.h"

#define CELL(a, b) (map[(a*MAPWIDTH) + b])

#define MAPWIDTH 200
#define MAPHEIGHT 200

#define VIEW_xOFF 20
#define VIEW_yOFF 5

#define FPS 40
#define SPEEDCAP 25

#define CAMW 40
#define CAMH 20

typedef struct{

    char code;

} Cell;

Cell *map;

void initMap(void){

    int i, j;

    if ( (map = malloc( (MAPWIDTH * MAPHEIGHT) * sizeof(Cell)) ) == NULL) {
        printf("Map Memory Error");
        abort();
    }

    srand(time(NULL));

    for ( i = 0; i < MAPWIDTH; i++){
        for (j = 0; j < MAPHEIGHT; j++){
            
            if ( rand() % 50 == 0 ) {
                CELL(i,j).code = 'A' + (rand() % 10);
            } else {
                CELL(i,j).code = '.';
            }
        }
    }
}

int main(void)
{
        terminal_open();
        terminal_set("window.title='Camera Prototype v2'");

        int camX = 0; int camY = 0;
        int hSpeed = 0; int vSpeed = 0;
        int i, j;

        initMap();

        int c;
        int key;

        int running = 1;
        while(running) {
            
            camX += hSpeed;
            camY += vSpeed;

            for ( j = 0; j < CAMH; j++) {
                for (i = 0; i < CAMW; i++){

                    c = CELL((camX+i), (camY+j)).code;
                    terminal_put(20 + i, 4 + j, c);
                }
            }

            terminal_refresh();

            while ( running && terminal_has_input() ){

                key = terminal_read();
                if ( key == TK_CLOSE || key == TK_ESCAPE )
                     running = 0;
            }

            if ( terminal_state( TK_LEFT )) {
                if (hSpeed < SPEEDCAP && camX > 0 ) {
                    hSpeed -= 1;
                    if (camX < 0){
                        hSpeed = 0;
                        camX = 0;
                    }
                }
            }
            else if ( terminal_state( TK_RIGHT )) {
                if (hSpeed < SPEEDCAP && camX < MAPWIDTH - CAMW) {
                    hSpeed += 1;
                    if (camX >= MAPWIDTH - CAMW){
                        hSpeed = 0;
                        camX = MAPWIDTH - CAMW;
                    }
                }
            } else {
                if (camX > 0 || camX < MAPWIDTH - CAMW ) {
                    hSpeed -= ( hSpeed > 0) ? 1: ( ( hSpeed < 0) ? -1: 0);
                } else {
                    hSpeed = 0;
                }
            }
            if (terminal_state( TK_UP )) {
                if (vSpeed < SPEEDCAP && camY > 0 ) {
                    vSpeed -= 1;
                    if (camY < 0){
                        vSpeed = 0;
                        camY = 0;
                    }
                }
            }
            else if ( terminal_state( TK_DOWN )) {
                if (vSpeed < SPEEDCAP && camY < MAPHEIGHT - CAMH ) {
                    vSpeed += 1;
                    if (camY > MAPHEIGHT - CAMH){
                        vSpeed = 0;
                        camY = MAPHEIGHT - CAMH;
                    }
                }
            } else {
                if (camY > 0 || camY < MAPHEIGHT - CAMH ) {
                    vSpeed -= ( vSpeed > 0) ? 1: ( ( vSpeed < 0) ? -1: 0);
                } else {
                    vSpeed = 0;
                }
            }
            terminal_delay(1000 / FPS);
    }
    terminal_close();
}


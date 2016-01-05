/*
    Camera Test 3
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
#define VIEW_yOFF 2 

#define FPS 60 
#define SPEEDCAP 25

#define CAM_W 40
#define CAM_H 20

typedef struct{
    
    int camX; int camY;
    int hpo; int vpo;       // Horizontal and Vertical Pixel Offset
    int hSpeed; int vSpeed;

} CameraInfo;

typedef struct{

    char code;

} Cell;

int tileWidth;
int tileHeight;
Cell *map;

void initMap(void);
void initCamera(CameraInfo *, int x, int y);
void drawBorder(void);
void scrollMap(CameraInfo *);

int main(void)
{
    CameraInfo camera;
    terminal_open();
    terminal_set("window.title='Camera Prototype 3'");
    
    initMap();
    initCamera(&camera, 100, 100);
    drawBorder();

    int key;
    int running = 1;
    while (running) {

        scrollMap(&camera);
        terminal_refresh();

        while ( running && terminal_has_input() ){
            key = terminal_read();
            if ( key == TK_CLOSE || key == TK_ESCAPE ) running = 0;
        }   

        if ( terminal_state( TK_LEFT )) {
            if (camera.hSpeed < SPEEDCAP && camera.hpo > 0 ) {
                    camera.hSpeed -= 1;
                } else {
                    if (camera.hpo <= 0) camera.hSpeed = 0;
                }
            }
            else if ( terminal_state( TK_RIGHT )) {
                if (camera.hSpeed < SPEEDCAP && camera.hpo / tileWidth < MAPWIDTH - CAM_W) {
                    camera.hSpeed += 1;
                }
                if (camera.hpo / tileWidth >= MAPWIDTH - CAM_W) camera.hSpeed = 0;
            } else {
                if (camera.hpo > 0 || camera.hpo / tileWidth < MAPWIDTH - CAM_W ) {
                    camera.hSpeed -= ( camera.hSpeed > 0) ? 1: ( ( camera.hSpeed < 0) ? -1: 0);
                } else {
                    camera.hSpeed = 0;
                }
            }
            // Vert
            if (terminal_state( TK_UP )) {
                if (camera.vSpeed < SPEEDCAP && camera.vpo > 0 ) {
                    camera.vSpeed -= 1;
                }
                if (camera.vpo <= 0) camera.vSpeed = 0;
            }
            else if ( terminal_state( TK_DOWN )) {
                if (camera.vSpeed < SPEEDCAP && camera.vpo / tileHeight < MAPHEIGHT - CAM_H ) {
                    camera.vSpeed += 1;
                }
                if (camera.vpo / tileHeight >= MAPHEIGHT - CAM_H ) camera.vSpeed = 0;
            } else {
                if (camera.vpo > 0 || camera.vpo / tileHeight < MAPHEIGHT - CAM_H ) {
                    camera.vSpeed -= ( camera.vSpeed > 0) ? 1: ( ( camera.vSpeed < 0) ? -1: 0);
                } else {
                    camera.vSpeed = 0;
                }
            }
            terminal_delay(1000 / FPS);
    }
    
    terminal_close();
}

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
            }
            else if( i % 10 == 0 ){
                 CELL(i, j).code = '-';
            } else {
                CELL(i,j).code = '.';
            }
        }
    }
}

void initCamera(CameraInfo *camera, int x, int y)
{
        camera->camX = x;
        camera->camY = y;
        camera->hpo = x * terminal_state(TK_CELL_WIDTH);
        camera->vpo = y * terminal_state(TK_CELL_HEIGHT);
        camera->hSpeed = 0;
        camera->vSpeed = 0;
}

void drawBorder(void)
{
    int i, j;

    for ( i = VIEW_xOFF - 2; i <= VIEW_xOFF + CAM_W + 6; i++ ) {
        terminal_put(i, 1, '#');
        terminal_put(i, 24, '#'); 
    }

    for ( j = 2; j < 24; j++) {
        terminal_put(VIEW_xOFF - 2, j, '#');
        terminal_put(VIEW_xOFF + CAM_W + 6, j, '#');
    }
}

void scrollMap( CameraInfo *cam )
{
    tileWidth = terminal_state(TK_CELL_WIDTH);
    tileHeight = terminal_state(TK_CELL_HEIGHT);

    int viewWidth = CAM_W * tileWidth;
    int viewHeight = CAM_H * tileHeight;
    int xBord = MAPWIDTH - CAM_W; int yBord = MAPHEIGHT - CAM_H;

    int tx, ty;         //Remainder of Offset / Tilesize
    int jx, jy;         // Map Looping info.
    int hc, vc;
    int mapX, mapY;
    int xPixel, yPixel;

    if ( cam->hpo < 0 ) cam->hpo = 0;
    else if ((cam->hpo / tileWidth) > xBord) cam->hpo = (xBord - 1) * tileWidth;
    else if ((cam->hpo + cam->hSpeed < 0)) cam->hpo = 0;
    else cam->hpo += cam->hSpeed;
    
    if (cam->vpo < 0) cam->vpo = 0;
    else if (cam->vpo / tileHeight > yBord) cam->vpo = (yBord - 1) * tileHeight;
    else if (cam->vpo + cam->vSpeed < 0) cam->vpo = 0;
    else cam->vpo += cam->vSpeed;

    tx = cam->hpo % tileWidth; // get Remainder of tile division
    ty = cam->vpo % tileHeight;
        
    cam->camX = (cam->hpo - tx) / tileWidth; // Gives (x, y) coordinates.
    cam->camY = (cam->vpo - ty) / tileHeight;
        
    jx = cam->camX < 0? (cam->camX) % MAPWIDTH: MAPWIDTH + (cam->camX % MAPWIDTH);
    jy = cam->camY < 0? (cam->camY) % MAPHEIGHT: MAPHEIGHT + (cam->camY % MAPHEIGHT);
        
    hc = ceil( (viewWidth + tileWidth - tx) / (float)tileWidth );
    vc = ceil( (viewHeight + tileHeight -ty) / (float)tileHeight);

    for ( yPixel = 0; yPixel < vc; yPixel++ ) {
        for (xPixel = 0; xPixel < hc; xPixel++) {
        
            mapX = (xPixel + jx) % MAPWIDTH;
            mapY = (yPixel + jy) % MAPHEIGHT; 
                    
            terminal_put_ext(VIEW_xOFF + xPixel, VIEW_yOFF + yPixel, (xPixel - 1)-tx, 
                                        (yPixel - 1) - ty, CELL(mapX, mapY).code, NULL);
        }
    }

    terminal_printf(0,3, "iX =%d  iY=%d  ", cam->camX, cam->camY);
    terminal_printf(0,4, "hSpeed=%d ", cam->hSpeed);
    terminal_printf(0,5, "vSpeed=%d ", cam->vSpeed);
}


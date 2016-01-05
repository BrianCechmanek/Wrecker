#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "BearLibTerminal.h"

#define CELL(a, b) (map[(b*MAPWIDTH) + a])

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
    int tileWidth; int tileHeight;
    bool scrollMode;

} CameraInfo;

typedef struct{
    int tileType;
} Cell;

enum tiles
{
    NOTHING = 0,
    FLOOR,
    WALL,
    MACHINE,

    NUM_OF_TILETYPES
};
typedef struct shipTile
{
    int tileType;
    int xLoc, yLoc;
} shipTile;

typedef struct Ship
{
        char name[20];
        int width, height;
        shipTile *shipModel;
        int xLoc, yLoc;
        short dx, dy; //speed
        short face; // 0 up, right 1, down 2, left 3.
} Ship;

// N = 0, E = 2, S = 4, W = 6.
enum directions
{
        NORTH,
        N_EAST,
        EAST,
        S_EAST,
        SOUTH,
        S_WEST,
        WEST,
        N_WEST
};

Cell *map;
char tileList[4] = " .#C";

int dirList[8][2] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

int getTileType(char c)
{
    if ( c == ' ' ) return 0;
    if ( c == '.' ) return 1;
    if ( c == '#' ) return 2;
    if ( c == 'C' ) return 3;
}

void initMap(void)
{
    int i, j;
    if ( (map = malloc( (MAPWIDTH * MAPHEIGHT) * sizeof(Cell)) ) == NULL) {
        printf("Map Memory Error");
        abort();
    }

    srand(time(NULL));

    for ( i = 0; i < MAPWIDTH; i++){
        for (j = 0; j < MAPHEIGHT; j++){
            CELL(i,j).tileType = 0;
        }
    }
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

void initCamera(CameraInfo *camera, int x, int y)
{
        camera->tileWidth = terminal_state(TK_CELL_WIDTH);
        camera->tileHeight = terminal_state(TK_CELL_HEIGHT);
        camera->camX = x;
        camera->camY = y;
        camera->hpo = x * camera->tileWidth;
        camera->vpo = y * camera->tileHeight;
        camera->hSpeed = 0;
        camera->vSpeed = 0;
        camera->scrollMode = 0;
}

void moveCameraToShip(CameraInfo *camera, Ship *theShip)
{   
    int x = theShip->shipModel[0].xLoc + (theShip->width / 2);
    int y = theShip->shipModel[0].yLoc + (theShip->height / 2);
    camera->camX = x - (CAM_W / 2);
    camera->camY = y - (CAM_H / 2);
    camera->hpo = camera->camX * camera->tileWidth;
    camera->vpo = camera->camY * camera->tileHeight;
    camera->hSpeed = 0;
    camera->vSpeed = 0;
}

void scrollMap( CameraInfo *cam )
{

    int viewWidth = CAM_W * cam->tileWidth;
    int viewHeight = CAM_H * cam->tileHeight;
    int xBord = MAPWIDTH - CAM_W; int yBord = MAPHEIGHT - CAM_H;

    int tx, ty;         //Remainder of Offset / Tilesize
    int jx, jy;         // Map Looping info.
    int hc, vc;
    int mapX, mapY;
    int xPixel, yPixel;

    if ( cam->hpo < 0 ) cam->hpo = 0;
    else if ((cam->hpo / cam->tileWidth) > xBord) cam->hpo = (xBord - 1) * cam->tileWidth;
    else if ((cam->hpo + cam->hSpeed < 0)) cam->hpo = 0;
    else cam->hpo += cam->hSpeed;
    
    if (cam->vpo < 0) cam->vpo = 0;
    else if (cam->vpo / cam->tileHeight > yBord) cam->vpo = (yBord - 1) * cam->tileHeight;
    else if (cam->vpo + cam->vSpeed < 0) cam->vpo = 0;
    else cam->vpo += cam->vSpeed;

    tx = cam->hpo % cam->tileWidth; // get Remainder of tile division
    ty = cam->vpo % cam->tileHeight;
        
    cam->camX = (cam->hpo - tx) / cam->tileWidth; // Gives (x, y) coordinates.
    cam->camY = (cam->vpo - ty) / cam->tileHeight;
        
    jx = cam->camX < 0? (cam->camX) % MAPWIDTH: MAPWIDTH + (cam->camX % MAPWIDTH);
    jy = cam->camY < 0? (cam->camY) % MAPHEIGHT: MAPHEIGHT + (cam->camY % MAPHEIGHT);
        
    hc = ceil( (viewWidth + cam->tileWidth - tx) / (float)cam->tileWidth );
    vc = ceil( (viewHeight + cam->tileHeight -ty) / (float)cam->tileHeight);

    for ( yPixel = 0; yPixel < vc; yPixel++ ) {
        for (xPixel = 0; xPixel < hc; xPixel++) {
        
            mapX = (xPixel + jx) % MAPWIDTH;
            mapY = (yPixel + jy) % MAPHEIGHT; 
                    
            terminal_put_ext(VIEW_xOFF + xPixel, VIEW_yOFF + yPixel, (xPixel - 1)-tx, 
                                        (yPixel - 1) - ty, tileList[CELL(mapX, mapY).tileType], NULL);
        }
    }

    terminal_printf(0,3, "iX =%d  iY=%d  ", cam->camX, cam->camY);
    terminal_printf(0,4, "hSpeed=%d ", cam->hSpeed);
    terminal_printf(0,5, "vSpeed=%d ", cam->vSpeed);
}

void loadShip( Ship *newShip, char *filename )
{
    FILE *openedFile;
    char lineBuffer[60];

    int i = 0; int j = 0;
    char c;

    if ( (openedFile = fopen(filename, "r")) == NULL ) {
        printf("Error opening File: %s\n", filename);
        exit(1);
    }
    
    fgets( lineBuffer, 60, openedFile );
    sscanf(lineBuffer, "\"%[^\"]\"", newShip->name);
    fgets( lineBuffer, 60, openedFile);
    sscanf(lineBuffer, "%d %d", &newShip->width, &newShip->height);
    
    if ( (newShip->shipModel = malloc( sizeof(shipTile) * newShip->width * newShip->height)) == NULL){
        printf("Ship Memory Error\n");
        exit(1);
    }

    fgets(lineBuffer, 60, openedFile); // Skip Line
    
    while ( (fgets(lineBuffer, 60, openedFile)) != NULL ) {
        for ( i = 0; i < newShip->width; i++ ){
            c = lineBuffer[i];
            newShip->shipModel[j * newShip->width +i].tileType = getTileType(c);
            newShip->shipModel[j * newShip->width +i].xLoc = i;
            newShip->shipModel[j * newShip->width +i].yLoc = j;
        }
        j++;
    }
    fclose(openedFile);
}

void placeShip( Ship *theShip, int x, int y)
{
    int i, j;
    theShip->xLoc = x; theShip->yLoc = y;
    for( i = 0; i < theShip->width; i++){
        for( j = 0; j < theShip->height; j++){

            CELL((i+x), (j+y)).tileType = theShip->shipModel[j * theShip->width + i].tileType;
            theShip->shipModel[j * theShip->width + i].xLoc = x + i;
            theShip->shipModel[j * theShip->width + i].yLoc = y + j;

        }
    }
}

void clearShip( Ship *theShip )
{
    int i, j;
    int x = theShip->shipModel[0].xLoc;
    int y = theShip->shipModel[0].yLoc;

    for (j = 0; j < theShip->height; j++) {
        for ( i = 0; i < theShip->width; i++) {
            CELL((i+x), (j+y)).tileType = NOTHING;
        }
    }
}
bool moveShip( Ship *theShip, int direction )
{
    int i;
    int x, y;
    int dx = dirList[direction][0];
    int dy = dirList[direction][1];

    // collision logic check
    clearShip( theShip );
    /*
        Logic here is:
            1. Clear flags/info of old tile.
            2. Set new tile flags/info.
            3. Move Ship values.
     */
    for (i = 0; i < theShip->width * theShip->height; i++){
        x = theShip->shipModel[i].xLoc;
        y = theShip->shipModel[i].yLoc;
        CELL((x+dx), (y+dy)).tileType = theShip->shipModel[i].tileType;
        theShip->shipModel[i].xLoc += dx;
        theShip->shipModel[i].yLoc += dy;
    }
    return true;
}

int main(void)
{

    char *filename = "Uboat.txt";
    int i, j;
    Ship theShip;
    CameraInfo theCamera;

    terminal_open();
    terminal_set("window.title='Ship + Camera Prototype'");

    initMap();
    loadShip(&theShip, filename);
    placeShip(&theShip, 110, 110);
    initCamera( &theCamera, 100, 100);
    drawBorder();

    terminal_printf(0,0, "%s", theShip.name);

    int key;
    int running = 1;
    while (running) {

        scrollMap(&theCamera);
        terminal_refresh();

        while ( running && terminal_has_input() ){
            key = terminal_read();
            
            if ( key == TK_CLOSE || key == TK_ESCAPE ) running = 0;
            else if (key == TK_SPACE){
                moveCameraToShip(&theCamera, &theShip);
            }
            if ( key == TK_TAB ) {
                theCamera.scrollMode = !theCamera.scrollMode;
                theCamera.hSpeed = 0;
                theCamera.vSpeed = 0;
            }

        }   
        if (theCamera.scrollMode){
            if ( terminal_state( TK_LEFT )) {
                if (theCamera.hSpeed < SPEEDCAP && theCamera.hpo > 0 ) {
                        theCamera.hSpeed -= 1;
                    } else {
                        if (theCamera.hpo <= 0) theCamera.hSpeed = 0;
                    }
                }
                else if ( terminal_state( TK_RIGHT )) {
                    if (theCamera.hSpeed < SPEEDCAP && theCamera.hpo / theCamera.tileWidth < MAPWIDTH - CAM_W) {
                        theCamera.hSpeed += 1;
                    }
                    if (theCamera.hpo / theCamera.tileWidth >= MAPWIDTH - CAM_W) theCamera.hSpeed = 0;
                } else {
                    if (theCamera.hpo > 0 || theCamera.hpo / theCamera.tileWidth < MAPWIDTH - CAM_W ) {
                        theCamera.hSpeed -= ( theCamera.hSpeed > 0) ? 1: ( ( theCamera.hSpeed < 0) ? -1: 0);
                } else {
                        theCamera.hSpeed = 0;
                }
                }
                // Vert
            if (terminal_state( TK_UP )) {
                    if (theCamera.vSpeed < SPEEDCAP && theCamera.vpo > 0 ) {
                        theCamera.vSpeed -= 1;
                    }
                    if (theCamera.vpo <= 0) theCamera.vSpeed = 0;
                }
            else if ( terminal_state( TK_DOWN )) {
                    if (theCamera.vSpeed < SPEEDCAP && theCamera.vpo / theCamera.tileHeight < MAPHEIGHT - CAM_H ) {
                        theCamera.vSpeed += 1;
                    }
                    if (theCamera.vpo / theCamera.tileHeight >= MAPHEIGHT - CAM_H ) theCamera.vSpeed = 0;
            } else {
                    if (theCamera.vpo > 0 || theCamera.vpo / theCamera.tileHeight < MAPHEIGHT - CAM_H ) {
                        theCamera.vSpeed -= ( theCamera.vSpeed > 0) ? 1: ( ( theCamera.vSpeed < 0) ? -1: 0);
                    } else {
                        theCamera.vSpeed = 0;
                    }
            }
        } else {
            if (key == TK_UP) moveShip(&theShip, NORTH);
            if (key == TK_DOWN) moveShip(&theShip, SOUTH);
            if (key == TK_LEFT) moveShip(&theShip, WEST);
            if (key == TK_RIGHT) moveShip(&theShip, EAST);
        }

        terminal_delay(1000 / FPS);
    }
     
    terminal_close();
    free(theShip.shipModel);
    free(map);
}

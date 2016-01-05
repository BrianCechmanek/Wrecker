#include "ships.h"

void initMap(void)
{
    int i, j;
    if ( (map = malloc( (MAPWIDTH * MAPHEIGHT) * sizeof(Cell)) ) == NULL) {
        printf("Map Memory Error");
        abort();
    }

    memset( map, 0, sizeof(Cell));
    srand(time(NULL));

    for ( i = 0; i < MAPWIDTH; i++){
        for (j = 0; j < MAPHEIGHT; j++){
            CELL(i,j).tileType = 0;
        }
    }
}

int getTileType(char c)
{
    if ( c == '.' ) return 1;
    if ( c == '#' ) return 2;
    if ( c == 'C' ) return 3;
    
    return 0;
}

void initCamera(CameraInfo *camera)
{
        camera->tileWidth = terminal_state(TK_CELL_WIDTH);
        camera->tileHeight = terminal_state(TK_CELL_HEIGHT);
        camera->camX = 1;
        camera->camY = 1;
        camera->hpo = 1 * camera->tileWidth;
        camera->vpo = 1 * camera->tileHeight;
        camera->hSpeed = 0;
        camera->vSpeed = 0;
        camera->scrollMode = 0;
}

void lockCamera( CameraInfo * theCamera )
{
    int x, y;
    Ship *theShip;
    if ( playerInfo.pilot == NULL ) {
        x = playerInfo.playerAlien->xLoc;
        y = playerInfo.playerAlien->yLoc;
    } else {
        theShip = playerInfo.pilot;
        x = theShip->shipModel[0].xLoc + (theShip->width / 2);
        y = theShip->shipModel[0].yLoc + (theShip->height / 2);
    }
    theCamera->camX = x - (CAM_W / 2);
    theCamera->camY = y - (CAM_H / 2);
    theCamera->hpo = theCamera->camX * theCamera->tileWidth;
    theCamera->vpo = theCamera->camY * theCamera->tileHeight;
    theCamera->hSpeed = 0;
    theCamera->vSpeed = 0;
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

int getCellAppearance( int x, int y )
{
    alien *theAlien;
    int newType;

    if ( CELL(x, y).cellFlags & (HAS_ALIEN) ){
        theAlien = findAlien(x, y);
        newType = theAlien->alienType;
        return alienList[newType].code;
    }
    else{
        return tileList[ CELL(x,y).tileType ].code;
    }
}

void drawMap( CameraInfo *cam )
{

    int viewWidth = CAM_W * cam->tileWidth;
    int viewHeight = CAM_H * cam->tileHeight;
    int xBord = MAPWIDTH - CAM_W; int yBord = MAPHEIGHT - CAM_H;

    int tx, ty;         //Remainder of Offset / Tilesize
    int jx, jy;         // Map Looping info.
    int hc, vc;

    int mapX, mapY;
    int xPixel, yPixel;
    int tileCode;

    if ( cam->hpo < 0 ) cam->hpo = 0;
    else if ((cam->hpo / cam->tileWidth) > xBord) cam->hpo = (xBord - 1) * cam->tileWidth;
    else if ((cam->hpo + cam->hSpeed < 0)) cam->hpo = 0;
    else cam->hpo += cam->hSpeed;
    
    if (cam->vpo < 0) cam->vpo = 0;
    else if (cam->vpo / cam->tileHeight > yBord) cam->vpo = (yBord - 1) * cam->tileHeight;
    else if (cam->vpo + cam->vSpeed < 0) cam->vpo = 0;
    else cam->vpo += cam->vSpeed;
    
    if( cam->scrollMode ) {
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
                tileCode = getCellAppearance( mapX, mapY );
                        
                terminal_put_ext(VIEW_xOFF + xPixel, VIEW_yOFF + yPixel, (xPixel - 1)-tx, 
                                            (yPixel - 1) - ty, tileCode, NULL);
            }
        }
    }
    else{
        int i, j;
        
        for ( j = 0; j < CAM_H; j++){
            for( i = 0; i < CAM_W; i++){

                mapX = cam->camX + i;
                mapY = cam->camY + j;

                tileCode = getCellAppearance(mapX, mapY);
                terminal_put( VIEW_xOFF + i, VIEW_yOFF +j, tileCode);
            }
        }
    }
    
}


#include <stdio.h>
#include <stdbool.h>

#include "first.h"

pCell sectorMap[SCOLS][SROWS];
displayBuffer dMap[60][25]; // Perhaps increase this, then only display part of it? less recalc.

playerGlobals gameInfo;

// lines up with enum DIRECTIONS
int dirList[8][2] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

tileEntry tileList[4] = {
// Char     foreColor   backColor   flags
{' ',       "black",     "black",   (T_IS_VACUUM)},
{'.',       "white",    "dark grey", 0},
{'#',       "dark grey","light grey", (T_OBSTRUCTS_MOVEMENT | T_OBSTRUCTS_VISION)},
{'&',       "green",    "light grey", (T_OBSTRUCTS_MOVEMENT)}
}

alienType alienCatalog[2] = {
//   ID       Name        CharCode    foreColor       flags
    {0,     "PLAYER",       '@',       "white",        0}
}

void initGameInfo(void) {
    gameInfo.playerAlien = initPlayer();
    gameInfo.camX = 0;
    gameInfo.camY = 0;
    int health = 100;
    int active = 1;
    bool camMoved = 0;
    bool running = 1;
}
    
void initSectorMap(void)
{
    int i, j;
    for ( i = 0; i < SCOLS; i++ ){
        for ( j = 0; j < SROWS; j++){
            sectorMap[i][j].tileType = 0;
            sectorMap[i][j].cellFlags = 0;
        }
    }
}

// Dont need this yet. Probably want Complexity.
void buildCell( pCell *newCell, int newTileType)
{
        newCell->tileType = newTileType;
}

void placeSquare( int x, int y, int w, int h )
{
        int i, j;
        i = x;
        j = y;
    
        while ( j < (y + h) - 1 ) {
            sectorMap[x][j].tileType = WALL;
            sectorMap[x + (w - 1)][j].tileType = WALL;
            j++;
        }
        while ( i <= (x + w) - 1){
            sectorMap[i][y].tileType = WALL;
            sectorMap[i][y+ (h - 1)].tileType= WALL;
            i++;
        }
}

void cameraToPlayer(void){
    gameInfo.camX = gameInfo.playerAlien->xLoc - (DCOLS / 2);
    gameInfo.camY = gameInfo.playerAlien->yLoc - (DROWS / 2);
    gameInfo.camMoved = true;
}

bool movePlayer( int direction ){
    
    int i;
    alien *player = gameInfo.playerAlien;
    int x = player->xLoc; int y = player->yLoc;
    int dx = dirList[direction][0]; int dy = dirList[direction][1];
    int destTile =  sectorMap[x + dx][y + dy].tileType;
    
    // Is tile in Map?
    
    if ( (tileList[destTile].flags & T_OBSTRUCTS_MOVEMENT ) ) {
        return false;
    //else if Defender Logic.
    } else {
        sectorMap[x][y].flags &= ~HAS_PLAYER; // Clear HAS_PLAYER bit.
        sectorMap[x+dx][y+dy].flags |= HAS_PLAYER; // Set HAS_PLAYER bit.
        player->xLoc += dx;
        player->yLoc += dy;
    }

    return true;
}

bool placeObject(){
}

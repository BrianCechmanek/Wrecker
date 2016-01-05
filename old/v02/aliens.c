#include "ships.h"

alien *initAlien(void)
{
    alien *newAlien;
    newAlien = malloc( sizeof(alien) );
    memset(newAlien, 0, sizeof(alien));
    newAlien->xLoc = -1;
    newAlien->yLoc = -1;
    newAlien->next = NULL;

    return newAlien;
}
    
alien *buildAlien( int typeNum )
{
    alien *theAlien;
    theAlien = initAlien();
    theAlien->alienType = typeNum;
}

void addToAlienChain( alien *theAlien )
{
   theAlien->next = alienChain;
   alienChain = theAlien;
}

alien *initPlayer(CameraInfo *theCamera)
{
    alien *thePlayer = buildAlien(PLAYER);
    playerInfo.playerAlien = thePlayer;
    playerInfo.pilot = NULL;
    playerInfo.camera = theCamera;
    playerInfo.selectMode = 0;
    return thePlayer;
}

bool placeAlien( alien *theAlien, int x, int y)
{
    //are coordinates in map??
    // Is this a valid spot to put an alien?
    theAlien->xLoc = x;
    theAlien->yLoc = y;
    
    if (theAlien->alienType == PLAYER){
        CELL(x, y).cellFlags |= (HAS_PLAYER | HAS_ALIEN);
    }
    else {
        CELL(x,y).cellFlags |= (HAS_ALIEN);
    }
    
    addToAlienChain( theAlien );
    return 1;
}

bool genShipAliens( int num, int alienType, Ship *theShip )
{
    int i;
    int tileLoc;
    alien *theAlien;
    for ( i = 0; i < num; i++ ){
        if ( (tileLoc = getFreeShipTile( theShip )) == -1 ){
            return 0;
        }
        theAlien = buildAlien(alienType);
        placeAlien( theAlien, theShip->shipModel[tileLoc].xLoc, theShip->shipModel[tileLoc].yLoc);
    }
    return true;
}

alien *findAlien(int x, int y)
{
    alien *tmp;
    for( tmp = alienChain; tmp != NULL && (tmp->xLoc != x || tmp->yLoc != y); tmp = tmp->next );

    return tmp;
}

bool moveAlien(alien *theAlien, int direction, bool forceMove)
{
    int x = theAlien->xLoc; int y = theAlien->yLoc;
    int dx = dirList[direction][0];
    int dy = dirList[direction][1];
    
    if (!forceMove){
        if ( (tileList[CELL((x+dx),(y+dy)).tileType].terrainFlags & T_OBSTRUCTS_MOVEMENT)
            || (CELL((x+dx),(y+dy)).cellFlags & HAS_ALIEN ) ){
            return 0;
        }
    }
    if (theAlien->alienType == PLAYER){
        CELL(x,y).cellFlags &= ~(HAS_PLAYER);
        CELL((x+dx), (y+dy)).cellFlags |= (HAS_PLAYER);
    }
    CELL(x, y).cellFlags &= ~(HAS_ALIEN);

    CELL((x+dx), (y+dy)).cellFlags |= (HAS_ALIEN);
    theAlien->xLoc += dx;
    theAlien->yLoc += dy;

    return 1;
}

int getDirection(void)
{
        terminal_set("input.filter = [escape, arrows]"); 
        int blocking = 1;
        do {

        int key = terminal_read();
        if (key == TK_ESCAPE ) return -1;
        if (key == TK_UP) return NORTH;
        if (key == TK_DOWN) return SOUTH;
        if (key == TK_LEFT) return WEST;
        if (key == TK_RIGHT) return EAST;
        } while (blocking);

        terminal_set("input.filter = []");
    
}


bool alienUse( alien *theAlien )
{
    if (theAlien->alienType == PLAYER){
        int dir = getDirection();
        if (dir < 0) return false;

        int dx = dirList[dir][0];
        int dy = dirList[dir][1];
        Machine *theMachine= getMachineAt( (playerInfo.playerAlien->xLoc + dx),
                                (playerInfo.playerAlien->yLoc + dy));
        int type = theMachine->type;
         machineList[type].fn(theMachine, theAlien);
         return true;
    }

}
    

// Return to the digital nether whence you came ET    
void freeAliens(void)
{
    alien *tmp;

    while( alienChain ){
        tmp = alienChain;
        alienChain = alienChain->next;
        free(tmp);
    }
}

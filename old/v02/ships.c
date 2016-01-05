#include "ships.h"

void loadShip( Ship *newShip, char *filename )
{
    FILE *openedFile;
    char lineBuffer[60];

    int newTile;

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
    newShip->shipMachines = NULL;
    

    fgets(lineBuffer, 60, openedFile); // Skip Line
    while ( (fgets(lineBuffer, 60, openedFile)) != NULL ) {
        for ( i = 0; i < newShip->width; i++ ){
            c = lineBuffer[i];
            newTile = getTileType(c);
            if (newTile == MACHINE){
                Machine *newMachine = buildMachine(c);
                newMachine->xLoc = i;
                newMachine->yLoc = j;
                newMachine->parentShip = newShip;
                newMachine->next = newShip->shipMachines;
                newShip->shipMachines = newMachine;
                
                newShip->shipModel[j * newShip->width +i].flags |= HAS_MACHINE;
            }
            newShip->shipModel[j * newShip->width +i].tileType = newTile;
            newShip->shipModel[j * newShip->width +i].xLoc = i;
            newShip->shipModel[j * newShip->width +i].yLoc = j;
        }
        j++;
    }
    fclose(openedFile);
}

void addShipToChain( Ship *theShip )
{
    theShip->next = shipChain;
    shipChain = theShip;
}

void placeShip( Ship *theShip, int x, int y)
{
    int i, j;
    int newTileType;
    theShip->xLoc = x; theShip->yLoc = y;
    for( i = 0; i < theShip->width; i++){
        for( j = 0; j < theShip->height; j++){

            newTileType =  theShip->shipModel[j * theShip->width + i].tileType;
            if (newTileType != NOTHING) {
                CELL((i+x), (j+y)).cellFlags |= (HAS_SHIP);
                if (newTileType == MACHINE){
                    CELL((i+x), (j+y)).cellFlags |= (HAS_MACHINE);
                }
            }
            CELL((i+x), (j+y)).tileType = newTileType;
            theShip->shipModel[j * theShip->width + i].xLoc = x + i;
            theShip->shipModel[j * theShip->width + i].yLoc = y + j;
        }
    }
    addShipToChain( theShip );
}

Ship *findShipAt(int x, int y)
{
    if( CELL(x,y).cellFlags & (HAS_SHIP) ){
        Ship *tmp = shipChain;
        while ( tmp ){
            if ( (x >= tmp->xLoc && x <= tmp->xLoc * tmp->width) &&
                 (y >= tmp->yLoc && y <= tmp->yLoc * tmp->height)){
                    return tmp;
            }
            tmp = tmp->next;
        }
    }
    return NULL;
}


void clearShip( Ship *theShip )
{
    int i, j;
    int x = theShip->shipModel[0].xLoc;
    int y = theShip->shipModel[0].yLoc;

    for (j = 0; j < theShip->height; j++) {
        for ( i = 0; i < theShip->width; i++) {
            CELL((i+x), (j+y)).cellFlags &= ~(HAS_SHIP);
            CELL((i+x), (j+y)).tileType = NOTHING;
        }
    }
}

// Returns index of available tile from shipModel;
int getFreeShipTile( Ship *theShip )
{
    int tileStack[1000];
    int count = 0;
    shipTile *theTile;

    int i;
    for ( i = 0; i < theShip->width * theShip->height; i++){
        theTile = &theShip->shipModel[i];
        if (theTile->tileType != 0 && !(tileList[theTile->tileType].terrainFlags & T_OBSTRUCTS_MOVEMENT)
            && !(CELL((theTile->xLoc),(theTile->yLoc)).cellFlags & HAS_ALIEN) ) {

            tileStack[count] = i;
            count++;
        }
    }
    if(count == 0)
        return -1;
    return tileStack[rand() % count - 1];
}

void moveShipContents( Stack *theStack, int direction)
{
        Entity *new;
        int dx = dirList[direction][0];
        int dy = dirList[direction][1];

        while (( new = stackPop( theStack )) != NULL) {
            if (new->type == ALIEN_E) {
                alien *theAlien = (alien*)new->object;
                moveAlien( theAlien, direction, 1 );
            }
            else if(new->type == MACH_E){
                Machine *theMachine = (Machine*)new->object;
                moveMachine( theMachine, direction );
            }
            //else if (new->type == ITEM_E) {
        }
}

bool moveShip( Ship *theShip, int direction )
{
    int i;
    int x, y;
    int dx = dirList[direction][0];
    int dy = dirList[direction][1];
    alien *theAlien;
    Stack moveStack = newStack();

    // collision logic check

    clearShip( theShip );
    theShip->xLoc += dx;
    theShip->yLoc += dy;
    for (i = 0; i < theShip->width * theShip->height; i++){
        x = theShip->shipModel[i].xLoc;
        y = theShip->shipModel[i].yLoc;

        if (theShip->shipModel[i].tileType != NOTHING){
            CELL((x+dx), (y+dy)).cellFlags |= (HAS_SHIP);
        }

        CELL((x+dx), (y+dy)).tileType = theShip->shipModel[i].tileType;
        if (theShip->shipModel[i].flags & (HAS_MACHINE)) {
            Machine *theMachine = getMachineAt(x, y);
            stackPush( theMachine, MACH_E, &moveStack);
        }
        if ( CELL(x, y).cellFlags & HAS_ALIEN ) {
            theAlien = findAlien(x, y);
            stackPush( theAlien, ALIEN_E, &moveStack);
        }
        theShip->shipModel[i].xLoc += dx;
        theShip->shipModel[i].yLoc += dy;
           
    }
    
    moveShipContents( &moveStack, direction );
    return true;
}

bool movePlayer(int direction)
{
    if( playerInfo.pilot == NULL ){
        if (moveAlien( playerInfo.playerAlien, direction, 0) == 1){
            lockCamera( playerInfo.camera );
            return 1;
        }
    }
    else {
        Ship *theShip = playerInfo.pilot;
        moveShip( theShip, direction );
        lockCamera( playerInfo.camera );
        return 1;
        
    }
    return 0;
}


        



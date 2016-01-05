#include "ships.h"


Machine *initMachine(void)
{
    Machine *newMachine = malloc(sizeof(Machine));
    if (newMachine == NULL){
        printf("Memory Initialization Error");
        exit(1);
    }

    newMachine->type = -1;
    newMachine->xLoc = -1;
    newMachine->yLoc = -1;
    newMachine->active = 0;
    
    return newMachine;
}

int getMachineType( char c )
{
    if( c == 'C') return 0;
}

Machine *buildMachine( char c)
{
    Machine *newMachine = initMachine();
    int mType = getMachineType(c);
    newMachine->type = mType;
    newMachine->active = machineList[mType].startCode;
    return newMachine;
}

Machine *getMachineAt(int x, int y)
{
    Machine *tmp = NULL;
    if ( CELL(x, y).cellFlags & (HAS_SHIP)){
        Ship *theShip = findShipAt(x, y);
        tmp = theShip->shipMachines;
    } 
    //for(; tmp != NULL || (tmp->xLoc != x || tmp->yLoc != y); tmp = tmp->next );
    return tmp;
}

bool moveMachine( Machine *theMachine, int direction )
{
    if( theMachine == NULL ) return 0;
    int x = theMachine->xLoc; int y = theMachine->yLoc;
    int dx = dirList[direction][0];
    int dy = dirList[direction][1];

    CELL(x, y).cellFlags &= ~(HAS_MACHINE);

    CELL((x+dx), (y+dy)).cellFlags |= (HAS_MACHINE);
    theMachine->xLoc += dx;
    theMachine->yLoc += dy;

    return 1;
}

void togglePilot( Machine *theMachine, alien *theAlien)
{
    if (theAlien == playerInfo.playerAlien){
        if (!theMachine->active && !playerInfo.pilot){
            playerInfo.pilot = theMachine->parentShip;
            theMachine->active = 1;
        } else {
            playerInfo.pilot = NULL;
            theMachine->active = 0;
        }
    }
}
    
    
        


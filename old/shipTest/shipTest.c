#include <stdio.h>
#include <stdlib.h>
#include "bearLibTerminal.h"

typedef struct Ship
{
        char name[20];
        short shipType;
        Compartment *compartmentArray;
        int position;
        short dx, dy; //speed
        short face; // 0 up, right 1, down 2, left 3.
} Ship;

typedef struct Compartment
{
        char name[20];
        int *shape;
        int offX, offY;
        Machine *machines;
        Part *smallParts;
                 
} Compartment;

typedef struct Machine
{
        char name[20];
        int type;
        int kind;
        int *shape;

        int comX;
        int comY;
        int powered;
        int status;

        Part *partsList;

} Machine;

enum shipType {
    CRUISER
}

typedef struct shipTable {
        char name[20];
        int type;
        int numCompartments;
        unsigned long compartmentTypes;


typedef struct Part
{
        char name[20];
        int type;
        int kind;
        int xLoc, yLoc;
        int health;
        int active;
} Part;

//offset invokes separate function, works as a grid, but!
// relative to center compartment. So compartment(0, 3) = is three
// ROOMS above root compartment, connected to something in between.
void buildCompartment( Compartment *comType, int offset )
{
}

void buildMachine()
{
}

void addParts(void)
{
}

Ship *initShip(){
        
        Ship *newShip;
        newShip = calloc(1, sizeof(Ship));
        return newShip;
}

Ship buildShip( Ship *newShip, int newShipType, )
{
       newShip = initship();
        
}



int main(void)
{
        Ship *newShip;
        terminal_open;
        buildShip(type, newShip);
        
        placeShip( newShip, x, y );

        for(;;){
            // game loop stuff.

            while (terminal_read != TK_CLOSE || TK_ESCAPE){
                int code = 

            }
        }
}

         
        


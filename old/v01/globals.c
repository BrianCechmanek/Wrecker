/*
 *   globals.c
 *   Will contain our global variables etc.
 *   Monster/Item catalogs? Or should we pull those in from 
 *   text files to allow moddability etc.
 */

#include "wrecker.h"

pcell pmap[SCOLS][SROWS]; // Sector Columns/ Sector rows.
displayBuffer dmap[DCOLS][DROWS]; // Maybe DCOLS/DROWS + a bit extra for scrolling?

sectorData *sectors;
creature player;
playerCharacter spaceRogue;
creature *alienChain;
creature *graveyard;
item *floorItems;


/*
 * Include Monster/Item Catalogs here; or cook them into .txt's
 */

const itemTable weaponList[NUMBER_OF_WEAPONS] = {
    { "Fists",  "", "", "", {0, 2, 2}, "Lefty and Righty, xenomorphs look out!" },
    { "Mk 1 Blaster", "Rusty", "common", 100, 1, {5, 10, 3}, "Nothing beats a good blaster at your side."}
};


/*
 *  Wrecker 2015
 *  Aliens.c
 *  Functions pertaining to creature interactions.
 */
#include <stdio.h>
#include <string.h>

creature *initializeAlien(void)
{
    creature *newAlien;
    newAlien = malloc( sizeof(creature) );
    memset(newAlien, 0, sizeof(creature));
    return newAlien;
}

creature *findAlien(short x, short y) 
{
        creature *tmp;
        for (tmp = alienChain; tmp != NULL && (tmp->xLoc != x || tmp->yLoc != y); tmp->nextAlien);
        return tmp;
}

//TODO
void buildAlienInto( creature *alien, enum alienType )
{
}

void addAlienToChain( creature *alien )
{
        alien->nextAlien = alienChain;
        alienChain = alien;
}

void freeAlienChain(void)
{
        alien *victim;
        while ( alienChain != NULL ) {
            victim = alienChain;
            alienChain = alienChain->next;
            free(victim)
        }
}

bool canSeeAlien( creature *alien )
{
    if (playerCanSee( alien->xLoc, alien->yLoc ) && !(alien->flags & (DORMANT | INVISIBLE) ) {
        return true;
    }
    else return false;
}

void printMonsterName( char *buff, creature *alien, size_t buffSize) {
}

bool alienInClass( creature *alien, creatureClass inClass )
{
}

bool alienIsEnemy( creature *alien ) {
}

bool alienIsAlly( creature *alien ) {
}

bool alienIsNeutral( creature *alien){
}

creature *cloneMonster( creature* alien, short cloneX, short cloneY )
{
}

// Function to populate aliens in Sector.
void populateAliens(void)
{
}

void moveAlien( creature *alien, short x, short y)
{
}

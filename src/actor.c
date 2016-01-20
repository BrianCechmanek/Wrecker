/*
 * Actor.c
 * Actor Factory
 * WIP
 */
/*
#include "wrecker.h"
#include "diana.h"

typedef struct comListing
{
    unsigned int component;
    void *defaultValue;
} comListing;

typedef unsigned int actorID;

actorID spawnActor( enum actorType )
{
    actorID newActor;
    WRECK(spawn, &newActor);
    
    List *cList = getListComponents( actorType );
    for (int i = 0; i < cList->count; i++){
        comListing *new_c = (comListing*)cList[i];
        WRECK(setComponent, newActor, new_c->component, new_c->defaultValue);
    }
    freeList(cList);
}

List *getListComponents( enum actorType )
{
    List *cList = createList();
}

*/

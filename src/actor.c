/*
 * Actor.c
 * Actor Factory
 * WIP
 */
/*
#include "actor.h"

#include "aliens.h"
#include "dataStructures.h"
#include "dbg.h"
#include "diana.h"
#include "components.h"
#include "health.h"
#include "wrecker.h"
#include "util.h"

#include <stdbool.h>
#include <string.h>

//Forward Prototype declarations
static List *getComponents( int race);

//In-Scope Globals (limited to actor.c)
static bool defaultActorComponentsLoaded = false;
static componentEntry defaultActorComponents[10] = { 0 };
static int numDefaultComponents = 0;

eType_c ACTOR_ETYPE_DEFAULT = { "NewActor", E_ACTOR, false, false };

// We need to run this after we register all of our components in order to
// make the componentID's point to the ID the ECS gives them.
// Im positive there was a better way to do this but my brain is tired.
void initDefaultActorComponentList(void)
{
    componentEntry defaults[] = { 
                    //Comp ID,      Address of Default
                    {eTypeID,       &ACTOR_ETYPE_DEFAULT},
                    {PositionID,    &POSITION_DEFAULT},
                    {VelocityID,    &VELOCITY_DEFAULT},
                    {RenderID,      &RENDER_DEFAULT}
                    {HealthID,      HEALTH_COMPONENT_DEFAULT}
    };
    
    numDefaultComponents = ARRAY_SIZE(defaults);
    for (int i = 0; i < numDefaultComponents; i++){
        defaultActorComponents[i] = defaults[i];
    }
    defaultActorComponentsLoaded = true;
}

// Returns a generic entity of the desired race;
// Components are attached with Default values. Modifications happen through
// buildActor().
entID newActor(int race)
{
    check(race <  NUM_OF_ALIEN_RACES && race >= 0, "Trying to fetch components for invalid race type.");
    entID theActor;
    WRECK(spawn, &theActor);
    
    //Lookup the components we want to assign to this entity. 
    List *cList = getComponents( race );
    check(cList, "Error retrieving components for race %d", race);

    // Move through the list of components, attach them one by one.
    componentEntry *new_c = (componentEntry *) cList->first;
    while (new_c != NULL){
        WRECK(setComponent, theActor, new_c->component, new_c->data);
    }
    freeList(cList);

    return newActor;

error:
    return 0;
}

void buildActor( entID theActor, int rank )
{
    eType_c *actorType;
    WRECK(getComponent, theActor, eTypeID, (void **)&actorType);
    check( strcmp( actorType->name, "NewActor") == 0, "buildActor failed: Actor already built!");

    applyRankModifiers(theActor);
    // LOOKUP TABLE HERE.
    // attach race_c in aliens?
    // DONT FORGET TO REGISTER RACE_C 



error:
    return;
}

//TODO GetRaceComponents();
static List *getComponents( int race)
{
    check(race <  NUM_OF_ALIEN_RACES && race >= 0, "Trying to fetch components for invalid race type.");
    // Check to make sure our components have been registered.
    if (defaultActorComponentsLoaded == false) {
        initDefaultActorComponentList();
    }

    // Add Default Actor Components to our list.
    List *cList = createList();
    for (int i = 0; i < numDefaultComponents; i++){
        listAppend( cList, &defaultActorComponents[i] );
    }
    
    // Add Race specific Components.
    raceComponents *raceSpecific = getRaceComponents( race );
    if (raceSpecific) {
        for (int i = 0; i < raceSpecific->num; i++){
            listAppend(cList, &raceSpecific->components[i]);
        }
    }

    return cList;

error:
    return NULL;
}

*/

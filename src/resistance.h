#ifndef RESISTANCE_H
#define RESISTANCE_H

#include "damage.h"
#include "wrecker.h"

// The ECS unique identifier for resistance components.
extern componentID ResistanceID;

struct resistanceComponent
{
    // Function pointer that you must set if you want to resist damage.
    void (*resist)(entID entityId, struct resistanceComponent *this, struct damageInstance *damage);
};
extern const struct resistanceComponent RESISTANCE_COMPONENT_DEFAULT;

/*
 * Reduce damage from a damage instance, modifying the damage instance in-place.
 */
void resistance_reduceDamage(entID entityId, struct resistanceComponent *resistance, struct damageInstance *damage);

#endif

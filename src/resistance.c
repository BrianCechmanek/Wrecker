#include "dbg.h"
#include "resistance.h"
#include "wrecker.h"

// The ECS unique identifier for resistance components.
componentID ResistanceID;

const struct resistanceComponent RESISTANCE_COMPONENT_DEFAULT = { 0 };

/*
 * Reduce damage from a damage instance, modifying the damage instance in-place.
 */
void resistance_reduceDamage(entID entityId, struct resistanceComponent *resistance, struct damageInstance *damage)
{
    // It's possible that the function pointer wasn't properly set
    if (resistance->resist)
    {
        // Call the custom resitance function
        resistance->resist(entityId, resistance, damage);
    }
    else
    {
        log_warn("unset resist member of resistanceComponent; damage won't be resisted");
    }
}

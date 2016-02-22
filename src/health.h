#ifndef HEALTH_H
#define HEALTH_H

#include "damage.h"
#include "wrecker.h"

// The ECS unique identifier for health components.
extern componentID HealthID;

struct healthComponent
{
    // Callback fired when a heal is applied to this health component
    void (*healed)(entID entityId, struct healthComponent *, int amount);
    // Callback fired when damage is applied to this health component, even if completely mitigated
    void (*damaged)(entID entityId, struct healthComponent *, struct damageInstance *damage);
    // Callback fired when this health component has current <= 0 after receiving damage
    void (*depleted)(entID entityId, struct healthComponent *);
    // Current amount of health that this entity has
    // Can be negative, but 0 or below is considered "depleted"
    int current;
    // Maximum health isn't a strict limit, you can choose to respect it
    unsigned max;
};
extern const struct healthComponent HEALTH_COMPONENT_DEFAULT;

/*
 * Apply healing to this health component.
 * If you want to respect the maximum health limit, set limit=true
 * Otherwise you can overheal
 */
void health_applyHealing(struct diana *diana, entID entityId, struct healthComponent *health, int amount, bool limit);

/*
 * Apply an instance of damage to this health component.
 * The health component will try to resist the damage using any other components attached to the entity.
 */
void health_applyDamage(struct diana *diana, entID entityId, struct healthComponent *health, struct damageInstance *damage);

#endif

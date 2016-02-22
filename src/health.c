#include "dbg.h"
#include "health.h"
#include "resistance.h"
#include "wrecker.h"

// The ECS unique identifier for health components.
componentID HealthID;

const struct healthComponent HEALTH_COMPONENT_DEFAULT = { 0, 0, 0, 0, 0 };

void health_applyHealing(struct diana *diana, entID entityId, struct healthComponent *health, int amount, bool limit)
{
    assert(amount > 0);
    int actualAmount = amount;

    // Constrain heals if we're supposed to respect the max health limit
    if (limit && health->current + amount > health->max)
    {
        actualAmount = health->max - health->current;
    }

    // Safety check, should never heal below 0
    if (actualAmount < 0)
    {
        actualAmount = 0;
    }

    health->current += actualAmount;

    if (health->healed)
    {
        health->healed(entityId, health, amount);
    }
}

static void __applyResistances(struct diana *diana, entID entityId, struct damageInstance *damage)
{
    // TODO it's worth being able to attach multiple resistance components to an entity and handling how they are applied here
    struct resistanceComponent *resistance = NULL;
    diana_getComponent(diana, entityId, ResistanceID, (void **)&resistance);
    if (resistance != NULL)
    {
        resistance_reduceDamage(entityId, resistance, damage);
    }
}

void health_applyDamage(struct diana *diana, entID entityId, struct healthComponent *health, struct damageInstance *damage)
{
    assert(damage->amount >= 0);

    // Before applying any sort of damage, resist it
    __applyResistances(diana, entityId, damage);

    // Safety check, should never damage below 0
    int amount = damage->amount;
    if (amount < 0)
    {
        amount = 0;
    }

    int prev = health->current;
    health->current -= amount;

    if (health->damaged)
    {
        health->damaged(entityId, health, damage);
    }

    if (prev > 0 && health->current <= 0 && health->depleted)
    {
        health->depleted(entityId, health);
    }
}

#ifdef TEST_DAMAGE
#include <diana.h>
#include <stdio.h>

#include "../health.h"
#include "../resistance.h"

static void __initDiana(struct diana **diana)
{
    allocate_diana(malloc, free, diana);
    diana_createComponent(*diana, "health", sizeof(struct healthComponent), DL_COMPONENT_FLAG_INLINE, &HealthID);
    diana_createComponent(*diana, "resitance", sizeof(struct resistanceComponent), DL_COMPONENT_FLAG_INLINE, &ResistanceID);
    diana_initialize(*diana);
}

static void __healed(entID entityId, struct healthComponent *health, int amount)
{
    printf("Entity %u healed by %d, current HP: %d\n", entityId, amount, health->current);
}

static void __damaged(entID entityId, struct healthComponent *health, struct damageInstance *damage)
{
    printf("Entity %u damaged by %d, current HP: %d\n", entityId, damage->amount, health->current);
}

static void __depleted(entID entityId, struct healthComponent *health)
{
    printf("Entity %u has been depleted of health, current HP: %d\n", entityId, health->current);
}

static void __resist(entID entityId, struct resistanceComponent *resist, struct damageInstance *damage)
{
    int reduction = 1;
    damage->amount -= reduction;
    printf("Entity %u Resisted %d damage\n", entityId, reduction);
}

static void __setupEntity(struct diana *diana, entID *e, struct healthComponent *health, struct resistanceComponent *resistance)
{
    diana_spawn(diana, e);
    diana_setComponent(diana, *e, HealthID, health);
    if (resistance != NULL)
    {
        diana_setComponent(diana, *e, ResistanceID, resistance);
    }
    diana_signal(diana, *e, DL_ENTITY_ADDED);
}

static void __testEntity(struct diana *diana, entID e)
{
    struct healthComponent *health;
    diana_getComponent(diana, e, HealthID, (void **)&health);

    struct damageInstance damage;
    damage.type = DAMAGE_TYPE_PHYSICAL;
    damage.amount = 1;
    health_applyDamage(diana, e, health, &damage);

    damage.amount = 10;
    health_applyDamage(diana, e, health, &damage);

    health_applyHealing(diana, e, health, 10, true);

    health_applyDamage(diana, e, health, &damage);
}

int main(void)
{
    struct diana *diana;
    __initDiana(&diana);

    // Build components for testing

    // Simple health component
    struct healthComponent health = HEALTH_COMPONENT_DEFAULT;
    health.healed = __healed;
    health.damaged = __damaged;
    health.depleted = __depleted;
    health.current = 10;
    health.max = 10;

    // Resistance component that doesn't reduce damage (default)
    struct resistanceComponent resistanceDefault = RESISTANCE_COMPONENT_DEFAULT;

    // Resistance component that reduces damage by a flat amount
    struct resistanceComponent resistance = RESISTANCE_COMPONENT_DEFAULT;
    resistance.resist = __resist;

    // Build entities for testing

    // Test an entity with no resistances
    entID e1;
    __setupEntity(diana, &e1, &health, NULL);
    __testEntity(diana, e1);

    // Test an entity with a resistance component that does nothing
    entID e2;
    __setupEntity(diana, &e2, &health, &resistanceDefault);
    __testEntity(diana, e2);

    // Build a test entity with a resistance component that does something
    entID e3;
    __setupEntity(diana, &e3, &health, &resistance);
    __testEntity(diana, e3);

    return 0;
}
#endif // TEST_DAMAGE

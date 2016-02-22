#ifndef DAMAGE_H
#define DAMAGE_H

// Enumerate different damage types
// TODO expand on these, hopefully a single enum is sufficient!
enum damageType
{
    DAMAGE_TYPE_PHYSICAL = 0,
    DAMAGE_TYPE_COUNT
};

// Store an instance of damage
// Damage should be rolled by revelant components, then passed around in instances of absolute damage
struct damageInstance
{
    enum damageType type;
    int amount;
};

#endif

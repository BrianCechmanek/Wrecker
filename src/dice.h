#ifndef _DICE_H
#define _DICE_H
/*
 * Wrecker  v0.01
 * Dice.h
 * Dice Rolling Functions, attached to our KRNG,
 */
#include "krng.h"
#include <stdint.h>

typedef struct Dice{
    int numRolls;
    int numFaces;
    float multiplier;
    float addsub;
} Dice;

Dice    new_rand_dice( char *s );
int     rand_rollDice( struct krng *rng, Dice *d);
int     rand_rollDice_s(struct krng *rng, char *s );
#endif

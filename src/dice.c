/*
 * Wrecker v.01
 * dice.c
 * systems for generating random numbers based on dice rolls.
 */
#include "dice.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "dbg.h"
#include <math.h>

/*
 *  Takes standard D&D dice input as string.
 *  Ex:
 *      new_rand_dice("3d6")
 */
Dice new_rand_dice( char *s )
{
    Dice d = {1, 1, 1.0f, 0.0f };
    char * p = s;
    char tmp[128];
    size_t l;
    // Get Multiplier
    if ( l = strcspn(p, "*x") < strlen(p)) {
        strcpy(tmp, p);
        tmp[l] = '\0';
        d.multiplier = atof(tmp);
        p += l + 1;
    }

    // Get Rolls
    l = strcspn(p, "dD");
    strcpy(tmp, p);
    tmp[l] = '\0';
    d.numRolls = atoi(tmp);
    p += 1+1;

    // Get Faces
    l = strcspn(p, "-+");
    strcpy(tmp, p);
    tmp[l] = '\0';
    d.numFaces = atoi(tmp);
    p += 1;

    // Get Add/Sub 
    if (strlen(p) > 0) {
        int sign = (*p == '+') ? 1 : (-1);
        p++;
        d.addsub = (float) (atof(p) * sign);
    }
    return d;
}

int rand_rollDice( struct krng *rng, Dice *d) 
{
    int rolls;
    int result = 0;
    for( rolls = 0; rolls < d->numRolls; rolls++){
        result += krng_rand_rangei( rng, 1, d->numFaces);
    }
    return (int) (result + d->addsub) * d->multiplier;
}

int rand_rollDice_s(struct krng *rng, char *s )
{
    Dice d = new_rand_dice(s);
    return rand_rollDice( rng, &d );
}

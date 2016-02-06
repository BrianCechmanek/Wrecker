#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdint.h>

typedef void *RNG;
/*
typedef struct RNG_c
{
    RNG instance;
} RNG_c;
*/
typedef enum {
    DISTRIBUTION_LINEAR,
    DISTRIBUTION_GAUSSIAN,
    DISTRUBITUION_GAUSSIAN_RANGE,
} RNG_distribution_t;

typedef struct Dice{
    int numRolls;
    int numFaces;
    float multiplier;
    float addsub;
} Dice;

typedef struct mersenneData_t{
    RNG_distribution_t distribution;
    uint64_t mt[312];
    int mti;
} mersenneData_t;

/*
 * RNG Init
 * Random Ints/float within range.
 * RNG destructor
 */

RNG     new_RNG( uint64_t seed );
int     rand_int(RNG instance, int min, int max);
float   rand_float(RNG instance, float min, float max);
double  rand_double(RNG instance, double min, double max);
void    delete_RNG( RNG instance );

/*
 * Dice rolling functions. 
 * Should work with input like "2 * 3d6 + 5"
 */

Dice    new_rand_dice( char *s );
int     rand_rollDice( RNG instance, Dice *d);
int     rand_rollDice_s(RNG instance, char *s );
#endif

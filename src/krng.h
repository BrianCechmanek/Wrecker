#ifndef KRNG_H
#define KRNG_H

#include <stdint.h>

// Struct to store KISS rng state
struct krng
{
    uint32_t z, w, jsr, jcong;
};

// Good default seed to use
static uint32_t KRNG_DEFAULT_SEED[] = { 12345, 65435, 34221, 12345 };

// Seed an rng with an array of 4 integers
void krng_seed(struct krng *krng, uint32_t *seed);

// Get a random integer in the range min <= x <= max
int krng_rand_rangei(struct krng *rng, int min, int max);

// Get a random float in the range 0 < x < 1
float krng_rand_unif(struct krng *rng);

// Get a random float in the range -1 < x < 1
float krng_rand_vnif(struct krng *rng);

#endif

#ifndef KRNG_H
#define KRNG_H

#include <stdint.h>

// Forward declaration
struct krng;

// Good default seed to use
uint32_t KRNG_DEFAULT_SEED[] = { 12345, 65435, 34221, 12345 };

// Seed krng with an array of 4 integers,
// returning that structure that will hold the rng state
struct krng krng_seed(uint32_t *seed);

// Get a random integer in the range min <= x <= max
unsigned krng_rand_int_range(struct krng *rng, unsigned min, unsigned max);

// Get a random float in the range 0 < x < 1
float krng_rand_unif(struct krng *rng);

// Get a random float in the range -1 < x < 1
float krng_rand_vnif(struct krng *rng);

#endif

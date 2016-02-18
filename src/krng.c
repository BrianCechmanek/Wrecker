#include "dbg.h"
#include "krng.h"

#include <stdio.h>

// KISS rng implementation from:
// https://programmingpraxis.com/2010/10/05/george-marsaglias-random-number-generators/

// Constants for computation
static const float KRNG_UNIF_CONST = 1.0f / (float)0xFFFFFFFF;
static const float KRNG_VNIF_CONST = 2.0f / (float)0xFFFFFFFF;

void krng_seed(struct krng *rng, uint32_t *seed)
{
    rng->z = seed[0];
    rng->w = seed[1];
    rng->jsr = seed[2];
    rng->jcong = seed[3];
}

// Macro math for inline code
#define znew  (z=36969*(z&65535)+(z>>16))
#define wnew  (w=18000*(w&65535)+(w>>16))
#define MWC   ((znew<<16)+wnew)
#define SHR3  (jsr^=(jsr<<17), jsr^=(jsr>>13), jsr^=(jsr<<5))
#define CONG  (jcong=69069*jcong+12345)
#define KISS  ((MWC^CONG)+SHR3)

static uint32_t krng_rand_int(struct krng *rng)
{
    uint32_t z = rng->z;
    uint32_t w = rng->w;
    uint32_t jsr = rng->jsr;
    uint32_t jcong = rng->jcong;

#if 0
// Alternative to macro math, if needed.
    z = 36969 * (z & 65535) + (z >> 16);
    w = 18000 * (w & 65535) + (w >> 16);
    uint32_t shr3;
    shr3 = jsr ^= jsr << 17, jsr ^= jsr >> 13, jsr ^= jsr << 5;
    uint32_t cong;
    cong = jcong = 69069 + jcong + 1234567;
    uint32_t mwc = (z << 16) + w;
    uint32_t kiss = (mwc ^ cong) + shr3;
#endif
    uint32_t kiss = KISS;

    rng->z = z;
    rng->w = w;
    rng->jsr = jsr;
    rng->jcong = jcong;

    return kiss;
}

unsigned krng_rand_int_range(struct krng *rng, unsigned min, unsigned max)
{
    assert(max >= min);
    float unif = krng_rand_unif(rng);
    float scaled = unif * (float)(max - min + 1);
    return ((unsigned)scaled) + min;
}

float krng_rand_unif(struct krng *rng)
{
    uint32_t kiss = krng_rand_int(rng);
    return kiss * KRNG_UNIF_CONST;
}

float krng_rand_vnif(struct krng *rng)
{
    // Because randomness is good across all bits in KISS,
    // the last bit can be converted into a sign using a cast
    int32_t kiss = (int32_t)krng_rand_int(rng);
    return kiss * KRNG_VNIF_CONST;
}

#ifdef TEST_KRNG
static void __reset_hits(unsigned *hits, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        hits[i] = 0;
    }
}

static char *__check_string(int hits, int min_tolerance, int max_tolerance)
{
    if (hits < min_tolerance)
    {
        return "(low)";
    }
    else if (hits > max_tolerance)
    {
        return "(high)";
    }
    return "";
}

int main()
{
    struct krng rng;
    krng_seed(&rng, KRNG_DEFAULT_SEED);
    int i;
    uint32_t check;
    for (i = 1; i < 1000001; i++)
    {
        check = krng_rand_int(&rng);
    }
    // FIXME this test should print 0, but it doesn't right now. I'll fix the code but it's not super important atm
    printf("%u %u\n", check, check-1372460312U);

    // Test the generators we will use in the game
    // The best way to test is to generte a LOT of numbers and check the distribution
    const int min = 1;
    const int max = 10;
    const int count = max - min + 1;
    const int iterations = 1000000;
    const float tolerance = .05f;
    const int expected_hits = iterations / count;
    const int min_tolerance = (1.0f - tolerance) * expected_hits;
    const int max_tolerance = (1.0f + tolerance) * expected_hits;
    unsigned hits[count] = { 0 };

    // Test krng_rand_int_range
    printf("Testing krng_rand_int_range\n");
    __reset_hits(hits, count);
    for (i = 0; i < iterations; i++)
    {
        uint32_t k = krng_rand_int_range(&rng, min, max);
        hits[k - min] += 1;
    }

    printf("Distribution after %u iterations:\n", iterations);
    for (i = 0; i < count; i++)
    {
        uint32_t k = i + min;
        char *check_result = __check_string(hits[i], min_tolerance, max_tolerance);
        printf("%u = %u %s\n", k, hits[i], check_result);
    }

    // Test krng_rand_unif
    printf("Testing krng_rand_unif\n");
    __reset_hits(hits, count);
    for (i = 0; i < iterations; i++)
    {
        float f = krng_rand_unif(&rng);
        int index = (int)(f * count);
        assert(index >= 0 && index < count);
        hits[index] += 1;
    }

    printf("Distribution after %u iterations:\n", iterations);
    const float unif_interval = 1.0f / (float)count;
    for (i = 0; i < count; i++)
    {
        float bottom = i * unif_interval;
        float top = (i + 1) * unif_interval;
        char *check_result = __check_string(hits[i], min_tolerance, max_tolerance);
        printf("%f .. %f = %u %s\n", bottom, top, hits[i], check_result);
    }

    // Test krng_rand_vnif
    printf("Testing krng_rand_vnif\n");
    __reset_hits(hits, count);
    for (i = 0; i < iterations; i++)
    {
        float f = krng_rand_vnif(&rng);
        float countf = (float)count;
        int index = (f * countf + countf) / 2.0f;
        // FIXME it's possible that exaclty 1.0 is generated,
        // which can cause the index to be out-of-range
        // This is only a floating point error, but shouldn't happen if it can be helped
        if (index >= count)
        {
            index = count - 1;
        }
        hits[index] += 1;
    }

    printf("Distribution after %u iterations:\n", iterations);
    const float vnif_interval = 1.0f / (float)count;
    for (i = 0; i < count; i++)
    {
        float bottom = (i * 2 - count) * vnif_interval;
        float top = ((i + 1) * 2 - count) * vnif_interval;
        char *check_result = __check_string(hits[i], min_tolerance, max_tolerance);
        printf("%f .. %f = %u %s\n", bottom, top, hits[i], check_result);
    }

    return 0;
}
#endif

#include "random.h"

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "dbg.h"

#define HIGH_BITS 0xFFFFFFFF80000000ULL
#define LOW_BITS 0x7FFFFFFFULL
#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ULL

static float rand_div=1.0f/(float)(0xffffffff);
static double rand_div_double = 1.0 / (double)(0xffffffff);

void twisterInit( uint64_t seed, mersenneData_t *d )
{
    d->mt[0] = seed;
    for (int i = 1; i < 312; i++){
        d->mt[i] = (6364136223846793005ULL * (d->mt[i-1] ^ (d->mt[i-1] >> 62)) + i);
    }
}

uint64_t mt_rand( mersenneData_t *d )
{
    int i;
    uint64_t x;
    uint64_t mag01[2] = {0ULL, MATRIX_A};

    if ( d->mti = 312 ){
        for (i = 0; i < 312 - MM; i++) {
            x = (d->mt[i]&HIGH_BITS) | ( d->mt[i+1] & LOW_BITS );
            d->mt[i] = d->mt[i+ MM] ^ ( x >> 1) ^ mag01[(int)(x&1ULL)];
        }
        for(;i < 312 - 1; i++){
            x = (d->mt[i] & HIGH_BITS) | (d->mt[i+1] & LOW_BITS);
            d->mt[i] = d->mt[i+(MM - 312)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
        }
        x = (d->mt[312 - 1 ] & HIGH_BITS) | (d->mt[0] & LOW_BITS);
        d->mt[312 - 1] = d->mt[MM - 1] ^ (x>>1) ^ mag01[(int)(x & 1ULL) ] ;

        d->mti = 0;
    }
    
    x = d->mt[d->mti++];

    x ^= (x >> 29) & 0x5555555555555555ULL;
    x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
    x ^= (x << 37) & 0xFFF7EEE000000000ULL;
    x ^= (x >> 43);

    return x;
}

/* Generates a float between 0, 1 */
    //return (float)mt_rand( r ) * rand_div;
static float random_f01( mersenneData_t *r)
{
    return (mt_rand( r ) >> 11) * (1.0 / 9007199254740991.0);
}

RNG  new_RNG( uint64_t seed )
{
    mersenneData_t *r = (mersenneData_t *) emalloc(sizeof(*r));
    memset(r, 0, sizeof(mersenneData_t));
    r->mti = 312;
    
    twisterInit( seed, r );
    r->distribution = DISTRIBUTION_LINEAR;
    return (RNG) r;
}

int rand_int(RNG instance, int min, int max){
    mersenneData_t *r;
    int delta;
    if (max == min) return min;
    else if (max < min) {
        int tmp = max;
        max = min;
        min = tmp;
    }
    r = (mersenneData_t *) instance;
    delta = max - min + 1;

    return (mt_rand( r ) % delta) + min;
}

float rand_float(RNG instance, float min, float max)
{
    mersenneData_t *r;
    float delta, f;
    if (max == min) return min;
    else if (max < min) {
        float tmp = max;
        max = min;
        min = tmp;
    }
    r = (mersenneData_t *) instance;
    delta = max - min ;
    f = delta * (float) random_f01( r );

    return min + f;
}

double rand_double(RNG instance, double min, double max)
{
    mersenneData_t *r;
    double delta, f;
    if (max == min) return min;
    else if (max < min) {
        double tmp = max;
        max = min;
        min = tmp;
    }
    r = (mersenneData_t *) instance;
    delta = max - min ;
    f = delta * (double)random_f01( r );

    return min + f;
}

void delete_RNG( RNG instance )
{
    if (instance == NULL ){ 
        log_info("Error deleting RNG instance.");
        return;
    }
    free(instance);
}

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

int rand_rollDice( RNG instance, Dice *d) 
{
    int rolls;
    int result = 0;
    for( rolls = 0; rolls < d->numRolls; rolls++){
        result += rand_int( instance, 1, d->numFaces);
    }
    return (int) (result + d->addsub) * d->multiplier;
}

int rand_rollDice_s(RNG instance, char *s )
{
    Dice d = new_rand_dice(s);
    return rand_rollDice( instance, &d );
}
/*
int main(void)
{
    RNG myRNG = new_RNG(987654321);
    int a;
    float b, f;
    uint64_t c;
    double d;

    c = mt_rand( (mersenneData_t*)myRNG );
    a = rand_int( myRNG, 1, 10);
    b = rand_float( myRNG, 1.0f, 10.0f);
    d = rand_double( myRNG, 1.0, 10.0);
    f = random_f01( (mersenneData_t*)myRNG );

    printf("A = %d\nB = %f\nC = %" PRIu64 "\nD = %lf \nF = %f\n", a, b, c, d, f);
    delete_RNG(myRNG);
    
    printf("Ten ints");
    for (int i = 0; i < 10; i++) printf(" %d", rand_int( myRNG, 1, 100) );
    printf("\nTen Doubles");
    for (int i = 0; i < 10; i++) printf(" %lf", rand_double( myRNG, 1.0, 5.0) );
    printf("\nTen Floats");
    for (int i = 0; i < 10; i++) printf(" %f", rand_double( myRNG, 1.0, 2.0) );

    return 0;
}
*/

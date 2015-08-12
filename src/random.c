#include <time.h>

#include "lib/mt19937ar.c"

#include "random.h"

unsigned long int seed() {
    unsigned long int seed_value = time(NULL);
    init_genrand(seed_value);
    return seed_value;
}

unsigned int roll_die(unsigned int max) { return genrand_int32() % max; }

unsigned int rand_range(unsigned int min, unsigned int max) {
    return (genrand_int32() % (max - min)) + min;
}

int32_t rand_delta() {
    int delta;
    delta = roll_die(4) - 1;
    if (delta < 1) {
        delta--;
    }
    return delta;
}

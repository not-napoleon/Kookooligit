#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <stdint.h>

unsigned long int seed();
unsigned int roll_die(unsigned int max);
unsigned int rand_range(unsigned int min, unsigned int max);
int rand_delta(); /* returns -2, -1, 1 or 2, for map stuff */

#endif

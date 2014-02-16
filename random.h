#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <stdint.h>

uint32_t seed();
uint32_t roll_die(uint32_t max);
uint32_t rand_range(uint32_t min, uint32_t max);

#endif
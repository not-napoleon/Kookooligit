#include <random.h>
#include <mtwist.h>
#include <mtwist.c>

uint32_t seed() {
  return mt_seed();
}


uint32_t roll_die(uint32_t max) {
  return mt_lrand() % max;
}

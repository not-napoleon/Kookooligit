#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <stdint.h>
#include <stdbool.h>

struct Player {
    unsigned int max_y;
    int curr_y;
    unsigned int turns;
    char *status;
    short int ticks;
    unsigned int stale_status : 1;
};

void init_player();
void free_player();
void incr_distance(bool forward);
void incr_turns();
char *get_player_status();
struct Creature *get_player_creature();

#endif

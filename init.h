#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED


#include <game.h>
#include <messages.h>
#include <map.h>


int get_configuration(GameConfiguration *config);
void initilize(GameState *state);
void free_game_state(GameState *state);
GameState *allocate_game_state();

#endif

#ifndef SEED_FILL_H_INCLUDED
#define SEED_FILL_H_INCLUDED

#include <map.h>
#include <tile.h>

/*
 * Fill space using a cellular automata "seed" model.  See:
 * https://code.google.com/p/jetblade/wiki/MapGeneration
 * for algorithm description
 */

void splat(MapSection *map, const int matrix_x, const int matrix_y,
    const TileTypeIndex *terrain_walls, const int terrain_count);
#endif

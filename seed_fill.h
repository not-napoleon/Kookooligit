#ifndef SEED_FILL_H_INCLUDED
#define SEED_FILL_H_INCLUDED

#include <map.h>
#include <tile.h>

void splat(MapSection *map, const int matrix_x, const int matrix_y,
    const TileTypeIndex *terrain_walls, const int terrain_count);
#endif

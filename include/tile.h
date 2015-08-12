#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <stdbool.h>

#include "sprite.h"

typedef enum {
    OffGrid,         // Space outside the map
    OpenSpace,       // passable, unoccupied space
    ImpassableWall,  // impassable wall
    TILE_TYPE_COUNT
} TileTypeIndex;

typedef struct TileType {

    /*Game properties*/
    unsigned int is_passable : 1;

    /*Basic graphics data*/
    char *description;
    char *name;
    enum sprite_ids sprite_id;

} TileType;

typedef struct Tile {
    /*
     * A map tile
     */
    // flags
    unsigned int is_explored : 1;
    unsigned int is_lit : 1;

    /* I feel weird about putting an ID reference here without importing
     * creature.h.
     */
    int creature_id;
    TileType *type;
} Tile;

TileType *tile_data[TILE_TYPE_COUNT];

const char *get_tile_description(TileType);
Tile *get_default_tile();
void init_tile_types();
void free_tile_types();
bool tiles_initilized();

#endif

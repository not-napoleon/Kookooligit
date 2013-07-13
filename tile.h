#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <stdbool.h>

typedef enum {
  OffGrid, // Space outside the map
  OpenSpace, // passable, unoccupied space
  ImpassableWall, // impassable wall
  RockWall, // 'asteroid' terrain wall
  RockCorridor, // 'asteroid' terrain open space
  TechWall, // 'constructed' terrain wall
  TechCorridor, // 'constructed' terrain open space
  TILE_TYPE_COUNT
} TileTypeIndex;

typedef struct TileType {

  /*Game properties*/
  unsigned int is_passable :1;

  /*Basic graphics data*/
  char *description;
  char *glyph; /* SDL expects a null terminated string to render */

} TileType;


typedef struct Tile {
  /*
   * A map tile
   */
  //flags
  unsigned int is_explored :1;
  unsigned int is_lit :1;

  TileType *type;
} Tile;

TileType *tile_data[TILE_TYPE_COUNT];

const char* get_tile_description(TileType);
void init_tile_types();
void free_tile_types();
bool tiles_initilized();

#endif

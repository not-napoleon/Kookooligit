#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

typedef enum {
  OffGrid, // Space outside the map
  OpenSpace, // passable, unoccupied space
  ImpassableWall, // impassable wall
  TILE_TYPE_COUNT
} TileType;

typedef struct Tile {
  /*
   * A map tile
   */
  //flags
  unsigned int is_passable :1;
  unsigned int is_explored :1;
  unsigned int is_lit :1;

  TileType type;
} Tile;

const char* get_tile_description(TileType);

#endif

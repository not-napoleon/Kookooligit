#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

typedef struct Point {
  /*
   * An x,y point on the map grid
   */
  int x;
  int y;
} Point;

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
  unsigned int contains_player :1;

  TileType type;
} Tile;


#define MAP_SECTION_SIZE 64

typedef struct MapSection {
  Tile matrix[MAP_SECTION_SIZE][MAP_SECTION_SIZE];
  Point center;
} MapSection;

int is_passable_point(MapSection * map, Point p);
int generate_map(MapSection *map);
int get_visible_region(MapSection *map, int at_width, int line_height,
    Point *top_left, Point *bottom_right);

#endif
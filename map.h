#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <stdbool.h>
#include <tile.h>
#include <map_section.h>

#define VISION_RADIUS 22
#define MAP_SECTION_BUFFER 5

typedef struct InfiniteMap {
  MapSection *section_list[MAP_SECTION_BUFFER];
  int current_section;
  Point at_location;
  Point cursor_location;
  Point camera_location;
} InfiniteMap;

InfiniteMap *init_infinite_map();
void free_infinite_map(InfiniteMap *map);
Tile get_tile(const InfiniteMap *map, int x, int y);
bool is_passable_point(InfiniteMap * map, Point p);
bool is_opaque_point(InfiniteMap *map, Point p);
int generate_initial_map(InfiniteMap *map);
int get_tile_grid(InfiniteMap *map, int window_x_chars, int window_y_chars,
    Point *at_location, Point *cursor_location, Tile **tile_grid);
bool attempt_move(InfiniteMap *map, int dx, int dy);

#endif

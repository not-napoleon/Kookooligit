#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <stdbool.h>
#include <tile.h>

typedef struct Point {
  /*
   * An x,y point on the map grid
   */
  int x;
  int y;
} Point;


#define VISION_RADIUS 22
#define MAP_SECTION_SIZE 64

typedef struct MapSection {
  Tile matrix[MAP_SECTION_SIZE][MAP_SECTION_SIZE];
  Point center;
  int x_size;
  int y_size;
  int *top_x_positions;
  int *top_widths;
  int *bottom_x_positions;
  int *bottom_widths;
} MapSection;

MapSection *init_map_section();
void free_map_section(MapSection *map);
Tile get_tile(const MapSection *map, int x, int y);
bool is_passable_point(MapSection * map, Point p);
bool is_opaque_point(MapSection *map, Point p);
int generate_map(MapSection *map, int *x_positions, int *widths);
int generate_initial_map(MapSection *map);
int get_visible_region(MapSection *map, int at_width, int line_height,
    Point *top_left, Point *bottom_right);

#endif

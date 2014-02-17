#ifndef MAP_SECTION_H_INCLUDED
#define MAP_SECTION_H_INCLUDED

#define MAP_SECTION_SIZE 64

#include <tile.h>

typedef struct Point {
  /*
   * An x,y point on the map grid
   */
  int x;
  int y;
} Point;

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
void dark_map_section(MapSection *map);
int generate_map_section(MapSection *map, int *x_positions, int *widths);

#endif

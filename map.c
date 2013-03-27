#include <stdio.h>
#include <map.h>

int is_passable_point(MapSection *map, Point p) {
  return map->matrix[p.x][p.y].is_passable;
}

int generate_map(MapSection *map) {
  int x;
  int y;
  for(x = 0; x < MAP_SECTION_SIZE; x++) {
    for(y = 0; y < MAP_SECTION_SIZE; y++) {
      if( ((((x/8) % 2) == 0) && (((y/8) % 2) == 0))
       || ( x == 0) || (x == 63) || (y == 0) || (y == 63) ){
        map->matrix[x][y].type = ImpassableWall;
        map->matrix[x][y].is_passable = 0;
      } else {
        map->matrix[x][y].type = OpenSpace;
        map->matrix[x][y].is_passable = 1;
      }
      map->matrix[x][y].is_explored = 0;
      map->matrix[x][y].contains_player = 0;
    }
  }
  printf("Map generated\n");
  return 0;
}

int get_visible_region(MapSection *map, int window_x_chars, int window_y_chars,
    Point *top_left, Point *bottom_right){
  printf("map center is (%i, %i)\n", map->center.x, map->center.y);
  top_left->x = map->center.x - (window_x_chars / 2);
  top_left->y = map->center.y - (window_y_chars / 2);

  bottom_right->x = map->center.x + (window_x_chars / 2);
  bottom_right->y = map->center.y + (window_y_chars / 2);

  printf("visible region spans (%i, %i) to (%i, %i)\n",
      top_left->x, top_left->y, bottom_right->x, bottom_right->y);

  return 0;
}

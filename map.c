#include <stdlib.h>
#include <stdio.h>
#include <map.h>
#include <fov.h>

bool is_passable_point(MapSection *map, Point p) {
  return map->matrix[p.x][p.y].is_passable;
}

bool is_opaque_point(MapSection *map, Point p) {
  return map->matrix[p.x][p.y].is_passable == 0;
}

bool wrapper_is_opaque(void *map, int x, int y) {
  /*
   * wrap native is_opaque to match libfov's required signature
   */
  Point p;
  p.x = x;
  p.y = y;
  return is_opaque_point( (MapSection*)map, p);
}

int dark_map(MapSection *map, int x_dimension, int y_dimension) {
  int x,y;
  printf("darkening map\n");
  for (x = 0; x < x_dimension; x++) {
    for (y = 0; y < y_dimension; y++){
      map->matrix[x][y].is_lit = 0;
    }
  }
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
    }
  }
  dark_map(map, MAP_SECTION_SIZE, MAP_SECTION_SIZE);
  printf("Map generated\n");
  return 0;
}

void light_tile(void *vmap, int x, int y, int dx, int dy, void *src) {
  /*
   * callback for lib fov to light a map tile
   */
  MapSection *map;
  map = (MapSection*)vmap;
  map->matrix[x][y].is_lit = 1;
  map->matrix[x][y].is_explored = 1;
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


int calculate_visible_tiles(MapSection *map, Point at_location) {
  dark_map(map, MAP_SECTION_SIZE, MAP_SECTION_SIZE);

  // make sure the tile the player is on is lit.  Player could land on an unlit
  // tile by, e.g., teleport.  Or game start.
  light_tile(map, at_location.x, at_location.y, 0, 0, NULL);

  printf("calculating field of vision\n");
  fov_settings_type *fov_settings;
  fov_settings = malloc( sizeof(fov_settings_type) );
  fov_settings_init(fov_settings);
  fov_settings_set_opacity_test_function(fov_settings, *wrapper_is_opaque);
  fov_settings_set_apply_lighting_function(fov_settings, *light_tile);

  fov_circle(fov_settings, map, NULL, at_location.x, at_location.y, VISION_RADIUS);

  fov_settings_free(fov_settings);
  free(fov_settings);
  return 0;
}




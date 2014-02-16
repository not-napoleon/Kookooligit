#include <stdlib.h>
#include <stdio.h>

#include <fov.h>
#include <map.h>
#include <random.h>

#define LOGGING_ENABLED
#include <log.h>

/*
 * Map Generation Parameters
 */

const int roughness = 30;
const int windiness = 30;
const int passes = 3;

MapSection *init_map_section() {
  MapSection *new;
  new = malloc(sizeof(MapSection));
  new->x_size = MAP_SECTION_SIZE;
  new->y_size = MAP_SECTION_SIZE;
  new->top_x_positions = (int *)malloc(sizeof(int) * passes);
  new->top_widths = (int *)malloc(sizeof(int) * passes);
  new->bottom_x_positions = (int *)malloc(sizeof(int) * passes);
  new->bottom_widths = (int *)malloc(sizeof(int) * passes);
  return new;
}

void free_map_section(MapSection *map) {
  free(map->top_x_positions);
  free(map->top_widths);
  free(map->bottom_x_positions);
  free(map->bottom_widths);
  free(map);
}

bool is_passable_point(MapSection *map, Point p) {
  return map->matrix[p.x][p.y].type->is_passable == 1;
}

bool is_opaque_point(MapSection *map, Point p) {
  if (map->matrix[p.x][p.y].type == NULL) {
    CRITICAL("Got NULL tile type data for coord %d, %d\n", p.x, p.y);
    exit(1);
  }
  return map->matrix[p.x][p.y].type->is_passable == 0;
}

bool wrapper_is_opaque(void *map, int x, int y) {
  /*
   * wrap native is_opaque to match libfov's required signature
   */
  Point p;
  p.x = x;
  p.y = y;
  bool res;
  res = is_opaque_point( (MapSection*)map, p);
  return res;
}

void dark_map(MapSection *map, int x_dimension, int y_dimension) {
  int x,y;
  TRACE("darkening map\n");
  for (x = 0; x < x_dimension; x++) {
    for (y = 0; y < y_dimension; y++){
      map->matrix[x][y].is_lit = 0;
    }
  }
}

int rand_delta() {
  int delta;
  delta = roll_die(4) - 1;
  if (delta < 1) {
    delta--;
  }
  return delta;
}

void extrude_tunnel_row(int *x, int *width, const int x_min, const int x_max,
    const int width_min, const int width_max) {
  /* Adjust track randomly */
  if (roll_die(100) < roughness) {
    *width += rand_delta();
  }
  if (roll_die(100) < windiness) {
    *x += rand_delta();
  }
  /* Correct to keep in bounds */
  if (*width < width_min) {
    *width = width_min;
  }
  if (*width > width_max) {
    *width = width_max;
  }
  if (*x > x_max) {
    *x = x_max;
  }
  if (*x < x_min) {
    *x = x_min;
  }

}

int generate_map(MapSection *map, int *x_positions, int *widths) {
  TRACE("Generating map\n");
  if (tiles_initilized == false) {
    CRITICAL("Generate map called with uninitilized tile data\n");
    exit(1);
  }
  int x;
  int y;
  for(x = 0; x < map->x_size; x++) {
    for(y = 0; y < map->y_size; y++) {
      // start with a blank map
      map->matrix[x][y].type = tile_data[ImpassableWall];
      map->matrix[x][y].is_explored = 1;
    }
  }

  int width;
  int i;

  const int y_max = map->y_size - 2;
  const int y_min = 1;
  for(i = 0; i < passes; i++){
    width = widths[i];
    x = x_positions[i];
    /* bottom to top loop */
    int y_start = y_max;
    int y_end = y_min;
    for(y = y_start; y >= y_end; (y_start < y_end ? y++ : y--)) {
      int tmp_x;
      DEBUG("y: %d, x: %d, width: %d\n", y, x, width);
      extrude_tunnel_row(&x, &width,
          2, map->x_size - 5,  /* x min & max */
          3, map->x_size);  /* width min & max */
      for (tmp_x = x; tmp_x < x + width; tmp_x++) {
        if (tmp_x >= map->x_size - 2) {
          break;
        }
        map->matrix[tmp_x][y].type = tile_data[OpenSpace];
      }
      if (y == y_min) {
        map->top_x_positions[i] = x;
        map->top_widths[i] = width;
      }
      if (y == y_max) {
        map->bottom_x_positions[i] = x;
        map->bottom_widths[i] = x;
      }
    }
  }
  // Mark the whole map as unlit
  dark_map(map, map->x_size, map->y_size);
  INFO("Map generated\n");
  return 0;
}

int generate_initial_map(MapSection *map) {
  /* Generate a map section with random starting values */
  int *x_positions;
  int *widths;
  int i;
  x_positions = (int *)malloc(sizeof(int) * passes);
  widths = (int *)malloc(sizeof(int) * passes);

  for(i = 0; i < passes; i++){
    widths[i] = rand_range(3, (map->x_size / passes) - 2);
    x_positions[i] = roll_die(map->x_size / passes) * (i + 1);
  }
  generate_map(map, x_positions, widths);
  free(x_positions);
  free(widths);
  return 0;
}


Tile get_tile(const MapSection *map, int x, int y) {
  if (x < 0 || y < 0 || x >= map->x_size || y >= map->y_size) {
    /*
     * Asked for an out-of-bounds tile, return OffGrid
     */
    return (Tile){0, 0, tile_data[OffGrid]};
  } else {
    return map->matrix[x][y];
  }
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
  DEBUG("map center is (%i, %i)\n", map->center.x, map->center.y);
  top_left->x = map->center.x - (window_x_chars / 2);
  top_left->y = map->center.y - (window_y_chars / 2);

  bottom_right->x = map->center.x + (window_x_chars / 2);
  bottom_right->y = map->center.y + (window_y_chars / 2);

  DEBUG("visible region spans (%i, %i) to (%i, %i)\n",
      top_left->x, top_left->y, bottom_right->x, bottom_right->y);

  return 0;
}


int calculate_visible_tiles(MapSection *map, Point at_location) {
  dark_map(map, map->x_size, map->y_size);

  // make sure the tile the player is on is lit.  Player could land on an unlit
  // tile by, e.g., teleport.  Or game start.
  light_tile(map, at_location.x, at_location.y, 0, 0, NULL);

  DEBUG("calculating field of vision\n");
  fov_settings_type *fov_settings;
  fov_settings = malloc( sizeof(fov_settings_type) );
  fov_settings_init(fov_settings);
  fov_settings_set_opacity_test_function(fov_settings, *wrapper_is_opaque);
  fov_settings_set_apply_lighting_function(fov_settings, *light_tile);

  fov_circle(fov_settings, map, NULL, at_location.x, at_location.y, VISION_RADIUS);

  fov_settings_free(fov_settings);
  free(fov_settings);
  TRACE("Field of vision finished\n");
  return 0;
}




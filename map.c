#include <stdlib.h>
#include <stdio.h>

#include <fov.h>
#include <map.h>
#include <random.h>

#define LOGGING_ENABLED
#include <log.h>

InfiniteMap *init_infinite_map() {
  InfiniteMap *new;
  new = (InfiniteMap *)malloc(sizeof(InfiniteMap));
  int i;
  for (i = 0; i < MAP_SECTION_BUFFER; i++) {
    new->section_list[i] = init_map_section();
  }
  return new;
}

void free_infinite_map(InfiniteMap *map) {
  int i;
  for (i = 0; i < MAP_SECTION_BUFFER; i++) {
    free_map_section(map->section_list[i]);
  }
  free(map);
}

Tile get_tile(const InfiniteMap *map, const int x, const int y) {
  //TRACE("Getting tile (%d, %d)\n", x, y);
  MapSection *sec = map->section_list[map->current_section];
  if (x < 0 || y < 0 || x >= sec->x_size || y >= sec->y_size) {
    /*
     * Asked for an out-of-bounds tile, return OffGrid
     */
    //TRACE("Defaulting to offgrid tile");
    return (Tile){0, 0, tile_data[OffGrid]};
  } else {
    return sec->matrix[x][y];
  }
}

bool is_passable_point(InfiniteMap *map, Point p) {
  return get_tile(map, p.x, p.y).type->is_passable == 1;
}

bool is_opaque_point(InfiniteMap *map, Point p) {
  return get_tile(map, p.x, p.y).type->is_passable == 0;
}

bool wrapper_is_opaque(void *map, int x, int y) {
  /*
   * wrap native is_opaque to match libfov's required signature
   */
  Point p;
  p.x = x;
  p.y = y;
  bool res;
  res = is_opaque_point( (InfiniteMap*)map, p);
  return res;
}

void dark_map(InfiniteMap *map) {
  int i;
  for (i = 0; i < MAP_SECTION_BUFFER; i++) {
    dark_map_section(map->section_list[i]);
  }
}

int generate_initial_map(InfiniteMap *map) {
  /* Start in the middle of the buffer */
  map->current_section = 2;
  /* Generate the first map section with random starting values */
  int *x_positions;
  int *widths;
  int i;
  x_positions = (int *)malloc(sizeof(int) * 3);
  widths = (int *)malloc(sizeof(int) * 3);

  MapSection *sec = map->section_list[map->current_section];

  for(i = 0; i < 3; i++){
    widths[i] = rand_range(3, (sec->x_size / 3) - 2);
    x_positions[i] = roll_die(sec->x_size / 3) * (i + 1);
  }
  generate_map_section(sec, x_positions, widths);
  free(x_positions);
  free(widths);

  /* TODO: Build out the other four sections */
  /* set cursor and at locations */
  map->at_location.y = map->section_list[map->current_section]->y_size - 2;
  for (map->at_location.x = 0;
      map->at_location.x < map->section_list[map->current_section]->x_size;
      map->at_location.x++){
    DEBUG("Considering start position (%d, %d)\n", map->at_location.x, map->at_location.y);
    if (is_passable_point(map, map->at_location)){
      break;
    }
  }
  map->cursor_location = map->at_location;
  map->camera_location = map->at_location;
  return 0;
}


void light_tile(void *vmap, int x, int y, int dx, int dy, void *src) {
  /*
   * callback for lib fov to light a map tile
   */
  InfiniteMap *map;
  map = (InfiniteMap*)vmap;
  MapSection *sec;
  sec = map->section_list[map->current_section];
  sec->matrix[x][y].is_lit = 1;
  sec->matrix[x][y].is_explored = 1;
}


int get_tile_grid(InfiniteMap *map, const int window_x_chars, const int window_y_chars,
    Point *at_location, Point *cursor_location, Tile **tile_grid) {
  /*
   * Get a grid of tiles, and the at_location and cursor location relative to
   * that grid, centered on the current camera location and dimensioned as
   * window_x_characters by window_y_characters.
   *
   * Basically, gets the set of tiles to render.
   */

  /* TODO: Use camera location here, not at location */
  const int x_start = map->at_location.x - (window_x_chars / 2);
  const int x_end = map->at_location.x + (window_x_chars / 2);
  const int y_start = map->at_location.y - (window_y_chars / 2);
  const int y_end = map->at_location.y + (window_y_chars / 2);

  DEBUG("Selecting map tiles from (%d, %d) to (%d, %d)\n", x_start, y_start, x_end, y_end);
  DEBUG("at_location is %d, %d\n", map->at_location.x, map->at_location.y);
  int x, y;
  Tile t;
  for (x = x_start; x <= x_end; x++) {
    for (y = y_start; y <= y_end; y++) {
      /*DEBUG("looking for tile at (%d, %d), storing at (%d, %d)\n",*/
          /*x, y, x - x_start, y - y_start);*/
      t = get_tile(map, x, y);
      tile_grid[x - x_start][y - y_start] = t;
    }
  }
  at_location->x = map->at_location.x - x_start;
  at_location->y = map->at_location.y - y_start;
  cursor_location->x = map->cursor_location.x - x_start;
  cursor_location->y = map->cursor_location.y - y_start;
  return 0;
}


int calculate_visible_tiles(InfiniteMap *map, Point at_location) {
  DEBUG("calculating field of vision\n");
  dark_map(map);
  // make sure the tile the player is on is lit.  Player could land on an unlit
  // tile by, e.g., teleport.  Or game start.
  light_tile(map, at_location.x, at_location.y, 0, 0, NULL);
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

bool attempt_move(InfiniteMap *map, int dx, int dy) {
  /* TODO: Support moving things other than the at */
  Point target_point;
  target_point = map->at_location;

  target_point.x = target_point.x + dx;
  target_point.y = target_point.y + dy;

  if (get_tile(map, target_point.x, target_point.y).type->is_passable == 1) {
    /* Move allowed */
    map->at_location = target_point;
    return true;
  } else {
    return false;
  }
}

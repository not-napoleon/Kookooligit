#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "fov.h"
#include "map.h"
#include "random.h"
#include "sprite.h"

/*#define LOGGING_ENABLED*/
#include "log.h"

void dump_edge_parameters(InfiniteMap *map) {
  int i, j;
  MapSection *sec;
  for(i = 0; i < MAP_SECTION_BUFFER; i++) {
    sec = map->section_list[i];
    DEBUG("Section %d top:    x1=%d, w1=%d;\tx2=%d, w2=%d;\tx3=%d, w3=%d\n", i,
        sec->top_x_positions[0], sec->top_widths[0],
        sec->top_x_positions[1], sec->top_widths[1],
        sec->top_x_positions[2], sec->top_widths[2]);
    DEBUG("Section %d bottom: x1=%d, w1=%d;\tx2=%d, w2=%d;\tx3=%d, w3=%d\n", i,
        sec->bottom_x_positions[0], sec->bottom_widths[0],
        sec->bottom_x_positions[1], sec->bottom_widths[1],
        sec->bottom_x_positions[2], sec->bottom_widths[2]);
  }
}

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

int roto_indx(int indx, int slots) {
  int r = indx % slots;
  return r < 0 ? slots + r : r;
}

Tile get_tile(const InfiniteMap *map, int x, int y) {
  MapSection *sec = map->section_list[map->current_section];
  if (x < 0 || x >= sec->x_size) {
    /*
     * Asked for an out-of-bounds tile, return OffGrid
     */
    //TRACE("Defaulting to offgrid tile");
    /* TODO: DON'T FUCKING DO THIS HERE! */
    return (Tile){0, 0, 0, tile_data[OffGrid]};
  }
  /*TRACE("Getting tile (%d, %d)\n", x, y);*/
  if (y < 0) {
    int section_index = roto_indx((map->current_section - 1), MAP_SECTION_BUFFER);
    sec = map->section_list[section_index];
    y = sec->y_size + y;
  } else if (y >= sec->y_size) {
    y = y - sec->y_size;
    int section_index = roto_indx((map->current_section + 1), MAP_SECTION_BUFFER);
    sec = map->section_list[section_index];
  }
  /*TRACE("mapped to (%d, %d)\n", x, y);*/
  return sec->matrix[x][y];
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
  return is_opaque_point( (InfiniteMap*)map, p);
}

void dark_map(InfiniteMap *map) {
  int i;
  for (i = 0; i < MAP_SECTION_BUFFER; i++) {
    dark_map_section(map->section_list[i]);
  }
}

int generate_initial_map(InfiniteMap *map) {
  /* Generate the first map section with random starting values */
  int *x_positions;
  int *widths;
  int i;
  x_positions = (int *)malloc(sizeof(int) * 3);
  widths = (int *)malloc(sizeof(int) * 3);

  MapSection *sec = map->section_list[0];

  DEBUG("Generating section 0\n");
  for(i = 0; i < 3; i++){
    widths[i] = rand_range(3, (sec->x_size / 3) - 2);
    x_positions[i] = roll_die(sec->x_size / 3) * (i + 1);
  }
  generate_map_section(sec, x_positions, widths, false);
  free(x_positions);
  free(widths);

  dump_edge_parameters(map);

  /* Build out the other four sections */
  for(i = 1; i < MAP_SECTION_BUFFER; i++) {
    DEBUG("Generating section %d\n", i);
    generate_map_section(
        map->section_list[i],
        map->section_list[i-1]->bottom_x_positions,
        map->section_list[i-1]->bottom_widths, false);
    dump_edge_parameters(map);
  }
  /* set cursor and at locations */
  map->current_section = 2;
  map->at_location.y = map->section_list[map->current_section]->y_size - 2;
  for (map->at_location.x = 0;
      map->at_location.x < map->section_list[map->current_section]->x_size;
      map->at_location.x++){
    DEBUG("Considering start position (%d, %d)\n", map->at_location.x, map->at_location.y);
    /* TODO: Deal with not finding a start position; it's a bug, so exit*/
    if (is_passable_point(map, map->at_location)){
      break;
    }
  }
  map->cursor_location = map->at_location;
  map->camera_location.y = map->at_location.y;
  /* TODO: calculate this as the center of the map */
  map->camera_location.x = 33;
  return 0;
}


void light_tile(void *vmap, int x, int y, int dx, int dy, void *src) {
  /*
   * callback for lib fov to light a map tile
   */
  InfiniteMap *map;
  map = (InfiniteMap*)vmap;
  MapSection *sec = map->section_list[map->current_section];
  /*DEBUG("Lighting tile at %d, %d\n", x, y);*/
  if (y < 0) {
    /*DEBUG("y < 0 case\n");*/
    int section_index = roto_indx((map->current_section - 1), MAP_SECTION_BUFFER);
    sec = map->section_list[section_index];
    y = sec->y_size + y;
  } else if (y >= sec->y_size) {
    /*DEBUG("y >= y_size case\n");*/
    y = y - sec->y_size;
    int section_index = roto_indx((map->current_section + 1), MAP_SECTION_BUFFER);
    sec = map->section_list[section_index];
  } else {
    /*DEBUG("Default case\n");*/
  }
  /*DEBUG("remapped to at %d, %d\n", x, y);*/
  sec->matrix[x][y].is_lit = 1;
  sec->matrix[x][y].is_explored = 1;
}

int get_tile_grid(InfiniteMap *map, const int window_x_chars,
    const int window_y_chars, const bool draw_cursor,
    struct Drawable **tile_grid) {
  /*
   * Get a grid of tiles, and the at_location and cursor location relative to
   * that grid, centered on the current camera location and dimensioned as
   * window_x_characters by window_y_characters.
   *
   * Basically, gets the set of tiles to render.
   */

  const int x_start = map->camera_location.x - (window_x_chars / 2);
  const int x_end = map->camera_location.x + (window_x_chars / 2);
  const int y_start = map->camera_location.y - (window_y_chars / 2);
  const int y_end = map->camera_location.y + (window_y_chars / 2);

  DEBUG("Selecting map tiles from (%d, %d) to (%d, %d)\n", x_start, y_start, x_end, y_end);
  DEBUG("at_location is %d, %d\n", map->at_location.x, map->at_location.y);
  DEBUG("camera adjusted x range is %d to %d\n", x_start, x_end);
  DEBUG("camera adjusted y range is %d to %d\n", y_start, y_end);
  int x, y;
  Tile t;
  struct Drawable d;
  for (x = x_start; x <= x_end; x++) {
    for (y = y_start; y <= y_end; y++) {
      DEBUG("looking for tile at (%d, %d), storing at (%d, %d)\n",
          x, y, x - x_start, y - y_start);
      /* we always need the tile to get lighting info */
      t = get_tile(map, x, y);
      d.is_lit = t.is_lit;
      d.is_explored = t.is_explored;

      /* Figure out what to draw on this square */
      if (x == map->at_location.x 
          && y == map->at_location.y) {
        /* Draw player case */
        DEBUG("Drawing player at %d, %d\n", x, y);
        d.sprite_id = Player_sprite;
      } else {
      /* Draw Tile case */
        d.sprite_id = t.type->sprite_id;
      }

      /* Decide if we should show the cursor */
      if (draw_cursor
          && x == map->cursor_location.x
          && y == map->cursor_location.y) {
        DEBUG("Drawing cursor at %d, %d\n", x, y);
        d.draw_cursor = 1;
      } else {
        d.draw_cursor = 0;
      }
      tile_grid[x - x_start][y - y_start] = d;
    }
  }
  return 0;
}


int calculate_visible_tiles(InfiniteMap *map, Point at_location) {
  /*DEBUG("calculating field of vision\n");*/
  dark_map(map);
  /* make sure the tile the player is on is lit.  Player could land on an unlit
   * tile by, e.g., teleport.  Or game start. */
  light_tile(map, at_location.x, at_location.y, 0, 0, NULL);
  fov_settings_type *fov_settings;
  fov_settings = malloc( sizeof(fov_settings_type) );
  fov_settings_init(fov_settings);
  fov_settings_set_opacity_test_function(fov_settings, *wrapper_is_opaque);
  fov_settings_set_apply_lighting_function(fov_settings, *light_tile);

  fov_circle(fov_settings, map, NULL, at_location.x, at_location.y, VISION_RADIUS);

  fov_settings_free(fov_settings);
  free(fov_settings);
  /*TRACE("Field of vision finished\n");*/
  return 0;
}

bool attempt_move(InfiniteMap *map, int delta_x, int delta_y) {
  /* TODO: Support moving things other than the at */
  Point target_point;
  target_point = map->at_location;

  target_point.x = target_point.x + delta_x;
  target_point.y = target_point.y + delta_y;

  const int offset = MAP_SECTION_BUFFER / 2;
  if (get_tile(map, target_point.x, target_point.y).type->is_passable == 1) {
    /* Move allowed */

    /* Rotate sections if necessary
     * TODO: Refactor this
     */
    if (target_point.y < 0) {
      int to_discard = roto_indx((map->current_section + offset), MAP_SECTION_BUFFER);
      int edge = roto_indx((map->current_section - offset), MAP_SECTION_BUFFER);
      map->current_section = roto_indx((map->current_section - 1), MAP_SECTION_BUFFER);
      DEBUG("Moving to section %d, recycling section %d, spawning new edge "
          "from %d\n", map->current_section, to_discard, edge);
      DEBUG("Before section generation:\n");
      dump_edge_parameters(map);
      generate_map_section(map->section_list[to_discard],
          map->section_list[edge]->top_x_positions,
          map->section_list[edge]->top_widths, true);
      target_point.y += map->section_list[map->current_section]->y_size;
      DEBUG("After section generation:\n");
      dump_edge_parameters(map);
    } else if (target_point.y >= map->section_list[map->current_section]->y_size) {
      int to_discard = roto_indx((map->current_section - offset), MAP_SECTION_BUFFER);
      int edge = roto_indx((map->current_section + offset), MAP_SECTION_BUFFER);
      target_point.y -= map->section_list[map->current_section]->y_size;
      map->current_section = roto_indx((map->current_section + 1), MAP_SECTION_BUFFER);
      DEBUG("Moving to section %d, recycling section %d, spawning new edge "
          "from %d\n", map->current_section, to_discard, edge);
      DEBUG("Before section generation:\n");
      dump_edge_parameters(map);
      generate_map_section(map->section_list[to_discard],
          map->section_list[edge]->bottom_x_positions,
          map->section_list[edge]->bottom_widths, false);
      DEBUG("After section generation:\n");
      dump_edge_parameters(map);
    }

    map->at_location = target_point;
    map->camera_location.y = map->at_location.y;
    return true;
  } else {
    return false;
  }
}

bool attempt_cursor_move(InfiniteMap *map, int delta_x, int delta_y,
    int x_bound, int y_bound) {
  /* Seems like passing the x & y bounds (of the visibile window region) here
   * is somewhat backwards.  Mind this for a possible refactor*/
  Point target_point;
  target_point = map->cursor_location;

  target_point.x = target_point.x + delta_x;
  target_point.y = target_point.y + delta_y;

  DEBUG("attempting to move cursor to %d, %d\n", target_point.x,
      target_point.y);
  if (target_point.x < map->camera_location.x - (x_bound / 2)
      || target_point.x > map->camera_location.x + (x_bound / 2)
      || target_point.y < map->camera_location.y - (y_bound / 2)
      || target_point.y > map->camera_location.y + (y_bound / 2)) {
    DEBUG("Cursor move rejected\n");
    DEBUG("Bounds are x: %d - %d, y: %d - %d\n",
      map->camera_location.x - (x_bound / 2),
      map->camera_location.x + (x_bound / 2),
      map->camera_location.y - (y_bound / 2),
      map->camera_location.y + (y_bound / 2));

    return false;
  }
  map->cursor_location = target_point;
  return true;
}

Tile get_cursor_tile(InfiniteMap *map) {
  /* Return the tile under the cursor */
  return get_tile(map, map->cursor_location.x, map->cursor_location.y);
}

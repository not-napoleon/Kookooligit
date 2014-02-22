#include <stdlib.h>

#include <map_section.h>
#include <random.h>
#include <tile.h>

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

void dark_map_section(MapSection *map) {
  int x,y;
  TRACE("darkening map section\n");
  for (x = 0; x < map->x_size; x++) {
    for (y = 0; y < map->y_size; y++){
      map->matrix[x][y].is_lit = 0;
    }
  }
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

int generate_map_section(MapSection *map, int *x_positions, int *widths) {
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

  const int y_max = map->y_size - 1;
  const int y_min = 0;
  for(i = 0; i < passes; i++){
    width = widths[i];
    x = x_positions[i];
    /* bottom to top loop */
    int y_start = y_max;
    int y_end = y_min;
    DEBUG("Start of pass %d, w: %d, x: %d\n", i, width, x);
    for(y = y_start; y >= y_end; (y_start < y_end ? y++ : y--)) {
      int tmp_x;
      /*DEBUG("y: %d, x: %d, width: %d\n", y, x, width);*/
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
        DEBUG("Saving top postition for pass %d as w: %d, x: %d\n", i, width, x);
        map->top_x_positions[i] = x;
        map->top_widths[i] = width;
      }
      if (y == y_max) {
        DEBUG("Saving bottom position for pass %d as w: %d, x: %d\n", i, width, x);
        map->bottom_x_positions[i] = x;
        map->bottom_widths[i] = width;
      }
    }
  }
  // Mark the whole map as unlit
  dark_map_section(map);
  INFO("Map generated\n");
  return 0;
}

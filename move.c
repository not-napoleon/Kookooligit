#include "move.h"
#include "map.h"
#include "tile.h"

bool attempt_move(InfiniteMap *map, int delta_x, int delta_y) {
  /* TODO: Support moving things other than the at */
  Point target_point;
  target_point = map->at_location;

  target_point.x = target_point.x + delta_x;
  target_point.y = target_point.y + delta_y;
  Tile *target_tile = get_tile(map, target_point.x, target_point.y);

  if (target_tile->type->is_passable == 1) {
    /* Move allowed */

    /* Rotate sections if necessary
     */
    if (target_point.y < 0) {
      rotate_buffers(map, false);
      target_point.y += map->section_list[map->current_section]->y_size;
    } else if (target_point.y >= map->section_list[map->current_section]->y_size) {
      rotate_buffers(map, true);
      target_point.y -= map->section_list[map->current_section]->y_size;
    }

    map->at_location = target_point;
    map->camera_location.y = map->at_location.y;
    return true;
  } else {
    return false;
  }
}

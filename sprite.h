#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <point.h>

/* This is basically the master list of everything that can be drawn.  As such,
 * it's roughly the concatenation of the list of tiles, creatures and items,
 * but keeping it as it's own list seems helpful.
 */
enum sprite_ids {
  Player_sprite,
  OffGrid_sprite,
  OpenSpace_sprite,
  ImpassableWall_sprite,
  SPRITE_TYPE_COUNT
};


void draw_sprite_at_point(enum sprite_ids sprite_id, Point p);
void init_sprites();


#endif

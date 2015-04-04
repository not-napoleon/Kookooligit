#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include <stdint.h>

#include "graphics_wrapper.h"

/* This is basically the master list of everything that can be drawn.  As such,
 * it's roughly the concatenation of the list of tiles, creatures and items,
 * but keeping it as it's own list seems helpful.
 */
enum sprite_ids {
  Player_sprite,
  OffGrid_sprite,
  OpenSpace_sprite,
  ImpassableWall_sprite,
  BattleBot_sprite,
  SPRITE_TYPE_COUNT
};


struct Drawable {
  /* Something that we can draw.  Basically, a sprite ID + flags for cursor
   * and lighting, for now.
   */
  enum sprite_ids sprite_id;
  uint8_t draw_cursor :1; /* Set the background to cursor color */
  uint8_t is_lit :1;      /* Used to set the darkness of the sprite */
  uint8_t is_explored :1; /* Don't draw if not explored */
};

void draw_sprite_at_point(const struct Drawable *drawable, const int x,
    const int y, const Rect *window);
void init_sprites_from_font(const char *font_path, const int point_size);
void close_sprites();
int get_char_width(const int pixel_width);
int get_char_height(const int pixel_height);

#endif

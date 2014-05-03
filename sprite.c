#include "SDL.h"
#include "SDL_ttf.h"

#include <stdbool.h>

#include <color_palette.h>
#include <log.h>
#include <graphics_wrapper.h>
#include <sprite.h>
#include <SDL_tools.h>


static bool _was_initilized = false;
SDL_Texture *sprite_cache[SPRITE_TYPE_COUNT];

void draw_sprite_at_point(struct Drawable *drawable, Point p) {
  if (!_was_initilized) {
    CRITICAL("Tried to draw an uninitilized sprite\n");
    exit(1);
  }
  /* TODO: Draw cursor if applicable */

  /* If the tile is unexplored, nothing left to do, leave it blank and go
   * home. */
  if (drawable->is_explored == 0) {
    return;
  }
  SDL_Texture *glyph = sprite_cache[drawable->sprite_id];
  /* Darken (which is to say make transparent) if the tile is unlit. */
  if (drawable->is_lit == 0) {
    SDL_SetTextureAlphaMod(glyph, 128);
  } else {
    SDL_SetTextureAlphaMod(glyph, 255);
  }

  /* TODO: Stage the texture for drawing */
  SDL_Rect write_coords;
  write_coords.x = p.x;
  write_coords.y = p.y;
  /* TODO: Get at_width and line height here */
  SDL_RenderCopy(get_main_renderer(), sprite_cache[drawable->sprite_id],
      NULL, &write_coords);

}

SDL_Texture *load_sprite_from_font(const char *glyph, const Color fg) {
  TTF_Font* map_font = get_map_font();
  SDL_Surface *tmp = TTF_RenderText_Solid(map_font, glyph, convert_color(fg));
  SDL_Texture *retval = SDL_CreateTextureFromSurface(get_main_renderer(), tmp);
  SDL_FreeSurface(tmp);
  return retval;
}


void init_sprites() {
  sprite_cache[Player_sprite] = load_sprite_from_font("@", color_lit);
  sprite_cache[OffGrid_sprite] = load_sprite_from_font(" ", color_lit);
  sprite_cache[OpenSpace_sprite] = load_sprite_from_font(".", color_lit);
  sprite_cache[ImpassableWall_sprite] = load_sprite_from_font("#", color_lit);
}

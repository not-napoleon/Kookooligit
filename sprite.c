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

void draw_sprite_at_point(enum sprite_ids sprite_id, Point p,
    bool show_cursor, bool darken) {
  if (!_was_initilized) {
    CRITICAL("Tried to draw an uninitilized sprite\n");
    exit(1);
  }
  SDL_Texture *glyph = sprite_cache[sprite_id];
  if (darken) {
    SDL_SetTextureAlphaMod(glyph, 128);
  } else {
    SDL_SetTextureAlphaMod(glyph, 255);
  }

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

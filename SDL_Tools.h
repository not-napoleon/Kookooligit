/* 
 * Tools for converting between internal representations and SDL data types
 */

#ifndef SDL_TOOLS_H_INCLUDED
#define SDL_TOOLS_H_INCLUDED

#include <graphics_wrapper.h>
#include "SDL.h"
#include "SDL_ttf.h"

SDL_Color convert_color(Color c);
void set_map_font(const char *font_path, const int point_size);
void set_message_font(const char *font_path, const int point_size);
void set_status_font(const char *font_path, const int point_size);
void set_command_font(const char *font_path, const int point_size);

TTF_Font* get_map_font();
TTF_Font* get_message_font();
TTF_Font* get_status_font();
TTF_Font* get_command_font();

void close_fonts();

#endif

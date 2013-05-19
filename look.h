#ifndef LOOK_H_INCLUDED
#define LOOK_H_INCLUDED

#include "SDL.h"
#include "SDL_ttf.h"

#include <graphics_wrapper.h>

int render_look_message(const char* message, SDL_Surface *screen,
    const Rect *dstrect, TTF_Font *font);
#endif

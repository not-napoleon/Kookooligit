#ifndef LOOK_H_INCLUDED
#define LOOK_H_INCLUDED

#include "SDL.h"
#include "SDL_ttf.h"

int render_look_message(const char* message, SDL_Surface *screen,
    const SDL_Rect *dstrect, TTF_Font *font);
#endif

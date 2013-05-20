#ifndef LOOK_H_INCLUDED
#define LOOK_H_INCLUDED

#include "SDL_ttf.h"

#include <graphics_wrapper.h>

int render_look_message(const char* message, const Rect *dstrect, TTF_Font *font);
#endif

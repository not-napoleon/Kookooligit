#ifndef RENDER_TEXT_H_INCLUDED
#define RENDER_TEXT_H_INCLUDED

#include "SDL.h"
#include "SDL_ttf.h"

typedef struct SurfaceNode *SurfaceNodePtr;

typedef struct SurfaceNode {
  SDL_Surface *surface;
  SurfaceNodePtr next;
} SurfaceNode;

SurfaceNodePtr make_surface_node(SDL_Surface *surface);
void free_surface_list(SurfaceNodePtr slist);
int build_word_list(char *text, SurfaceNodePtr *dest, SDL_Color color,
    TTF_Font *font);
int get_message_height(SurfaceNodePtr msg, int w);
int render_message_to_window(const SDL_Rect *dstrect, SDL_Surface *screen,
    SurfaceNodePtr text, int skip_line_height, int rows);

#endif

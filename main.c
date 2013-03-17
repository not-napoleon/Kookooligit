#include <stdlib.h>
// For Messages
#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"
/*
 * Screen list
 */

typedef struct SurfaceNode *SurfaceNodePtr;

typedef struct SurfaceNode {
  SDL_Surface *surface;
  SurfaceNodePtr next;
} SurfaceNode;

SurfaceNodePtr make_surface_node(SDL_Surface *surface) {
  SurfaceNodePtr node = malloc( sizeof(SurfaceNode) );
  node->surface = surface;
  node->next = NULL;
  return node;
}

void free_surface_list(SurfaceNodePtr slist) {
  while (slist != NULL) {
    SurfaceNodePtr tmp = slist;
    slist = slist->next;
    SDL_FreeSurface(tmp->surface);
    free(tmp);
  }
}


/*
 * Message logic here
 */

typedef struct Message{
  char *text; // the raw text
  SurfaceNodePtr rendered_words;
} Message;

typedef struct MessageNode *MessageNodePtr;

typedef struct MessageNode {
  // Linked list of messages
  Message *data;
  MessageNodePtr next;
  MessageNodePtr prev;
} MessageNode;

typedef struct MessageList {
  MessageNodePtr first; // Most recent message
  MessageNodePtr last;  // Oldest message in queue
  int length;
} MessageList;

void free_message_queue(MessageList *queue) {
  while (queue->first != NULL) {
    MessageNodePtr temp = queue->first;
    queue->first = queue->first->next;
    free(temp);
  }
  free(queue);
}

void free_message(Message *message) {
  free_surface_list(message->rendered_words);
  free(message);
}

MessageList *init_message_list() {
  MessageList *mlist;
  mlist = malloc( sizeof(MessageList) );
  mlist->first = NULL;
  mlist->last = NULL;
  mlist->length = 0;
  return mlist;
}

int add_message(MessageList *mlist, char *text, TTF_Font *font) {
  Message *msg = malloc( sizeof(Message) );
  msg->text = text;
  msg->rendered_words = NULL;
  // Build the message object
  SDL_Color color = {255,255,255};
  char *curr_word = strtok(text, " ");
  SurfaceNodePtr curr_surface;
  SurfaceNodePtr prev_surface = NULL;
  do {
    curr_surface = make_surface_node( TTF_RenderText_Solid(font, curr_word, color) );
    if (curr_surface->surface == NULL) {
      fprintf(stderr, "Unable to draw text <<%s>>: %s\n", curr_word, SDL_GetError());
      return -1;
    }
    // Don't lose the start of the list
    if (msg->rendered_words == NULL) {
      msg->rendered_words = curr_surface;
    }
    if (prev_surface != NULL) {
      prev_surface->next = curr_surface;
    }
    prev_surface = curr_surface;

  } while ( (curr_word = strtok(NULL, " ")) != NULL);

  // Build the message node
  MessageNodePtr new_node = malloc( sizeof(MessageNode) );
  new_node->data = msg;

  // Attach the message node to the message list
  new_node->next = mlist->first;
  mlist->first->prev = new_node;
  mlist->first = new_node;
  mlist->length++;

  // TODO: trim to max length

}

int DrawText( SDL_Surface *screen, TTF_Font *font, const char* text,
    int x, int y, int w, int h) {
  SDL_Color color = {255,255,255};
  SDL_Surface *text_surface;

  printf("attempting to render text <<%s>>\n", text);

  text_surface = TTF_RenderText_Solid(font, text, color);
  if (text_surface != NULL) {
    if (SDL_BlitSurface(text_surface, NULL, screen, NULL) < 0) {
      fprintf(stderr, "Blit failed with error: %s\n", SDL_GetError());
      exit(1);
    }
    SDL_FreeSurface(text_surface);
  } else {
    fprintf(stderr, "Unable to draw text: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
  return 1;
}

/*
 * Game logic here
 */


typedef struct GameConfiguration {
  char *font_path;
  int point_size;
  //Main window
  int window_w;
  int window_h;

  //Message window
  int message_x;
  int message_y;
  int message_w;
  int message_h;

} GameConfiguration;

typedef struct GameState {
  int is_running;
  GameConfiguration *config;
  SDL_Surface *screen;
  TTF_Font *font;
  MessageList *messages;
} GameState;

GameState *allocate_game_state() {
  /*Allocate a new game state struct
   */
  GameState *state;
  state = malloc(sizeof(GameState));
  state->config = malloc(sizeof(GameConfiguration));
  state->messages = init_message_list();
  return state;
}

void free_game_state(GameState *state) {
  /* Recursively free GameState struct
   */
  free(state->config);
  free_message_queue(state->messages);
  free(state);
}

int get_configuration(GameConfiguration *config) {
  /* Load game configuration data into the caller provided struct
   */
  config->font_path = "/Library/Fonts/Courier New.ttf";
  config->point_size = 16;
  config->window_w = 1024;
  config->window_h = 768;

  config->message_x = 0;
  config->message_y = 640;
  config->message_w = config->window_w;
  config->message_h = config->window_h - config->message_y;

  return 0;
}

void initilize(GameState *state) {
  if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  if (TTF_Init() < 0) {
    fprintf(stderr, "Unable to init SDL_ttf: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(TTF_Quit);

  state->screen = SDL_SetVideoMode(state->config->window_w, state->config->window_h, 16, 0);
  if (state->screen == NULL) {
    fprintf(stderr, "Unable to set SDL video mode: %s\n", SDL_GetError());
    exit(1);
  }

  // Load font
  state->font = TTF_OpenFont(state->config->font_path, state->config->point_size);
  if (state->font == NULL) {
    fprintf(stderr, "Unable to load font %s: %s\n", state->config->font_path,
        SDL_GetError());
    exit(1);
  }
  // Check for fixed with
  if (TTF_FontFaceIsFixedWidth(state->font) == 0) {
    fprintf(stderr, "Font is not fixed width, chances are nothing will look right");
  }
  // Turn off kerning, since we want our characters to line up in a grid
  if (TTF_GetFontKerning(state->font) != 0) {
    TTF_SetFontKerning(state->font, 0);
  }

  state->is_running = 1;
}

void cleanup(GameState *state) {
  free_game_state(state);
}

void handle_events(GameState *state, SDL_Event *event) {
  switch (event->type) {
    case SDL_KEYDOWN:
      printf("The %s key was pressed!\n",
        SDL_GetKeyName(event->key.keysym.sym));
      if (event->key.keysym.sym == 'q') {
        printf("Got quit signal from pressing q\n");
        state->is_running = 0;
      }
      break;
    case SDL_QUIT:
      printf("Got quit signal by magic\n");
      state->is_running = 0;
  }
}

void SDL_SurfaceInfo(char * name, SDL_Surface *thing)
{
  printf("Surface %s: w:%d h:%d bpp:%d\n", name, thing->w, thing->h, thing->format->BitsPerPixel);
}

int main(int argc, char *argv[]) {

  GameState *state = allocate_game_state();
  get_configuration(state->config);
  initilize(state);

  SDL_Event *event;

  DrawText(state->screen, state->font, "Hello World", 0, 0, state->screen->w, state->screen->h);
  while (state->is_running) {
    while (SDL_PollEvent(event)) {
      handle_events(state, event);
    }
  }

  cleanup(state);

  return 0;
}

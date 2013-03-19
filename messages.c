#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include <messages.h>
/*
 * Screen list
 *
 * this will probably need to get refactored eventually
 */

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
  // Set the spacing data based on the font
  msg->skip_line_height = TTF_FontSkipLine(font);
  TTF_SizeText(font, " ", &msg->space_width, NULL);
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

int build_single_message(Message *m, int w) {
  /*
   * Internal function to construct a single message, possibly of multiple
   * lines.
   */

}

int render_messages(int x, int y, int w, int h, SDL_Surface screen,
    MessageList *mlist) {
  MessageNodePtr curr;
  curr = mlist->first;
  int available_height = h;
  while (curr != NULL and available_height > 0) {
  // While messages left & space left
    // assemble the next message
    // blit the message into the temp buffer
  }
  // blit the temp buffer into the screen
  // render the screen
}

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

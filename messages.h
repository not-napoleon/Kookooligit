#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

typedef struct SurfaceNode *SurfaceNodePtr;

typedef struct SurfaceNode {
  SDL_Surface *surface;
  SurfaceNodePtr next;
} SurfaceNode;


typedef struct Message{
  char *text; // the raw text
  SurfaceNodePtr rendered_words;
  // Since we're storing rendered words, we need the spacing to assemble them at
  int skip_line_height;
  int space_width;
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


MessageList *init_message_list();
int add_message(MessageList *mlist, char *text, TTF_Font *font);
int render_messages(int x, int y, int w, int h, SDL_Surface screen,
    MessageList *mlist);
void free_message_queue(MessageList *queue);
void free_message(Message *message);

#endif

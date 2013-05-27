#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

#include <graphics_wrapper.h>
#include <render_text.h>

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
int add_message(MessageList *mlist, char *text);
void free_message_queue(MessageList *queue);
void free_message(Message *message);
int render_messages(const Rect *dstrect, MessageList *mlist);

#endif

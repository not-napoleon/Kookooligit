#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

typedef enum {
  NoOp,
  MoveLeft, MoveRight, MoveUp, MoveDown,
  MoveUpLeft, MoveDownLeft, MoveUpRight, MoveDownRight,
  EnterLookMode, ExitLookMode,
  Quit,
  COMMAND_TYPE_COUNT
} CommandCode;

CommandCode get_command();
int bind_command_name(CommandCode command, const char *symbol);
void free_command_mapping();

#endif

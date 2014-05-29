#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include "command_list.h"

enum CommandCode get_command();
int bind_command_name(enum CommandCode command, const char *symbol);
void free_command_mapping();

#endif

#ifndef COMMAND_LIST_H_INCLUDED
#define COMMAND_LIST_H_INCLUDED

enum CommandCode {
    NoOp,
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    MoveUpLeft,
    MoveDownLeft,
    MoveUpRight,
    MoveDownRight,
    EnterLookMode,
    ExitLookMode,
    Quit,
    COMMAND_TYPE_COUNT
};

enum CommandCode string_to_command_code(const char *input_string);

#endif

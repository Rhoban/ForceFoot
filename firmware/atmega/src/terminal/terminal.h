#ifndef _TERMINAL_H
#define _TERMINAL_H
#include <avr/pgmspace.h>

#define TERMINAL_BUFFER_SIZE 64
#define TERMINAL_MAX_COMMANDS 32

/**
 * Prototype of a terminal command
 */
typedef void terminal_command_func(char *parameters);

/**
 * A command definition for the terminal
 */
struct terminal_command
{
    const char *name;
    const char *description;
    terminal_command_func *command;
};

/**
 * Registers a command
 */
void terminal_register(const struct terminal_command *command);

#define TERMINAL_COMMAND(name, description) terminal_command_func terminal_command_ ## name; \
    \
    PROGMEM const char terminal_command_name_ ## name [] = #name; \
    PROGMEM const char terminal_command_description_ ## name [] = description; \
    \
    PROGMEM const struct terminal_command terminal_command_definition_ ## name = { \
        terminal_command_name_ ## name , terminal_command_description_ ## name , terminal_command_ ## name \
    }; \
    \
    __attribute__((constructor)) \
    void terminal_command_init_ ## name () {  \
        terminal_register(&terminal_command_definition_ ## name ); \
    } \
    \
    void terminal_command_ ## name (char *parameters)

/**
 * Terminal ticking
 */
void terminal_tick();

/**
 * Initializing terminal
 */
void terminal_init();

#endif // _TERMINAL_H

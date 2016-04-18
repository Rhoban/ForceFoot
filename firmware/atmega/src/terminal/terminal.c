#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <terminal/terminal.h>
#include <uart/uart.h>
#include <uart/dir.h>
#include <util/pgmspace.h>

static char terminal_buffer[TERMINAL_BUFFER_SIZE];

static unsigned int terminal_size = 0;

static unsigned int terminal_pos = 0;

static const struct terminal_command *terminal_commands[TERMINAL_MAX_COMMANDS];

static unsigned int terminal_command_count = 0;

/**
 * Registers a command
 */
void terminal_register(const struct terminal_command *command)
{
    terminal_commands[terminal_command_count++] = command;
}

/**
 * Internal helping command
 */
TERMINAL_COMMAND(help, "Displays the help about commands")
{
    unsigned int i;

    uart_puts("Available commands :\r\n\r\n");

    for (i=0; i<terminal_command_count; i++) {
        struct terminal_command command;

        pgm_read_object(&command, terminal_commands[i], sizeof(command));
        pgm_read_string(terminal_buffer, command.name, TERMINAL_BUFFER_SIZE);

        uart_puts(terminal_buffer);
        uart_puts(": ");
        pgm_read_string(terminal_buffer, command.description, TERMINAL_BUFFER_SIZE);
        uart_puts(terminal_buffer);
        uart_puts("\r\n");
    }
}

void terminal_prompt()
{
    dir_set(1);
    uart_puts("$ ");
    dir_set(0);
}

void terminal_init()
{
    terminal_prompt();
}

/***
 * Executes the given command with given parameters
 */
void terminal_execute(char *command_name, unsigned int command_name_length, char *parameters)
{
    char buffer[command_name_length];
    unsigned int i;
    char command_found = 0;
    dir_set(1);

    for (i=0; i<terminal_command_count; i++) {
        struct terminal_command command;

        pgm_read_object(&command, terminal_commands[i], sizeof(command));
        pgm_read_string(buffer, command.name, command_name_length+2);

        if (strlen(buffer) == command_name_length && strncmp(terminal_buffer, buffer, command_name_length) == 0) {
            command.command(parameters);

            command_found = 1;
            break;
        }
    }

    if (!command_found) {
        uart_puts("Unknown command: ");
        for (i=0; i<command_name_length; i++) {
            uart_putc(terminal_buffer[i]);
        }
        uart_puts("\r\n");
    }
    dir_set(0);
}

/***
 * Process the receive buffer to parse the command and executes it
 */
void terminal_process()
{
    char *saveptr ,*parameters;
    unsigned int command_name_length;
                
    uart_puts("\r\n");

    strtok_r(terminal_buffer, " ", &saveptr);
    parameters = strtok_r(NULL, " ", &saveptr);

    if (saveptr != NULL) {
        *(saveptr - 1) = ' ';
    }

    if (parameters != NULL) {
        command_name_length = parameters - terminal_buffer - 1;
    } else {
        command_name_length = strlen(terminal_buffer);
    }

    if (command_name_length > 0) {
        terminal_execute(terminal_buffer, command_name_length, parameters);
    }

    terminal_pos = 0;
    terminal_size = 0;
    terminal_prompt();
}

/**
 * Ticking the terminal, this will cause lookup for characters and eventually
 * a call to the process function on new lines
 */
void terminal_tick()
{
    unsigned int uartChar;
    char c;

    while ((uartChar = uart_getc()) != UART_NO_DATA) {
        c = uartChar & 0xff;

        if (c == '\r' || c == '\n') {
            terminal_buffer[terminal_pos] = '\0';
            terminal_process();
        } else if (c == '\x7f') {
            if (terminal_pos > 0) {
                terminal_pos--;
                terminal_size--;
                ///dir_set(1);
                //uart_puts("\x8 \x8");
                //dir_set(0);
            }
        } else {
            terminal_buffer[terminal_pos] = c;
            dir_set(1);
            uart_putc(c);
            dir_set(0);

            if (terminal_pos < TERMINAL_BUFFER_SIZE-1) {
                terminal_pos++;
            }

            if (terminal_size < terminal_pos) {
                terminal_size = terminal_pos;
            }
        }
    }
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <uart/uart.h>
#include <avr/sleep.h>
#include <adc/adc.h>
#include <avr/wdt.h>
#include <terminal/terminal.h>
#include <uart/dir.h>
#include "hx711.h"

int main (void)
{
    wdt_disable();
    dir_set(0);
    dir_init();

    cli();
    // Initializing UART
    uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(BAUDRATE, F_CPU));
    sei();

    //printf("\r\nStarting!\r\n");
    hx711_init();
    //terminal_init();

    while (1) {
        // terminal_tick();
    }
}

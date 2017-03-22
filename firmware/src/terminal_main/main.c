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

int main (void)
{
    wdt_disable();

    cli();
    // Initializing UART
    uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(BAUDRATE, F_CPU));
    sei();

#ifndef NO_ADC
    adc_run(1);
#endif

    printf("\r\nStarting!\r\n");
    terminal_init();

    while (1) {
        terminal_tick();
    }
}

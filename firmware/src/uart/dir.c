#include <avr/io.h>
#include <util/delay.h>
#include <uart/uart.h>
#include "dir.h"

void dir_set(int en)
{
    if (!en) {
        // Waits for the TX buffer to be empty
        while (uart_tx_has_data());
    } else {
        // Disable receiver
        uart_enable_receiver(0);
    }

    _delay_us(20);
    if (en) PORTD |= _BV(PD2);
    else PORTD &= ~_BV(PD2);
    _delay_us(20);

    if (!en) {
        // Enables the receiver
        uart_enable_receiver(1);
    }
}

void dir_init()
{
    dir_set(0);
    DDRD |= _BV(PD2);
}

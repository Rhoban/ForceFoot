#include <avr/io.h>
#include "led.h"

void led_init()
{
    DDRD |= _BV(PD5);
}

void led_set(int en)
{
    if (en) PORTD |= _BV(PD5);
    if (!en) PORTD &= ~_BV(PD5);
}

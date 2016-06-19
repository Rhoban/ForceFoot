#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <uart/dir.h>
#include "hx711.h"

int32_t hx711_values[4];

void hx711_clock_set(int index, int en)
{
    switch (index) {
        case 0:
            if (en) PORTC |= _BV(PC0);
            if (!en) PORTC &= ~_BV(PC0);
            break;
        case 1:
            if (en) PORTC |= _BV(PC2);
            if (!en) PORTC &= ~_BV(PC2);
            break;
        case 2:
            if (en) PORTD |= _BV(PD3);
            if (!en) PORTD &= ~_BV(PD3);
            break;
        case 3:
            if (en) PORTB |= _BV(PB0);
            if (!en) PORTB &= ~_BV(PB0);
            break;
    }
}

int hx711_read(int index)
{
    switch (index) {
        case 0:
            return ((PINC&_BV(PC1))!=0);
            break;
        case 1:
            return ((PINC&_BV(PC3))!=0);
            break;
        case 2:
            return ((PIND&_BV(PD4))!=0);
            break;
        case 3:
            return ((PINB&_BV(PB1))!=0);
            break;
    }
    return 0;
}

int hx711_available(int index)
{
    return !hx711_read(index);
}

#define VALUE_SIGN(value, length) \
        ((value < (1<<(length-1))) ? \
          (value) \
          : (value-(1<<length)))

int hx711_read_bit(int index)
{
    hx711_clock_set(index, 1);
    _delay_us(1);
    int result = hx711_read(index);
    hx711_clock_set(index, 0);
    _delay_us(1);
    return result;
}

int32_t hx711_sample(int index)
{
    uint32_t result = 0;
    int k;
    for (k=0; k<24; k++) {
        result <<= 1;
        result |= hx711_read_bit(index);
    }
    hx711_read_bit(index);

    uint32_t comp = 1;
    comp <<= 23;

    if (result < comp) {
        return result;
    } else {
        uint32_t shift = 1;
        shift <<= 24;
        return result - shift;
    }
}

static int current = 0;
void hx711_tick()
{
    if (hx711_available(current)) {
        hx711_values[current] = hx711_sample(current);
    }

    current++;
    if (current >= 4) current = 0;
}

void hx711_init()
{
    hx711_clock_set(0, 0);
    hx711_clock_set(1, 0);
    hx711_clock_set(2, 0);
    hx711_clock_set(3, 0);

    // Clocks as outputs
    DDRC |= _BV(PC0) | _BV(PC2);
    DDRD |= _BV(PD3);
    DDRB |= _BV(PB0);

    // Enabling pull up on inputs
    PORTC |= _BV(PC1) | _BV(PC3);
    PORTD |= _BV(PD4);
    PORTB |= _BV(PB1);
    
    int k;
    for (k=0; k<4; k++) {
        hx711_values[k] = 0;
    }
 
    /*
    // XXX: Debug: print all the values
    dir_set(1);
    _delay_ms(100);
    while (1) {
        int k;
        for (k=0; k<4; k++) {
            while (!hx711_available(k));
            printf("%ld\t", hx711_sample(k));
        }
        printf("\r\n");
    }
    */
}

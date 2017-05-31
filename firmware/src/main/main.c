#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <uart/uart.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <terminal/terminal.h>
#include <uart/dir.h>
#include <dxl/dxl.h>
#include <dxl/registers.h>
#include "hx711.h"
#include "led.h"

#define DXL_MODEL               5004
#define DXL_ID                  124
// #define DXL_RETURN_DELAY        250

// Dynamixel registers
struct dxl_registers registers;

// Is this id handled by the module ?
ui8 dxl_check_id(ui8 id)
{
    return (id == DXL_ID);
}

// Write in some registers
void dxl_write_data(ui8 id, ui8 addr, ui8 *values, ui8 length)
{
    // XXX: Ignored
}

static void putValue(int32_t val, unsigned char *data)
{
    data[0] = (val >> 0)&0xff;
    data[1] = (val >> 8)&0xff;
    data[2] = (val >> 16)&0xff;
}

// A register is read
void dxl_read_data(ui8 id, ui8 addr, ui8 *values, ui8 length, ui8 *error)
{
    *error = 0;

#ifdef DXL_RETURN_DELAY
    _delay_us(DXL_RETURN_DELAY);
#endif

    int k;
    unsigned char *rvalues = &registers.ram.presentPosition;
    for (k=0; k<4; k++) {
        putValue(hx711_values[k], rvalues+3*k);
    }

    memcpy(values, ((ui8*)&registers)+addr, length);
}

int main (void)
{
    wdt_disable();
    // Direction pin
    dir_set(0);
    dir_init();

    cli();
    // Initializing UART
    uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(BAUDRATE, F_CPU));
    sei();
    
    // Registers
    registers.eeprom.id = DXL_ID;
    registers.eeprom.modelNumber = DXL_MODEL;
    registers.eeprom.firmwareVersion = 1;

    // Initializing DXL module
    dxl_init();

    // Initializing HX711s
    hx711_init();

    led_init();
    led_set(0);

    while (1) {
        // Reading the balances
        hx711_tick();

        // Reading serial data
        while (uart_available()) {
            ui8 c = uart_getc()&0xff;
            dxl_push_incoming_byte(c); 
        }
    }
}

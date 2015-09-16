#include <stdlib.h>
#include <string.h>
#include <wirish/wirish.h>
#include <servos.h>
#include <terminal.h>
#include <main.h>
#include "dxl.h"
#include "dxl_protocol.h"
#include "HX711.h"

// Configuration of the foot
#define DXL_ID          123
#define DXL_MODEL       5000
#define DXL_BAUD        1000000

// #define DEBUG
// #define DXL_SERIALUSB

struct dxl_registers registers;
struct dxl_packet packet, answer;

/** 
 * Hardware
 */

// HX711 chips
HX711 b1(15, 16);
HX711 b2(17, 18);
HX711 b3(19, 20);
HX711 b4(21, 22);
HX711 *balances[] = {&b1, &b2, &b3, &b4};
#define BALANCES (sizeof(balances)/sizeof(HX711*))
int currentBalance = 0;

// Dynamixel bus
#define DIRECTION       2
#ifdef DXL_SERIALUSB
#define port            SerialUSB
#else
#define port            Serial3
#endif

/**
 * Debugging, shows the values of all balances
 */
TERMINAL_COMMAND(values, "Show balance values")
{
#ifdef DEBUG
    while (!SerialUSB.available()) {
        terminal_io()->print(b1.value);
        terminal_io()->print(" ");
        terminal_io()->print(b2.value);
        terminal_io()->print(" ");
        terminal_io()->print(b3.value);
        terminal_io()->print(" ");
        terminal_io()->print(b4.value);
        terminal_io()->println();
        delay(10);
        for (int k=0; k<BALANCES; k++) {
            balances[k]->tick();
        }
    }
#endif
}

bool dxl_check_id(ui8 id)
{
    return (id==registers.eeprom.id);
}

void dxl_write_data(ui8 id, ui8 addr, ui8 *values, ui8 length)
{
    memcpy(((ui8 *)(&registers))+addr, values, length);
}

static void putValue(int val, unsigned char *data)
{
    data[0] = (val >> 16)&0xff;
    data[1] = (val >> 8)&0xff;
    data[2] = (val >> 0)&0xff;
}

void dxl_read_data(ui8 id, ui8 addr, ui8 *values, ui8 length, ui8 *error)
{
    *error = 0;

    putValue(b1.value, registers.ram.values);
    putValue(b2.value, registers.ram.values+3);
    putValue(b3.value, registers.ram.values+6);
    putValue(b4.value, registers.ram.values+9);

    memcpy(values, ((ui8*)&registers)+addr, length);
}

/**
 * Forwarding dxl
 */
TERMINAL_COMMAND(forward, "Forward")
{
#ifdef DEBUG
    digitalWrite(DIRECTION, HIGH);
    port.write("Hello");
#ifndef DXL_SERIALUSB
    port.waitDataToBeSent();
#endif
    digitalWrite(DIRECTION, LOW);

    while (!SerialUSB.available()) {
        if (port.available()) {
            SerialUSB.write(port.read());
        }
    }
#endif
}

/**
 * Sending a DXL packet
 */
void send_packet(struct dxl_packet *packet)
{
    ui8 buffer[256];
    int n = dxl_write_packet(packet, buffer);

    digitalWrite(DIRECTION, HIGH);
    delay_us(30);
    port.write(buffer, n);
#ifndef DXL_SERIALUSB
    port.waitDataToBeSent();
#endif
    delay_us(40);
    digitalWrite(DIRECTION, LOW); 
}

/**
 * Setup function
 */
void setup()
{
    pinMode(BOARD_LED_PIN, OUTPUT);
    digitalWrite(BOARD_LED_PIN, HIGH);

    // Dynamixel bus
#ifndef DXL_SERIALUSB
    port.begin(DXL_BAUD);
#endif
    digitalWrite(DIRECTION, LOW);
    pinMode(DIRECTION, OUTPUT);
    dxl_packet_init(&packet);
    dxl_packet_init(&answer);

    registers.eeprom.id = DXL_ID;
    registers.eeprom.modelNumber = DXL_MODEL;
    registers.eeprom.firmwareVersion = 1;

#ifdef DEBUG
    // Terminal
    terminal_init(&SerialUSB);
#endif

    // Initializing balances
    for (int k=0; k<BALANCES; k++) {
        balances[k]->init();
    }

}

/**
 * Loop function
 */
void loop()
{
    // Reading balances data, if any
    balances[currentBalance++]->tick();
    if (currentBalance >= BALANCES) {
        currentBalance = 0;
    }

#ifdef DEBUG
    // Ticking terminal
    terminal_tick();
#endif

    // Ticking dxl
    while (port.available()) {
        dxl_packet_push_byte(&packet, port.read());
        if (packet.process) {
            dxl_process(&packet, &answer);
            if (answer.process) {
                send_packet(&answer);
            }
            answer.process = false;
            packet.process = false;
        }
    }
}

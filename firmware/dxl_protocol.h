#ifndef DXL_PROTOCOL_H
#define DXL_PROTOCOL_H

#include "dxl.h"

#define DXL_PING        0x01
#define DXL_READ_DATA   0x02
#define DXL_WRITE_DATA  0x03
#define DXL_REG_WRITE   0x04
#define DXL_ACTION      0x05
#define DXL_RESET       0x06
#define DXL_SYNC_WRITE  0x83
#define DXL_SYNC_READ   0x84
#define DXL_NO_ERROR    0x00

void dxl_process(struct dxl_packet *packet, struct dxl_packet *answer);
bool dxl_check_id(ui8 id);
void dxl_write_data(ui8 id, ui8 addr, ui8 *values, ui8 length);
void dxl_read_data(ui8 id, ui8 addr, ui8 *values, ui8 length, ui8 *error);

struct dxl_registers
{
    struct dxl_eeprom {
        unsigned short  modelNumber;
        unsigned char firmwareVersion;
        unsigned char id;
        unsigned char baudrate;
        unsigned char returnDelay;
        unsigned short cwLimit;
        unsigned short ccwLimit;
        unsigned char _dummy;
        unsigned char temperatureLimit;
        unsigned char lowestVoltage;
        unsigned char highestVoltage;
        unsigned short maxTorque;
        unsigned char returnStatus;
        unsigned char alarmLed;
        unsigned char alarmShutdown;
    } eeprom __attribute((packed));

    unsigned char _dummy2[4]; 

    struct dxl_ram {
        unsigned char torqueEnable;
        unsigned char led;
        unsigned char cwComplianceMargin;
        unsigned char ccwComplianceMargin;
        unsigned char cwComplianceSlope;
        unsigned char ccwComplianceSlope;
        unsigned short goalPosition;
        unsigned short movingSpeed;
        unsigned short torqueLimit;

        unsigned char values[12];

        /*
        unsigned short presentPosition;
        unsigned short presentSpeed;
        unsigned short presentLoad;
        unsigned char presentVoltage;
        unsigned char presentTemperature;
        */

        unsigned char registeredInstruction;
        unsigned char _dummy3;
        unsigned char moving;
        unsigned char lock;
        unsigned short tare;
    } ram __attribute__((packed));

    char eeprom_dirty;
} __attribute__((packed));

#endif // DXL_PROTOCOL_H

#ifndef DXL_REGISTERS_H
#define DXL_REGISTERS_H

__attribute__((packed))
struct dxl_registers
{
    volatile struct dxl_eeprom {
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
    } eeprom;

    volatile unsigned char _dummy2[5];

    volatile struct dxl_ram {
        unsigned char torqueEnable;
        unsigned char led;
        unsigned char cwComplianceMargin;
        unsigned char ccwComplianceMargin;
        unsigned char cwComplianceSlope;
        unsigned char ccwComplianceSlope;
        unsigned short goalPosition;
        unsigned short movingSpeed;
        unsigned short torqueLimit;
        unsigned short presentPosition;
        unsigned short presentSpeed;
        unsigned short presentLoad;
        unsigned char presentVoltage;
        unsigned char presentTemperature;
        unsigned char registeredInstruction;
        unsigned char _dummy3;
        unsigned char moving;
        unsigned char lock;
        unsigned short punch;
    } ram;

    volatile char eeprom_dirty;
};

#endif // DXL_REGISTERS_H

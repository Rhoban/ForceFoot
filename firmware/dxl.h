#ifndef DXL_H
#define DXL_H

// Protocol definition
#define DXL_BROADCAST   0xFE

// Size limit for a buffer containing a dynamixel packet
#define DXL_BUFFER_SIZE 300

// Maximum parameters in a packet
#define DXL_MAX_PARAMS  220

typedef unsigned char ui8;

/**
 * A dynamixel packet
 */
struct dxl_packet {
    ui8 id; 
    union {
        ui8 instruction;
        ui8 error;
    };  
    ui8 parameter_nb;
    ui8 parameters[DXL_MAX_PARAMS];
    bool process;
    ui8 dxl_state;
};

void dxl_packet_init(volatile struct dxl_packet *packet);
void dxl_packet_push_byte(volatile struct dxl_packet *packet, ui8 b);
int dxl_write_packet(volatile struct dxl_packet *packet, ui8 *buffer);
void dxl_copy_packet(volatile struct dxl_packet *from, volatile struct dxl_packet *to);
ui8 dxl_compute_checksum(volatile struct dxl_packet *packet);

/**
 * A Dynamixel Device which is on the bus
 */
struct dxl_device
{
    void (*tick)(volatile struct dxl_device *self);
    void (*process)(volatile struct dxl_device *self, volatile struct dxl_packet *packet);
    void (*process_redirected)(volatile struct dxl_device *self, volatile struct dxl_device *from, volatile struct dxl_packet *packet);
    volatile struct dxl_packet packet;
    volatile struct dxl_device *redirect_packets;
    volatile struct dxl_device *next;
    volatile void *data;
};

void dxl_device_init(volatile struct dxl_device *device);

/**
 * A bus is composed of one master and some slaves
 */
struct dxl_bus
{
    volatile struct dxl_device *master;
    volatile struct dxl_device *slaves;
};

/**
 * Initialize the bus and run its main loop
 */
void dxl_bus_init(struct dxl_bus *bus);
void dxl_bus_tick(struct dxl_bus *bus);

/**
 * Sets the master of add a slave on the bus
 */
void dxl_set_master(struct dxl_bus *bus, volatile struct dxl_device *master);
void dxl_add_slave(struct dxl_bus *bus, volatile struct dxl_device *slave);

#endif // DXL_H

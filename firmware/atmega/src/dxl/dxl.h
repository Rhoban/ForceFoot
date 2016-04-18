/*****************************************************************************/
/* protocol.h
 * Dynamixel protocol
 *
 * Date: 2012/03
 * Rhoban Project
 *****************************************************************************/
#ifndef DXL_H
#define DXL_H
#include <dxl/types.h>

// Dynamixel registers
#define DXL_MODEL_NB_ADDR       0
#define DXL_TORQUE_ENABLE       24
#define DXL_GOAL_POSITION       30
#define DXL_LED                 25

// A dynamixel packet representation
struct dxl_packet {
    ui8 id;
    union {
        ui8 instruction;
        ui8 error;
    };
    ui8 parameter_nb;
    ui8 parameters[128];
};

// Enable bufferization ?
//#define DXL_BUFFERIZE

// I/O buffer sizes
#ifdef DXL_BUFFERIZE
#define DXL_INCOMING_BUFFER_SIZE 1024
#define DXL_OUTGOING_PAQUET_SIZE 1024
#endif

// Initialize the dxl module
void dxl_init();

// Push an incoming byte to the dynamixel protocol
void dxl_push_incoming_byte(ui8 b);

// Run the scheduler on the buffer
void dxl_protocol_scheduler();

// Protocol status
#define DXL_COMMAND_AVOIDED 0
#define DXL_ERROR 1
#define DXL_CHECKSUM_ERROR 2
#define DXL_HEAD_ERROR 3
#define DXL_INCOMING_QUEUE_OVERFLOW 4

extern ui8 protocol_status;
#define dxl_check_protocol_status(bit) (protocol_status & (1 << bit))
#define dxl_set_protocol_status(bit) (protocol_status = (protocol_status | (1 << bit)))
void dxl_reset_protocol_status();

/**
 * Methods to implement
 */

// Is this id handled by the module ?
ui8 dxl_check_id(ui8 id);

// The dynamixel module sends a packet (through UART for instance)
void dxl_send_bytes(ui8 * bytes, ui8 nb);

// Write in some registers
void dxl_write_data(ui8 id, ui8 addr, ui8 *values, ui8 length);

// A register is read
void dxl_read_data(ui8 id, ui8 addr, ui8 *values, ui8 length, ui8 *error);

#endif // DXL_H

/*****************************************************************************/
/* protocol.c
 * Dynamixel protocol
 *
 * Date: 2012/03
 * Rhoban Project
 *****************************************************************************/
#include <dxl/dxl.h>
#include <dxl/types.h>
#include <dxl/byte_queue.h>
#include <util/delay.h>
#include <avr/io.h>
#include <uart/uart.h>
#include <uart/dir.h>
#include <main/led.h>

// Protocol definition
#define DXL_PING        0x01
#define DXL_READ_DATA   0x02
#define DXL_WRITE_DATA  0x03
#define DXL_REG_WRITE   0x04
#define DXL_ACTION      0x05
#define DXL_RESET       0x06
#define DXL_SYNC_WRITE  0x83
#define DXL_NO_ERROR    0x00
#define DXL_BROADCAST   0xFE

ui8 protocol_status = 0;

void dxl_reset_protocol_status() {
    protocol_status = 0;
}

// Data bufferization
#ifdef DXL_BUFFERIZE
ui8 incoming_buffer[DXL_INCOMING_BUFFER_SIZE];
ui8_queue incoming_queue;

ui8 answers[DXL_OUTGOING_PAQUET_SIZE];
unsigned int answers_size = 0;
#endif

// Internal functions
void dxl_process_command();
ui8 dxl_compute_checksum();
void dxl_push_answer_instruction();
void dxl_flush_answer();
void uart_putc(ui8 byte);
void dxl_push_byte(ui8 b);

// The current command (being read or written)
struct dxl_packet current_command;
ui8 dxl_state;

// Initializes the dynamixel module
void dxl_init() {
    dxl_state = 0;

#ifdef DXL_BUFFERIZE
    init_queue(&incoming_queue, incoming_buffer, DXL_INCOMING_BUFFER_SIZE);
#endif
}

// Computes the dynamixel checksum of the current command
ui8 dxl_compute_checksum() {
    int i;
    ui32 sum = 0;

    sum += current_command.id;
    sum += current_command.instruction;
    sum += current_command.parameter_nb+2;
    
    for (i=0; i<current_command.parameter_nb; i++) {
        sum += current_command.parameters[i];
    }

    sum = ~(sum & 0xFF);
    
    return (ui8) sum;
}


// Sends the answer
void dxl_push_answer_instruction() {
    int i;

    dir_set(1);

#ifdef DXL_BUFFERIZE
    answers_size = 0;
#endif

    uart_putc(0xFF);
    uart_putc(0xFF);
    uart_putc((ui8)current_command.id);
    uart_putc((ui8)(current_command.parameter_nb+2));
    uart_putc((ui8)current_command.error);

    for (i=0; i<current_command.parameter_nb; i++) {
        uart_putc(current_command.parameters[i]);
    }

    uart_putc(dxl_compute_checksum());
	
    dxl_state = 0;
    dir_set(0);
}

// Flushes the answer if there is bufferization
void dxl_flush_answer() {
#ifdef DXL_BUFFERIZE
    if (answers_size > 0) {
        uart_putcs(answers, answers_size);
        answers_size=0;
    }
#endif
}

// Dispatches a command
void dxl_process() {
    if (dxl_check_id(current_command.id) || current_command.id == DXL_BROADCAST) {
        switch (current_command.instruction) {
            case DXL_PING: 
                // Answers the ping
                if (current_command.id != DXL_BROADCAST) {
                    current_command.error = DXL_NO_ERROR;
                    current_command.parameter_nb = 0;
                    dxl_push_answer_instruction();
                }
                break;

            case DXL_WRITE_DATA:
                // Write data
                dxl_write_data(current_command.id,
                        current_command.parameters[0],
                        &current_command.parameters[1],
                        current_command.parameter_nb-1);
                break;

            case DXL_SYNC_WRITE: {
                                     ui8 addr = current_command.parameters[0];
                                     int length = current_command.parameters[1] + 1;
                                     int K = (current_command.parameter_nb-2) / length;
                                     int i;

                                     for (i=0; i<K; i++) {
                                         if (dxl_check_id(current_command.parameters[2+i*length])) {
                                             dxl_write_data(current_command.parameters[2+i*length],
                                                     addr,
                                                     &current_command.parameters[2+i*length+1],
                                                     length-1);
                                         }
                                     }
                                 }
                                 break;

            case DXL_READ_DATA:
                                 // Read some data
                                 if (current_command.id != DXL_BROADCAST) {
                                     ui8 addr = current_command.parameters[0];
                                     int length = current_command.parameters[1];

                                     dxl_read_data(current_command.id, addr, current_command.parameters, length, &current_command.error);

                                     current_command.parameter_nb = length;
                                     dxl_push_answer_instruction();
                                 }
        }
    }
}

// Parses a command
void dxl_push_byte(ui8 b) {
    switch (dxl_state) {
        case 0:
        case 1:
            if (b != 0xFF) {
                goto pc_error;
            }
            break;
        case 2:
            led_set(1);
            current_command.id = b;
            break;
        case 3:
            current_command.parameter_nb = b - 2;
            break;
        case 4:
            current_command.instruction = b;
            break;
        default:
            if (dxl_state - 4 > current_command.parameter_nb) {
                goto pc_ended;

            } else {
                current_command.parameters[dxl_state - 5] = b;
            }

            if (dxl_state > 128) {
                goto pc_error;
            }
    }
        
    dxl_state++;
    return;

pc_ended:
    if (dxl_compute_checksum() == b) {
        dxl_process();
    } else {
        dxl_set_protocol_status(DXL_CHECKSUM_ERROR);
    }

    dxl_state = 0;

#ifdef DXL_BUFFERIZE
    reset_queue(&incoming_queue);
#endif

    return;
pc_error:
    dxl_set_protocol_status(DXL_ERROR);

    dxl_state = 0;

#ifdef DXL_BUFFERIZE
    reset_queue(&incoming_queue);
#endif
}

// Pushes a byte in the dynamixel module
void dxl_push_incoming_byte(ui8 b) {
#ifdef DXL_BUFFERIZE
    if (queue_byte_nb(&incoming_queue) < DXL_INCOMING_BUFFER_SIZE)
        queue_push_byte(&incoming_queue, b);
    else
        dxl_set_protocol_status(DXL_INCOMING_QUEUE_OVERFLOW);
#else
    dxl_push_byte(b);
#endif
}

// Calls the scheduler (useless if there is no buffer)
void dxl_protocol_scheduler() {
#ifdef DXL_BUFFERIZE
    while (queue_byte_nb(&incoming_queue)) {
        dxl_push_byte(queue_get_byte(&incoming_queue));
    }

    dxl_flush_answer();
#endif
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wirish/wirish.h>
#include "dxl.h"

volatile int dxl_debug = 0;

void dxl_packet_init(volatile struct dxl_packet *packet)
{
    packet->dxl_state = 0;
    packet->process = false;
}

/**
 * Writes the given packet to the buffer
 */
int dxl_write_packet(volatile struct dxl_packet *packet, ui8 *buffer)
{
    int i;
    unsigned int pos = 0;

    buffer[pos++] = 0xff;
    buffer[pos++] = 0xff;
    buffer[pos++] = packet->id;
    buffer[pos++] = packet->parameter_nb+2;
    buffer[pos++] = packet->instruction;

    for (i=0; i<packet->parameter_nb; i++) {
        buffer[pos++] = packet->parameters[i];
    }

    buffer[pos++] = dxl_compute_checksum(packet);

    return pos;
}

void dxl_copy_packet(volatile struct dxl_packet *from, volatile struct dxl_packet *to)
{
    memcpy((void *)to, (void *)from, sizeof(struct dxl_packet));
}

ui8 dxl_compute_checksum(volatile struct dxl_packet *packet) {
    int i;
    unsigned int sum = 0;

    sum += packet->id;
    sum += packet->instruction;
    sum += packet->parameter_nb+2;
    
    for (i=0; i<packet->parameter_nb; i++) {
        sum += packet->parameters[i];
    }

    sum = ~(sum & 0xFF);
    
    return (ui8) sum;
}

void dxl_packet_push_byte(volatile struct dxl_packet *packet, ui8 b)
{
    switch (packet->dxl_state) {
        case 0:
        case 1:
            if (b != 0xFF) {
                goto pc_error;
            }
            break;
        case 2:
            packet->id = b;
            break;
        case 3:
            packet->parameter_nb = b - 2;
            break;
        case 4:
            packet->instruction = b;
            break;
        default:
            if (packet->dxl_state - 4 > packet->parameter_nb) {
                goto pc_ended;

            } else {
                packet->parameters[packet->dxl_state - 5] = b;
            }

            if (packet->dxl_state - 4 > DXL_MAX_PARAMS) {
                goto pc_error;
            }
    }
        
    packet->dxl_state++;
    return;

pc_ended:
    if (dxl_compute_checksum(packet) == b) {
        packet->process = true;
    }

    packet->dxl_state = 0;
    return;
pc_error:
    packet->dxl_state = 0;
}

void dxl_device_init(volatile struct dxl_device *device)
{
    device->next = NULL;
    device->data = NULL;
    device->tick = NULL;
    device->process = NULL;
    device->redirect_packets = NULL;
    device->process_redirected = NULL;
    dxl_packet_init(&device->packet);
}

void dxl_set_master(struct dxl_bus *bus, volatile struct dxl_device *master)
{
    bus->master = master;
}

void dxl_add_slave(struct dxl_bus *bus, volatile struct dxl_device *slave)
{
    slave->next = bus->slaves;
    bus->slaves = slave;
}

void dxl_bus_init(struct dxl_bus *bus)
{
    if (bus != NULL) {
        bus->master = NULL;
        bus->slaves = NULL;
    }
}

/**
 * The execution of the bus loop which reads & dispatch packets
 */
void dxl_bus_tick(struct dxl_bus *bus)
{
    volatile struct dxl_device *slave;

    // If the master has a packet, dispatch it to all the slaves
    bus->master->tick(bus->master);

    // If there is a packet to process from the master
    volatile struct dxl_packet *master_packet = &bus->master->packet;
    if (master_packet->process) {
        for (slave = bus->slaves; slave != NULL; slave = slave->next) {
            if (slave->redirect_packets == NULL) {
                slave->process(slave, master_packet);
            }
        }
        master_packet->process = false;
    }

    // Look if the slaves have packets for master
    for (slave = bus->slaves; slave != NULL; slave = slave->next) {
        if (slave->tick != NULL) {
            slave->tick(slave);
        }

        if (slave->packet.process) {
            if (slave->redirect_packets != NULL) {
                if (slave->redirect_packets->process_redirected != NULL) {
                    slave->redirect_packets->process_redirected((volatile struct dxl_device *)slave->redirect_packets, slave, &slave->packet);
                }
            } else {
                bus->master->process(bus->master, &slave->packet);
            }

            slave->packet.process = false;
        }
    }
}

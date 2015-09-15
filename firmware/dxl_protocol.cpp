#include <terminal.h>
#include <wirish/wirish.h>
#include "dxl_protocol.h"

void dxl_process(struct dxl_packet *packet, struct dxl_packet *answer)
{
    if (dxl_check_id(packet->id) || packet->id == DXL_BROADCAST) {
        answer->id = packet->id;

        switch (packet->instruction) {
            case DXL_PING: 
                // Answers the ping
                if (packet->id != DXL_BROADCAST) {
                    answer->error = DXL_NO_ERROR;
                    answer->parameter_nb = 0;
                    answer->process = true;
                }
                break;

            case DXL_WRITE_DATA:
                // Write data
                dxl_write_data(
                        packet->id,
                        packet->parameters[0],
                        (ui8 *)&packet->parameters[1],
                        packet->parameter_nb-1);
                break;

            case DXL_SYNC_WRITE: {
                     ui8 addr = packet->parameters[0];
                     int length = packet->parameters[1] + 1;
                     int K = (packet->parameter_nb-2) / length;
                     int i;

                     for (i=0; i<K; i++) {
                         if (dxl_check_id(packet->parameters[2+i*length])) {
                             dxl_write_data(packet->parameters[2+i*length],
                                     addr,
                                     (ui8 *)&packet->parameters[2+i*length+1],
                                     (ui8)(length-1));
                         }
                     }
                 }
                 break;

            case DXL_READ_DATA:
                 // Read some data
                 if (packet->id != DXL_BROADCAST) {
                     ui8 addr = packet->parameters[0];
                     int length = packet->parameters[1];

                     if (length < sizeof(packet->parameters)) {
                         answer->process = true;
                         dxl_read_data(packet->id, addr, (ui8 *)answer->parameters, length, (ui8 *)&answer->error);
                         answer->parameter_nb = length;
                     }
                }   
        }
    }
}

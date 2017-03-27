#include "fifo.h"
#include "packet.h"

#define MAX_PACKET_SIZE 32

extern struct fifo_t fifo_send;
static uint8_t packet[MAX_PACKET_SIZE];

static uint8_t get_packet_from(struct fifo_t *fifo, uint8_t *packetbuf, uint8_t len)
{
    return 0;
}


void send_task(void)
{
    uint8_t type;

    while(1)
    {
        if(fifo_empty(&fifo_send))
        {
            //yeild();
        }
        else
        {
            get_packet_from(&fifo_send, packet, MAX_PACKET_SIZE);
            if(packet_validate(packet))
            {
                type = packet_type((struct packet_t *)packet);

                switch(type)
                {
                    case OTA:
                    {        
                    }
                    break;

                    case CMD:
                    {
                    }
                    break;

                    case DATA:
                    {
                    }
                    break;
                }
            }
        }
    }
}



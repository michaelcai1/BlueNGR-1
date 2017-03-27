#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "fifo.h"
#include "debug.h"
#include "packet.h"

#define DEBUG_BUFFER_SIZE 32

struct packet_t;
extern struct fifo_t fifo_recv;
static uint8_t debug_buf[DEBUG_BUFFER_SIZE];

void debug_log(const uint8_t type, const uint8_t *fmt, ...)
{
    uint8_t len;
    uint8_t count = 0;
    va_list argptr;

    va_start(argptr, fmt);
    len = vsnprintf((char *)debug_buf, DEBUG_BUFFER_SIZE, (char *)fmt, argptr);
    va_end(argptr); 

    struct packet_t *packet = packet_make(DEBUG, debug_buf, len);
    len = packet_len(packet);

    for(uint8_t *p = (uint8_t *)packet; count < len; count++)
    {
        if(!fifo_full(&fifo_recv))
        {
            fifo_insert(&fifo_recv, p[count]);
        }
        else
        {
            //yeild();
        }
    }

    packet_free(packet);
}

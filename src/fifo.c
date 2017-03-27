#include "fifo.h"
#define FIFO_BUFFER_SIZE    100

struct fifo_t
{
    uint16_t head;
    uint16_t tail;
    uint8_t  buf[FIFO_BUFFER_SIZE];
};

struct fifo_t fifo_recv;
struct fifo_t fifo_send;

void fifo_init(struct fifo_t *fifo)
{
    fifo->head = fifo->tail = 0;
}

BOOL fifo_empty(struct fifo_t *fifo)
{
    return (fifo->head == fifo->tail);
}

BOOL fifo_full(struct fifo_t *fifo)
{
    return ((fifo->tail + 1) % FIFO_BUFFER_SIZE == fifo->head);
}

void fifo_insert(struct fifo_t *fifo, uint8_t c)
{
    fifo->buf[fifo->tail] = c;
    fifo->tail = (fifo->tail + 1) % FIFO_BUFFER_SIZE;
}

uint8_t fifo_delete(struct fifo_t *fifo)
{
    uint8_t c = fifo->buf[fifo->head];
    fifo->head = (fifo->head + 1) % FIFO_BUFFER_SIZE;
    return c;
}

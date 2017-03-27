#ifndef _FIFO_H_
#define _FIFO_H_

#include "hal_types.h"

struct fifo_t;

void fifo_init(struct fifo_t *fifo);
BOOL fifo_empty(struct fifo_t *fifo);
BOOL fifo_full(struct fifo_t *fifo);
void fifo_insert(struct fifo_t *fifo, uint8_t c);
uint8_t fifo_delete(struct fifo_t *fifo);
#endif

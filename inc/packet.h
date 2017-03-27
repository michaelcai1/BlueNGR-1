#ifndef _PACKET_H_
#define _PACKET_H_
#include "hal_types.h"

#define OTA    0x00
#define CMD    0x20
#define DATA   0x40
#define EVENT  0x60
#define DEBUG  0x80

struct packet_t;

struct packet_t* packet_make(uint8_t type, uint8_t *data, uint8_t len);
void packet_free(struct packet_t* packet);
BOOL packet_validate(struct packet_t* packet);
uint8_t packet_length(struct packet_t* packet);
uint8_t packet_type(struct packet_t* packet);

#endif /*_PACKET_H_*/

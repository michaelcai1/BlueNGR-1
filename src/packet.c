#include "packet.h"
#include <stdio.h>
#include <stdlib.h>

#define PACKET_DATA_LEN(packet)  (uint8_t)(packet->head.type & 0x1f)
#define PACKET_TYPE(packet) (uint8_t)(packet->head.type & 0xe0)

struct packet_t
{
    union
    {
        uint8_t type;
        uint8_t crc;
    } head;
    uint8_t data[0];
};

static uint8_t crc(uint8_t *data, uint8_t len)
{
    return 0;
}

struct packet_t* packet_make(uint8_t type, uint8_t *data, uint8_t len)
{
    struct packet_t *packet = malloc(sizeof(struct packet_t) + len);
    if(!packet) return NULL;

    packet->head.type = type | len;
    packet->head.crc = crc(data, len);

    return packet;
}

void packet_free(struct packet_t* packet)
{
    free(packet);
}

BOOL packet_validate(struct packet_t* packet)
{
    uint8_t value = crc(packet->data, PACKET_DATA_LEN(packet));
    return (packet->head.crc == value);
}

uint8_t packet_length(struct packet_t* packet)
{
    return (sizeof(struct packet_t) + PACKET_DATA_LEN(packet)); 
}

uint8_t packet_type(struct packet_t* packet)
{
    return PACKET_TYPE(packet);
}

#ifndef PACKET_H
#define PACKET_H
#include "stdint.h"
#include "string.h"

typedef _Bool bool;
#define HEADER 28


/*
    SAMPLE PACKET STRUCTURE
    4 BYTE CHAR ARRAY
    
    1 BYTE HEADER
    2 BYTE MESSAGE    
    1 BYTE CHECKSUM

    e.g [28, 'R', 'Q', checksum]
    
*/

typedef enum {
    PACKET_VALID,
    INVALID_HEADER,
    INVALID_CHECKSUM
} p_status;


typedef enum {
    COMM_NOT_INIT,
    COMM_IN_PROGRESS,
    ECHO_MODE
} comm_status;

typedef struct {
    uint8_t header;
    uint8_t msg[2];
    uint8_t checksum;
} packet_t;

/*
    Prepare packet to send in-place
*/
void prepare_packet(packet_t* packet, char* cmd);

/*
    Evaluates the packet
    Return true if the packet is valid
    Writes the received command into "cmd"
*/
p_status unpack(packet_t packet, char* cmd);

void packet_to_buffer(packet_t packet, uint8_t *buffer);
void buffer_to_packet(packet_t* packet, uint8_t *buffer);
uint8_t calculate_checksum(packet_t packet);
#endif

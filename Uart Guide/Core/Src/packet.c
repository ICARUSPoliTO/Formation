#include "packet.h"

/*
    Prepare packet to send in-place
*/
void prepare_packet(packet_t* packet, char* cmd) {
    packet->msg[0] = cmd[0];
    packet->msg[1] = cmd[1];

    uint8_t checksum = calculate_checksum(*packet);
    packet->checksum = checksum;
}

uint8_t calculate_checksum(packet_t packet) {
    uint8_t res = 0;
    res ^= packet.header;
    res ^= packet.msg[0];
    res ^= packet.msg[1];

    return res;
}

void buffer_to_packet(packet_t* packet ,uint8_t *buffer) {
    packet->header = buffer[0];
    packet->msg[0] = buffer[1];
    packet->msg[1] = buffer[2];
    packet->checksum = buffer[3];
}

void packet_to_buffer(packet_t packet ,uint8_t *buffer) {
    buffer[0] = packet.header;
    buffer[1] = packet.msg[0];
    buffer[2] = packet.msg[1];
    buffer[3] = packet.checksum;
}

/*
    Evaluates the packet
    Return true if the packet is valid
    Writes the received command into "cmd"
*/
p_status unpack(packet_t packet, char* cmd) {
    if (packet.header != HEADER) {
        return INVALID_HEADER;
    }
    uint8_t checksum = calculate_checksum(packet);
    if (checksum != packet.checksum) {
        return INVALID_CHECKSUM;
    }
    cmd[0] = packet.msg[0];
    cmd[1] = packet.msg[1];
    return PACKET_VALID;
}

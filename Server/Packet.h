#ifndef SERVER_PACKET_H
#define SERVER_PACKET_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct data_packet{
    uint16_t checksum;
    uint16_t len;
    uint32_t seq_no;
    char data[512];
    bool operator<(const data_packet& rhs) const {
        return seq_no > rhs.seq_no;
    }
};

struct ack_packet{
    uint16_t checksum;
    uint16_t len;
    uint32_t ack_no;
};

data_packet create_data_packet(vector<char> data, uint32_t &seq_no);
ack_packet create_ack_packet(uint32_t &ack_no);



#endif //SERVER_PACKET_H

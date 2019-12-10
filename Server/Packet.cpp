#include "Packet.h"


data_packet create_data_packet(vector<char> given_data, uint32_t &seq_no){
    struct data_packet p;
    memset(&p, 0, sizeof(p));
    for(int i=0;i<given_data.size();i++){
        p.data[i] = given_data[i];
    }
    p.len = (uint16_t) given_data.size();
    p.seq_no = seq_no;
    seq_no++;
    return p;
}

ack_packet create_ack_packet(uint16_t &ack_no){
    struct ack_packet p;
    memset(&p, 0, sizeof(p));
    p.len = 8;
    p.ack_no = ack_no;
    ack_no++;
    return p;
}
#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
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
#include <queue>
#include "Utils.h"
#include "Timer.h"
#include "Packet.h"
using namespace std;

#define MSS 512

void send_packet(data_packet packet,int socket, sockaddr_in new_addr);
void slow_start(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet packets[], sockaddr_in new_addr, int socket);
void congestion_avoidance(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet packets[], sockaddr_in new_addr, int socket);
void fast_recovery(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd);
void fill_window(priority_queue<data_packet> window, priority_queue<data_packet> packets, double &cwnd, sockaddr_in new_addr, int socket);
void empty_window(priority_queue<data_packet> window, bool isAcked[], int &base);
void handle_client(string fn, int new_socket, sockaddr_in new_addr);
void start_timer(Timer &timer, bool isAcked[], double &ssthresh, double &cwnd, int &dup_ack, state &st, int &base, data_packet packets[],sockaddr_in new_addr, int socket);

#endif //SERVER_SERVER_H

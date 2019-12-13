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
#include <chrono>
#include <ctime>
#include <thread>
#include <mutex>
#include "Utils.h"
#include "Packet.h"
using namespace std;

#define MSS 512

#define TIMEOUTINTERVAL 100000

void slow_start(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet missing_packet, sockaddr_in new_addr, int socket, int seed, double plp);
void congestion_avoidance(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet missing_packet, sockaddr_in new_addr, int socket, int seed, double plp);
void fast_recovery(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet missing_packet, sockaddr_in new_addr, int socket, int seed, double plp);
void handle_client(string fn, int new_socket, sockaddr_in new_addr, int seed, double plp);
int recv_packet(int new_socket, ack_packet &ack, sockaddr_in &cliaddr, int &time_passed);
void handle_timeout(data_packet missing_packet,double &cwnd, double &ssthresh, int &dup_ack, state &st, bool isAcked[], data_packet packets[], sockaddr_in new_addr, int new_socket, int seed, double plp);
void send_packet(data_packet &packet, int socket, sockaddr_in &cur_addr, int seed, double plp);
#endif //SERVER_SERVER_H

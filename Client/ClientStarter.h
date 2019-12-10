#ifndef CLIENT_CLIENTSTARTER_H
#define CLIENT_CLIENTSTARTER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include "Utils.h"
#include "Packet.h"
#include <queue>
using namespace std;

int create_socket();
void initiate_connection(int base_port, string filename, int client_socket);

#endif //CLIENT_CLIENTSTARTER_H

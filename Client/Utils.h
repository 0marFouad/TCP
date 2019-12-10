#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

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
#include <sstream>
#include <fstream>
using namespace std;

vector<char> string2vector(string s);
void read_args(int &server_ip, int &server_port, string &filename);

#endif //CLIENT_UTILS_H

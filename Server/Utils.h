#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

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


enum state{SlowStart, CongestionAvoidance, FastRecovery};
vector<char> file2vector(string filename);
string vector2string(vector<char> v);
void print_vector(vector<char> v);
vector<vector<char>> split_vector(vector<char> file_vector, int max_size);
void read_args(int &port, int &seed_value, double &prop);


#endif //SERVER_UTILS_H

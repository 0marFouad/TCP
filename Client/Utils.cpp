#include "Utils.h"

vector<char> string2vector(string s){
    vector<char> v;
    for(int i=0;i<s.length();i++){
        v.push_back(s[i]);
    }
    return v;
}

void read_args(int &server_ip, int &server_port, string &filename){
    std::string line;
    std::ifstream infile("client.in");
    getline(infile, line);
    istringstream is1(line);
    is1 >> server_ip;
    getline(infile, line);
    istringstream is2(line);
    is2 >> server_port;
    getline(infile, line);
    istringstream is3(line);
    is3 >> filename;
}
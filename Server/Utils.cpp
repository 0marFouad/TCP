#include "Utils.h"

vector<vector<char>> split_vector(vector<char> file_vector, int max_size){
    vector<vector<char>> splitted;
    if(file_vector.size() <= max_size){
        splitted.push_back(file_vector);
        return splitted;
    }
    int i=0;
    for(int i=0;i<file_vector.size();i+=max_size){
        vector<char> chunk;
        for(int j=0;j<max_size && i+j<file_vector.size();j++){
            chunk.push_back(file_vector[i+j]);
        }
        splitted.push_back(chunk);
    }
    return splitted;
}

vector<char> file2vector(string filename){
    std::ifstream input_stream(filename);
    vector<char> v;
    std::string str;
    while (std::getline(input_stream, str))
    {
        for(int i=0;i<str.length();i++){
            v.push_back(str[i]);
        }
        v.push_back('\n');
    }
    return v;
}



void print_vector(vector<char> v){
    std::cout << "Read " << v.size() << " numbers" << std::endl;
    std::cout << "numbers read in:\n";
    std::copy(v.begin(), v.end(), std::ostream_iterator<char>(std::cout));
    std::cout << std::endl;
}

void read_args(int &port, int &seed_value, double &prop){
    std::string line;
    std::ifstream infile("server.in");
    getline(infile, line);
    istringstream is1(line);
    is1 >> port;
    getline(infile, line);
    istringstream is2(line);
    is2 >> seed_value;
    getline(infile, line);
    istringstream is3(line);
    is3 >> prop;
}

string vector2string(vector<char> v){
    string s;
    for(int i=0;i<v.size();i++){
        s += v[i];
    }
    return s;
}

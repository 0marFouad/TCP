#include "ServerStarter.h"
int create_socket(int &my_port){
    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(my_port);
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    my_port++;
    return sockfd;
}

string extract_filename(data_packet d){
    string fn;
    for(int i=0;i<d.len;i++){
        fn += d.data[i];
    }
    return fn;
}

std::ifstream::pos_type isFileExist(const char* filename){
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

void accept(int sockfd){
    int n;
    struct data_packet filename_packet;
    struct sockaddr_in cliaddr;
    unsigned int clntLen = sizeof(cliaddr);
    while((n = recvfrom(sockfd, &filename_packet, sizeof(filename_packet), 0, ( struct sockaddr *) &cliaddr, &clntLen)) > 0){
        string fn = extract_filename(filename_packet);
        int filesize = isFileExist(&fn[0]);
        if(filesize != -1){
            std::string s = std::to_string(filesize);
            static const char arr[] = {'C','o','n','n','e','c','t','i','o','n',' ','A','c','c','e','p','t','e','d'};
            vector<char> vec (arr, arr+19);
            for(int i=0;i<s.length();i++){
                vec.push_back(s[i]);
            }
            uint32_t i = 0;
            data_packet ad = create_data_packet(vec,i);
            int cur_port = 8040;
            int new_socket = create_socket(cur_port);
            sendto(new_socket, &ad, sizeof(ad), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
            //start communication
            handle_client(fn, new_socket, cliaddr);
        }else{
            static const char arr[] = {'C','o','n','n','e','c','t','i','o','n',' ','R','e','f','u','s','e','d'};
            vector<char> vec (arr, arr+18);
            uint32_t i = 0;
            data_packet ad = create_data_packet(vec,i);
            sendto(sockfd, &ad, sizeof(ad), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr));
        }
    }
}

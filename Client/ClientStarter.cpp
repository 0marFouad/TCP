#include "ClientStarter.h"

struct sockaddr_in servaddr;
int create_socket(){
    int sock;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    return sock;
}

void communicate(int num_of_segments, sockaddr_in servaddr, int client_socket, string fn){
    priority_queue<data_packet> pq;
    for(int i=0;i<num_of_segments;i++){
        struct data_packet data;
        memset(&data, 0, sizeof(data));
        unsigned int servAddrLen = sizeof(servaddr);
        int n = recvfrom(client_socket, &data, sizeof(data), MSG_WAITALL, (struct sockaddr *) &servaddr, &servAddrLen);
        printf("Received Sequence Number : %d\n",data.seq_no);
        ack_packet ack = create_ack_packet(data.seq_no);
        sendto(client_socket, &ack, sizeof(ack), MSG_CONFIRM, (struct sockaddr *) &servaddr, sizeof(servaddr));
        pq.push(data);
    }
    std::vector<char> file_vector;
    while(!pq.empty()){
        data_packet d = pq.top();
        pq.pop();
        for(int i=0;i<d.len;i++){
            file_vector.push_back(d.data[i]);
        }
    }
    ofstream output_file(fn);
    ostream_iterator<char> output_iterator(output_file, "");
    copy(file_vector.begin(), file_vector.end(), output_iterator);
    close(client_socket);
    printf("Your File in Successfully Received");
}

void initiate_connection(int base_port, string filename, int client_socket){
    memset(&servaddr, 0, sizeof(servaddr));
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(base_port);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    uint32_t i = 0;
    struct data_packet file_packet = create_data_packet(string2vector(filename), i);
    sendto(client_socket, &file_packet, (int)sizeof(file_packet), 0, (struct sockaddr *) &servaddr, sizeof(servaddr));
    struct data_packet response;
    memset(&response, 0, sizeof(response));
    unsigned int servLen = sizeof(servaddr);
    int n = recvfrom(client_socket, &response, (int)sizeof(data_packet), 0, (struct sockaddr *) &servaddr, &servLen);
    if(response.len == 18){
        printf("Connection Refused\n");
        printf("%d\n",ntohs(servaddr.sin_port));
        close(client_socket);
    }else{
        printf("Connection Accepted\n");
        int filesize = 0;
        for(int i=19;i<response.len;i++){
            filesize = 10*filesize + (response.data[i] - '0');
        }
        printf("%d\n",filesize);
        printf("%d\n",ntohs(servaddr.sin_port));
        //start communicating with server
        int number_of_segments = filesize/512;
        if(filesize%512 != 0){
            number_of_segments++;
        }
        communicate(number_of_segments, servaddr, client_socket, filename);
        close(client_socket);
    }
}



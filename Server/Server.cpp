#include "Server.h"

data_packet create_packet(vector<char> v,uint32_t  &seq_no){
    return create_data_packet(v,seq_no);
}

bool is_packet_loss(int seed, double plp){
    return rand()%100 < (plp * 100);
}

void send_packet(data_packet &packet, int socket, sockaddr_in &cur_addr, int seed, double plp){
    if(!is_packet_loss(seed, plp)){
        sendto(socket, &packet, sizeof(packet), 0, (const struct sockaddr *) &cur_addr, sizeof(cur_addr));
    }else{
        printf("Packet Loss: %d\n",packet.seq_no);
    }
}


int recv_packet(int new_socket, ack_packet &ack, sockaddr_in &cliaddr, int &time_passed){
    struct timeval tv;
    tv.tv_sec = ((TIMEOUTINTERVAL - time_passed)/1000000);
    tv.tv_usec = ((TIMEOUTINTERVAL - time_passed)%1000000);
    setsockopt(new_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    unsigned int cliAddrLen = sizeof(cliaddr);
    return recvfrom(new_socket, &ack, sizeof(ack), 0, ( struct sockaddr *) &cliaddr, &cliAddrLen);
}

void handle_client(string fn, int new_socket, sockaddr_in new_addr, int seed, double plp) {
    //Initializing Server Data
    bool isAcked[1024] = {false};
    data_packet packets[1024];
    state st;
    double cwnd = 1;
    double ssthresh = 128;
    uint32_t seq_no = 1;
    int dup_ack = 0;

    vector<char> reading_file = file2vector(fn);
    vector<vector<char>> splitted_file = split_vector(reading_file, MSS);
    priority_queue<data_packet> packets_queue;
    for(int i=0;i<splitted_file.size();i++){
        data_packet p = create_packet(splitted_file[i],seq_no);
        packets_queue.push(p);
    }
    priority_queue<data_packet> window;
    auto timer_start = std::chrono::high_resolution_clock::now();
    int time_passed = 0;
    while(!packets_queue.empty() || !window.empty()){
        for(int i=window.size();i<cwnd && !packets_queue.empty();i++){
            data_packet to_be_sent = packets_queue.top();
            packets_queue.pop();
            send_packet(to_be_sent, new_socket, new_addr, seed, plp);
            window.push(to_be_sent);
        }
        ack_packet ack;
        int n = 0;
        struct sockaddr_in cliaddr;
        auto timer_end = std::chrono::high_resolution_clock::now();
        time_passed = (int)std::chrono::duration_cast<std::chrono::microseconds>(timer_end - timer_start).count();
        while((n = recv_packet(new_socket, ack, cliaddr, time_passed)) > 0){
            while(isAcked[window.top().seq_no] && !window.empty()){
                window.pop();
                timer_start = std::chrono::high_resolution_clock::now();
            }
            if(st == SlowStart){
                printf("Going to SlowStart\n");
                data_packet x = window.top();
                slow_start(ack, isAcked, dup_ack, st, ssthresh, cwnd, x, new_addr, new_socket, seed, plp);
            }else if(st == FastRecovery){
                printf("Going to FastRecovery\n");
                data_packet x = window.top();
                fast_recovery(ack, isAcked, dup_ack, st, ssthresh, cwnd, x, new_addr, new_socket, seed, plp);
            }else{
                printf("Going to Congestion Avoidance\n");
                data_packet x = window.top();
                congestion_avoidance(ack, isAcked, dup_ack, st, ssthresh, cwnd, x, new_addr, new_socket,seed,plp);
            }
            //empty window
            while(isAcked[window.top().seq_no] && !window.empty()){
                window.pop();
                timer_start = std::chrono::high_resolution_clock::now();
            }
            //fill window
            while(window.size() <  (int) cwnd && !packets_queue.empty()){
                data_packet p = packets_queue.top();
                packets_queue.pop();
                send_packet(p, new_socket, new_addr, seed, plp);
                window.push(p);
            }
        }
        if(n < 0){
            data_packet x = window.top();
            handle_timeout(x,cwnd,ssthresh,dup_ack,st,isAcked,packets,cliaddr,new_socket,seed,plp);
            timer_start = std::chrono::high_resolution_clock::now();
        }
    }
}

void slow_start(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet missing_packet, sockaddr_in new_addr, int socket, int seed, double plp){
    if(isAcked[ack.ack_no]){
        dup_ack++;
        if(dup_ack == 3){
            dup_ack = 0;
            st = FastRecovery;
            ssthresh = cwnd/2;
            cwnd = ssthresh + 3;
            printf("Window Size: %f\n",cwnd);
            send_packet(missing_packet, socket, new_addr, seed, plp);
        }
    }else{
        cwnd += 1;
        printf("Window Size: %f\n",cwnd);
        dup_ack = 0;
        if(cwnd >= ssthresh){
            st = CongestionAvoidance;
        }
    }
    isAcked[ack.ack_no] = true;
}

void congestion_avoidance(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet missing_packet, sockaddr_in new_addr, int socket, int seed, double plp){
    if(isAcked[ack.ack_no]){
        dup_ack++;
        if(dup_ack == 3){
            dup_ack = 0;
            st = FastRecovery;
            ssthresh = cwnd/2;
            cwnd = ssthresh + 3;
            printf("Window Size: %f\n",cwnd);
            send_packet(missing_packet, socket, new_addr, seed, plp);
        }
    }else{
        cwnd += 1/cwnd;
        printf("Window Size: %f\n",cwnd);
        dup_ack = 0;
    }
    isAcked[ack.ack_no] = true;
}

void fast_recovery(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet missing_packet, sockaddr_in new_addr, int socket, int seed, double plp){
    if(isAcked[ack.ack_no]){
        cwnd += 1;
        send_packet(missing_packet, socket, new_addr, seed, plp);
        printf("Window Size: %f\n",cwnd);
    }else{
        cwnd = ssthresh;
        printf("Window Size: %f\n",cwnd);
        dup_ack = 0;
        st = CongestionAvoidance;
    }
    isAcked[ack.ack_no] = true;
}

void handle_timeout(data_packet missing_packet,double &cwnd, double &ssthresh, int &dup_ack, state &st, bool isAcked[], data_packet packets[], sockaddr_in new_addr, int new_socket, int seed, double plp){
    ssthresh = cwnd/2;
    cwnd = 1;
    printf("Window Size: %f\n",cwnd);
    dup_ack = 0;
    st = SlowStart;
    send_packet(missing_packet, new_socket, new_addr, seed, plp);
}


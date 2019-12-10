#include "Server.h"

data_packet create_packet(vector<char> v,uint32_t  &seq_no){
    return create_data_packet(v,seq_no);
}

void start_timer(Timer &timer, bool isAcked[], double &ssthresh, double &cwnd, int &dup_ack, state &st, int &base, data_packet packets[],sockaddr_in new_addr, int socket, socklen_t len){
    timer.setTimeout([&](){
        ssthresh = cwnd/2;
        cwnd = 1;
        dup_ack = 0;
        st = SlowStart;
        //find first unacked packet
        data_packet missing_packet;
        bool found = false;
        for(int i=base;i<cwnd;i++){
            if(!isAcked[i]){
                missing_packet = packets[i];
                found = true;
                break;
            }
        }
        if(found){
            send_packet(missing_packet, socket, new_addr);
            timer.start();
        }else{
            timer.stop();
        }
    });
    timer.setSingleShot(true);
    timer.setInterval(Timer::Interval(20000));
    timer.start(true);
}

void send_packet(data_packet packet, int socket, sockaddr_in cur_addr){
    sendto(socket, &packet, sizeof(packet), 0, (const struct sockaddr *) &cur_addr, sizeof(cur_addr));
}

void handle_client(string fn, int new_socket, sockaddr_in new_addr) {
    //Initializing Server Data
    //Timer timer;
    bool isAcked[500] = {false};
    data_packet packets[500];
    state st;
    double cwnd = 1;
    double ssthresh = 128;
    int base = 1;
    uint32_t seq_no = 1;
    int dup_ack = 0;

    //start_timer(timer, isAcked, ssthresh, cwnd, dup_ack, st, base, packets, new_addr, new_socket);

    vector<char> reading_file = file2vector(fn);
    vector<vector<char>> splitted_file = split_vector(reading_file, MSS);
    priority_queue<data_packet> packets_queue;
    for(int i=0;i<splitted_file.size();i++){
        data_packet p = create_packet(splitted_file[i],seq_no);
        packets[p.seq_no] = p;
        packets_queue.push(p);
    }
    priority_queue<data_packet> window;
    while(!packets_queue.empty()){
        for(int i=0;i<cwnd && !packets_queue.empty();i++){
            data_packet to_be_sent = packets_queue.top();
            packets_queue.pop();
            window.push(to_be_sent);
            send_packet(to_be_sent, new_socket, new_addr);
        }
        ack_packet ack;
        int n = 0;
        struct sockaddr_in cliaddr;
        unsigned int cliAddrLen = sizeof(cliaddr);
        while((n = recvfrom(new_socket, &ack, sizeof(ack), MSG_WAITALL, ( struct sockaddr *) &cliaddr, &cliAddrLen)) > 0){
            if(st == SlowStart){
                slow_start(ack, isAcked, dup_ack, st, ssthresh, cwnd, packets, new_addr, new_socket);
            }else if(st == FastRecovery){
                fast_recovery(ack, isAcked, dup_ack, st, ssthresh, cwnd);
            }else{
                congestion_avoidance(ack, isAcked, dup_ack, st, ssthresh, cwnd, packets, new_addr, new_socket);
            }
            //empty window
            while(isAcked[window.top().seq_no] && !window.empty()){
                window.pop();
            }
            base = window.top().seq_no;
            //fill window
            while(window.size() <  (int) cwnd && !packets_queue.empty()){
                data_packet p = packets_queue.top();
                packets_queue.pop();
                send_packet(p, new_socket, new_addr);
                window.push(p);
            }
        }
    }
}

void slow_start(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet packets[], sockaddr_in new_addr, int socket){
    if(isAcked[ack.ack_no]){
        dup_ack++;
        if(dup_ack == 3){
            dup_ack = 0;
            st = FastRecovery;
            ssthresh = cwnd/2;
            cwnd = ssthresh + 3;
            send_packet(packets[ack.ack_no], socket, new_addr);
        }
    }else{
        cwnd += 1;
        dup_ack = 0;
        if(cwnd >= ssthresh){
            st = CongestionAvoidance;
        }
    }
    isAcked[ack.ack_no] = true;
}

void congestion_avoidance(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd, data_packet packets[], sockaddr_in new_addr, int socket){
    if(isAcked[ack.ack_no]){
        dup_ack++;
        if(dup_ack == 3){
            dup_ack = 0;
            st = FastRecovery;
            ssthresh = cwnd/2;
            cwnd = ssthresh + 3;
            send_packet(packets[ack.ack_no], socket, new_addr);
        }
    }else{
        cwnd += 1/cwnd;
        dup_ack = 0;
    }
    isAcked[ack.ack_no] = true;
}

void fast_recovery(ack_packet ack,bool isAcked[], int &dup_ack, state &st, double &ssthresh, double &cwnd){
    if(isAcked[ack.ack_no]){
        cwnd += 1;
    }else{
        cwnd = ssthresh;
        dup_ack = 0;
        st = CongestionAvoidance;
    }
    isAcked[ack.ack_no] = true;
}

void empty_window(priority_queue<data_packet> window, bool isAcked[], int &base){
    while(isAcked[window.top().seq_no] && !window.empty()){
        window.pop();
    }
    base = window.top().seq_no;
}

void fill_window(priority_queue<data_packet> window, priority_queue<data_packet> packets, double &cwnd, sockaddr_in new_addr, int socket){
    while(window.size() <  (int) cwnd && !packets.empty()){
        data_packet p = packets.top();
        packets.pop();
        send_packet(p, socket, new_addr);
        window.push(p);
    }
}


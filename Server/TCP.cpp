#include "TCP.h"

state current_state = SlowStart;
int dupAck = 0;
int cwnd = 1; //512
int ssthresh = 128; //64K
int sequence_no = 1;



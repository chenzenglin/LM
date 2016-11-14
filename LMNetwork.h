#ifndef LMNETWORK_H
#define LMNETWORK_H
#include "LMCore.h"
#include "LMJson.h"
#include "LMFileRecv.h"

class LMNetwork
{
public:
    int _udpfd;
    pthread_t _tid;
    char _buf[4096];

    static LMNetwork *instance();

    static void *thread_func(void *ptr);

    void *_thread_func(void *);

    void send(string msg, uint32_t ip = 0xffffffff);

private:
    LMNetwork();

    void handle_online(LMJson &json, uint32_t peerip);

    void handle_online_ack(LMJson &json, uint32_t peerip);

    void handle_send_msg(LMJsosn &json);

    void handle_send_file(LMJson &json, uint32_t peerip);
};

#endif // LMNETWORK_H

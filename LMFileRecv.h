#ifndef LMFILERECV_H
#define LMFILERECV_H
#include "LM.h"

class LMFileRecv
{
public:
    pthread_t _tid;
    string _peername;
    string _peerpath;
    uint32_t _peerip;
    char _buf[1024];

    LMFileRecv(string peername, string path, uint32_t peerip);

    static void *thread_func(void *arg);

    void run();

    void recv_file(int fd);

    char *get_line(FILE *fp);
};

#endif // LMFILERECV_H

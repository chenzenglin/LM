#ifndef LMFILESERVICE_H
#define LMFILESERVICE_H
#include "LMFileSend.h"

class LMFileService
{
public:
    pid_t _pid;
    int _listenfd;
    bool _b_child_process_end;
    map<pid_t, LMFileSend *> _file_send_process;

    static LMFileService *instance();

    void start();

    void sub_process_run();

    static void child_process_end(int sig);

private:
    LMFileService();
};

#endif // LMFILESERVICE_H

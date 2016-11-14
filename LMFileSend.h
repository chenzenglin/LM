#ifndef LMFILESEND_H
#define LMFILESEND_H
#include "LM.h"

class LMFileSend
{
public:
    pid_t _pid;
    int _fd;

    LMFileSend(int newfd, uint32_t ip);

    void process_send(int newfd, uint32_t ip);

    void send_file(char *path);

    char *change_cwd(char *path);

    int file_type(char *path);

    uint64_t file_size(char *path);

    void send_reg(char *path, FILE *fp);

    void send_dir(char *path, FILE *fp);
};

#endif // LMFILESEND_H

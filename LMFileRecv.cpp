#include "LMFileRecv.h"

LMFileRecv::LMFileRecv(string peername, string path, uint32_t peerip)
{
    _peername = peername;
    _peerpath = path;
    _peerip = peerip;
    pthread_create(&_tid, NULL, thread_func, this);
    pthread_detach(_tid);
}

void *LMFileRecv::thread_func(void *arg)
{
    LMFileRecv *This = (LMFileRecv *)arg;
    This->run();
    delete This;
}

void LMFileRecv::run()
{
       int fd = socket(AF_INET, SOCK_STREAM, 0);
       struct sockaddr_in addr;
       addr.sin_family = AF_INET;
       addr.sin_port = htons(LM_PORT_TCP);
       addr.sin_addr.s_addr = _peerip;
       int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
       if(ret < 0)
       {
           printf("error connect sender\n");
           return;
       }
       write(fd, _peerpath.c_str(), strlen(_peerpath.c_str()));
       write(fd, "\n", 1);
       recv_file(fd);
       close(fd);
}

void LMFileRecv::recv_file(int fd)
{
    FILE *fp = fdopen(fd, "r");
    char *p;
    while(1)
    {
        p = get_line(fp);
        if(strcmp(p, LM_FILEEOF) == 0)
        {
            break;
        }
        if(strcmp(p, LM_SEPARATE) != 0)
        {
            return;
        }
        p = get_line(fp);
        if(strcmp(p, LM_DIR) == 0)
        {
            p = get_line(fp);
            mkdir(p, 0777);
        }
        else if(strcmp(p, LM_REG) == 0)
        {
            p = get_line(fp);
            FILE *wfp = fopen(p, "w");
            uint64_t filelen = atoll(get_line(fp));
            while(filelen > 0)
            {
                int readlen = filelen > sizeof(_buf) ? sizeof(_buf) : filelen;
                int ret = fread(_buf, 1, readlen, fp);
                fwrite(_buf, 1, ret, wfp);
                filelen -= ret;
            }
            fclose(wfp);
        }
        else
        {
            printf("error file type\n");
            return;
        }
    }
    fclose(fp);
}

char *LMFileRecv::get_line(FILE *fp)
{
    fgets(_buf, sizeof(_buf), fp);
    _buf[strlen(_buf) - 1] = 0;
    return _buf;
}

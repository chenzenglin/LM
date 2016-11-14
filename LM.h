#ifndef LM_H
#define LM_H

#include <stdarg.h>
#include <inttypes.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/epoll.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <utime.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#define LM_PORT_UDP 23250
#define LM_PORT_TCP 23251
#define LM_CMD "cmd"
#define LM_ONLINE "online"
#define LM_ONLINEACK "onlineack"
#define LM_NAME "name"
#define LM_LIST "list"
#define LM_SEND "send"
#define LM_SENDF "sendf"
#define LM_MSG "msg"
#define LM_PATH "path"
#define LM_DIR "d"
#define LM_REG "r"
#define LM_SEPARATE "5h5h"
#define LM_FILEEOF "0"
#define LM_FILE_TYPE_REG 0
#define LM_FILE_TYPE_DIR 1
#define LM_FILE_TYPE_LNK 2
#define LM_FILE_TYPE_OTH 3

#endif // LM_H

#include "LMNetwork.h"

LMNetwork *LMNetwork::instance()
{
    static LMNetwork *theOne = NULL;
    if(!theOne)
        theOne = new LMNetwork;
    return theOne;
}

void *LMNetwork::thread_func(void *ptr)
{
    LMNetwork *This = instance();
    return This->_thread_func(ptr);
}

void *LMNetwork::_thread_func(void *)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    while(1)
    {
        memset(_buf, 0, sizeof(_buf));
        recvfrom(_udpfd, _buf, sizeof(_buf), 0, (struct sockaddr *)&addr, &len);
        list<uint32_t> &ips = LMCore::instance()->_ips;
        auto it = find(ips.begin(), ips.end(), addr.sin_addr.s_addr);
        if(it != ips.end())
        {
            continue;
        }
        LMJson json;
        if(!json.parse(_buf))
        {
            continue;
        }
        string cmd = json.get(LM_CMD);
        if(cmd == LM_ONLINE)
        {
            handle_online(json, addr.sin_addr.s_addr);
        }
        else if(cmd == LM_ONLINEACK)
        {
            handle_online_ack(json, addr.sin_addr.s_addr);
        }
        else if(cmd == LM_SEND)
        {
            handle_send_msg(json);
        }
        else if(cmd == LM_SENDF)
        {
            handle_send_file(json, addr.sin_addr.s_addr);
        }
    }
}

void LMNetwork::send(string msg, uint32_t ip)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LM_PORT_UDP);
    addr.sin_addr.s_addr = ip;
    sendto(_udpfd, msg.c_str(), strlen(msg.c_str()), 0, (struct sockaddr *)&addr, sizeof(addr));
}

LMNetwork::LMNetwork()
{
    _udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(LM_PORT_UDP);
    addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(_udpfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret < 0)
    {
        printf("bind error\n");
        close(fd);
        exit(1);
    }
    int opt = 1;
    setsockopt(_udpfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    pthread_create(&_tid, NULL, thread_func, NULL);
}

void LMNetwork::handle_online(LMJson &json, uint32_t peerip)
{
    string name = json.get(LM_NAME);
    LMCore::instance()->add_user(peerip, name);
    LMCore resp;
    resp.add(LM_CMD, LM_ONLINEACK);
    resp.add(LM_NAME, LMCore::instance()->name);
    send(resp.print(), peerip);
}

void LMNetwork::handle_online_ack(LMJson &json, uint32_t peerip)
{
    string name = json.get(LM_NAME);
    LMCore::instance()->add_user(peerip, name);
}

void LMNetwork::handle_send_msg(LMJosn &json)
{
    string name = json.get(LM_NAME);
    string msg = JSOON.GET(LM_MSG);
    printf("%s say:%s\n", name.c_str(), msg.c_str());
}

void LMNetwork::handle_send_file(LMJson &json, uint32_t peerip)
{
    string name = json.get(LM_NAME);
    string path = json.get(LM_PATH);
    printf("ready to recv file %s, from %s\n", path.c_str(), name.c_str());
    new LMFileRecv(name, path, peerip);
}

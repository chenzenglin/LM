#include "LMUserInput.h"

LMUserInput::LMUserInput()
{

}

void LMUserInput::loop()
{
    get_cmd();
    split_cmd();
    handle_cmd();
}

void LMUserInput::get_cmd()
{
    while(1)
    {
        fgets(_buf, sizeof(_buf), stdin);
        if(strlen(_buf) > 1)
        {
            break;
        }
    }
    _buf[strlen(_buf) - 1] = 0;
}

void LMUserInput::split_cmd()
{
    _args.clear();
    char *saveptr = NULL;
    char *first = strtok_r(_buf, ":", &saveptr);
    char *content = strtok_r(NULL, "\0", &saveptr);
    char *cmd = strtok_r(first, " \t", &saveptr);
    char *user = strtok_r(NULL, "\0", &saveptr);
    _args.push_back(cmd);
    if(user)
    {
        _args.push_back(user);
    }
    if(content)
    {
        _args.push_back(content);
    }
}

void LMUserInput::handle_cmd()
{
    if(_args[0] == LM_LIST)
    {
        handle_list();
    }
    if(_args[0] == LM_SEND)
    {
        handle_send();
    }
    if(_args[0] == LM_SENDF)
    {
        handle_sendf();
    }
}

void LMUserInput::handle_list()
{
    LMCore *core = LMCore::instance();
    printf("user list:\n");
    for(auto it = core->_others.begin(); it != core->_others.end(); it++)
    {
        LMOther *other = it->second;
        string name = other->_name;
        string ip = LMUtil::ipaddr(other->_ip);
        printf(" %s(%s)\n", name.c_str(), ip.c_str());
    }
}

void LMUserInput::handle_send()
{
    if(_args.size() < 3)
    {
        printf("args error\n");
        return;
    }
    string &ip = _args[1];
    string &msg = _args[2];
    bool b = LMUtil::is_broadcast(ip);
    LMJson json;
    json.add(LM_CMD, LM_SEND);
    json.add(LM_NAME, LMCore::instance()->_name);
    if(b)
    {
        json.add(LM_MSG, msg + "[broadcast]");
    }
    else
    {
        json.add(LM_MSG, msg);
    }
    LMNetwork::instance()->send(json.print(), inet_addr(ip.c_str()));
}

void LMUserInput::handle_sendf()
{
    if(_args.size() < 3)
    {
        printf("args error\n");
        return;
    }
    string &ip = _args[1];
    string &path = _args[2];
    LMJson json;
    json.add(LM_CMD, LM_SENDF);
    json.add(LM_NAME, LMCore::instance()->_name);
    json.add(LM_PATH, path);
    LMNetwork::instance()->send(json.print(), inet_addr(ip.c_str()));
}

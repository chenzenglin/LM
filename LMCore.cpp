#include "LMCore.h"
#include "LMUtil.h"

void LMCore::add_user(uint32_t ip, string name)
{
    LMLock lock;
    LMOther *other;
    auto it = _others.find(ip);
    if(it == _others.end())
    {
        other = new LMOther;
        other->_ip = ip;
        other->_name = name;
        _others[ip] = other;
    }
    else
    {
        _others[ip]->_name = name;
    }
}

LMCore::LMCore()
{
    _ips = LMUtil::get_local_ip_address();
    _name = LMUtil::get_hostname();
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_mutex, &attr);
}

LMLock::LMLock()
{
    pthread_mutex_lock(&LMCore::instance()->_mutex);
}

LMLock::~LMLock()
{
    pthread_mutex_unlock(&LMCore::instance()->_mutex);
}

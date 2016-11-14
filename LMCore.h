#ifndef LMCORE_H
#define LMCORE_H

#include "LM.h"

class LMOther
{
public:
    string _name;
    uint32_t _ip;
};

class LMCore
{
public:
    string _name;
    list<uint32_t> _ips;
    map<uint32_t, LMOther *> _others;
    pthread_mutex_t _mutex;

    void add_user(uint32_t ip, string name);

    static LMCore *instance()
    {
        static LMCore *theOne = NULL;
        if(!theOne)
            theOne = new LMCore;
        return theOne;
    }

private:
    LMCore();
};

class LMLock
{
public:
    LMLock();

    ~LMLock();
};

#endif // LMCORE_H

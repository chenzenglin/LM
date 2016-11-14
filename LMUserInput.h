#ifndef LMUSERINPUT_H
#define LMUSERINPUT_H
#include "LMCore.h"
#include "LMUtil.h"
#include "LMNetwork.h"

class LMUserInput
{
public:
    char _buf[4096];
    vector<string> _args;

    LMUserInput();

    void loop();

private:
    void get_cmd();

    void split_cmd();

    void handle_cmd();

    void handle_list();

    void handle_send();

    void handle_sendf();
};

#endif // LMUSERINPUT_H

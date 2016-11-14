#ifndef LMJSON_H
#define LMJSON_H
#include "LM.h"
#include "cJSON.h"

class LMJson
{
public:
    LMJson();

    ~LMJson();

    void add(string key, string value);

    string print();

    bool parse(char *buf);

    string get(string key);

private:
    cJSON *_root;

    LMJson(const LMJson &);

    LMJson &operator=(const LMJson &);
};

#endif // LMJSON_H

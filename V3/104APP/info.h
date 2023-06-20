#ifndef INFO_H
#define INFO_H

#include "config.h"

class Info
{
private:
    unsigned char *info;
    int len;
public:
    Info();
    ~Info() = default;
    unsigned char* getInfo();
    void setInfo(unsigned char *data,int len);
    int getLen();
    void showInfo();
};

#endif // INFO_H

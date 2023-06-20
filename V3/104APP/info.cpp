#include "info.h"

Info::Info()
{
    info = new unsigned char[BUF_LEN];
    memset(info,0,BUF_LEN);
    len = 0;
}

unsigned char *Info::getInfo()
{
    return info;
}

void Info::setInfo(unsigned char *data, int len)
{
    memcpy(info,data,len);
    this->len = len;
}

int Info::getLen()
{
    return len;
}

void Info::showInfo()
{
    show_hex((char*)info,len);
}

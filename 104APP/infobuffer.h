#ifndef INFOBUFFER_H
#define INFOBUFFER_H

#include "info.h"
#include "config.h"
#include <queue>

class InfoBuffer
{
private:
    queue<Info> Q;
    int MaxSize;
    int Size;
public:
    InfoBuffer();
    ~InfoBuffer() = default;
    int GetMaxSize();
    int GetSize();
    bool push(unsigned char *data, int len);
    bool pop(Info &top);
};

#endif // INFOBUFFER_H

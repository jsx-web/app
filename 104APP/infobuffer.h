#ifndef INFOBUFFER_H
#define INFOBUFFER_H

#include "info.h"
#include "config.h"
#include <queue>
#include <mutex>
#include <condition_variable>

class InfoBuffer
{
private:
    queue<Info> Q;
    int MaxSize;
    int Size;
    std::mutex mtx;
    std::condition_variable cv;
public:
    InfoBuffer();
    ~InfoBuffer() = default;
    int GetMaxSize();
    int GetSize();
    //添加
    bool push(unsigned char *data, int len);
    //多线程不建议用
    bool pop(Info &top);
    //等待消息，同步的，只有接收到消息才会返回。
    void wait(Info &top);
    //轮询消息，无论是否接收到消息轮询消息会立刻返回
    bool poll(Info &top);
};

#endif // INFOBUFFER_H

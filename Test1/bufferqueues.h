#ifndef BUFFERQUEUES_H
#define BUFFERQUEUES_H

#include<queue>
#include "config.h"


class BufferQueues
{
private:
//    queue<int> q;
    queue<unsigned char[BUF_LEN]> buffer;
    int BufferMax;          //队列的最大容量
    int Backtrace_index;    //如果丢失，回溯节点
    int NowTrace_index;     //最新的位置
    int CountNow;           //对列的当前容量

public:
    BufferQueues();
    ~BufferQueues();

    int getBufferMax();
    void setBufferMax(int buffermax);
    int getBacktrace_index();
    void setBacktrace_index(int backtrace_index);
    int getNowTrace_index();
    void setNowTrace_index(int nowtrace_index);
    int getCountNow();
    void setCountNow(int countnow);

    void add(unsigned char value[BUF_LEN]);
    void show_NowIndex();
    unsigned char* getBuffer(int index);
    unsigned char* buffercpy(unsigned char** Dest,unsigned char* Src);


};

#endif // BUFFERQUEUES_H

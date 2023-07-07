#include "infobuffer.h"

InfoBuffer::InfoBuffer()
{
    MaxSize = BUFFERSIZE;
}

int InfoBuffer::GetMaxSize()
{
    std::unique_lock<std::mutex> lck(mtx);
    return MaxSize;
}

int InfoBuffer::GetSize()
{
    std::unique_lock<std::mutex> lck(mtx);
    return Q.size();
}

bool InfoBuffer::push(unsigned char *data, int len)
{
    std::unique_lock<std::mutex> lck(mtx);
    if(len <= BUF_LEN)
    {
        int size = Q.size();
        if(size <= MaxSize)
        {
            Info Data;
            Data.setInfo(data,len);
            Q.push(Data);
            cv.notify_one();
            return true;
        }
        else
        {
            cout << "InfoBuffer is full" << endl;
            return false;
        }
    }
    else
    {
        cout << "InfoData so long(>=256)" << endl;
        return false;
    }
}

bool InfoBuffer::pop(Info &top)
{
    if(!Q.empty())
    {
        top = Q.front();
        Q.pop();
        return true;
    }
    else
    {
       cout << "BuffInfo is empty" << endl;
       return false;
    }
}

void InfoBuffer::wait(Info &top)
{
    std::unique_lock<std::mutex> lck(mtx);
    while (!Q.size()) cv.wait(lck);     //如果队列未空,就阻塞等待,wait函数释放锁,返回
    top = Q.front();
    Q.pop();
}

bool InfoBuffer::wait_time(Info &top)
{
    std::unique_lock<std::mutex> lck(mtx);
    while (!Q.size())
    {
        if(cv.wait_for(lck,std::chrono::seconds(TESTFRAMETIMEOUT)) == std::cv_status::timeout)     //如果队列未空,就阻塞等待,wait函数释放锁,15s超时
        {
            return false;
        }
    }
    top = Q.front();
    Q.pop();
    return true;
}

bool InfoBuffer::poll(Info &top)
{
    QThread::msleep(300);
    std::unique_lock<std::mutex> lck(mtx);
    if (Q.size())
    {
        top = Q.front();
        Q.pop();
        return true;
    }
    return false;
}

bool InfoBuffer::clearBuffer()
{
    while(Q.size())
    {
        Q.pop();
    }
    if(Q.size() == 0)
    {
        return true;
    }
    return false;
}

bool InfoBuffer::top(Info &top)
{
    QThread::msleep(300);
    std::unique_lock<std::mutex> lck(mtx);
    if (Q.size())
    {
        top = Q.front();
        return true;
    }
    return false;
}

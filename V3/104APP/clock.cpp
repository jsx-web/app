#include "clock.h"


Clock::Clock(int interval)
{
    start = 0;
    end = 0;
    this->interval = interval;
}

void Clock::Start()
{
    start = clock();
}

clock_t Clock::GetStart()
{
    return start;
}

void Clock::End()
{
    end = clock();
}

clock_t Clock::GetEnd()
{
    end = clock();
    return end;
}

bool Clock::IsTimeOut()
{
    end = clock();
    double clc=(double)(end-start)/CLOCKS_PER_SEC;
    if(clc>interval)
    {
//        cout << "timeout " << endl;
        return true;
    }
    return false;
}

void Clock::Flash()
{
    start = clock();    //起始时间刷新成当前时间
    end = clock();      //结束时间刷新成当前时间
}

void Clock::Stop()
{
    start = 0;
    end = 0;
}

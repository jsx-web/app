#ifndef CLOCK_H
#define CLOCK_H

#include<ctime>
#include "config.h"

class Clock
{
private:
    clock_t start;
    clock_t end;
    int interval;
public:
    Clock(int interval = 5);
    void Start();
    clock_t GetStart();
    void End();
    clock_t GetEnd();
    bool IsTimeOut();
    void Flash();

};

#endif // CLOCK_H

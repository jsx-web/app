#ifndef FRAME_H
#define FRAME_H

#include "config.h"
#include <iostream>

using namespace std;

class Frame
{
private:
    unsigned char APCI[6];
    unsigned char *ASDU;
    unsigned char *APDU;
    char type;

public:
    Frame();
    Frame(char FrameTpye);
    ~Frame();
    unsigned char* GetAPDU();
    char GerTpye();
    bool SetTotalCall();
    bool SetStartFrame();
    int Getlen();
};

#endif // FRAME_H

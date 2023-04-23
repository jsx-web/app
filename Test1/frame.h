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
    unsigned char* GetAPCI();
    unsigned char* GetASDU();
     int Getlen();
    char GerTpye();

    bool SetinitFrame();
    bool SetTotalCall();

protected:
    bool setAPDU();
};

#endif // FRAME_H

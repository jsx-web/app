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
protected:
    bool setAPDU();
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
    bool SetClockSyn();
    bool SetClockRead();
    bool SetReset();
    bool SetDirCall(char* DirName,char CallFlage = 0x00,st_cp56time2a start = {.Time = 0x00000000000000},st_cp56time2a end = {.Time = 0x00000000000000});
    bool SetReadFile(char* FileName);



};

#endif // FRAME_H

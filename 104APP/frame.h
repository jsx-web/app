#ifndef FRAME_H
#define FRAME_H

#include "config.h"
#include <fileoperation.h>

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

    bool setSFrame(int VS, int ACK);
    void updataCON(int VS, int ACK);
    bool setTestFrame();
    bool SetinitFrame();
    bool SetTotalCall();
    bool SetClockSyn();
    bool SetClockRead();
    bool SetReset();
    bool SetDirCall(char* DirName,char CallFlage = 0x00,st_cp56time2a start = {.Time = 0x00000000000000},st_cp56time2a end = {.Time = 0x00000000000000});
    bool SetReadFile(char* FileName);
    bool SetWriteFile(char* FileName);
    bool SetWriteData(QString filepath,int &offset);
    bool SetReadFixedValueNum();
    bool SetToggleFixedValueNum(int FixedValueNum);
    bool SetReadValue(int Value,QStringList ObjAddr);
    bool SetPerset(int ValueNum,int status,QStringList addrlsit,QStringList taglist,QStringList valuelist);



};

#endif // FRAME_H

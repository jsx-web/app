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

    bool setSFrame(int VR);
    void updataCON(int VS, int ACK);
    bool setTestFrame();
    bool SetinitFrame();
    bool SetTotalCall();
    bool SetClockSyn(unsigned char *CP56Time2aNOW);
    bool SetClockRead();
    bool SetReset();
    bool SetDirCall(char* DirName,char CallFlage = 0x00,st_cp56time2a start = {.Time = 0x00000000000000},st_cp56time2a end = {.Time = 0x00000000000000});
    bool SetReadFile(char* FileName);
    bool SetReadConfirm(unsigned char *FileID, unsigned char *FileOffset, unsigned char desc);
    bool SetWriteFile(char* FileNamePath);
    bool SetWriteData(QString filepath,int &offset);
    bool SetReadFixedValueNum();
    bool SetToggleFixedValueNum(int FixedValueNum);
    bool SetReadValue(int Value,QStringList ObjAddr);
    bool SetPerset(int ValueNum,int status,QStringList addrlsit,QStringList taglist,QStringList valuelist);
    bool SetPersetCancel(int ValueNum);
    bool SetCuring(int ValueNum);
    bool SetEnergyCall();
    bool SetUpdataStart();
    bool SetRunUpdata();
    bool SetAbandonUpdata();


};

#endif // FRAME_H

#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include "frame.h"
#include <QString>
#include "client.h"
#include "config.h"

using namespace std;

class Session
{
private:
    unsigned char *recvBuf;
    int bufLen;
    Frame *SendFrame;
    Frame *RecvFrame;
    QString FrameName;
    bool isInit; // 初始化成功/失败
    Client *client_session;
public:
    Session();
    ~Session();
    unsigned char* GetrecvBuf();
    int GetBufLen();
    void SetRecvBuf(char* recv);
    bool EncodeBuff(unsigned char *recvbuf, int len, unsigned char* C,
                    unsigned char T1 = 0x00, unsigned char VSQ = 0x00, unsigned char *COT = nullptr,
                    unsigned char *ASDU_addr=nullptr, unsigned char *obj_addr = nullptr,
                    unsigned char QOI=0x00, unsigned char *obj = nullptr);
    
    void FrameSend(Client *&client,Frame &Frame);
    //void RecvThread();
    //void FrameRecv(Client *&client,Frame &Frame);
    bool InitSession(Client *&client);
    bool InitSessionSuccess(Client *client);
    bool TotalCallSession(Client *&client);
    bool TotalCallSessionSuccess(Client *client);
    



};



#endif // SESSION_H

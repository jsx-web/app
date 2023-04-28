#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include "frame.h"
#include <QString>
#include "client.h"
#include "config.h"
#include "file.h"
#include "fileoperation.h"
#include <QTextEdit>
#include <QTextBrowser>

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
    QTextEdit *recv_Edit;
    void updata_recv_edit(unsigned char* data);
    QTextBrowser *debug_Edit;
    void updata_debug_edit(unsigned char* data);
    Session();
    ~Session();
    unsigned char* GetrecvBuf();
    int GetBufLen();
    Frame GetSendFrame();
    void SetRecvBuf(char* recv);
    bool EncodeBuff(unsigned char *recvbuf, unsigned char len, unsigned char* C,
                    unsigned char T1 = 0x00, unsigned char VSQ = 0x00, unsigned char *COT = nullptr,
                    unsigned char *ASDU_addr=nullptr, unsigned char *obj_addr = nullptr,
                    unsigned char QOI=0x00, unsigned char *obj = nullptr);
    
    void FrameSend(Client *&client,Frame &Frame);
    //void RecvThread();
    //void FrameRecv(Client *&client,Frame &Frame);
    // 初始化
    bool InitSession(Client *&client);
    bool InitSessionSuccess(Client *client);
    //总召
    bool TotalCallSession(Client *&client);
    bool TotalCallSessionSuccess(Client *client);
    //时钟同步
    Frame ClockSynSession(Client *client);
    bool ClockSynSessionSuccess(Client *client,Frame &Frame);
    //时钟读取
    bool ClockReadSession(Client *client);
    bool ClockReadSessionSuccess(Client *client);
    //复位
    bool ResetSession(Client *client);
    bool ResetSessionSuccess(Client *client);
    //目录召唤
    bool DirCallSession(Client *client,char* DirName);
    bool DirCallSessionSuccess(Client *client);
    //文件读取
    bool ReadFileSession(Client *client,char* DirName);
    bool ReadFileSessionSuccess(Client *client);

signals:
    void Recv(char* buf);  //接收数据信号
    
};



#endif // SESSION_H

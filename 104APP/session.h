#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "frame.h"
#include "terminal.h"
#include "file.h"
#include "fileoperation.h"
#include "clock.h"
#include "infobuffer.h"
#include "config.h"


class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr);
    ~Session();
private:
    unsigned char *recvBuf;
    InfoBuffer *recvInfoBuffer;
    int bufLen;
    bool isInit;                //初始化成功/失败
    bool isClose;               //会话关闭
    Terminal *SessionTerminal;  //会话的套接字
    Clock *t0;                  //t0:建立连接的超时的计时器
    Frame *SendFrame;
    //V（S）为发送状态变量，V（R）为接收状态变量，ACK为当前已经正确收到的所有I格式
    int V_S;    //发送状态变量
    int V_R;    //接收状态变量
    int ACK;    //当前已经正确收到的所有I格式
public:
    unsigned char* GetrecvBuf();
    int GetBufLen();
    Terminal * GetTerminal();
    Frame GetSendFrame();
    void SessionClose();
    bool IsConnect();
    void SetRecvBuf(char* recv);
    bool EncodeBuff(unsigned char *recvbuf, unsigned char len, unsigned char* C,
                        unsigned char T1 = 0x00, unsigned char VSQ = 0x00, unsigned char *COT = nullptr,
                        unsigned char *ASDU_addr=nullptr, unsigned char *obj_addr = nullptr);
    void FrameSend(Frame &frame);
    int FrameRecv(unsigned char (&recvBuff)[BUF_LEN]);
    //I格式C域的序号编号serial number
    bool IFrameCBlocksSerialNumber(char (&C)[4]);
    //I格式C域的序号判定
    bool IFrameCBlocksSerialConfirm(char (&C)[4]);

signals:
    //----------连接是否成功-------
    void ConnectRet(int status,QString IP , qint16 Port);
    //-----------接收框显示--------
    void append_data(int author,char type,QString data);/*@author： 0是主站，1是从站*/
    //---------调试消息显示-------
    void UpdataDebugEdit(QString msg);
    //--------根据model写入数据----
    void UpdataTableView(int modelID,int modellen,QStringList data);
    //--------总召唤数据显示-------
    void ShowDataTotalCall(int packid,QString T1,QString obj_addr,QString data,QString QDS,QStringList TimeList);
    //--------时钟控制的显示------
    void ColckSyn(unsigned char T1,unsigned char*Time,int len);

public slots:

public:
    //接收帧显示
    void updata_recv_edit(int author,unsigned char* data);
    //建立连接
    bool ConnectSession(QString IP,qint16 Port);
    // 初始化
    bool InitSession();
    bool InitSessionSuccess();
    //总召
    bool TotalCallSession();
    bool TotalCallSessionSuccess();
    bool TotalCallData(int packid,unsigned char T1,unsigned char* obj_addr,unsigned char* data);
    //时钟同步
    Frame ClockSynSession();
    bool ClockSynSessionSuccess(Frame &Frame);

};

#endif // SESSION_H

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
    FileOperation *wfileop;
    int ValueMAX;
    int ValueMIN;
    bool isPerset;      //是否进行预置
public:
    bool Signals_Slot;  //信号和信号槽连接标识
    unsigned char* GetrecvBuf();
    int GetBufLen();
    Terminal * GetTerminal();
    Frame GetSendFrame();
    bool GetIsPerset();
    void SetIsPerset(bool PerSet);
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
    void ShowDataTotalCall(int packid,QString T1,QString obj_addr,QString data,QString QDS,QString TimeList);
    //--------时钟控制的显示------
    void ColckControl(int operation, char sender, QString Time);
    //--------复位的显示---------
    void ResetShow(int status, QString decs);
    //-------文件目录的显示-------
    void ShowDirname(char* name,int n);
    //---------进度条控制--------
    void ReadMax(int maxSize);  //读文件最大值信号
    void ReadValue(int Value);//Value值更新信号
    void WriteMax(int maxSize);  //写文件最大值信号
    void WriteValue(int Value);//Value值更新信号
   //---------激活标签控制---------
   void ReadLabelEnable(int status);
   void WriteLabelEnable(int status);
   //-----远程参数读写-------------
   void RemoteValueControl(int operation, int ValueNum, int ValueMin, int ValueMax,
                                           unsigned char Flag,QString ObjAddr, unsigned char Tag,
                                           QString Value);
   //----------电能量召唤数据显示---------
   void ShowDataEnergyCall(int packid,QString T1,QString obj_addr,QString data,QString QDS,QString Time);

public slots:

public:
    //接收帧显示
    void updata_recv_edit(int author,unsigned char* data);
    //测试链路
    bool TestSession();
    bool TestSessionSuccess();
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
    bool ClockSynSessionSuccess(Frame &frame);
    //时钟读取
    bool ClockReadSession();
    bool ClockReadSessionSuccess();
    //复位
    bool ResetSession();
    bool ResetSessionSuccess();
    //目录召唤
    bool DirCallSession(char* DirName);
    bool DirCallSessionSuccess();
    //文件读取
    bool ReadFileSession(char* RFileName);
    bool ReadFileSessionSuccess(QString DirName=QString());
    //文件写入
    Frame WriteFileSession(char* WFileName);
    bool WriteFileSessionEnable(Frame &frame);
    bool WriteFileSessionConfirm(Frame &frame,int offset);
    bool WriteFileSessionSuccess(Frame &frame,QString filename);
    //------远程参数读写-------
    //读取定值区号
    //读定值区号
    bool ReadFixedValueNumSession();
    bool ReadFixedValueNumSessionSuccess();
    //切换定值区号
    bool SetFixedValueNumSession(int FixedValueNum);
    bool SetFixedValueNumSessionSuccess(int FixedValueNum);
    //读多个或全部参数
    bool ReadValueSession(int Value,QStringList ObjAddr);
    bool ReadValueSessionSuccess(int Value);
    //参数预置
    bool PersetSession(int ValueNum,int status,QStringList addr,QStringList tag,QStringList value);
    bool PersetSessionSuccess(int ValueNum,int status);
    //参数取消预置
    bool PersetCancelSession(int ValueNum);
    bool PersetCancelSessionSuccess(int ValueNum);
    //参数固化
    bool CuringSession(int ValueNum);
    bool CuringSessionSuccess(int ValueNum);
    //电能量召唤
    bool EnergyCallSession();
    bool EnergyCallSessionSuccess();
    bool EnergyCallData(int packid,unsigned char T1,unsigned char* obj_addr,unsigned char* data);
    //软件升级
    bool UpdataVersionSession();
    bool UpdataVersionSessionSuccess();

};

#endif // SESSION_H

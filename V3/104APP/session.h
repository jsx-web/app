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
//    std::mutex mtx1;
    std::mutex mtxV_R;
    unsigned char *Recv_buf;
    unsigned char *Msg_buf;
    int msgBufLen = 0;
    InfoBuffer *recvInfoBuffer;
    InfoBuffer *I_InfoBuffer;    //存放接收帧(I帧)
    InfoBuffer *U_InfoBuffer;    //存放接收帧(U帧)
    Frame *SFrame;
    Frame *ReportSFrame;
    InfoBuffer *S_InfoBuffer;    //存放接收帧(S帧)
    Frame *TestFrame;
    InfoBuffer *TEST_InfoBuffer; //存放测试帧(测试)
    InfoBuffer *Report_InfoBuffer; //存放上报帧(上报)
    
    int bufLen;
    bool isClose;               //会话关闭
    Terminal *SessionTerminal;  //会话的套接字
    Clock *t0;                  //t0:建立连接的超时的计时器           30s 如果在t0时间内未能成功建立连接，可能网络发生了故障，主站端应该向运行人员给出警告信息。
    Clock *t1;                  //t1:发送或测试 APDU 的超时          15s t1规定发送方发送一个I格式报文或U格式报文后,必须在t1的时间内得到接收方的认可,否则发送方认为TCP连接出现问题并应重新建立连接。
    Clock *t2;                  //t2:无数据报文时确认的超时,t2<t1     10s t2规定接收方在接收到I格式报文后,若经过t2时间未再收到新的I格式报文,则必须向发送方发送S格式帧对已经接收到的I格式报文进行认可,显然t2必须小于t1。
    Clock *t3;                  //t3:长期空闲状态下发送测试帧的超时     20s t3规定调度端或子站RTU端每接收一帧I帧、S帧或者U帧将重新触发计时器t3,若在t3内未接收到任何报文,将向对方发送测试链路帧
    Clock *test;                //发送测试帧的间隔                   12s
    Frame *SendFrame;
    //V（S）为发送状态变量，V（R）为接收状态变量，ACK为当前已经正确收到的所有I格式
    int V_S;    //发送状态变量
    int V_R;    //接收状态变量
    int ACK;    //当前已经正确收到的所有I格式
    FileOperation *wfileop;
    int ValueMAX;
    int ValueMIN;
    bool isPerset;      //是否进行预置
    bool UpdataStop;    //软件升级终端
    bool StartLevelUp;  //升级是否启动
    QString VersionFileName;
    bool WaitRecvTest;  //是否等待回复帧
    bool TestLink;      //测试链路是否存在
    int K;              // 发送序号和接收序号之间的最大差值   【发送方在发送K个I报文还未收到确认就应该关闭数据传送 默认值为12；从站使用】
    int W;              // 【接收方最迟收到W个I就必须要回复确认帧 默认值为8；主站使用】    // W 不能够超过 K 的2/3
    int RecvIFrameCount;// 接收I帧的计数
    int RecvReportFrameCount;// 接收上报帧的计数

public:
     bool wait_for_time(Info &top,InfoBuffer &buff);
public:
    bool isInit;                //初始化成功/失败
    bool Signals_Slot;  //信号和信号槽连接标识
    //Get方法
    unsigned char* GetrecvBuf();
    int GetBufLen();
    bool GetStartLevelUp();
    Terminal * GetTerminal();
    Frame GetSendFrame();
    bool GetIsPerset();
    bool GetWaitRecvTest();
    //Set方法
    void SetTestLink(bool status);
    void SetWaitRecvTest(bool status);
    void SetIsPerset(bool PerSet);
    void SetRecvBuf(char* recv);

    //其他方法
    void SessionClose();
    bool IsConnect();
    bool EncodeBuff(unsigned char *recvbuf, unsigned char len, unsigned char* C,
                        unsigned char T1 = 0x00, unsigned char VSQ = 0x00, unsigned char *COT = nullptr,
                        unsigned char *ASDU_addr=nullptr, unsigned char *obj_addr = nullptr);
    void FrameSend(Frame frame);
    int FrameRecv(unsigned char (&recvBuff)[BUF_LEN]);
    int RecvBuf();
    int FrameData(unsigned char *buff);

    // 取出I帧，并判断序号
    Info I_TakeInfo(int &n);
    //I格式C域的序号编号serial number
    bool IFrameCBlocksSerialNumber(char (&C)[4]);
    //I格式C域的序号判定
    bool IFrameCBlocksSerialConfirm(char (&C)[4]);

    //对VR原子操作
    void SetV_R();
    int GetV_R();

    //重置所有变量
    bool ReSetValue();

signals:
    //----------连接是否成功-------
    void ConnectRet(int status,QString IP , qint16 Port);
    //----------测试帧接收判定信号----
    void TestConfirm();
    void TestConfirmSuccess();
    void TestConfirmTimeOut();
    //-----------接收框显示--------
    void AppendData(int author,QString type,QString data);/*@author： 0是主站，1是从站*/
    void AppendTest(int author,QString type,QString data);/*@author： 0是主站，1是从站*/
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
    void UpdataMax(int Max);//软件升级最大值信号
    void UpdataValue(int Value);//Value值更新信号
   //---------激活标签控制---------
   void ReadLabelEnable(int status);
   void WriteLabelEnable(int status);
   void UpdataLabelEnable(int status);
   //-----远程参数读写-------------
   void RemoteValueControl(int operation, int ValueNum, int ValueMin, int ValueMax,
                                           unsigned char Flag,QString ObjAddr, unsigned char Tag,
                                           QString Value);
   //----------电能量召唤数据显示---------
   void ShowDataEnergyCall(int packid,QString T1,QString obj_addr,QString data,QString QDS,QString Time);
   //----------软件升级文件传输完成-------
   void UpdataSuccess();
   //-----------软件升级显示------------
   void VersionUpdata(int operation,QString VersionFile,int status);
   //-----------读文件传输确认------------
   void ReadConfirm(int status, unsigned char *FileID, unsigned char *FileOffset, unsigned char desc);
   //---------上报数据显示---------------
   void Report(int type,QString format,QString data);

public slots:
    void slotUpdataStop();
public:
    //发送S帧
    bool SFrameSendSession();
    bool ReportSFrameSendSession();
    //接收S帧
    bool SFrameRecvSession();
    //判定是否发送S帧
    bool SFrameSession();

    //接收线程
    void RecvData();
    //接收帧显示
    void updata_recv_edit(int author,unsigned char* data);/*@author： 0是主站，1是从站*/
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
    Frame ClockSynSession(unsigned char *CP56Time2aNOW);
    bool ClockSynSessionSuccess(unsigned char *CP56Time2aNOW);
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
    bool ReadConfirmSession(unsigned char *FileID, unsigned char *FileOffset, unsigned char desc);
    //文件写入
    Frame WriteFileSession(char* WFileName,int Operation);
    bool WriteFileSessionEnable(Frame &frame,int Operation);
    bool WriteFileSessionConfirm(Frame &frame,int offset,int Operation);
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
    bool UpdataVersionTransferSession(QString VersionFile);
    bool UpdataVersionSessionSuccess(QString VersionFile);
    //传输成功后确认
    bool RunUpdataSession();
    bool RunUpdataSessionSuccess();
    //传输成功后放弃
    bool AbandonUpdataSession();
    bool AbandonUpdataSessionSuccess();
    //处理上报帧
    bool ReportDataSession();

};

#endif // SESSION_H

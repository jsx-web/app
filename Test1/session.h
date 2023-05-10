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
#include <QTableView>
#include <QStandardItemModel>
#include <QProgressBar>
#include <QLabel>

using namespace std;

class Session:public QObject
{
    Q_OBJECT

private:
    unsigned char *recvBuf;
    int bufLen;
    Frame *SendFrame;
    Frame *RecvFrame;
    QString FrameName;
    bool isInit;        // 初始化成功/失败
    Client *client_session;
    FileOperation *wfileop;
    int t0;     //建立连接的超时                  30s
    int t1;     //发送或测试 APDU 的超时          15s
    int t2;     //无数据报文时确认的超时，t2<t1    10s
    int t3;     //长期空闲状态下发送测试帧的超时    20s
    int test_t4; //发送测试帧的时间间隔           25s
    clock_t t0_start;
    clock_t t0_end;
    double t0_clc;      //t0的计时器
    clock_t t1_start;
    clock_t t1_end;
    double t1_clc;      //t1的计时器
    clock_t t2_start;
    clock_t t2_end;
    double t2_clc;      //t2的计时器
    clock_t t3_start;
    clock_t t3_end;
    double t3_clc;      //t3的计时器
    clock_t test_start;
    clock_t test_end;
    double test_clc;    //test的计时器
    bool WaitRecvTest;//是否在等待接收测试帧
    //V（S）为发送状态变量，V（R）为接收状态变量，ACK为当前已经正确收到的所有I格式
    int V_S;    //发送状态变量
    int V_R;    //接收状态变量
    int ACK;    //当前已经正确收到的所有I格式

public:
    void updata_recv_edit(int author,unsigned char* data);
signals:
    //----------连接是否成功-------
    void ConnectRet(int status,QString IP , qint16 Port);
    //----------开启测试帧接收判定----
    void TestConfirm();
    void TestConfirmSuccess();
    void TestConfirmTimeOut();
private slots:

signals:
    //-----------接收框显示--------
    void append_data(int author,char type,QString data);/*@author： 0是主站，1是从站*/
    //----------召唤目录显示--------
    void show_dirname(char* name,int n);
signals:
    //---------调试消息显示-------
    void UpdataDebugEdit(QString msg);
    //---------进度条控制--------
    void ReadMax(int maxSize);  //读文件最大值信号
    void ReadValue(int Value);//Value值更新信号
    void WriteMax(int maxSize);  //写文件最大值信号
    void WriteValue(int Value);//Value值更新信号
   //---------激活标签控制---------
   void ReadLabelEnable(int status);
   void WriteLabelEnable(int status);

public:
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
    int FrameRecv(Client *&client,unsigned char (&recvBuff)[BUF_LEN]);
    
    //I格式C域的序号编号serial number
    bool IFrameCBlocksSerialNumber(char (&C)[4]);
    //I格式C域的序号判定
    bool IFrameCBlocksSerialConfirm(char (&C)[4]);
    //测试链路
    bool TestSession(Client *&client);
    bool TestSessionSuccess(Client *&client);
    //建立连接
    bool ConnectSession(Client *&client,QString IP,qint16 Port);
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
    bool ReadFileSession(Client *client,char* RFileName);
    bool ReadFileSessionSuccess(Client *client,QString DirName=QString());
    //文件写入
    Frame WriteFileSession(Client *client,char* WFileName);
    bool WriteFileSessionEnable(Client *client,Frame &frame);
    bool WriteFileSessionConfirm(Client *client,Frame &frame,int offset);
    bool WriteFileSessionSuccess(Client *client,Frame &frame,QString filename);

signals:
    void Recv(char* buf);  //接收数据信号
    
};

/*
//-----------------------------------------------*****--------------------------------------------//

//超时机制
int t1_calc; // 用于计数
int t1;    //  发送或者测试APDU的超时
// 从站端启动U格式测试过程后等待U格式测试应答的超时时间(超时处理：断开连接)
// 启动条件：发送U测试帧
// 关闭条件：接收U测试帧

int t2_calc; // 用于计数
int t2;    //  无数据报文t2<t1确认的超时
//	1)	以突发的传送原因向主站(客户)端上送了变化信息
//	2)	或以激活结束的传送原因向主站(客户)端上送了总召唤/电度召唤结束后
//	等待主站(客户)端回S格式的超时时间，若超过此时间还没有收到，就主动关闭TCP连接
//  处理完I格式帧后开始计时(置0)，接收到S帧置0
//  启动条件：发送完所有I帧
//  关闭条件：收到S/I帧

int t3_calc; // 用于计数
int t3;    //  长期闲置t3>t1状态下发送测试帧的超时
// T3:当RTU(服务器)端和主站(客户)端之间没有实际的数据交换时，任何一端启动U格式测试过程的最大间隔时间(超时处理：发送U测)
// 启动条件：建立连接
// 关闭条件：断开连接

int K;  // 发送序号和接收序号之间的最大差值   【发送方在发送K个I报文还未收到确认就应该关闭数据传送 默认值为12；从站使用】
int W;  // 【接收方最迟收到W个I就必须要回复确认帧 默认值为8；主站使用，这里不使用】
// W 不能够超过 K 的2/3

QTimer *timer1;     // 发送或者测试APDU的计时器
QTimer *timer2;     // 无数据报文确认的计时器
QTimer *timer3;     // 发送测试帧的计时器

//-----------------------------------------------*****--------------------------------------------//
*/



#endif // SESSION_H

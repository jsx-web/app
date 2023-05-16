#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string.h>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QObject>
#include <QFile>
#include <QDir>
#include <QTimer>


using namespace std;

//帧的最大容量
#define BUF_LEN 256
//缓冲区的最大容量
#define BUFFERSIZE 16

//I格式
#define I_BIT_1 0x00
#define I_BIT_3 0x00

//S格式
#define S_BIT_1 0x01
#define S_BIT_3 0x00

//U格式
#define U_BIT_1 0x03
#define U_BIT_3 0x00
#define U_BIT_1_START_ACT 0x0B    //STARTDT   确认
#define U_BIT_1_START_CON 0x07    //STARTDT   命令
#define U_BIT_1_STOP_ACT 0x23     //STOPDT    确认
#define U_BIT_1_STOP_CON 0x13     //STOPDT    命令
#define U_BIT_1_TEST_ACT 0x83     //TESTFR    确认
#define U_BIT_1_TEST_CON 0x43     //TESTFR    命令

//标识类型 TI
#define C_IC_NA_1 100   //召唤命令
#define C_CS_NA_1 103   //时钟同步
#define C_RP_NA_1 105   //复位进程命令
#define F_FR_NA_1 210   //文件传输
#define C_SR_NA_1 200   //切换定值区
#define C_RR_NA_1 201   //读定值区号
#define C_RS_NA_1 202   //读多个/全部参数
#define C_WS_NA_1 203   //写参数和定值


//TabableView对应ModelID和Len
#define TableVeiwInitID 1
#define TableVeiwInitLen 2

//默认文件存储目录
#define PATH "D:/app/data/"
//写文件的目录
#define WRITEPATH "D:/app/data/write/"
//建立连接的超时
#define CONNTIMEOUT 30
//发送或测试 APDU 的超时
#define TESTFRAMETIMEOUT 15
//无数据报文时确认的超时，NOFRAMETIMEOUT<TESTFRAMETIMEOUT
#define NOFRAMETIMEOUT 10
//长期空闲状态下发送测试帧的超时
#define FREETIMEOUT 20
//测试帧发送间隔
#define TESTTIME 25

char* Uchar_to_char(char* &Dest,unsigned char* Src,int Size);
void show_hex(char *buf , int len);
char* hex_to_char(char **dest , char *src); //报文转char
char *hex_to_char(char **dest, char *src,int len);//报文转字符
char* str_to_char(char **dest, char *src,int len);//字符转报文
char* my_strncpy(void* Dest,void* src,int size);
float hextofloat (unsigned int number);
//int tcpNonblockingRecv(int conn_sockfd, void *rx_buf, int buf_len, int timeval_sec, int timeval_usec);

#ifdef        __cplusplus//支持C,C++编译器
extern "C"
{
#endif

typedef union
{
    uint64_t Time;			// 低7字节是七个八位位组二进制时间
    struct
    {
        uint16_t msec;
        uint8_t min		: 6;
        uint8_t res1	: 1;
        uint8_t iv 		: 1;
        uint8_t hour 	: 5;
        uint8_t res2 	: 2;
        uint8_t su 		: 1;
        uint8_t mday 	: 5;
        uint8_t wday 	: 3;
        uint8_t month   : 4;
        uint8_t res3 	: 4;
        uint8_t year 	: 7;
        uint8_t res4 	: 1;
    }Compts;
}st_cp56time2a;

#ifdef        __cplusplus
}
#endif


bool Uchar_to_cp56time2a(unsigned char *date,st_cp56time2a *time);
bool cp56time2a_to_char(char (&date)[7],st_cp56time2a time);
QString get_cp56time2a_string(st_cp56time2a *time);
uint64_t get_cp56time2a_hex(st_cp56time2a *time);
void show_str(char *buf , int len);


#endif // CONFIG_H

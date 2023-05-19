#ifndef _SLAVE_H 
#define _SLAVE_H

#include <iostream>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
using std::string;
#define C_RP_NA_1 0x69
#define M_ME_NC_1 0x0D
#define F_FR_NA_1 0xD2
#define F_SUM_XX_1 23

#define jsxTEST 1
#define TEST 0
#if jsxTEST
#define FTP_PATH "/root/project/tcp/app104/app/"
#define FTP_PATH_FILE "/root/project/tcp/app104/app/iec104/"
#define FTP_PATH_RE_FILE "/root/project/tcp/app104/app/"
#endif

#if TEST
#define FTP_PATH "/mnt/d/XIAOD/XIAOH/code/"
#define FTP_PATH_FILE "/mnt/d/XIAOD/XIAOH/code/iec104/"
#define FTP_PATH_RE_FILE "/mnt/d/XIAOD/XIAOH/code/xiaoh/"
#endif
/*
    unsigned seed;  // Random generator seed
    // Use the time function to get a "seed” value for srand
    seed = time(0);
    srand(seed);
    // Now generate and print three random numbers
    cout << rand() << " " ;
*/
enum iec104_t//枚举各个格式
{
    IEC_CSTART=2, 
    IEC_CSTOP=4,
    IEC_CTESTFR=6,
};
class Iec104
{
friend int session(int connfd);//与客户端会话
friend void* k_session(void* connfd1);//持续会话
friend void Confirm_start(int connfd);
friend void Telemetry(int connfd);
public:
Iec104();
Iec104(char *ip,char *port);
int init_tcp();//ip端口绑定监听
public:
char* m_ip;
char* m_port;
static bool m_switch;//初始化是否完成
static unsigned char m_w1;//发送序号
static unsigned char m_w2;//发送序号
static unsigned char m_r1;//接收序号
static unsigned char m_r2;//接收序号
static unsigned char m_flag;
};
void change_wnumber(unsigned char *w);//序号转化
void Confirm_start(int connfd);//启动
void* k_session(void* connfd1);
void summon(int connfd);//总召唤确认
void user_data(int connfd);//发送用户数据
void summon_stop(int connfd);//总召唤结束
void clock_syn(int connfd,unsigned char *cmd);//时钟同步
void clock_reading(int connfd);//时钟读取
void Report(int connfd);//带时标遥信变位
void Telemetry(int connfd);//故障遥测上送
void reset(int connfd);//复位进程
void summon_dir(int connfd,unsigned char *rdir);//召唤目录
void read_file(int connfd,unsigned char * rfilename);//读文件过程
void write_file(int connfd,unsigned char *wfilename);//写文件过程
void *Report_data(void* connfd1);//遥测上报
void judge_number(unsigned char *data);////收发序号的判定
#endif
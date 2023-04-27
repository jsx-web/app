#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>
#include <QDebug>
#include<ctime>


//帧的最大容量
#define BUF_LEN 256

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



char* Uchar_to_char(char* &Dest,unsigned char* Src,int Size);
void show_hex(char *buf , int len);
char* hex_to_char(char **dest , char *src);
char* my_strncpy(void* Dest,void* src,int size);
float hextofloat (unsigned int number);
//int tcpNonblockingRecv(int conn_sockfd, void *rx_buf, int buf_len, int timeval_sec, int timeval_usec);


#endif // CONFIG_H

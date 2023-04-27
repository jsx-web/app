#include<iostream>
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
using namespace std;
#define FTP_PATH "/mnt/d/XIAOD/XIAOH/code/iec104/"
/*void * m_strcat(unsigned char*filenames,unsigned char*dest)
{
    unsigned char*ch=filenames;
    unsigned char*cd=dest;
    while(*ch)
    {
        ch++;
    }
    while(*cd)
    {
        *ch=*cd;
        ch++;
        cd++;
    }
    return filenames;
}
int main()
{
    unsigned char filenames[1024]={'0'};
    unsigned char i[5]={'0'};
    i[0]='c';
    i[1]='c';
    i[2]='c';
    i[3]='c';
    strcpy((char *)filenames,FTP_PATH);

    strcat((char *)filenames,(char *)i);
    //strcat((char *)filenames,FTP_PATH);
    //sprintf((char *)filenames,"%s",a);
    cout<<filenames<<endl;
    return 0;
}*/




/*int main()
{
    struct timeval tv;
    struct timezone tz;
    time_t timep;
    struct tm *p;

    time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//用localtime将秒数转化为struct tm结构体
    char *wday[] = {"7", "1", "2", "3", "4", "5", "6"};
    struct timeval curTime;
    gettimeofday(&tv, NULL);
    int milli = tv.tv_usec / 1000;
    printf("%d/%d/%d %02d:%02d:%02d:%02d %s\n", 1900 + p->tm_year, 1+ p->tm_mon, p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec,milli,wday[p->tm_wday]);//2019/2/28 14:18:31
    return 0; 
}*/
int main()
{
    static unsigned char i=0x7F;
    i=100;
    printf("%02x",i);
    putchar('\n');
    return 0; 
}
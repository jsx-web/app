#include<iostream>
#include <sys/time.h>
using namespace std;
int main()
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
}
/*int main()
{
    static unsigned char i=0x7F;
    static int k=0x00;
    static int ch=0x01;
    int flag = 0xfe;
    i*=2;
    i+=1;
    //i%=16;
    int h[100]={'0'};
    if(i==0xFF)
        k+=1;
    h[0]=i;
    h[0]+=100;
    h[1]=k;
    h[2] = 100;
    printf("%02x %02x ",i,k);
    putchar('\n');
    return 0; 
}*/
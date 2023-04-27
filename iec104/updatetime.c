#include <time.h>
#include <stdio.h>
int main()
{
    time_t timep;
    struct tm *p;

    time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//用localtime将秒数转化为struct tm结构体
    char *wday[] = {"7", "1", "2", "3", "4", "5", "6"};
    
    printf("%d/%d/%d %02d:%02d:%02d:%02d %s\n", 1900 + p->tm_year, 1+ p->tm_mon, p->tm_mday,p->tm_hour, p->tm_min, p->tm_sec,wday[p->tm_wday]);//2019/2/28 14:18:31

    return 0;
}

#include <iostream>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

#include<time.h>
#define FTP_PATH "/mnt/d/XIAOD/XIAOH/code/"
using namespace std;
int main()
{
    DIR * dirp = opendir(FTP_PATH);
    if(dirp == NULL)
    {
        cout<<"diropen fail"<<endl;//失败
    }
    struct dirent * p ;
    unsigned char filenames[1024];
    unsigned char data[1024];
    unsigned char s[255];
    printf("文件名: \n");
    while(1)
    {
        //printf("11\n");
        p = readdir(dirp);
        if(p == NULL)
            break;

        if(strcmp(p->d_name,".") == 0 || strcmp(p->d_name,"..") == 0)//排除 . 和 ..
        {
            continue;
        }
        time_t t;
        
        struct tm* z;
        struct stat statbuf;
	    // 获取文件大小
	    
        sprintf((char*)filenames,"%s",p->d_name);
        stat((const char*)filenames, &statbuf);
        int filesize = static_cast<int>(statbuf.st_size);
        int Time=(int)statbuf.st_mtime;//获取文件的创建时间
        t=Time+28800;//相加是确保时区是正确，否则与标准时间差八小时
        z=gmtime(&t);
        //strftime((char*)s,80,"%Y-%m-%d %H:%M:%S",z);//以 年月 日 时 分 秒的形式输出创建时间
        strftime((char*)s,80,"%Y%m%d%H%M%S",z);
        unsigned char year =  z->tm_year-100;
        printf("*******%02x\n",year);
        printf("*******%d\n",z->tm_mon);
        printf("*******%d\n",z->tm_mday);
        cout<<"文件名长度"<<strlen((const char*)filenames)<<endl;
        /*unsigned char filelong[5] ={'0'};
        filelong[0] = filesize%255;
        filesize/=255;
        filelong[1] = filesize%255;
        filesize/=255;
        filelong[2] = filesize%255;
        filesize/=255;
        filelong[3] = filesize%255;
        cout<<"长度：";*/
        union Data
        {
            unsigned int a;
            unsigned char filelong[4];
        }datalong;
        datalong.a=filesize;
        for(int i=0;i<4;i++)//cout<<"长度："<<filesize<<endl;
        {
            printf(" %02x ",datalong.filelong[i]);
        }
        printf("\n");
        unsigned char time[255]={'0'};
        //cout<<"时间："<<s<<endl;
        cout<<"时间长度："<<strlen((char*)s)<<endl;
        for(int i=0;i<10;i++)
        {
            //time[i]=s[i];
            printf("%02x",s[i]);
            cout<<"----";
        }
        cout<<endl;
        /*unsigned short ch=statbuf.st_mode;
        ch=ch-33150;
        cout<<"文件属性"<<ch<<endl;*/
        statbuf.st_mode-=33150;
        unsigned char ch =statbuf.st_mode;
        printf("文件属性 %02x ",ch);
        //printf("%s",filenames);
        //unsigned char data[1024] = {'0'};
        strcat((char *)data,(const char*)filenames);

        //strcat((char *)data,(const char*)statbuf.st_size);

        //strcat((char *)data,(const char*)statbuf.st_mode);
        //strcat((char *)data,(const char*)statbuf.st_mtime);

        //printf("%s",data);
        //int data1_len = 1024;
        printf("\n");
    }
    printf("%s",data);
    printf("\n");
    return 0; 
}
#include "slave.hpp"
using std::cout;
using std::endl;
bool Iec104::m_switch=false;
Iec104::Iec104()
{
    m_ip=(char *)"192.168.1.15";//"192.168.1.15"
    m_port=(char *)"2404";
}
Iec104::Iec104(char *ip,char *port)
{
    m_ip=ip;
    m_port=port;
}
int Iec104::init_tcp()
{
    // 创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        perror("socket failed:");
        return -1;
    }
    //构造服务端自己的网络地址
    struct sockaddr_in saddr;//服务端的网络地址
    memset(&saddr,0,sizeof(saddr));//把首地址为 &addr的连续sizeof(addr)个字节的数据都置为0 -》整个结构体都置为0
    saddr.sin_family = AF_INET;//IPV4协议族 
    saddr.sin_port = htons(atoi(m_port));//指定服务端自己的端口号，并进行字节序的转换
    saddr.sin_addr.s_addr = inet_addr(m_ip);//服务端自己的ip

    //绑定网络地址
    int r = bind(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(r == -1)
    {
        cout<<"bind failed:"<<endl;
        close(sockfd);
        return -1;
    }
    cout<<"bind success"<<endl;
    //监听客户端
    r = listen(sockfd,3);
    if(r == -1)
    {
        cout<<"listen failed:"<<endl;
        close(sockfd);
        return -1;
    }
    cout<<"listen success"<<endl;
    return sockfd;
}
int read_data(int connfd,unsigned char *data,int* cmd_len)
{
    unsigned char ch;
    int r = read(connfd,&ch,1);
    if(r==-1)//||ch!=0x68
    {
        cout<<("read error")<<endl;
        return -1;
    }else if(ch!=0x68)
    {
        cout<<"格式错误"<<endl;
        return -1;
    }
    read(connfd,&ch,1);
    *cmd_len=(int)ch;
    read(connfd,data,100);
    return 1;
}
void session(int connfd)
{
    //读取客户端发送过来的命令数据包
    unsigned char cmd[255]={'0'};//保存数据包
    int cmd_len = 1024;
    read_data(connfd,cmd,&cmd_len);
    cout<<"已经读到辣"<<endl;
    cout<<"帧总长： "<<cmd_len<<endl;
    if(cmd_len==4)//判断是哪种格式
    {
        if(cmd[0]&1 && cmd[0]&1<<1 && cmd[2]^1)
        {
            int i=2;
            while(!(cmd[0] & 1<<i))
            {
                i++;
            }
            //cout<<i<<endl;// 0?
            switch(i)
            {
                case IEC_CSTART:
                {
                    Confirm_start(connfd);
                    Iec104::m_switch=true;
                    cout<<"启动确认"<<endl;
                    break;
                }
                case IEC_CSTOP:
                {

                    break;
                }
                case IEC_CTESTFR:
                {

                    break;
                }
            }
        }
    }
    if(cmd_len==77 && Iec104::m_switch==true && cmd[4]==0x64)//总召
    {

        summon(connfd); 
        
    }else if(Iec104::m_switch==true&&cmd[4]==0x67)//时钟
    {

        clock_syn(connfd);

    }

    for(int i=0;i<cmd_len;i++)
    {
        printf("%02x ",cmd[i]);
    }
    cout<<endl;
}
void* k_session(void* connfd1)
{
    int connfd =*(int *)connfd1;
    while(1)
    {
        session(connfd);
    }
    close(connfd);
    exit(0);
}
void Confirm_start(int connfd)//启动确认
{
    unsigned char cstart[100] = {0x68,0x04,0x0B,0x00,0x00,0x00};
    send(connfd,cstart,cstart[1]+2,0);
    sleep(3);
    unsigned char initialize[100] ={0x68,0x08,0x0B,0x00,0x00,0x00,0x46,0x00,0x00,0x04};
    send(connfd,cstart,cstart[1]+2,0);
}
void summon(int connfd)//总召确认
{
    unsigned char sumbuf[100] = {0x68,0x4D,0x0B,0x00,0x00,0x00,0x0B,0x0B,0x0B,0x0B,0x0B};
    send(connfd,sumbuf,sumbuf[1]+2,0);
    sleep(3);
    user_data(connfd);
}
void summon_stop(int connfd)//总召结束
{
    unsigned char sumbufstop[100] = {0x68,0x4D,0x0B,0x00,0x00,0x00,0x0B,0x0B,0x0B,0x0B,0x0B};
    send(connfd,sumbufstop,sumbufstop[1]+2,0);
}
void user_data(int connfd)//传输用户数据
{
    unsigned seed; 
    seed = time(0);
    srand(seed);
    cout << rand() << " " ;
    int i=200;
    while(i--)
    {
        unsigned char udata[100] = {0x68,0x4D,0x0B,0x00,0x00,0x00};
        udata[20] = rand()%150+150;
        send(connfd,udata,udata[1]+2,0);
        sleep(1);
    }
    sleep(5);
    summon_stop(connfd);
}
void clock_syn(int connfd)
{
    time_t timep;
    struct tm *p;

    time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//用localtime将秒数转化为struct tm结构体
    char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


}
























/*
void cmd_put(int connfd)
{
    printf("cmd_put\n");
    unsigned char buf[100] = {0x68,};
    //回复客户端
    printf("put文件\n");
    send(connfd,buf,4,0);

    //正式传输数据：接收需要上传的文件
    char sed[1024]="请输入你要上传的文件名";
    write(connfd,sed,strlen(sed)+1);
    //接收需要上传的文件名
    unsigned char filenames[1024] = "";
    unsigned char filenames1[1024] = "";
    int len = 1024;
    int len1 = 1024;
    memset(filenames,0,1024);
    memset(filenames1,0,1024);
    read_ftp_data(connfd,filenames,&len);
    decode(filenames,len,filenames1,&len1);
    printf("%s\n",filenames1);
    unsigned char filenames0[50]={0};
    //打开文件
    strcpy(filenames0,FTP_PATH);
    strcat(filenames0,filenames1);
    printf("%s\n",filenames0);
    int fd = open(filenames0,O_RDWR|O_CREAT|O_TRUNC,0777);
    if(fd<0)
    {
        perror("open fail");
        return ;
    }
    //传输文件内容
    unsigned char nr[1024] = "";
    unsigned char nr1[1024] = "";
    len1=0;
    while(1)
    {
        //printf("收到了啊\n");
        len = 1024;
        len1 = 1024;
        memset(nr,0,1024);
        memset(nr1,0,1024);
        read_ftp_data(connfd,nr,&len);
        //printf("%d\n",len);
        decode(nr,len,nr1,&len1);
        //printf("%d\n",len1);
        //printf("%s\n",nr1);
        write(fd,nr1+1,len1-1);
        if(nr1[0]==1)
        {
            break;
        }
    }
    close(fd);
}*/
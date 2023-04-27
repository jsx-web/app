#include "slave.hpp"
using std::cout;
using std::endl;
bool Iec104::m_switch=false;
unsigned char Iec104::m_r1=0x00;//接收序号
unsigned char Iec104::m_r2=0x00;//接收序号
unsigned char Iec104::m_w1=0x00;//发送序号
unsigned char Iec104::m_w2=0x00;//发送序号
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
    int r = recv(connfd,data,255,0);
    for(int i=0;i<data[1]+2;i++)
    {
        printf("%02x ",data[i]);
    }
    if(r==-1)//||ch!=0x68
    {
        cout<<("read error")<<endl;
        close(connfd);
        sleep(2);
        return -1;
    }else if(data[0]!=0x68)
    {
        cout<<"格式错误"<<endl;
        return -1;
    }
    cout<<"读到的字节： "<<r<<endl;
    *cmd_len=data[1];
    cout<<"帧总长： "<<*cmd_len<<endl;
    return 1;
}
int session(int connfd)//命令判定
{
    //读取客户端发送过来的命令数据包
    unsigned char cmd[255]={'0'};//保存数据包
    int cmd_len = 255;
    int k=read_data(connfd,cmd,&cmd_len);
    if(k<0)
        return -1;
    cout<<"已经读到辣😘"<<endl;
    //cout<<"帧总长： "<<cmd_len<<endl;
    if(cmd_len==4)//判断是哪种格式
    {
        
        printf("Present Line: %d\n", __LINE__);
        if(cmd[2]&1 && cmd[2]&1<<1 && cmd[3]^1)//判断是不是启动帧
        {
            int i=2;
            while(!(cmd[2] & 1<<i))
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
    if(cmd_len==16 && cmd[6]==0x64)//总召&& Iec104::m_switch==true 
    {

        summon(connfd); 

    }else if(Iec104::m_switch==true&&cmd[6]==0x67 &&cmd[8]==0x06)//时钟同步
    {//0x68 0x14 0x00 0x00 0x00 0x00 0x67 0x01 0x06 0x00 0x01 0x00 0x00 0x00 0x00 0x94 0x07 05 0a 1c 07 04
    /*68：启动字符；14：APDU长度；02 00 0200：4个控制域8位位组；67：类型标识，时钟同步命令；01：可变结构限定词；0600：传送原因，激活；01 00：公共地址ASDU地址；00 00 00 信息体地址；
    94 07：秒和毫秒，低字节在前，0794H=1940，即1秒940毫秒；05：分，05分；0a：时，10时；1c：日，28日；07：月，7月；04：年，2004年。
    */
        clock_syn(connfd);

    }else if(Iec104::m_switch==true&&cmd[6]==0x67 &&cmd[8]==0x05)//时钟读取
    {
        clock_reading(connfd);
    }
    else if()
    {}
    /*for(int i=0;i<cmd_len+2;i++)
    {
        printf("%02x ",cmd[i]);
    }*/
    cout<<endl;
    return 1;
}
void* k_session(void* connfd1)
{
    int connfd =*(int *)connfd1;
    while(1)
    {
        int i=session(connfd);
        if(i<0)
            break;
    }
    close(connfd);
    exit(0);
}
void Confirm_start(int connfd)//启动确认
{
    unsigned char cstart[255] = {0x68,0x04,0x0B,0x00,0x00,0x00};
    send(connfd,cstart,cstart[1]+2,0);
    sleep(1);
    cout<<"启动确认"<<endl;
    unsigned char initialize[255] ={0x68,0x0e,0x00,0x00,0x00,0x00,0x46,0x01,0x04,0x00,0x00,0x00,0x00,0x00,0X00};
    //发送接收序号要改
    initialize[2]=Iec104::m_w1;
    send(connfd,initialize,initialize[1]+2,0);
    cout<<"初始化完成"<<endl;
}
void summon(int connfd)//总召确认
{
    unsigned char sumbuf[255] = {0x68,0x0E,0x00,0x00,0x00,0x00,0x64,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0X14};
    change_wnumber(sumbuf);
    sumbuf[2]=Iec104::m_w1;
    send(connfd,sumbuf,sumbuf[1]+2,0);
    sleep(1);
    cout<<"总召确认"<<endl;
    user_data(connfd);
}
void summon_stop(int connfd)//总召结束
{
    unsigned char sumbufstop[255] = {0x68,0x0E,0x00,0x00,0x00,0x00,0x64,0x01,0x0A,0x00,0x01,0x00,0x00,0x00,0x00,0X14};
    //发送接收序号要改
    change_wnumber(sumbufstop);
    send(connfd,sumbufstop,sumbufstop[1]+2,0);
    cout<<"总召确认"<<endl;
    sleep(1);
}
void user_data(int connfd)//传输用户数据 COT=20 SQ=1
{
    unsigned seed; 
    seed = time(0);
    srand(seed);
    cout << rand() << " " ;
    int i=20;//用户数据数量
    while(i--)
    {
        unsigned char udata[255] = {0x68,0x4D,0x00,0x00,0x00,0x00,0x0D,0x84,0x14,0x00,0x01,0x00,0X01,0X40,0X00,0xff,0xFF};//从ff开始传数据
        change_wnumber(udata);
        int num = udata[7]-128;
        for(int i=13;i<(13+num*5);i++)
        {
            udata[i] = rand()%155+100;
            udata[++i] = rand()%155+100;
            udata[++i] = rand()%155+100;
            udata[++i] = rand()%155+100;
            udata[++i] = 0;
        }
        udata[1] = num*5+11;
        send(connfd,udata,udata[1]+2,0);
        sleep(0.5);
    }
    sleep(1);
    summon_stop(connfd);
}
void clock_syn(int connfd)//时钟同步
{
    time_t timep;
    struct tm *p;
    struct timeval tv;
    time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//用localtime将秒数转化为struct tm结构体
    //char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    unsigned char utime[255] = {0x68,0x14,0x00,0x00,0x00,0x00,0x67,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00};//后面传时间
    change_wnumber(utime);
    gettimeofday(&tv, NULL);
    int milli = tv.tv_usec / 1000;//计算毫秒
    utime[15]=tv.tv_usec%255;//
    utime[16]=tv.tv_usec/255;
    utime[17]= p->tm_min;//分钟
    utime[18]= p->tm_hour;
    utime[19]= p->tm_mday;
    utime[20]= p->tm_mon +1;
    utime[21]= p->tm_year-100;

    send(connfd,utime,utime[1]+2,0);
}
void clock_reading(int connfd)//时钟读取
{
    time_t timep;
    struct tm *p;
    struct timeval tv;
    time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
    p = localtime(&timep);//用localtime将秒数转化为struct tm结构体
    //char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    unsigned char utime[255] = {0x68,0x14,0x00,0x00,0x00,0x00,0x67,0x01,0x05,0x00,0x01,0x00,0x00,0x00,0x00};//后面传时间
    change_wnumber(utime);
    gettimeofday(&tv, NULL);
    int milli = tv.tv_usec / 1000;//计算毫秒
    utime[15]=tv.tv_usec%255;//
    utime[16]=tv.tv_usec/255;
    utime[17]= p->tm_min;//分钟
    utime[18]= p->tm_hour;
    utime[19]= p->tm_mday;
    utime[20]= p->tm_mon +1;
    utime[21]= p->tm_year-100;

    send(connfd,utime,utime[1]+2,0);
}
void Telemetry(int connfd)//带时标的遥信变位事件
{
    if(Iec104::m_switch==true)
    {

    }
}
void reset(int connfd)//复位进程
{
    unsigned char resetbuf[100] = {0x68,0x4D,0x0B,0x00,0x00,0x00,0x0B,0x0B,0x0B,0x0B,0x0B};
    send(connfd,resetbuf,resetbuf[1]+2,0);
}
void summon_dir(int connfd)//召唤目录
{
    unsigned char dirbuf[100] = {0x68,0x4D,0x0B,0x00,0x00,0x00,0x0B,0x0B,0x0B,0x0B,0x0B};
    send(connfd,dirbuf,dirbuf[1]+2,0);
}
void read_file(int connfd)//读文件过程
{
    unsigned char filebuf[100] = {0x68,0x4D,0x0B,0x00,0x00,0x00,0x0B,0x0B,0x0B,0x0B,0x0B};
    send(connfd,filebuf,filebuf[1]+2,0);
    struct dirent * p ;
    unsigned char filenames[1024];
    while(1)
    {

    }
}
void write_file(int connfd)//写文件过程
{

}
void change_wnumber(unsigned char *w)//序号转化
{
    Iec104::m_w1=Iec104::m_w1+2;
    if(Iec104::m_w1>=0xff)
    {
        Iec104::m_w2++;
        Iec104::m_w1-=0xff;
    }
    w[2]=Iec104::m_w1;
    w[3]=Iec104::m_w2;
}
/*int read_data(int connfd,unsigned char *data,int* cmd_len)
{
    unsigned char ch='0';
    int r = read(connfd,&ch,1);
    if(r==-1)//||ch!=0x68
    {
        cout<<("read error")<<endl;
        close(connfd);
        sleep(2);
        return -1;
    }else if(ch!=0x68)
    {
        cout<<"格式错误"<<endl;
        return -1;
    }
    read(connfd,&ch,1);
    *cmd_len=(int)ch;
    cout<<"帧总长： "<<*cmd_len<<endl;
    read(connfd,data,*cmd_len);
    return 1;
}*/
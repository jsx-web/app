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
        
        printf("Present Line: %d\n", __LINE__);//打印行数
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
    else if(Iec104::m_switch==true&&cmd[6]==0x69 &&cmd[8]==0x06 && cmd[15]==0x01)//复位确认
    //{0x68,0x14,0x0B,0x00,0x00,0x00,C_RP_NA_1,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x01};
    {
        reset(connfd);
    }else if(Iec104::m_switch==true&&cmd[6]==0xD2 &&cmd[8]==0x06 && cmd[15]==0x02 && cmd[16]==0x01)//召唤目录
    {
        summon_dir(connfd,cmd);
    }else if(Iec104::m_switch==true&&cmd[6]==0xD2 &&cmd[8]==0x06 && cmd[15]==0x02 && cmd[16]==0x03)//读文件
    {
        read_file(connfd,cmd);
    }else if(Iec104::m_switch==true&&cmd[6]==0xD2 &&cmd[8]==0x06 && cmd[15]==0x02 && cmd[16]==0x07)//读文件
    {
        write_file(connfd,cmd);
    }
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
    send(connfd,initialize,initialize[1]+2,0);
    //change_wnumber(initialize);
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
    utime[19]= p->tm_mday+p->tm_wday*32;
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
    utime[19]= p->tm_mday+p->tm_wday*32;
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
void reset(int connfd)//复位进程确认
{
    unsigned char resetbuf[255] = {0x68,0x14,0x0B,0x00,0x00,0x00,C_RP_NA_1,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x01};
    change_wnumber(resetbuf);
    send(connfd,resetbuf,resetbuf[1]+2,0);
}
void Telemetry_up(int connfd)//遥测上报
{
    unsigned char Teleup[255] = {0x68,0x12,0x0B,0x00,0x00,0x00,M_ME_NC_1,0x81,0x14,0x00,0x01,0x00,0X01,0X40,0X00,0x01,0x02,0x03,0x04,0x00};
    change_wnumber(Teleup);
    send(connfd,Teleup,Teleup[1]+2,0);
}
void summon_dir(int connfd,unsigned char *rdir)//召唤目录
{                                                                                               //目录标识    
    unsigned char dirbuf[255] = {0x68,0x4D,0x0B,0x00,0x00,0x0F,F_FR_NA_1,0x00,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x02,0x02,0x00};
                            //  {0x68,0x4D,0x0B,0x00,0x00,0x0F,F_FR_NA_1,0x01,0x06,0x00,0x01,0x00,0x00,0x00,!0x02,0x02,0x02,0x00};
    dirbuf[17] = rdir[17];
    dirbuf[18] = rdir[18];
    dirbuf[19] = rdir[19];
    dirbuf[20] = rdir[20];
    //获取 FTP_PATH 目录下的所有文件
    unsigned char dirname[1024] ={'0'};
    unsigned char dir[255]={'0'};//目录名
    int k = rdir[21];
    for(int i=0;i<k;i++)
    {
        dir[i]=rdir[22+i];
    }
    strcpy((char *)dirname,FTP_PATH);//目录路径
    strcat((char *)dirname,(char *)dir);//指定路径下的目录
    cout<<"目录名"<<dirname<<endl;
    DIR * dirp = opendir((char*)dirname);//打开目录
    int sendnum=1;//计量回复数组下标
    if(dirp == NULL)//打开判定
    {
        cout<<"获取目录失败"<<endl;//失败
        dirbuf[17]=0x01;
        dirbuf[22]=0x00;
        dirbuf[1]=0x14;
        change_wnumber(dirbuf);
        send(connfd,dirbuf,dirbuf[1]+2,0);//失败无后续，直接返回
    }else
    {
        dirbuf[17]=0x00;
        dirbuf[22]=0x01;//判定读取成功
    
        struct dirent * p ;
        unsigned char filenames[1024];//文件名
        unsigned char data[1024];//文件名集合
        unsigned char s[255];
        int filenum=0;//文件数量
        
        while(1)//读取当前目录下的每一个文件
        {
            p = readdir(dirp);
            if(p == NULL)
                break;
            if(strcmp(p->d_name,".") == 0 || strcmp(p->d_name,"..") == 0)//排除 . 和 ..
            {
                continue;
            }
            /*后续格式
                名称长度
                文件名称
                文件属性
                文件大小
                文件最后一次修改时间
            */
            time_t t;//
            struct tm* z;//时间函数结构体
            struct stat statbuf;
            sprintf((char*)filenames,"%s",p->d_name);//将每一次读到的文件名写入
            stat((const char*)filenames, &statbuf);
            int filesize = static_cast<int>(statbuf.st_size);
            int Time=(int)statbuf.st_mtime;//获取文件的创建时间
            t=Time+28800;//相加是确保时区是正确，否则与标准时间差八小时
            z=gmtime(&t);
            //strftime((char*)s,80,"%Y-%m-%d %H:%M:%S",z);//以 年月 日 时 分 秒的形式输出创建时间
            dirbuf[F_SUM_XX_1+sendnum++]=strlen((const char*)filenames);//
            cout<<"文件名长度"<<strlen((const char*)filenames)<<"-----";
            int k = strlen((const char*)filenames);
            for(int i=0;i<k;i++)//文件名赋值
            {
                
                dirbuf[F_SUM_XX_1+sendnum]=filenames[i];
                cout<<"***"<<sendnum<<"****"<<endl;
                sendnum++;

            }//
            cout<<filenames<<"-----";
            statbuf.st_mode-=33150;
            unsigned char ch =statbuf.st_mode;
            dirbuf[F_SUM_XX_1+(sendnum++)] = ch;//文件属性赋值
            printf("文件属性：%02x",ch);
            union Data
            {
                unsigned int a;
                unsigned char filelong[4];
            }datalong;
            datalong.a=filesize;
            for(int i=0;i<4;i++)//cout<<"长度："<<filesize<<endl;
            {
                dirbuf[F_SUM_XX_1+sendnum]=datalong.filelong[i];
                sendnum++;
            }
            //cout<<"时间："<<s<<endl;
            dirbuf[F_SUM_XX_1+sendnum] = z->tm_min*332%256;
            sendnum++;
            dirbuf[F_SUM_XX_1+sendnum] = z->tm_sec*1000/256;//毫秒
            sendnum++;
            dirbuf[F_SUM_XX_1+sendnum] = z->tm_min;//分
            sendnum++;
            dirbuf[F_SUM_XX_1+sendnum] = z->tm_hour;//小时
            sendnum++;
            dirbuf[F_SUM_XX_1+sendnum] = z->tm_mday+z->tm_wday*32;//星期+几号
            sendnum++;
            dirbuf[F_SUM_XX_1+sendnum] = z->tm_mon+1;//月
            sendnum++;
            dirbuf[F_SUM_XX_1+sendnum] = z->tm_year-100;//年
            sendnum++;
            filenum++;
            strcat((char *)data,(const char*)filenames);//将每一个文件名加进文件目录
        }
        //
        dirbuf[23]=filenum;//文件数量
        dirbuf[1]=sendnum+20;
        change_wnumber(dirbuf);
        send(connfd,dirbuf,dirbuf[1]+2,0);
        cout<<endl<<data<<endl;
        cout<<"召唤目录成功"<<endl;
    }
    
}
void read_file(int connfd,unsigned char *rfilename)//读文件过程
{
    unsigned char filebuf[255] = {0x68,0x4D,0x00,0x00,0x00,0x00,F_FR_NA_1,0x00,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x02,0x04,0x00};
                              // {0x68,0x4D,0x0B,0x00,0x00,0x0F,F_FR_NA_1,0x00,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x02,0x02,0x00}; 59 30 30 5F 30 30 30 30 5F 32 30 31 37 30 39 32 35 5F 31 37 32 36 31 35 5F 31 34 33 2E 63
    unsigned char  rfile[255]={'0'};//文件名
    int k = rfilename[17];//文件名长度
    for(int i=0;i<k;i++)
    {
        rfile[i] = rfilename[18+i];
    }
    unsigned char filename[255];
    strcpy((char *)filename,FTP_PATH_FILE);//文件路径
    strcat((char *)filename,(char *)rfile);//指定路径下的文件
    cout<<"文件路径"<<(char *)filename<<endl;
    FILE* fd = fopen((char *)filename,"r");
    //FILE* fd = fopen(FTP_PATH_FILE,"r");
    int errNum=0;
    if(fd == NULL)
    {
        errNum = errno;
        printf("open fail errno = %d reason = %s \n", errNum, strerror(errNum));
        filebuf[17] = 0x01;
        filebuf[1] = 0x0f;
        change_wnumber(filebuf);
        cout<<"打开文件失败"<<endl;
        send(connfd,filebuf,filebuf[1]+2,0);//失败无后续
        
    }else
    {
        cout<<"打开文件成功"<<endl;
        filebuf[17] = 0x00;
        filebuf[18] = rfilename[17];
        int num =0;
        for(;num<rfilename[17];num++)
        {
            filebuf[19+num] = rfilename[18+num];
        }
        for(int i=0;i<4;i++,num++)
        {
            filebuf[19+num] = 0x00;
        }
        struct stat statbuf;//
        stat((char *)filename, &statbuf);
        int filesize = static_cast<int>(statbuf.st_size);//
        cout<<"文件内容长度："<<filesize<<endl;
        union Data
        {
            unsigned int a;
            unsigned char filelong[4];
        }datalong;
        datalong.a=filesize;
        for(int i=0;i<4;num++,i++)//cout<<"长度："<<filesize<<endl;
        {
            filebuf[19+num]=datalong.filelong[i];
        }
        change_wnumber(filebuf);
        filebuf[1]=17+num;
        send(connfd,filebuf,filebuf[1]+2,0);//成功读取，发送报文
        //文件数据传输
        unsigned char sendfile[255]={0x68,0x4D,0x00,0x00,0x00,0x00,F_FR_NA_1,0x00,0x05,0x00,0x01,0x00,0x00,0x00,0x00,0x02,0x05,0x00,0x00,0x00,0x00};//0x05后面四位是文件标识
        int data_len=0;//数据段号
        //printf("Present Line: %d\n", __LINE__);//打印行数
        //读取文件内容并传输
        int i=0;
        while(1)
        {
            memset(sendfile+17,0,238);
            int readsize = fread(sendfile+26,1,228,fd);
            
            data_len += readsize;
            if(readsize < 0)
            {
                cout<<"read fail"<<endl;
            }
            union Date
            {
                unsigned int a;
                unsigned char filelong[4];
            }datanumber;
            datanumber.a=data_len;//cout<<"数据段号："<<datanumber.a<<endl;
            for(int i=0;i<4;i++)
            {
                sendfile[21+i] = datanumber.filelong[i];
            }
            if(readsize<228)//判断是否还有后续
            {
                sendfile[25]=0x00;
            }else{
                sendfile[25]=0x01;
            }
            sendfile[26+readsize]=0x04;//校验码
            sendfile[1]=25+readsize;
            change_wnumber(sendfile);//发送序号+1
            send(connfd,sendfile,sendfile[1]+2,0);//发送文件数据报文
            if(readsize < 228 )
            {
                cout<<"已经发完最后一个数据包"<<endl;
                break;
            }
            usleep(100000);
        }
        
    }
    fclose(fd);
}
void write_file(int connfd,unsigned char *wfilename)//写文件过程
{
    unsigned char wfilebuf[255] = {0x68,0x4D,0x00,0x00,0x00,0x00,F_FR_NA_1,0x00,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x02,0x08,0x00};
    unsigned char  wfile[255]={'0'};//文件名
    int k = wfilename[17];//文件名长度
    for(int i=0;i<k;i++)
    {
        wfile[i] = wfilename[18+i];
    }
    unsigned char wfilesize[255]={'0'};//文件大小
    unsigned char filename[255]={'0'};
    strcpy((char *)filename,FTP_PATH_FILE);//文件路径
    strcat((char *)filename,(char *)wfile);//指定路径下的文件
    cout<<"文件路径"<<(char *)filename<<endl;
    FILE* fd = fopen((char *)filename,"w");
    int num=0;//记录发送数组的相对下标
    int errNum=0;
    if(fd == NULL)//打开写入文件失败
    {
        errNum = errno;
        printf("open fail errno = %d reason = %s \n", errNum, strerror(errNum));
        wfilebuf[17] = 0x01;
        wfilebuf[1] = 0x10;
        change_wnumber(wfilebuf);
        cout<<"打开文件失败"<<endl;
        send(connfd,wfilebuf,wfilebuf[1]+2,0);//失败无后续
        
    }else
    {
        cout<<"写入文件成功"<<endl;
        wfilebuf[17] = 0x00;
        wfilebuf[18] = wfilename[17];
        for(int i=0;i<wfilename[17];i++,num++)//写入文件名
        {
            wfilebuf[19+num] = wfilename[18+i];
        }
        for(int i=0;i<8;i++,num++)//写入文件ID和文件大小
        {
            wfilebuf[19+i] = wfilename[18+num];
        }
        wfilebuf[1]=18+num;
        change_wnumber(wfilebuf);
        send(connfd,wfilebuf,wfilebuf[1]+2,0);//成功发送确认
    }
    unsigned char data[255]={'0'};
    int len = 0;
    {
        
        sleep(1);
        read_data(connfd,data,&len);
        
    }
}
void change_wnumber(unsigned char *w)//序号转化
{
    Iec104::m_w1=Iec104::m_w1+2;
    if(Iec104::m_w1==0xFE)
    {
        Iec104::m_w2++;
        Iec104::m_w1-=0xFE;
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
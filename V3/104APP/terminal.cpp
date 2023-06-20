#include "terminal.h"

Terminal::Terminal()
{
    Terminalsocket = -2;
    if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
    {
        cout<<"failed to load winsock"<<endl;
    }
    else
    {
        cout<<"success to load winsock"<<endl;
    }
    Terminalsocket = socket(AF_INET,SOCK_STREAM,0);
    if (Terminalsocket == INVALID_SOCKET)
    {
        printf("Create Socket Failed! Error: %ld\n", GetLastError());
    }
    else
    {
        printf("Create Socket success!\n ");
        IP = QString("0.0.0.0");
        PORT = 0;
    }
    isconnect = false;
}

int Terminal::TerminaConnect(QString IP, quint16 Port)
{
    //服务器地址和端口
    SOCKADDR_IN addrSer;
    addrSer.sin_family = AF_INET;
    addrSer.sin_addr.s_addr = inet_addr(IP.QString::toLatin1().data());//toLatin1().data(),QString ->char*
    addrSer.sin_port = htons(Port);

    //连接服务端
    int ret = connect(Terminalsocket,(SOCKADDR*)&addrSer,sizeof(SOCKADDR));
    if(ret != 0)
    {
        //cout <<WSAGetLastError() <<endl;
        cout<<"connect server fail,Try connecting again..."<<endl;
        return -1;
    }
    else
    {
        cout << IP.QString::toLatin1().data() << endl;
        cout << Port << endl;
        isconnect = true;
        this->IP = IP;
        this->PORT = Port;
        cout<<"connect server success"<<endl;
    }
    cout << "-----------------------------------" << endl;
    return 0;
}

int Terminal::readtoBuf(char **recvBuff, int len)
{
    //消息接收
    int n = recv(Terminalsocket,*recvBuff,len,0);
    if(n>1){
        cout<<"recv:"<<*recvBuff<<endl;
        cout << "len: " << n << endl;
        show_hex(*recvBuff,n);
    }
    return n;
}

//int Terminal::readbit(char **recvBuff)
//{
//    int readlen = rereadcv(Terminalsocket,*recvBuff,2,0);
//    if(readlen < 0){cout << "readLen error" << endl;}
//    cout << "readlen: " << readlen << endl;
//    show_hex(*recvBuff,2);
//    printf("len = %02x",*recvBuff[1]);
//    int len = *(*recvBuff+1);
//    cout << "len: " << len << endl;
//    int n = recv(Terminalsocket,(*recvBuff+2),len,0);
//    cout << "n: " << n << endl;
//    if(n < 0) {cout << "readdata error" << endl;}
//    show_hex(*recvBuff,2+len);
//    return n+2;
//}

void Terminal::read()
{
    char recvBuff[BUF_LEN];
    while(1)
    {
        //消息接收
        int n = recv(Terminalsocket,recvBuff,BUF_LEN,0);
        if(n>1)
        {
            int len = recvBuff[1]+2;
            cout<<"recv:";
            //cout <<recvBuff<< endl;
            show_hex(recvBuff,len);
        }
    }
}

void Terminal::write(QString Qdata)
{
    char* data = Qdata.QString::toLatin1().data();
    //消息发送
    char sendBuff[BUF_LEN];
    strncpy(sendBuff,data,BUF_LEN);
    int ret = send(Terminalsocket,sendBuff,strlen(sendBuff) + 1,0);
    if( ret >0 )
    {
        cout << "send success " << endl << "context: ";
        qDebug() << Qdata;
    }
}

void Terminal::write(unsigned char *Udata)
{
    char* data;
    int len = Udata[1];
    show_hex((char*)Udata,len+2);
    data = (char*)malloc((len+2)*sizeof(char));
    Uchar_to_char(*&data,Udata,len+2);
    //消息发送
    char sendBuff[BUF_LEN];
    memset(sendBuff,0,BUF_LEN);
//    cout << len << endl;
    my_strncpy(sendBuff,data,BUF_LEN);
    if(len!=4&&Udata[2]!=0x43)
    {
        cout << "send buff:" << endl;
        show_hex(data,len+2);
        cout << "data:" << endl;
        show_hex(data,len+2);
    }
    int ret = send(Terminalsocket,sendBuff,len+2,0);
    if( ret >0)
    {
        if(len!=4&&Udata[2]!=0x43)
        {
            cout << "send success:" << ret << " bits" << endl;
        }
    }
}

bool Terminal::IsConnect()
{
    //cout << "Terminalsocket: " << Terminalsocket << endl;
    if(Terminalsocket > 2&&Terminalsocket < 65535*2)
    {
        isconnect = true;
        return true;
    }
    else
    {
        isconnect = false;
        return false;
    }
}

void Terminal::TerminalClose()
{
    Terminalsocket = closesocket(Terminalsocket);
    isconnect = false;
    cout << "close Terminalsocket: " << Terminalsocket << endl;
    cout << "socket close" << endl;
}

SOCKET Terminal::getSocket()
{
    return Terminalsocket;
}

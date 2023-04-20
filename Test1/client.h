#ifndef CLIENT_H
#define CLIENT_H

#include <windows.h>
#include <iostream>
#include <QString>
#include <WinSock2.h>
#include "config.h"

using namespace std;

class Client
{
private:
    WSADATA wsaData;
    SOCKET socketClient;
public:
    Client();
    ~Client();
    int clinetConnect(QString IP, quint16 Port);
    int readtoEidt(char **recvBuff,int len);
    void read();
    void write(QString Qdata);
    void write(unsigned char* Udata);
    bool socketEmpty();
    int get_socket();
    char* Uchar_to_char(char* &Dest,unsigned char* &Src,int Size);

};
#endif // CLIENT_H

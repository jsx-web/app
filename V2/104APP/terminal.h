#ifndef TERMINAL_H
#define TERMINAL_H



#include <winSock2.h>
#include <windows.h>
#include "config.h"

class Terminal
{
private:
    WSADATA wsaData;
    SOCKET Terminalsocket;
    QString IP;
    quint16 PORT;
public:
    bool isconnect;
public:
    Terminal();
    ~Terminal() = default;
    int TerminaConnect(QString IP, quint16 Port);
    int readtoBuf(char **recvBuff,int len);
    void read();
    void write(QString Qdata);
    void write(unsigned char* Udata);
    bool IsConnect();
    void TerminalClose();
    SOCKET getSocket();
};

#endif // TERMINAL_H

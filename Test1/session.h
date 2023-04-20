#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include "frame.h"
#include <QString>
#include "client.h"

using namespace std;

class Session
{
private:
    unsigned char *recBuf;
    int buf_len;
    Frame *SendFrame;
    QString FrameName;
public:
    Session();
    ~Session();
    void FrameSend(Client *&client,Frame &Frame);
    bool startFrame(Client *&client);
    bool TotalCallFrame(Client *&client);
};



#endif // SESSION_H

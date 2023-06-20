#include "session.h"

Session::Session(QObject *parent) : QObject(parent)
{
    Recv_buf = new unsigned char[256];
    memset(Recv_buf,0,256);
    Msg_buf = new unsigned char[256*10];
    memset(Msg_buf,0,256*10);
    bufLen = 0;
    isInit = false;
    isClose = false;
    SessionTerminal = new Terminal();
    t0 = new Clock(CONNTIMEOUT);        // 30s 如果在t0时间内未能成功建立连接,可能网络发生了故障,主站端应该向运行人员给出警告信息。
    t1 = new Clock(TESTFRAMETIMEOUT);   // 15s t1规定发送方发送一个I格式报文或U格式报文后,必须在t1的时间内得到接收方的认可,否则发送方认为TCP连接出现问题并应重新建立连接。
    t2 = new Clock(NOFRAMETIMEOUT);     // 10s t2规定接收方在接收到I格式报文后,若经过t2时间未再收到新的I格式报文,则必须向发送方发送S格式帧对已经接收到的I格式报文进行认可,显然t2必须小于t1。
    t3 = new Clock(FREETIMEOUT);        // 20s t3规定调度端或子站RTU端每接收一帧I帧、S帧或者U帧将重新触发计时器t3,若在t3内未接收到任何报文,将向对方发送测试链路帧
    test = new Clock(TESTTIME);
    SendFrame = nullptr;
    TestFrame = nullptr;
    SFrame = new Frame('S');
    recvInfoBuffer = new InfoBuffer();
    I_InfoBuffer = new InfoBuffer();
    U_InfoBuffer = new InfoBuffer();
    S_InfoBuffer = new InfoBuffer();
    TEST_InfoBuffer = new InfoBuffer();
    Report_InfoBuffer = new InfoBuffer();

    V_S = 0;    //发送状态变量
    V_R = 0;    //接收状态变量
    ACK = 0;    //当前已经正确收到的所有I格式
    Signals_Slot = false;
    isPerset = false;
    wfileop = nullptr;
    ValueMAX = -1;
    ValueMIN = -1;
    UpdataStop = false;
    StartLevelUp = false;
    WaitRecvTest = false;
    TestLink = true;

    K = 12;
    W = 8;              // 【接收方最迟收到W个I就必须要回复确认帧 默认值为8；主站使用】
    RecvIFrameCount = 0;// 接收I帧的计数
}

Session::~Session()
{
    SessionTerminal->TerminalClose();
}

bool Session::wait_for_time(Info &top, InfoBuffer &buff)
{
    std::unique_lock<std::mutex> lck(buff.mtx);
    while (!buff.Q.size())
    {
        if(buff.cv.wait_for(lck,std::chrono::seconds(10)) == std::cv_status::timeout)     //如果队列未空,就阻塞等待,wait函数释放锁,10s超时
        {
            //发送S帧
            SFrameSendSession();
            //再等待5s
            if(buff.cv.wait_for(lck,std::chrono::seconds(5)) == std::cv_status::timeout)     //如果队列未空,就阻塞等待,wait函数释放锁,5s超时
            {
                return false;
            }
        }
    }
    top = buff.Q.front();
    buff.Q.pop();
    return true;
}

bool Session::GetStartLevelUp()
{
    return StartLevelUp;
}

Terminal *Session::GetTerminal()
{
    return SessionTerminal;
}

Frame Session::GetSendFrame()
{
    if(SendFrame == nullptr)
    {
        cout << "SendFrame = nullptr" << endl;
    }
    return *SendFrame;
}

bool Session::GetIsPerset()
{
    return isPerset;
}

bool Session::GetWaitRecvTest()
{
    return WaitRecvTest;
}

void Session::SetTestLink(bool status)
{
    TestLink = status;
}

void Session::SetWaitRecvTest(bool status)
{
    WaitRecvTest = status;
//    cout << "WaitRecvTest: " << WaitRecvTest << endl;
}

void Session::SetIsPerset(bool PerSet)
{
    isPerset = PerSet;
}

void Session::SessionClose()
{
    SessionTerminal->TerminalClose();
    isClose = true;
}

bool Session::IsConnect()
{
    return SessionTerminal->IsConnect();
}

void Session::SetRecvBuf(char *recv)
{
    int n = recv[2]+2;
    my_strncpy(Recv_buf,recv,n);
}

bool Session::EncodeBuff(unsigned char *recvbuf, unsigned char len,
                         unsigned char *C, unsigned char T1, unsigned char VSQ,
                         unsigned char *COT, unsigned char *ASDU_addr, unsigned char *obj_addr)
{
    bool sure = true;
    if(0x68 != recvbuf[0]){cout << "0x68" << endl;sure=false;}
    if(len != recvbuf[1]){cout << "len" << endl;sure=false;}
    if(((recvbuf[2]&(unsigned char)0x01) != 0x00)&&((recvbuf[4]&(unsigned char)0x01) != 0x00))
    {
        cout << "C1" << endl;
        sure=false;
    }
    else    // 如果符合I帧的COT格式
    {
        char COT[4] = {0};
        my_strncpy(COT,C,4);
        if(!IFrameCBlocksSerialConfirm(COT))
        {
            cout << "C2" << endl;
            int Recv;
            show_hex(COT,4);
            Recv = COT[3]; Recv = (Recv<<7) + (COT[2]>>1);  //接收就是发送
            cout << "V_S=" << V_R;
            cout << " but Recv=" << Recv << endl;
            sure=false;
        }
    }
    if(len > 0x04)
    {
        if(T1 != recvbuf[6]){printf("T1=%02x ,recvbuf[6]=%02x\n",T1,recvbuf[6]);sure=false;}
        if(VSQ != recvbuf[7]){printf("VSQ=%02x ,recvbuf[7]=%02x\n",VSQ,recvbuf[7]);sure=false;}
        if(COT!=nullptr)
        {
            if(COT[0] != recvbuf[8] && COT[1] != recvbuf[9])
            {
                printf("COT[0]=%02x ,recvbuf[8]=%02x\n",COT[0],recvbuf[8]);
                printf("COT[1]=%02x ,recvbuf[8]=%02x\n",COT[1],recvbuf[9]);
                sure=false;
            }
        }
        if(ASDU_addr!=nullptr)
        {
            if(ASDU_addr[0] != recvbuf[10] && ASDU_addr[1] != recvbuf[11])
            {
                cout << "ASDU_addr" << endl;
                sure=false;
            }
        }
        if(obj_addr!=nullptr)
        {
            if(obj_addr[0] != recvbuf[12] && obj_addr[1] != recvbuf[13]&& obj_addr[1] != recvbuf[14])
            {
                cout << "obj_addr" << endl;
                sure=false;
            }
        }
    }
    if(sure){cout << "judge success" << endl;}
    else{cout << "judge error" << endl;}
    return sure;
}

void Session::FrameSend(Frame frame)
{

    if(frame.Getlen() != 6 && (*(frame.GetAPDU()+2)) != 0x43)
    {
        cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    }

    if(!SessionTerminal->IsConnect())
    {
        cout << "socket create fail,please create connect " << endl;
        emit ConnectRet(-4,0,0);
    }
    else
    {
//        show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);

        SessionTerminal->write(frame.GetAPDU());
    }
    if(frame.Getlen() != 6 && (*(frame.GetAPDU()+2)) != 0x43)
    {
        cout << "----------------------------" << endl;
    }

}

int Session::RecvBuf()
{
    memset(Recv_buf,0,BUF_LEN);
    int n=-3;
    if(SessionTerminal->IsConnect())
    {
        int recv_len = recv(SessionTerminal->getSocket(),(char*)Recv_buf,BUF_LEN,0);
        if(recv_len > 0)
        {
            //将接收缓冲区的数据拷贝到消息缓冲区
            memcpy(Msg_buf+msgBufLen, Recv_buf, recv_len);
            //消息缓冲区的数据末尾后移
            msgBufLen += recv_len;
            //判断消息缓冲区的数据长度是否大于等于包头长度
            while(msgBufLen > 2)
            {
                //存放帧数据
                unsigned char *Buff = new unsigned char[BUF_LEN];
                my_strncpy(Buff,Msg_buf,2);
                //判断消息缓冲区内数据长度是否大于等于报文长度 避免少包问题
                if(msgBufLen >= Buff[1])
                {
                    //消息缓冲区内剩余未处理数据的长度
                    int len = Buff[1];
                    int size = msgBufLen - (len+2);
                    my_strncpy(Buff+2,Msg_buf+2,len);
                    show_hex((char*)Buff,len+2);
                    //将帧数据处理放入对应的队列
                    n = FrameData(Buff);

                    //将消息缓冲区剩余未处理的数据前移
                    memcpy(Msg_buf, Msg_buf + (2+len), size);
                    //更新Msg_Buf的长度
                    msgBufLen = size;
                }
                else
                {
                    //消息缓冲区数据不足
                    break;
                }
            }
        }

    }
    else
    {
        cout << "socket create fail,please create connect " << endl;
        n = -4;
    }
    return n;
}

int Session::FrameData(unsigned char *buff)
{
    int n = -3;
    if(buff[0] == 0x68)
    {
        n = buff[1]+2;

        if(n == 6)  //如果是U帧和S帧
        {
            if((buff[2] == U_BIT_1_TEST_ACT)) //TEST帧
            {
//               cout << "testFrame recv" << endl;
                TEST_InfoBuffer->push(buff,n);
            }
            else if((buff[2] == U_BIT_1_STOP_ACT)||(buff[2] == U_BIT_1_START_ACT)) //U帧
            {
                U_InfoBuffer->push(buff,n);
            }
            else if((buff[2] == 0x01)&&((buff[4]&0x01) == 0x00))   //S帧
            {
                S_InfoBuffer->push(buff,n);
            }
        }
        else
        {
            if(((buff[2]&0x01) == 0x00) && ((buff[4]&0x01) == 0x00)) //如果是I帧
            {
                cout << "-----------"<< __FUNCTION__ << "-----------------" << endl;
                if((buff[8] == 0x03) && (buff[9] == 0x00))//上报帧
                {
                    cout << "Report recv" << endl;
                    Report_InfoBuffer->push(buff,n);
                    V_R++;
                }
                else
                {
                    //处理
                    cout << "I_InfoBuffer recv " << n << " bits" << endl;
                    cout << "ACK: " << ACK << "V_R: " << V_R << "V_S: " << V_S << endl;
                    if(I_InfoBuffer->push(buff,n)){show_hex((char*)buff,n);}
//                        V_R++;
                }
                cout << "---------------------------------" << endl;
            }
        }
    }
    return n;
}

int Session::FrameRecv(unsigned char (&recvBuff)[BUF_LEN])
{
    int n=-3;
    n = RecvBuf();
    return n;
}

Info Session::I_TakeInfo(int &n)
{
    Info Recv;
    n = -3;
//    I_InfoBuffer->wait(Recv);
//    cout << "wait I frame" << endl;
    if(!(wait_for_time(Recv,*I_InfoBuffer)))
    {
        cout << "time out" << endl;
        n = -4;
        return Recv;
    }

        cout << "I_ACK: " << ACK << " I_V_R: " << GetRecvSerial(Recv.getInfo()) << " I_V_S: " << V_S << endl;

        if(V_R == GetRecvSerial(Recv.getInfo())){n = Recv.getLen();    V_R++;}    //序号正常
        else if (V_R > GetRecvSerial(Recv.getInfo()))
        {
            cout << "Repeat sending" << endl;
            n = -3;
            show_hex((char*)Recv.getInfo(),Recv.getLen());
            I_InfoBuffer->clearBuffer();
            SFrameSession();
        }
        else
        {
            cout << "message is missing" << endl;
            n = -2;
            show_hex((char*)Recv.getInfo(),Recv.getLen());
            I_InfoBuffer->clearBuffer();
            SFrameSession();
        }
//    n = Recv.getLen();
    return Recv;
}

bool Session::IFrameCBlocksSerialNumber(char (&C)[4])
{
    //发送序号&&接收序号
    if(V_S<0x7fff&&V_R<0x7fff)
    {
        if(V_S > 0xfe)   //0xFF
        {
            C[0] = (V_S&0x7f)<<1;
            C[1] = (V_S>>7)&0xff;
        }else
        {
            C[0] = (V_S&0x7f)<<1;
            C[1] = 0;
        }
        V_S++;
        if(V_R > 0xfe)   //0xFF
        {
            C[2] = (V_R&0x7f)<<1;
            C[3] = (V_R>>7)&0xff;
        }else
        {
            C[2] = (V_R&0x7f)<<1;
            C[3] = 0;
        }
        return true;
    }
    else
    {
        cout << "V_S or V_R is out of rang" << endl;
        return false;
    }
}

bool Session::IFrameCBlocksSerialConfirm(char (&C)[4])
{
    //为了保证通信的兼容性，接收方在接收数据时只检查接收序列号，发送序列号不予检查。
    int RcvNum = (C[3]<<7)+(C[2]>>1);
    if(RcvNum == V_S)
    {
        cout << "SerialConfirm is correct" << endl;
        return true;
    }
    else
    {
        cout << "SerialConfirm is wrongs" << endl;
        return false;
    }
}

void Session::slotUpdataStop()
{
    UpdataStop = true;
}

bool Session::SFrameSendSession()
{
    SFrame->setSFrame(ACK);
    FrameSend(*SFrame);
    emit updata_recv_edit(2,SFrame->GetAPDU());
    return true;
}

bool Session::SFrameRecvSession()
{
    t2->Start();
    Info Recv;
    while(!(t2->IsTimeOut()))
    {
        if(S_InfoBuffer->poll(Recv))
        {
            unsigned char recvbuff[6];
            memset(recvbuff,0,6);
            my_strncpy(recvbuff,Recv.getInfo(),6);
            if((recvbuff[2]==0x01)&&((recvbuff[3]&0x01)==0x00))
            {
                int m = 0;
                m = recvbuff[3];
                m = (m<<7) + ((recvbuff[1])>>1);
                cout << "m: " << m << endl;
                if(m == V_R)
                {
                    emit UpdataDebugEdit(QString("V_R is right,V_R = %1,recv = %2").arg(QString::number(V_R)).arg(QString::number(m)));
                    t2->Flash();
                    return true;
                }
                else
                {
                    emit UpdataDebugEdit(QString("V_R is error,V_R = %1,recv = %2").arg(QString::number(V_R)).arg(QString::number(m)));
                    t2->Stop();
                    return false;
                }
            }
        }
    }
    t2->Stop();
    emit UpdataDebugEdit(QString("recv V_R,time out!!!"));
    return false;
}

bool Session::SFrameSession()
{
//    cout << __FUNCTION__ << endl;
//    cout << "ACK" << ACK << endl;
    if(ACK%W==0&&ACK>=W)
    {
        SFrameSendSession();
//        return SFrameRecvSession();
        return true;
    }
    return false;
}

void Session::RecvData()
{
    unsigned char recvBuff[BUF_LEN];
    while(this->IsConnect())
    {
        if(FrameRecv(recvBuff) == -4)
        {
            break;
        }
    }
    cout << "RecvData is Over" << endl;
}

void Session::updata_recv_edit(int author, unsigned char *data)
{
    char *recv_ch;
    QString recv_str;
    char type;
    QString Type;
    QString PS;
    if(data[0] == 0x68)
    {

        int n = data[1] + 2;
        recv_ch = new char[n];

        cout << "recvEidt_updata "<< n << " bit" <<":" ;//<<  bufrecv << endl;
        //show_hex(data,n);
        SetRecvBuf((char*)data);
        hex_to_char(&recv_ch,(char*)data);
        cout << recv_ch << endl;
        recv_str = QString(recv_ch);
        //qDebug() << "recv_str: " << recv_str;
        if(n > 6)
        {
            char c1 = data[2]&0x01;
            char c2 = data[4]&0x01;
            if((c1 == 0x00)&&(c2 == 0x00))
            {
                type = 'I';
            }
            else
            {
                type = '-';
            }
        }else if(n == 6)
        {
            char c1 = data[2]&0x03;
            char c2 = data[4]&0x01;
            if((c1 == 0x01)&&(c2 == 0x00))
            {
                type = 'S';
            }
            else if((c1 == 0x03)&&(c2 == 0x00))
            {
                type = 'U';
                if(data[2] == 0x83)
                {
                    PS = QString("(测试·从)");
                }else if(data[2] == 0x43)
                {
                    PS = QString("(测试·主)");
                }
            }else
            {
                type = '-';
            }
        }else
        {
            type = '-';
        }
    }else
    {
         recv_str = QString((char*)data);
    }
    //将数据更新到TableView中
    Type = QString(type)+PS;
    emit AppendData(author,Type,recv_str);
}

bool Session::TestSession()     //发送测试帧
{
//    cout << __FUNCTION__ << endl;
    test->Start();  //测试帧发送间隔
    while(TestLink&this->IsConnect())
    {
        if(!WaitRecvTest)   //没有等待测试帧,该发送测试帧了
        {
            if(test->IsTimeOut())   //计时,到了发送测试帧时间
            {
                //发送测试帧
//                cout << "Send TestFrame" << endl;
//                cout << "-----------"<< __FUNCTION__  <<"-----------------" << endl;
                TestFrame = new Frame('U');
//                cout << "is_setFrame: " << endl;
                TestFrame->setTestFrame();
                //updata_recv_edit(2,TestFrame->GetAPDU());
                emit AppendTest(1,QString("测试·主"),QString("68 04 43 00 00 00"));
                this->FrameSend(*TestFrame);
                WaitRecvTest = true;
                emit TestConfirm();         //通知主界面开启一个线程去判断在t1内是否收到测试帧
                test->Flash();              //刷新时间计时
//                cout << "---------------------------------" << endl;
            }
        }
    }
    return true;
}

bool Session::TestSessionSuccess()  //判定是否收到测试帧超时
{
    unsigned char recvbuff[BUF_LEN];
    memset(recvbuff,0,BUF_LEN);
    Info RecvTest;
    t1->Start();        //开启测试帧接收判定
    while(WaitRecvTest&&this->IsConnect()) //判断是否等待测试回复帧
    {
        //判断是否超时
//        cout << "Recv" << endl;
        if(t1->IsTimeOut())     //超时
        {
//            cout << "-----------"<< __FUNCTION__  <<"-----------------" << endl;
//            cout << "recv testFrame timeout" << endl;
            t1->Stop();
            emit TestConfirmTimeOut();
//            cout << "------------------------------------------------" << endl;
            return false;
        }
        else                    //未超时,尝试去看接收队列，是否有东西
        {
            if(TEST_InfoBuffer->poll(RecvTest))
            {
//                cout << "recv testFrame" << endl;
                my_strncpy(recvbuff,RecvTest.getInfo(),RecvTest.getLen());
//                show_hex((char*)recvbuff,recvbuff[1]+2);
//                cout << "1" << endl;
                t1->Stop();
//                cout << "2" << endl;
                emit TestConfirmSuccess();
//                cout << "return true" << endl;

                return true;
            }
        }
    }
    return true;
}

bool Session::ConnectSession(QString IP, qint16 Port)
{
    cout << "connect ==== session " << endl;
    int ret = -1;
    t0->Start();
    while(!isClose)
    {
        if(t0->IsTimeOut()) //如果超时
        {
            cout << "connect timeout " << endl;
            emit ConnectRet(0,IP,Port);
            return false;
        }
        else    //未超时
        {
            ret = SessionTerminal->TerminaConnect(IP,Port);
            if(ret != 0)
            {
                //连接中
                emit ConnectRet(2,IP,Port);
            }
            else
            {
                //连接成功
                emit ConnectRet(1,IP,Port);
                return true;
            }
        }
    }
    return false;
}

bool Session::InitSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    SendFrame = new Frame('U');
    cout << "is_setFrame: " << SendFrame->SetinitFrame() << endl;
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    //this->FrameRecv(client_session,*RecvFrame);
    cout << "---------------------------------" << endl;
    return true;
}

bool Session::InitSessionSuccess()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    bool init1 = false;
    bool init2 = false;
    int count = 0;
    QString initType;
    while(sure)
    {
        //消息接收
//        int n = FrameRecv(recvBuff);
        Info Recv;
        int n = -3;
        if(count == 0 && !init1)
        {
            if(!(wait_for_time(Recv,*U_InfoBuffer)))
//            if(!(U_InfoBuffer->wait_time(Recv)))
            {
                cout << "time out" << endl;
                break;
            }
            n = Recv.getLen();
        }
        else
        {
            Recv = I_TakeInfo(n);
        }

        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            //int len = recvBuff[1]+2;
//            cout<<"recv:";
            //show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
            if(count == 0 && !init1)
            {
                // 68 04 0b 00 00 00
                unsigned char C[4] = {0x0b,0x00,0x00,0x00};
                sure = EncodeBuff(recvBuff,4,C);
                init1 = true;
                if(sure&&init1)
                {
                    count ++;
                    cout << "init first package, count = "<< count << ";" << endl;
                }
            }
            else if(count == 1&& !init2)
            {
                //0x68,0x0e,0x00,0x00,0x00,0x00,0x46,0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x02
                unsigned char C[4] = {0x00,0x00,0x00,0x00};
                unsigned char COT[2] = {0x04,0x00};//传送原因 COT = 4
                unsigned char ASDU_addr[2] = {0x00,0x00};
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                //标识类型 TI = 0x46 可变帧长限定词 VSQ  = 0x01
                sure = EncodeBuff(recvBuff,0x0e,C,0x46,0x01,COT,ASDU_addr,obj_addr);
                if((recvBuff[15] != 0x00 ||recvBuff[15] != 0x01||recvBuff[15] != 0x02))
                {
                    if(recvBuff[15] != 0x00)initType = QString("当地电源合上");
                    else if(recvBuff[15] != 0x01)initType = QString("当地手动复位");
                    else if(recvBuff[15] != 0x02)initType = QString("远方复位");
                }
                else
                {
                    sure=false;
                }
                init2 = true;
                if(sure&&init1)
                {
                    count ++;
                    ACK++;
                    cout << "init second package, count = "<< count << ";" << endl;
                    SFrameSession();
                }

            }
            if(count == 2)
            {
                cout << "Init success!!!!" << endl;
                emit UpdataDebugEdit(QString("Init success!!!!"));
                QStringList data;
                data << "Init success" << initType;
                emit UpdataTableView(TableVeiwInitID,TableVeiwInitLen,data);
                isInit = true;
                return isInit;
            }

        }
//        else
//        {
//            Sleep(3000);
//            cout << "wait package ing....." << endl;
//        }
    }
    isInit = false;
    cout << "Init fail!!!!" << endl;
    emit UpdataDebugEdit(QString("Init fail!!!!"));
    return isInit;
}

bool Session::TotalCallSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetTotalCall() << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    V_S++;
    cout << "---------------------------------" << endl;
    return true;
}

bool Session::TotalCallSessionSuccess()
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    int count = -1;
    bool Confirm = false;
    while(sure)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);
        Info Recv;
        int n = -3;

//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        int datalen = 0;

        if(n < 0) {break;}
        
        if(n>1)
        {
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            //0x68,0x0E,0x00,0x00,0x00,0x00,0x64,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x14  ->  激活确认
            updata_recv_edit(1,recvBuff);
            if(count < 0)
            {
                unsigned char C[4] = {0x00,0x00,0x00,0x00};
                my_strncpy(C,recvBuff+2,4);
                unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
                unsigned char ASDU_addr[2] = {0x01,0x00};
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
                unsigned char len = recvBuff[1];
                sure = EncodeBuff(recvBuff,len,C,0x64,0x01,COT,ASDU_addr,obj_addr);
                if(recvBuff[len+1] != 0x14){sure = false;cout << "QOI is error" << endl;return sure;}
                Confirm = true;
                if(sure && Confirm)
                {
                    count ++;
                    cout << "init zero package, count = "<< count << ";" << endl;
                    cout << "Init TC success!!!!" << endl;
                    emit UpdataDebugEdit(QString("Init TC success!!!!"));
                    ACK++;
                    SFrameSession();
                }
            }
            else if(count >= 0)
            {
                // 68 12 00 00 00 00 0D 81 14 00 01 00 01 40 00 3F 7B 29 A7 00
                //0x68,0x4D,0x00,0x00,0x00,0x00,0x0D,0x84,0x14,0x00,0x01,0x00,0x01,0x40,0x00,0xff,0xFF
                unsigned char C[4] = {0x00,0x00,0x00,0x00};
                my_strncpy(C,recvBuff+2,4);
                unsigned char T1 = recvBuff[6];
                if(T1 != 0x64)  // 如果不是召唤结束帧
                {
                    if(T1 == 0x01)          //<1>∶＝单点信息
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 1;
                    }
                    else if(T1 == 0x03)     //<3>∶＝双点信息
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 1;

                    }else if(T1 == 0x09)    //<9>∶＝测量值，归一化值
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 3;
                    }
                    else if(T1 == 0x0B)     //<11>∶＝测量值，标度化值
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 3;
                    }
                    else if(T1 == 0x0D)     //<13>∶＝测量值，短浮点数
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 5;
                    }
                    else if(T1 == 0x1E)     //<30>∶＝带 CP56Time2a 时标的单点信息
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 8;
                    }
                    else if(T1 == 0x1F)     //<31>∶＝带 CP56Time2a 时标的双点信息
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 8;
                    }
                    else
                    {
                        emit UpdataDebugEdit(QString("package data of T1 error!!!!"));
                        return false;
                    }
                    unsigned char VSQ = recvBuff[7];
                    unsigned char COT[2] = {0x14,0x00};//传送原因 COT = 20
                    unsigned char ASDU_addr[2] = {0x01,0x00};
                    union ADDR
                    {
                        unsigned char arr[3];
                        int addr;
                    }obj_addr;
                    my_strncpy(obj_addr.arr,recvBuff+12,3);
                    //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
                    unsigned char len = recvBuff[1];
                    printf("len = %02x,recvBuff[1] =%02x\n",len,recvBuff[1]);
                    sure = EncodeBuff(recvBuff,len,C,T1,VSQ,COT,ASDU_addr,obj_addr.arr);
                    if(sure==true)
                    {
                        unsigned char data[5];
                        printf("VSQ: %02x,SQ: %02x,CON: %d\n",VSQ,VSQ>>7,VSQ&(0x7f));
                        if(((VSQ&0x80)>>7) == 1)
                        {
                            for(int i = 0;i<(VSQ&(0x7f));i++)
                            {
                                my_strncpy(data,recvBuff+15+i*datalen,datalen);
                                TotalCallData(count,T1,obj_addr.arr,data);
                                obj_addr.addr++;
                            }
                        }
                        else
                        {
                            for(int i = 0;i<(VSQ&(0x7f));i++)
                            {
                                my_strncpy(obj_addr.arr,recvBuff+12+i*(datalen+3),3);
                                my_strncpy(data,recvBuff+15+i*(datalen+3)+3,datalen);
                                TotalCallData(count,T1,obj_addr.arr,data);
                            }
                        }
                        count ++;
                        cout << "totalcall recv "<< count-1 <<" package, count = "<< count << ";" << endl;
                        ACK++;
                        SFrameSession();
                    }
                }
                else  // 如果是召唤结束帧
                {
                     unsigned char C[4] = {0x00,0x00,0x00,0x00};
                     my_strncpy(C,recvBuff+2,4);
                     unsigned char COT[2] = {0x0a,0x00};//传送原因 COT = 10
                     unsigned char ASDU_addr[2] = {0x01,0x00};
                     unsigned char obj_addr[3] = {0x00,0x00,0x00};
                     //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
                     unsigned char len = recvBuff[1];
                     sure = EncodeBuff(recvBuff,len,C,0x64,0x01,COT,ASDU_addr,obj_addr);
                     if(recvBuff[len+1] != 0x14){sure = false;cout << "QOI is error" << endl;return sure;}
                     if(sure)
                     {
                         cout << "Totalcall End" << endl;
                         emit UpdataDebugEdit(QString("TotalCall End!!!!"));
                         ACK++;
                         SFrameSession();
                         cout << "************************************" << endl;
                         return sure;
                     }
                }
            }
        }
//        cout << endl;
    }
    cout << "recv error!!!"<< "package error: " <<count << endl;
    emit UpdataDebugEdit(QString("TotalCall Recv Error!!!!!!!"));
    cout << "************************************" << endl;
    return true;
}

bool Session::TotalCallData(int packid,unsigned char T1, unsigned char *obj_addr, unsigned char *data)
{
    QString Type;
    QString Addr;
    QString Data;
    QString QDS;
    QString Time;

    if(T1 == 0x01)          //<1>∶＝单点信息
    {
        show_hex((char*)obj_addr,3);
        show_hex((char*)data,1);
        char *addr;
        hex_to_char(&addr,(char*)obj_addr,2);
        char tmp;
        tmp = addr[0];addr[0] = addr[2];addr[2] = tmp;
        tmp = addr[1];addr[1] = addr[3];addr[3] = tmp;

        Type = QString("单点信息");
        Addr = QString(addr);
        char *data1;
        hex_to_char(&data1,(char*)data,1);
        Data = QString(data1);
        QDS = QString("单点遥信信息%5(0开1合),"
                        "有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封)").arg(QString::number((unsigned char)(data[0]>>7)&0x1)
                                                        ).arg(QString::number((unsigned char)(data[0]>>6)&0x1)
                                                              ).arg(QString::number((unsigned char)(data[0]>>5)&0x1)
                                                                    ).arg(QString::number((unsigned char)(data[0]>>4)&0x1)
                                                                          ).arg(QString::number((unsigned char)data[0]&0x1));
    }
    else if(T1 == 0x03)     //<3>∶＝双点信息
    {
        show_hex((char*)obj_addr,3);
        show_hex((char*)data,1);
        char *addr;
        hex_to_char(&addr,(char*)obj_addr,2);
        char tmp;
        tmp = addr[0];addr[0] = addr[2];addr[2] = tmp;
        tmp = addr[1];addr[1] = addr[3];addr[3] = tmp;

        Type = QString("双点信息");
        Addr = QString(addr);
        char *data1;
        hex_to_char(&data1,(char*)data,1);
        Data = QString(data1);
        QDS = QString("双点遥信信息%5(0不确定1开2合3不确定),"
                        "有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封)").arg(QString::number((unsigned char)(data[0]>>7)&0x1)
                                                        ).arg(QString::number((unsigned char)(data[0]>>6)&0x1)
                                                              ).arg(QString::number((unsigned char)(data[0]>>5)&0x1)
                                                                    ).arg(QString::number((unsigned char)(data[0]>>4)&0x1)
                                                                          ).arg(QString::number((unsigned char)data[0]&0x2));

    }
    else if(T1 == 0x09)    //<9>∶＝测量值，归一化值
    {
        //68 10 00 00 00 00 09 81 14 00 01 00 01 40 00 00 01 00
        show_hex((char*)obj_addr,3);
        show_hex((char*)data,1);
        char *addr;
        hex_to_char(&addr,(char*)obj_addr,2);
        char tmp;
        tmp = addr[0];addr[0] = addr[2];addr[2] = tmp;
        tmp = addr[1];addr[1] = addr[3];addr[3] = tmp;

        Type = QString("归一化遥测值");
        Addr = QString(addr);
        int value = 0;
        value += data[1];
        value = (value<<8)+data[0];
        char *data1;
        hex_to_char(&data1,(char*)data,2);
        Data = QString(data1);
        Data = QString("%1(%2)").arg(QString::number(value)).arg(data1);
        QDS = QString("有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封),"
                              "溢出%5(0未1溢)").arg(QString::number((unsigned char)(data[2]>>7)&0x1)
                                                ).arg(QString::number((unsigned char)(data[2]>>6)&0x1)
                                                      ).arg(QString::number((unsigned char)(data[2]>>5)&0x1)
                                                            ).arg(QString::number((unsigned char)(data[2]>>4)&0x1)
                                                                  ).arg(QString::number((unsigned char)data[2]&0x1));
    }
    else if(T1 == 0x0B)     //<11>∶＝测量值，标度化值
    {
        show_hex((char*)obj_addr,3);
        show_hex((char*)data,1);
        char *addr;
        hex_to_char(&addr,(char*)obj_addr,2);
        char tmp;
        tmp = addr[0];addr[0] = addr[2];addr[2] = tmp;
        tmp = addr[1];addr[1] = addr[3];addr[3] = tmp;

        Type = QString("标度化遥测值");
        Addr = QString(addr);
        int value = 0;
        value += data[1];
        value = (value<<8)+data[0];
        char *data1;
        hex_to_char(&data1,(char*)data,2);
        Data = QString(data1);
        Data = QString("%1(%2)").arg(QString::number(value)).arg(data1);
        QDS = QString("有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封),"
                              "溢出%5(0未1溢)").arg(QString::number((unsigned char)(data[2]>>7)&0x1)
                                                ).arg(QString::number((unsigned char)(data[2]>>6)&0x1)
                                                      ).arg(QString::number((unsigned char)(data[2]>>5)&0x1)
                                                            ).arg(QString::number((unsigned char)(data[2]>>4)&0x1)
                                                                  ).arg(QString::number((unsigned char)data[2]&0x1));
    }
    else if(T1 == 0x0D)     //<13>∶＝测量值，短浮点数
    {
//        show_hex((char*)obj_addr,3);
//        show_hex((char*)data,4);
//        show_hex((char*)data+4,1);
        char *addr;
        hex_to_char(&addr,(char*)obj_addr,2);
        char tmp;
        tmp = addr[0];addr[0] = addr[2];addr[2] = tmp;
        tmp = addr[1];addr[1] = addr[3];addr[3] = tmp;

        char *data1;
        //printf("%02x %02x %02x %02x",data[0],data[1],data[2],data[3]);
        union FLOAT
        {
            float a;
            unsigned char b[4];
        }f;
        my_strncpy(f.b,data,4);
        printf("value = %f \n" , f.a);
        hex_to_char(&data1,(char*)data,4);
        Type = QString("测量值,短浮点数");

        Addr = QString(addr);
        Data = QString("%1(%2)").arg(QString::number(f.a)).arg(data1);
        QDS = QString("有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封),"
                              "溢出%5(0未1溢)").arg(QString::number((unsigned char)(data[4]>>7)&0x1)
                                                ).arg(QString::number((unsigned char)(data[4]>>6)&0x1)
                                                      ).arg(QString::number((unsigned char)(data[4]>>5)&0x1)
                                                            ).arg(QString::number((unsigned char)(data[4]>>4)&0x1)
                                                                  ).arg(QString::number((unsigned char)data[4]&0x1));


    }
    else if(T1 == 0x1E)     //<30>∶＝带 CP56Time2a 时标的单点信息
    {
        //68 15 00 00 00 00 1e 81 14 00 01 00 01 40 00 00 02 01 02 03 04 05 17
        show_hex((char*)obj_addr,3);
        show_hex((char*)data,1);
        char *addr;
        hex_to_char(&addr,(char*)obj_addr,2);
        char tmp;
        tmp = addr[0];addr[0] = addr[2];addr[2] = tmp;
        tmp = addr[1];addr[1] = addr[3];addr[3] = tmp;

        Type = QString("单点信息");
        Addr = QString(addr);
        char *data1;
        hex_to_char(&data1,(char*)data,1);
        Data = QString(data1);
        QDS = QString("单点遥信信息%5(0开1合),"
                        "有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封)").arg(QString::number((unsigned char)(data[0]>>7)&0x1)
                                                        ).arg(QString::number((unsigned char)(data[0]>>6)&0x1)
                                                              ).arg(QString::number((unsigned char)(data[0]>>5)&0x1)
                                                                    ).arg(QString::number((unsigned char)(data[0]>>4)&0x1)
                                                                          ).arg(QString::number((unsigned char)data[0]&0x1));



        //时标解析
        unsigned char time[7] = {0};
        my_strncpy(time,data+1,7);
        st_cp56time2a datatime;
        Uchar_to_cp56time2a(time,&datatime);
        Time = get_cp56time2a_string(&datatime);

    }
    else if(T1 == 0x1F)     //<31>∶＝带 CP56Time2a 时标的双点信息
    {
        show_hex((char*)obj_addr,3);
        show_hex((char*)data,1);
        char *addr;
        hex_to_char(&addr,(char*)obj_addr,2);
        char tmp;
        tmp = addr[0];addr[0] = addr[2];addr[2] = tmp;
        tmp = addr[1];addr[1] = addr[3];addr[3] = tmp;

        Type = QString("双点信息");
        Addr = QString(addr);
        char *data1;
        hex_to_char(&data1,(char*)data,1);
        Data = QString(data1);
        QDS = QString("双点遥信信息%5(0不确定1开2合3不确定),"
                        "有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封)").arg(QString::number((unsigned char)(data[0]>>7)&0x1)
                                                        ).arg(QString::number((unsigned char)(data[0]>>6)&0x1)
                                                              ).arg(QString::number((unsigned char)(data[0]>>5)&0x1)
                                                                    ).arg(QString::number((unsigned char)(data[0]>>4)&0x1)
                                                                          ).arg(QString::number((unsigned char)data[0]&0x2));

        //时标解析
        unsigned char time[7] = {0};
        my_strncpy(time,data+1,7);
        st_cp56time2a datatime;
        Uchar_to_cp56time2a(time,&datatime);
        Time = get_cp56time2a_string(&datatime);

    }
    emit ShowDataTotalCall(packid,Type,Addr,Data,QDS,Time);
    return true;
}

Frame Session::ClockSynSession(unsigned char *CP56Time2aNOW)
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetClockSyn(CP56Time2aNOW) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    unsigned char sendTime[7] = {0};
    for(int i=0;i<7;i++)
    {
        sendTime[i] = (unsigned char)(*(SendFrame->GetAPDU()+15+i));
    }
    //show_hex((char*)sendTime,7);
    st_cp56time2a STime;
    Uchar_to_cp56time2a(sendTime,&STime);
    QString Time = get_cp56time2a_string(&STime);
    emit ColckControl(1,'S',Time);
    V_S++;
    cout << "---------------------------------" << endl;
    return *SendFrame;
}

bool Session::ClockSynSessionSuccess(unsigned char *CP56Time2aNOW)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    bool Confirm = false;
    while(sure)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        
        if(n>1)
        {
            //68 14 00 00 00 00 67 01 07 00 01 00 00 00 00 ---> 0~14
            //0x01 0x02 0x03 0x04 0x81 0x09 0x10    --->15~21
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
//            cout << "Frame: ";
//            show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);
//            cout << endl;
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char ASDU_addr[2] = {0x01,0x00};
            unsigned char obj_addr[3] = {0x00,0x00,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,C_CS_NA_1,0x01,COT,ASDU_addr,obj_addr);
             char recvTime[7] = {0};
             char sendTime[7] = {0};
             cout << "SendTime: " << endl;
             show_hex((char*)CP56Time2aNOW,7);
            for(int i=0;i<7;i++)
            {
                sendTime[i] = (*(CP56Time2aNOW+i));
                recvTime[i] = (*(recvBuff+15+i));
            }
            st_cp56time2a STime;
            st_cp56time2a RTime;
            Uchar_to_cp56time2a((unsigned char*)sendTime,&STime);
            Uchar_to_cp56time2a((unsigned char*)recvTime,&RTime);
            QString Time = get_cp56time2a_string(&RTime);
            if(strncmp(recvTime,sendTime,7) != 0)
            {
                show_hex(sendTime,7);
                show_hex(recvTime,7);
                cout << "ClockSyn fali" << endl;
                emit UpdataDebugEdit(QString("ClockSyn fali!!!!"));
                ACK++;
                SFrameSession();
                emit ColckControl(2,'R',Time);
                sure = false;
                return false;
            }
            Confirm = true;
            if(sure && Confirm)
            {
                cout << "ClockSyn success!!!!" << endl;
                emit UpdataDebugEdit(QString("ClockSyn success!!!!"));
                ACK++;
                SFrameSession();
                emit ColckControl(3,'R',Time);
                return sure && Confirm;
            }
        }
    }
    cout << "ClockSyn recv error!!!" << endl;
    emit UpdataDebugEdit(QString("ClockSyn Recv Error!!!!!!!"));
    cout << "************************************" << endl;
    return sure;
}

bool Session::ClockReadSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetClockRead() << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());

    unsigned char sendTime[7] = {0};
    for(int i=0;i<7;i++)
    {
        sendTime[i] = (unsigned char)(*(SendFrame->GetAPDU()+15+i));
    }
    //show_hex((char*)sendTime,7);
    st_cp56time2a STime;
    Uchar_to_cp56time2a(sendTime,&STime);
    QString Time = get_cp56time2a_string(&STime);
    emit ColckControl(4,'S',Time);

    V_S++;
    cout << "---------------------------------" << endl;
    return true;
}

bool Session::ClockReadSessionSuccess()
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    bool Confirm = false;
    while(sure)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            //68 14 00 00 00 00 67 01 05 00 01 00 00 00 00 ---> 0~14
            //0x00 0x00 0x00 0x00 0x00 0x00 0x00    --->15~21
            int len = recvBuff[1]+2;
            cout<<"recv:";
            updata_recv_edit(1,recvBuff);
            show_hex((char*)recvBuff,len);
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char ASDU_addr[2] = {0x01,0x00};
            unsigned char obj_addr[3] = {0x00,0x00,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,C_CS_NA_1,0x01,COT,ASDU_addr,obj_addr);
            char recvTime[7] = {0};
            char Time[7] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
            for(int i=0;i<7;i++)
            {
                recvTime[i] = *(recvBuff+15+i);
            }
            show_hex(recvTime,7);

            st_cp56time2a RTime;
            Uchar_to_cp56time2a((unsigned char*)recvTime,&RTime);
            QString TimeStr = get_cp56time2a_string(&RTime);
            if(strncmp(recvTime,Time,7) == 0)
            {
                cout << "ClockRead fali" << endl;
                emit UpdataDebugEdit(QString("ClockRead fali!!!!"));
                ACK++;
                SFrameSession();
                emit ColckControl(5,'R',TimeStr);
                sure = false;
                return sure;
            }
            Confirm = true;
            if(sure && Confirm)
            {
                cout << "ClockRead success!!!!" << endl;
                emit UpdataDebugEdit(QString("ClockRead success!!!!"));
                ACK++;
                SFrameSession();
                emit ColckControl(6,'R',TimeStr);
                return sure && Confirm;
            }
        }
    }
    cout << "ClockRead recv error!!!" << endl;
    emit UpdataDebugEdit(QString("ClockRead Recv Error!!!!!!!"));
    cout << "************************************" << endl;
    return sure;
}

bool Session::ResetSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetReset() << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    V_S++;
    cout << "---------------------------------" << endl;
    return true;
}

bool Session::ResetSessionSuccess()
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    bool Confirm = false;
    while(sure)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            // 68 0e 00 00 00 00 69 01 06 00 01 00 00 00 00 01
            // 0x68 0x0e 0x00 0x00 0x00 0x00
            // 0x69 0x01 0x07 0x00 0x01 0x00 0x00 0x00 0x00 0x01
            // T1 = C_RP_NA_1(105) 复位进程命令
            int len = recvBuff[1]+2;
            cout<<"recv:";
            updata_recv_edit(1,recvBuff);
            show_hex((char*)recvBuff,len);
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char ASDU_addr[2] = {0x01,0x00};
            unsigned char obj_addr[3] = {0x00,0x00,0x00};
            //标识类型 TI = 0x69 可变帧长限定词 VSQ  = 0x01 召唤限定词 QRP = 0x01
            unsigned char size = recvBuff[1];
            unsigned char QRP = 0x01;
            sure = EncodeBuff(recvBuff,size,C,C_RP_NA_1,0x01,COT,ASDU_addr,obj_addr);
            QString desc = QString("未定义");;
            if(recvBuff[15] != QRP)
            {
                sure = false;
                cout << "QRP is error" << endl;
                emit ResetShow(0,desc);
                return sure;
            }
            else
            {
                if(recvBuff[15] == 0x00) desc = QString("进程总复位");
                else if(recvBuff[15] == 0x01) desc = QString("进程总复位");
                else if(recvBuff[15] == 0x02) desc = QString("复位事件缓冲器等待处理的时标消息");
            }
            Confirm = true;
            if(sure && Confirm)
            {
                cout << "Reset success!!!!" << endl;
                emit UpdataDebugEdit(QString("Reset success!!!!"));
                emit ResetShow(1,desc);
                ACK++;
                SFrameSession();
                return sure && Confirm;
            }
        }
    }
    cout << "Reset Recv fail !!!!" << endl;
    emit UpdataDebugEdit(QString("Reset Recv fail!!!!"));
    return sure && Confirm;
    cout << "************************************" << endl;
    return sure;
}

bool Session::DirCallSession(char *DirName)
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetDirCall(DirName) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    V_S++;
    cout << "---------------------------------" << endl;
    return true;
}

bool Session::DirCallSessionSuccess()
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    bool follow = true;
    int n = -3;
//    Info JUMP;
//    I_InfoBuffer->wait(JUMP);
//    JUMP = I_TakeInfo(n);
    while(sure)
    {
        /*
        68 27 00 00 00 00
        d2 00
        07 00
        01 00
        00 00 00   -> 0~14

        02          ->15
        02 00
        00 00 00 00
        01 01 04
        63 6f 64 65     -> x = 0x04(变量)
        00
        00 00 0f ff
        00 00 00 00 00 00 00
        */
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        
        if(n>1)
        {
            updata_recv_edit(1,recvBuff);
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            unsigned char C[4] = {0x00,0x00,0x00,0x00};   //----->这里一般是 0x00 0x00 0x00 0x00
            my_strncpy(C,recvBuff+2,4);
            //标识类型 TI = 0xD2 可变帧长限定词 VSQ  = 0x00
            unsigned char T1 = F_FR_NA_1;
            unsigned char VSQ  = recvBuff[7];
            unsigned char COT[2] = {0x05,0x00};//传送原因 COT = 7
            unsigned char ASDU_addr[2] = {0x01,0x00};
            unsigned char obj_addr[3] = {0x00,0x00,0x00};
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr,obj_addr);// 没有限定词，一般判断0~14
            if(recvBuff[15] != 0x02)
            {
                cout << "The additional packet type is not 0x02" << endl;
                emit UpdataDebugEdit(QString("The additional packet type is not 0x02"));
                sure = false;
            }
            if(recvBuff[16] != 0x02){
                sure = false;
                cout << "DirCall flag fail" << endl;
                emit UpdataDebugEdit(QString("DirCall flag fail"));
                return sure;
            }
            if(recvBuff[17] != 0x00){
                sure = false;
                cout << "DirCall ret fail" << endl;
                emit UpdataDebugEdit(QString("DirCall ret fail"));
                return sure;
            }
            char DirID[4] = {0};
            my_strncpy(DirID,recvBuff+18,4);

            //解析目录名
            int filecount = recvBuff[23];
            File *fileArray = new File[filecount];
            printf("\nfilecount: %d\n\n----start read DIR ----\n",filecount);

            for(int i=0 , j = 0;i<filecount;i++)
            {
                //cout << "i = " << i <<endl;
               //如果个数不对就是这有问题，char转int
                int NameSize = recvBuff[24+j];
                char *FileName = new char[NameSize];
                j = j+1;
                my_strncpy(FileName,recvBuff+(24+j),NameSize);
                //printf("fileNameSize: %02x\n" , NameSize);
                cout << "filename: ";
                show_str(FileName,NameSize);
                //show_hex(FileName,NameSize);
                j = j+NameSize;
                char property = recvBuff[24+j];
                //printf("property: %02x\n" , property);
                char *FileSize = new char[4];
                j = j+1;
                my_strncpy(FileSize,recvBuff+(24+j),4);
                char *Time = new char[7];
                j = j+4;
                my_strncpy(Time,recvBuff+(24+j),7);
                fileArray[i].setNameSize(NameSize);
                fileArray[i].setFileName(FileName);
                fileArray[i].setFileProperty(property);
                fileArray[i].setFileSize(FileSize);
                fileArray[i].setFileTime(Time);
                j = j+7;
                emit ShowDirname(FileName,NameSize);
            }
            printf("----end read DIR, %d file ----\n",filecount);


            if(recvBuff[22] == 0x00)
            {
                cout << "DirCall success,No follow-up" << endl;
                follow = false;
                emit UpdataDebugEdit(QString("DirCall success,%1 files").arg(QString::number(filecount)));
                ACK++;
                SFrameSession();
                return sure;
            }
            else if(recvBuff[22] == 0x01)
            {
                cout << "DirCall success,follow-up" << endl;
                follow = false;
                emit UpdataDebugEdit(QString("DirCall success,follow-up"));
                ACK++;
                SFrameSession();
            }
            else
            {
                cout << "follow-up flage error" << endl;
                emit UpdataDebugEdit(QString("follow-up flage error"));
                sure = false;
            }


            if(sure && follow)
            {
                cout << "DirCall success!!!!" << endl;
                emit UpdataDebugEdit(QString("DirCall success!!!!,have follow-up...."));
                ACK++;
                SFrameSession();
                //return sure && Confirm;
            }
        }
    }
    cout << "DirCall Recv fail !!!!" << endl;
    emit UpdataDebugEdit(QString("DirCall Recv fail!!!!"));
    cout << "************************************" << endl;
    return sure;
}

bool Session::ReadFileSession(char *RFileName)
{
    emit ReadLabelEnable(0);
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetReadFile(RFileName) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    V_S++;
    cout << "---------------------------------" << endl;
    return true;
}

bool Session::ReadFileSessionSuccess(QString DirName)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    bool Enable = false;
    int pack_count =0;
    FileOperation *fileop;
    fileop = new FileOperation();
    char *FileID;
    char *FileOffset;
    while(sure)
    {
        /* 读文件激活确认
        68 20 00 00 00 00		->APCI

        D2 00  ->T1 = F_FR_NA_1(210)
        07 00  ->  COT
        01 00  -> ASDU_addr
        00 00 00 -->obj_addr

        02  ->附加数据包类型(2:文件传输)  ->15
        04 	->读文件激活确认
        00	-> 结果： 0成功，1失败
        06  ->文件名长度                 -> 18

        name[x] = {}	->文件完整名(不带结束符) 00 00 00 00 00 00
        00 00 00 00     ->文件ID
        00 00 00 00     ->文件大小
        */

        /*读文件数据传输
        68 20 00 00 00 00		->APCI

        D2 00  ->T1 = F_FR_NA_1(210)
        06 00  ->  COT
        01 00  -> ASDU_addr
        00 00 00 -->obj_addr

        02  ->附加数据包类型(2:文件传输)  ->15
        04 	->读文件激活确认
        00 00 00 00     ->文件ID
        00 00 00 00     ->数据段号(文件内容的偏移指针值)
        00              ->后续标志(0:无后续，1有后续)
        filedata[x]     ->文件数据  x = recv[1]-25
        00              ->校验码
        */

        cout << endl;
        //消息接收
        memset(recvBuff,0,256);
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            updata_recv_edit(1,recvBuff);
            if(!Enable) //激活确认判断
            {
                int len = recvBuff[1]+2;
                cout<<"recv:";
                show_hex((char*)recvBuff,len);
                unsigned char C[4] = {0x00,0x00,0x00,0x00};   //----->这里一般是 0x00 0x00 0x00 0x00
                my_strncpy(C,recvBuff+2,4);
                //标识类型 TI = 0xD2 可变帧长限定词 VSQ  = 0x00
                unsigned char T1 = F_FR_NA_1;
                unsigned char VSQ  = recvBuff[7];
                unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
                unsigned char ASDU_addr[2] = {0x01,0x00};
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                unsigned char size = recvBuff[1];
                sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr,obj_addr);// 没有限定词，一般判断0~14,C在函数为用上
                if(!sure){cout << "Enable fail" << endl;return sure;}
                if(recvBuff[15] != 0x02)
                {
                    cout << "The additional packet type is not 0x02" << endl;
                    emit UpdataDebugEdit(QString("The additional packet type is not 0x02"));
                    sure = false;
                    return sure;
                }
                //4：读文件激活确认
                if(recvBuff[16] != 0x04){
                    cout << "ReadFile Enable fail" << endl;
                    emit UpdataDebugEdit(QString("ReadFile Enable fail"));
                    sure = false;
                    return sure;
                }
                if(recvBuff[17] != 0x00){
                    cout << "ReadFile Enable fail" << endl;
                    emit UpdataDebugEdit(QString("ReadFile Enable fail"));
                    sure = false;
                    return sure;
                }
                else
                {
                    int namesize = recvBuff[18];
                    fileop->setNameSize(namesize);  //文件名长度
                    char *Filename = new char[namesize];
                    my_strncpy(Filename,recvBuff+19,namesize);
                    fileop->setFileName(Filename);  // 文件名
                    FileID = new char[4];
                    my_strncpy(FileID,recvBuff+19+namesize,4);
                    fileop->setFileID(FileID);      //文件ID
                    char *FileSize = new char[4];
                    my_strncpy(FileSize,recvBuff+19+namesize+4,4);
                    fileop->setFileOpSize(FileSize);      //文件大小
                    cout << "ReadFile Enable success" << endl;
                    cout << "Filename:" << endl;
                    show_hex(Filename,namesize);
                    Enable = true;
                    emit ReadMax(fileop->getIntFileSize());
                    emit ReadLabelEnable(1);
                    emit UpdataDebugEdit(QString("ReadFile Enable success"));
                    ACK++;
                    SFrameSession();
                    if(Enable == false) return Enable;
                    char *Fname = new char[namesize+1];
                    memset(Fname,0,namesize+1);
                    my_strncpy(Fname,Filename,namesize);
//                    cout << "DirName: " << DirName.toUtf8().data() << endl;
                    printf("Fname: %s",Fname);
                    fileop->ClearFile(Fname,DirName.toUtf8().data());
                }
            }
            else    //进行文件传输
            {
                int len = recvBuff[1]+2;
                cout<<"recv:";
                show_hex((char*)recvBuff,len);
                unsigned char C[4] = {0x00,0x00,0x00,0x00};   //----->这里一般是 0x00 0x00 0x00 0x00
                my_strncpy(C,recvBuff+2,4);
                //标识类型 TI = 0xD2 可变帧长限定词 VSQ  = 0x00
                unsigned char T1 = F_FR_NA_1;
                unsigned char VSQ  = recvBuff[7];
                unsigned char COT[2] = {0x05,0x00};//传送原因 COT = 7
                unsigned char ASDU_addr[2] = {0x01,0x00};
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                unsigned char size = recvBuff[1];
                sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr,obj_addr);// 没有限定词，一般判断0~14,C在函数为用上
                if(recvBuff[15] != 0x02)
                {
                    cout << "The additional packet type is not 0x02" << endl;
                    emit UpdataDebugEdit(QString("The additional packet type is not 0x02"));
                    sure = false;
                }
                if(recvBuff[16] != 0x05){sure = false;}     //5：读文件数据
                else
                {
                    //文件ID校验
//                    char *FileID = new char[4];
//                    my_strncpy(FileID,recvBuff+17,4);
                    FileOffset = new char[4];
                    my_strncpy(FileOffset,recvBuff+17+4,4);
                    cout << "FileOffset: ";
                    show_hex(FileOffset,4);
                    cout << "file recving..." << endl;
                    fileop->setFileOffset(FileOffset);
                    int data_len = recvBuff[1]-25;
                    char *data = new char[data_len];
                    my_strncpy(data,recvBuff+26,data_len);
                    cout << "filedata: " << endl;
                    show_hex(data,data_len);
                    char crc = recvBuff[26+data_len];
                    cout << "CRC: ";
                    show_hex(&crc,1);
                    fileop->setCRC(crc);
                    if((unsigned char)crc == CRC_(data,data_len))
                    {
                        cout << "crc_cmp: " << ((unsigned char)crc == CRC_(data,data_len)) << endl;
                        cout << "recv success , this " << pack_count++ << " data package" << endl;
                        fileop->setFileDate(data,DirName.toUtf8().data(),data_len);
                        emit ReadValue(fileop->getIntOffset(fileop->getFileOffset()));
                        ACK++;
                        SFrameSession();
                    }
                    else
                    {
                        int crc_num = CRC_(data,data_len);
                        cout << "recv fail,crc error,RecvCRC = " << crc_num << endl;
                        emit UpdataDebugEdit(QString("read file recv fail,crc error! RecvCRC = %1").arg(crc_num,2,16,QLatin1Char('0')));
                        emit ReadConfirm(0,nullptr,nullptr,0x01);
                        return false;
                    }
                    if(recvBuff[17+4+4] == 0x00){
                        cout << "file recv ,but no follow-up" << endl;
                        cout << "fileSize :" << fileop->getIntFileSize() << endl;
                        emit ReadValue(fileop->getIntFileSize());
                        emit UpdataDebugEdit(QString("Readfile success!!!!"));
                        emit ReadConfirm(1,(unsigned char*)FileID,(unsigned char*)FileOffset,0x00);
                        emit ReadLabelEnable(2);
                        return true;
                    }
                }
            }
        }
    }
    cout << "Readfile Recv fail" << endl;
    emit UpdataDebugEdit(QString("Readfile Recv fail!!!!"));
    return sure;
}

bool Session::ReadConfirmSession(unsigned char *FileID, unsigned char *FileOffset, unsigned char desc)
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetReadConfirm(FileID,FileOffset,desc) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    V_S++;
    cout << "---------------------------------" << endl;
    return true;
}

/*
     @Operation: 写文件类型,0写文件 1升级文件传输
*/
Frame Session::WriteFileSession(char *WFileName,int Operation)
{
    if(Operation == 0)
    {
        //传输文件
        emit WriteLabelEnable(0);
    }
    else if(Operation == 1)
    {
        //升级文件传输
        emit UpdataLabelEnable(0);
    }
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetWriteFile(WFileName) << endl;
    SendFrame->updataCON(V_S,ACK);
    updata_recv_edit(0,SendFrame->GetAPDU());

    this->FrameSend(*SendFrame);
    cout << "---------------------------------" << endl;
    V_S++;
    return *SendFrame;
}

//写文件激活判定
/*
     @Operation: 写文件类型,0写文件 1升级文件传输
*/
bool Session::WriteFileSessionEnable(Frame &frame,int Operation)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    bool Enable = false;
    wfileop = new FileOperation();
    char *per_send;
    cout << frame.Getlen() << endl;
    per_send = new char[frame.Getlen()];
    my_strncpy(per_send,frame.GetAPDU(),frame.Getlen());
    while(sure)
    {
        /* 写文件激活确认
        68 20 00 00 00 00		->APCI

        D2 00  ->T1 = F_FR_NA_1(210)
        07 00  ->  COT
        01 00  -> ASDU_addr
        00 00 00 -->obj_addr

        02  ->附加数据包类型(2:文件传输)  ->15
        08 	->写文件激活确认
        00	-> 结果描述字： 0：成功 1：未知错误 2. 文件名不支持 3：长度超范围
        06  ->文件名长度                 -> 18

        name[x] = {}	->文件完整名(不带结束符) 00 00 00 00 00 00
        00 00 00 00     ->文件ID
        00 00 00 00     ->文件大小
        */

        /*读文件数据传输
        68 20 00 00 00 00		->APCI

        D2 00  ->T1 = F_FR_NA_1(210)
        06 00  ->  COT
        01 00  -> ASDU_addr
        00 00 00 -->obj_addr

        02  ->附加数据包类型(2:文件传输)  ->15
        04 	->读文件激活确认
        00 00 00 00     ->文件ID
        00 00 00 00     ->数据段号(文件内容的偏移指针值)
        00              ->后续标志(0:无后续，1有后续)
        filedata[x]     ->文件数据  x = recv[1]-25
        00              ->校验码
        */

        cout << endl;
        //消息接收
        memset(recvBuff,0,256);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        
        if(n>1)
        {
            updata_recv_edit(1,recvBuff);
            if(!Enable) //激活确认判断
            {
                int len = recvBuff[1]+2;
                cout<<"recv:";
                show_hex((char*)recvBuff,len);
                unsigned char C[4] = {0x00,0x00,0x00,0x00};   //----->这里一般是 0x00 0x00 0x00 0x00
                my_strncpy(C,recvBuff+2,4);
                //标识类型 TI = 0xD2 可变帧长限定词 VSQ  = 0x00
                unsigned char T1 = F_FR_NA_1;
                unsigned char VSQ  = 0x01;
                unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
                unsigned char ASDU_addr[2] = {0x01,0x00};
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                unsigned char size = recvBuff[1];
                sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr,obj_addr);// 没有限定词，一般判断0~14,C在函数为用上
                QString FileType;
                if(Operation == 0)
                    FileType = QString("WriteFile");
                else
                    FileType = QString("VersionFile");
                if(!sure)
                {
                    cout << FileType.toUtf8().data() <<" Enable fail" << endl;
                }
                if(recvBuff[15] != 0x02)
                {
                    cout << "The additional packet type is not 0x02" << endl;
                    emit UpdataDebugEdit(QString("The additional packet type is not 0x02"));
                    sure = false;
                    return sure;
                }
                //8：写文件激活确认
                if(recvBuff[16] != 0x08){
                    cout << FileType.toUtf8().data() << " Enable fail" << endl;
                    emit UpdataDebugEdit(QString("%1 Enable fail").arg(FileType));
                    sure = false;
                    return sure;
                }
                if(recvBuff[17] != 0x00){
                    if(recvBuff[17] == 0x01) // 1：未知错误
                    {
                        cout << FileType.toUtf8().data() << " Enable fail , unkonow error !!! " << endl;
                        emit UpdataDebugEdit(QString("%1 Enable fail , unkonow error !!!").arg(FileType));
//                        cout <<  " I_InfoBuffer of SIZE: " << I_InfoBuffer->GetSize() << endl;

                    }else if(recvBuff[17] == 0x02)  //2. 文件名不支持
                    {
                        cout << FileType.toUtf8().data() << " Enable fail , File names are not supported !!! " << endl;
                        emit UpdataDebugEdit(QString("%1 Enable fail , File names are not supported !!!").arg(FileType));
                    }else if(recvBuff[17] == 0x03)  //3：长度超范围
                    {
                        cout << FileType.toUtf8().data() << " Enable fail , Length out of range !!! " << endl;
                        emit UpdataDebugEdit(QString("%1 Enable fail , Length out of range !!!").arg(FileType));
                    }else
                    {
                        cout << FileType.toUtf8().data() << " Enable fail , retrun error !!! " << endl;
                        emit UpdataDebugEdit(QString("%1 Enable fail , retrun error !!!").arg(FileType));
                    }

                    sure = false;
                    return sure;
                }
                else
                {
                    int namesize = recvBuff[18];
                    int per_namesize = *(per_send+17);
                    wfileop->setNameSize(namesize);  //文件名长度
                    char *Filename = new char[len];
                    my_strncpy(Filename,recvBuff+19,namesize);

                    char *per_Filename = new char[len];
                    my_strncpy(per_Filename,per_send+18,per_namesize);
                    if(strncmp(Filename,per_Filename,namesize))
                    {
                        emit UpdataDebugEdit(QString("%1 Enable fail,Filename error").arg(FileType));
                        return Enable;
                    }
                    wfileop->setFileName(Filename);  // 文件名
                    char *FileID = new char[4];
                    my_strncpy(FileID,recvBuff+19+namesize,4);

                    char *per_FileID = new char[4];
                    my_strncpy(per_FileID,per_send+18+per_namesize,4);
                    if(strncmp(FileID,per_FileID,4))
                    {
                        emit UpdataDebugEdit(QString("%1 Enable fail,FileID error").arg(FileType));
                        return Enable;
                    }

                    wfileop->setFileID(FileID);      //文件ID
                    char *FileSize = new char[4];
                    my_strncpy(FileSize,recvBuff+19+namesize+4,4);

                    char *per_FileSize = new char[4];

                    my_strncpy(per_FileSize,per_send+18+per_namesize+4,4);
                    if(strncmp(FileID,per_FileID,4))
                    {
                        emit UpdataDebugEdit(QString("%1 Enable fail,FileSize error").arg(FileType));
                        return Enable;
                    }

                    wfileop->setFileOpSize(FileSize);      //文件大小
                    cout << FileType.toUtf8().data() << " Enable success" << endl;
                    Enable = true;
                    if(Enable)
                    {
                        if(Operation == 0)
                        {
                            //写文件
                            emit WriteMax(wfileop->getIntFileSize());
                            emit WriteLabelEnable(1);
                        }
                        else if(Operation == 1)
                        {
                            //升级文件传输
                            emit UpdataMax(wfileop->getIntFileSize());
                            emit UpdataLabelEnable(1);
                        }

                        emit UpdataDebugEdit(QString("%1 Enable success").arg(FileType));
                        qDebug() << wfileop->getIntFileSize();
                        ACK++;
                        SFrameSession();
                        return Enable;
                    }
                }
            }
        }
    }
    
    if(Operation == 0)
    {
        cout << "WriteFile Recv fail" << endl;
        emit UpdataDebugEdit(QString("WriteFile Recv fail!!!!"));
    }
    else
    {
        cout << "VersionFile Recv fail" << endl;
        emit UpdataDebugEdit(QString("VersionFile Recv fail!!!!"));
    }
    return false;
}

bool Session::WriteFileSessionConfirm(Frame &frame, int offset,int Operation)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    char *per_send;
    cout << frame.Getlen() << endl;
    per_send = new char[frame.Getlen()];
    my_strncpy(per_send,frame.GetAPDU(),frame.Getlen());
    while(sure)
    {
        /*
            写文件激活确认
            68 20 00 00 00 00		->APCI

            D2 00  ->T1 = F_FR_NA_1(210)
            05 00  ->  COT
            01 00  -> ASDU_addr
            00 00 00 -->obj_addr

            02  ->附加数据包类型(2:文件传输)  ->15
            0a 	->写文件数据 传输确认
            00 00 00 00     ->文件ID
            00 00 00 00     ->数据段号
            00	-> 结果描述字： 0：成功 1：未知错误 2.校验和错误 3.文件长度不对应 4.文件 ID 与激活 ID 不一致

        */
        //68 20 00 00 00 00 D2 00 05 00 01 00 00 00 00 02 0a 00 00 00 00 00 00 00 00 00
        cout << endl;
        //消息接收
        memset(recvBuff,0,256);
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            updata_recv_edit(1,recvBuff);
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            unsigned char C[4] = {0x00,0x00,0x00,0x00};   //----->这里一般是 0x00 0x00 0x00 0x00
            my_strncpy(C,recvBuff+2,4);
            //标识类型 TI = 0xD2 可变帧长限定词 VSQ  = 0x00
            unsigned char T1 = F_FR_NA_1;
            unsigned char VSQ  = 0x00;
            unsigned char COT[2] = {0x05,0x00};//传送原因 COT = 7
            unsigned char ASDU_addr[2] = {0x01,0x00};
            unsigned char obj_addr[3] = {0x00,0x00,0x00};
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr,obj_addr);// 没有限定词，一般判断0~14,C在函数为用上
            QString FileType;
            if(Operation == 0)
                FileType = QString("WriteFile");
            else
                FileType = QString("VersionFile");

            if(!sure){cout << FileType.toUtf8().data() << " judge Confirm fail" << endl;}
            if(recvBuff[15] != 0x02)
            {
                cout << "The additional packet type is not 0x02" << endl;
                emit UpdataDebugEdit(QString("The additional packet type is not 0x02"));
                sure = false;
                return sure;
            }
            //8：写文件传输确认
            if(recvBuff[16] != 0x0a){
                cout << FileType.toUtf8().data() << " Confirm operate flag fail" << endl;
                emit UpdataDebugEdit(QString("%1 Confirm operate flag fail").arg(FileType));
                sure = false;
                return sure;
            }

            int per_namesize = (unsigned char)per_send[17];
            //文件ID
            char *FileID = new char[4];
            my_strncpy(FileID,recvBuff+17,4);

            char *per_FileID = new char[4];
            my_strncpy(per_FileID,per_send+18+per_namesize,4);
            if(strncmp(FileID,per_FileID,4))
            {
                emit UpdataDebugEdit(QString("%1 Confirm fail,FileID error").arg(FileType));
                sure = false;
                return false;
            }

            //文件偏移量
            char *FileOffset = new char[4];
            my_strncpy(FileOffset,recvBuff+17+4,4);

            union Date
            {
                int a;
                char arr[4];
            }offsetd;
            offsetd.a = offset;

            if(strncmp(FileOffset,offsetd.arr,4))
            {
                emit UpdataDebugEdit(QString("%1 Confirm fail,FileOffset error").arg(FileType));
                sure = false;
                return false;
            }

            cout << FileType.toUtf8().data() << " Confirm success" << endl;

            if(recvBuff[25] != 0x00){
                if(recvBuff[17] != 0x01) // 1：未知错误
                {
                    cout << FileType.toUtf8().data() << " Confirm fail , unkonow error !!! " << endl;
                    emit UpdataDebugEdit(QString("%1 Confirm fail , unkonow error !!!").arg(FileType));
                }else if(recvBuff[17] != 0x02)  //2. 校验和错误
                {
                    cout << FileType.toUtf8().data() << " Confirm fail , Checksum error !!! " << endl;
                    emit UpdataDebugEdit(QString("%1 Confirm fail , Checksum error !!!").arg(FileType));
                }else if(recvBuff[17] != 0x03)  //3：文件长度不对应
                {
                    cout << FileType.toUtf8().data() << " Confirm fail , The file length does not correspond !!! " << endl;
                    emit UpdataDebugEdit(QString("%1 Confirm fail , The file length does not correspond !!!").arg(FileType));
                }else if(recvBuff[17] != 0x04)  //4. 文件 ID 与激活 ID 不一致
                {
                    cout << FileType.toUtf8().data() << " Confirm fail , The file ID does not match the activation ID !!! " << endl;
                    emit UpdataDebugEdit(QString("%1 Confirm fail , The file ID does not match the activation ID !!!").arg(FileType));
                }
                else
                {
                    cout << FileType.toUtf8().data() << " Confirm fail , retrun error !!! " << endl;
                    emit UpdataDebugEdit(QString("%1 Confirm fail , retrun error !!!").arg(FileType));
                }

                sure = false;
                return sure;
            }else
            {
                if(Operation == 0)
                {
                    //写文件
                    emit WriteLabelEnable(2);
                }
                else if(Operation == 1)
                {
                    //软件文件传输
                    emit UpdataLabelEnable(2);
                    emit UpdataSuccess();
                }

                emit UpdataDebugEdit(QString("%1 Confirm success").arg(FileType));
                ACK++;
                SFrameSession();
                return sure;
            }
        }
    }
    cout << "VersionFile Recv fail" << endl;
    emit UpdataDebugEdit(QString("VersionFile Recv fail!!!!"));
    return true;
}

bool Session::WriteFileSessionSuccess(Frame &frame, QString filename)
{
    bool ret = WriteFileSessionEnable(frame,0);
    if(ret)
    {
        // 写文件传输
        QFile  f(filename);
        int filesize= f.size();
        int offset = 0;
        emit UpdataDebugEdit(QString("WriteFile Transfering ....."));
        while(filesize-offset)
        {
            SendFrame->SetWriteData(filename,offset);
//            cout << filesize << endl;
//            cout << offset << endl;
            SendFrame->updataCON(V_S,ACK);
            updata_recv_edit(0,SendFrame->GetAPDU());
            Sleep(100);
            this->FrameSend(*SendFrame);
            V_S++;
            emit WriteValue(offset);
        }
        emit UpdataDebugEdit(QString("WriteFile Transfer End"));
        WriteFileSessionConfirm(frame,offset,0);
    }
    return true;
}

bool Session::ReadFixedValueNumSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetReadFixedValueNum() << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::ReadFixedValueNumSessionSuccess()
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure1 = true;
    bool sure2 = true;
    while(sure1||sure2)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            //68 14 00 00 00 00 C9 01 07 00 01 00 00 00 00 ---> 0~14
            //01 02
            //03 04
            //81 09 10 00 00 00 00     --->15~21
            //68 13 00 00 00 00 c9 01 07 00 01 00 00 00 00 10 00 00 00 ff 00
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
//            cout << "Frame: ";
//            show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);
//            cout << endl;
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char COT1[2] = {0x2F,0x00};//传送原因 COT = 47
            unsigned char T1 = C_RR_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            unsigned char obj_addr[3] = {0x00,0x00,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = recvBuff[7];
            unsigned char size = recvBuff[1];
            sure1 = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr,obj_addr);
            sure2 = EncodeBuff(recvBuff,size,C,T1,VSQ,COT1,ASDU_addr,obj_addr);

            unsigned char FixedValueNum[2] = {0};
            unsigned char FixedValueNumMin[2] = {0};
            unsigned char FixedValueNumMax[2] = {0};
            FixedValueNum[0] = recvBuff[15];
            FixedValueNum[1] = recvBuff[16];
            FixedValueNumMin[0] = recvBuff[17];
            FixedValueNumMin[1] = recvBuff[18];
            FixedValueNumMax[0] = recvBuff[19];
            FixedValueNumMax[1] = recvBuff[20];
//            my_strncpy(FixedValueNumMax,recvBuff+19,7);

            int a = FixedValueNum[1];
            a = (a<<8)+FixedValueNum[0];
            printf("%02x %02x , FixedValueNum: %d\n",FixedValueNum[0],FixedValueNum[1],a);
            int b = FixedValueNumMin[1];
            b = (b<<8)+FixedValueNumMin[0];
            ValueMIN = b;
            printf("%02x %02x ,FixedValueNumMin: %d\n",FixedValueNumMin[0],FixedValueNumMin[1],b);
            int c = FixedValueNumMax[1];
            c = (c<<8)+FixedValueNumMax[0];
            ValueMAX = c;
            printf("%02x %02x ,FixedValueNumMax: %d\n",FixedValueNumMax[0],FixedValueNumMax[1],c);


            if(recvBuff[8] == 0x07)
            {
                cout << " Read FixedValueNum success!!!! " << endl;
                emit UpdataDebugEdit(QString(" Read FixedValueNum success!!!! "));
                emit RemoteValueControl(1,a,ValueMIN,ValueMAX,'-',QString(""),'-',QString(""));
                ACK++;
                SFrameSession();
                return true;
            }
            else if(recvBuff[8] == 0x2F)
            {
                cout << " Read FixedValueNum end!!!! " << endl;
                emit UpdataDebugEdit(QString(" Read FixedValueNum end!!!!"));
                return false;
            }
        }
    }
    cout << " Read FixedValueNum Recv fail!!!! " << endl;
    emit UpdataDebugEdit(QString(" Read FixedValueNum Recv fail!!!! "));
    cout << "************************************" << endl;
    return sure1||sure2;
}

//切换定值区号
bool Session::SetFixedValueNumSession(int FixedValueNum)
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetToggleFixedValueNum(FixedValueNum) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::SetFixedValueNumSessionSuccess(int FixedValueNum)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure1 = true;
    bool sure2 = true;
    while(sure1||sure2)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            /*
            //68 0f 00 00 00 00
            //C8 01
            //06 00
            //01 00
            //00 00 00      -->obj_addr
            //00 00
            */
            //68 0f 00 00 00 00 C8 01 07 00 01 00 00 00 00 00 00
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
//            cout << "Frame: ";
//            show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);
//            cout << endl;
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char COT1[2] = {0x2F,0x00};//传送原因 COT = 47
            unsigned char T1 = C_SR_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            unsigned char obj_addr[3] = {0x00,0x00,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = recvBuff[7];
            unsigned char size = recvBuff[1];
            sure1 = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr,obj_addr);
            sure2 = EncodeBuff(recvBuff,size,C,T1,VSQ,COT1,ASDU_addr,obj_addr);

            unsigned char FixedValueNumarr[2] = {0};
            FixedValueNumarr[0] = recvBuff[15];
            FixedValueNumarr[1] = recvBuff[16];

            int a = FixedValueNumarr[1];
            a = (a<<8)+FixedValueNumarr[0];
            printf("FixedValueNum: %d\n", a);
            if(a!=FixedValueNum)
            {
                cout << " FixedValueNum is error,"<< "send: " << FixedValueNum << "recv: " << a << endl;
            }

            if(recvBuff[8] == 0x07)
            {
                cout << " Set FixedValueNum success!!!! " << endl;
                emit UpdataDebugEdit(QString(" Set FixedValueNum success!!!! "));
                emit RemoteValueControl(2,a,ValueMIN,ValueMAX,'-',QString(""),'-',QString(""));
                ACK++;
                SFrameSession();
                return true;
            }
            else if(recvBuff[8] == 0x2F)
            {
                cout << " Set FixedValueNum end!!!! " << endl;
                emit UpdataDebugEdit(QString(" Set FixedValueNum end!!!! "));
                return false;
            }
        }
    }
    cout << " Set FixedValueNum Recv fail!!!! " << endl;
    emit UpdataDebugEdit(QString(" Set FixedValueNum Recv fail!!!! "));
    cout << "************************************" << endl;
    return sure1||sure2;
}

bool Session::ReadValueSession(int Value, QStringList ObjAddr)
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
//    cout << "----sssssss---------" << endl;
    cout << "is_setFrame: " << SendFrame->SetReadValue(Value,ObjAddr) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
//    show_hex((char*)SendFrame->GetAPDU(),*(SendFrame->GetAPDU()+1));
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::ReadValueSessionSuccess(int Value)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure1 = true;
    bool sure2 = true;
    while(sure1||sure2)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {

            /*
            //68 0f 00 00 00 00
            //Ca 01
            //07 00
            //01 00
            //00 00         -->定值区号
            //00 00 00      -->obj_addr
            */
            //68 0f 00 00 00 00 Ca 01 07 00 01 00 00 00 00 00 00
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);

            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char COT1[2] = {0x2F,0x00};//传送原因 COT = 47
            unsigned char T1 = C_RS_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = recvBuff[7];
            unsigned char size = recvBuff[1];
            sure1 = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr);
            sure2 = EncodeBuff(recvBuff,size,C,T1,VSQ,COT1,ASDU_addr);
            if(!(sure1||sure2)){emit UpdataDebugEdit(QString("ReadValue fail"));return false;}
            unsigned char SN[2] = {0x00,0x00};  //定值区号
            SN[0] = recvBuff[12];
            SN[1] = recvBuff[13];
            int num = SN[1];
            num = ((num<<8)+SN[0]);
            printf("SN: %02x %02x , %d %d",SN[0],SN[1],num,Value);
            //参数特征标识
            unsigned char Flag = recvBuff[14];
            //信息体地址
            int count = VSQ&0x7f;
            if((VSQ&0x80)>>7 == 1)
            {
                printf("\nVSQ = %02x\n",VSQ);
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                obj_addr[0] = recvBuff[15];
                obj_addr[1] = recvBuff[16];
                obj_addr[2] = recvBuff[17];
                int obj_num = 0;
                obj_num = obj_addr[1];
                obj_num = (obj_num<<8) + obj_addr[0];
                int j = 0;
                for(int i=0;i<count;i++)
                {
                    unsigned char Tag = recvBuff[18+j];
                    int datalen = recvBuff[19+j];
                    unsigned char* Udata;
                    Udata = new unsigned char[datalen];
                    my_strncpy(Udata,recvBuff+20+j,datalen);
                    j = j+2+datalen;
                    char *data;
                    hex_to_char(&data,(char*)Udata,datalen);
                    emit RemoteValueControl(3,num,ValueMIN,ValueMAX,Flag,QString::number(obj_num++,16),Tag,QString(data));
                }
            }else if((VSQ&0x80)>>7 == 0)
            {
                printf("\nVSQ = %02x\n",VSQ);
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                int j = 0;
                for(int i=0;i<count;i++)
                {
                    obj_addr[0] = recvBuff[15+j];
                    obj_addr[1] = recvBuff[16+j];
                    obj_addr[2] = recvBuff[17+j];
                    printf("obj_addr = %02x %02x %02x\n",obj_addr[0],obj_addr[1],obj_addr[2]);
                    unsigned char Tag = recvBuff[18+j];
                    int datalen = recvBuff[19+j];
                    unsigned char* Udata;
                    Udata = new unsigned char[datalen];
                    my_strncpy(Udata,recvBuff+20+j,datalen);
                    j = j+2+datalen+3;
                    char *data;
                    hex_to_char(&data,(char*)Udata,datalen);
//                    char *OBJ;
//                    hex_to_char((&OBJ),(char*)obj_addr,3);
                    int obj_num = 0;
                    obj_num = obj_addr[1];
                    obj_num = (obj_num<<8) + obj_addr[0];
                    emit RemoteValueControl(3,num,ValueMIN,ValueMAX,Flag,QString::number(obj_num,16),Tag,QString(data));
                }
            }


            if((Flag&0x01) == 0x00)
            {
                cout << "ReadValue success!!! no follow-up" << endl;
                emit UpdataDebugEdit(QString("ReadValue success!!! no follow-up "));
                ACK++;
                SFrameSession();
                return sure1||sure2;
            }
            else
            {
                cout << "ReadValue success!!! hava follow-up" << endl;
                emit UpdataDebugEdit(QString("ReadValue success!!! hava follow-up "));
                ACK++;
                SFrameSession();
            }

        }
    }
    cout << "ReadValue Recv fail!!!" << endl;
    emit UpdataDebugEdit(QString("ReadValue Recv fail"));
    return sure1||sure2;
}

bool Session::PersetSession(int ValueNum, int status, QStringList addr, QStringList tag, QStringList value)
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetPerset(ValueNum,status,addr,tag,value) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
//    show_hex((char*)SendFrame->GetAPDU(),*(SendFrame->GetAPDU()+1));
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::PersetSessionSuccess(int ValueNum, int status)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    while(sure)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {

            /*
            //68 0f 00 00 00 00
            //Ca 01
            //07 00
            //01 00
            //00 00         -->定值区号
            //00 00 00      -->obj_addr
            */
            //68 0f 00 00 00 00 Ca 01 07 00 01 00 00 00 00 00 00
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);

            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char T1 = C_WS_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = recvBuff[7];
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr);
            if(!(sure)){emit UpdataDebugEdit(QString("Perset fail"));return false;}
            unsigned char SN[2] = {0x00,0x00};  //定值区号
            SN[0] = recvBuff[12];
            SN[1] = recvBuff[13];
            int num = SN[1];
            num = ((num<<8)+SN[0]);
            printf("SN: %02x %02x , %d %d\n",SN[0],SN[1],num,ValueNum);
            //参数特征标识
            unsigned char Flag = recvBuff[14];
            printf("Flag: %02x, (Flag&0x80)>>7 : %d \n",Flag,(Flag&0x80)>>7);
            if(!(Flag&0x80)>>7){emit UpdataDebugEdit(QString("Perset IDFlag not PersetFalg(D7!=1)"));return false;}
            //信息体地址
            int count = VSQ&0x7f;
            if(status != (VSQ&0x80)>>7){emit UpdataDebugEdit(QString("Perset RecvVSQ not SendVSQ"));return false;}
            if((VSQ&0x80)>>7 == 1)
            {
                printf("\nVSQ = %02x\n",VSQ);
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                obj_addr[0] = recvBuff[15];
                obj_addr[1] = recvBuff[16];
                obj_addr[2] = recvBuff[17];
                int obj_num = 0;
                obj_num = obj_addr[1];
                obj_num = (obj_num<<8) + obj_addr[0];
                int j = 0;
                for(int i=0;i<count;i++)
                {
                    unsigned char Tag = recvBuff[18+j];
                    int datalen = recvBuff[19+j];
                    unsigned char* Udata;
                    Udata = new unsigned char[datalen];
                    my_strncpy(Udata,recvBuff+20+j,datalen);
                    j = j+2+datalen;
                    char *data;
                    hex_to_char(&data,(char*)Udata,datalen);
                    emit RemoteValueControl(4,num,ValueMIN,ValueMAX,Flag,QString::number(obj_num++,16),Tag,QString(data));
                }
            }else if((VSQ&0x80)>>7 == 0)
            {
                printf("\nVSQ = %02x\n",VSQ);
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                int j = 0;
                for(int i=0;i<count;i++)
                {
                    obj_addr[0] = recvBuff[15+j];
                    obj_addr[1] = recvBuff[16+j];
                    obj_addr[2] = recvBuff[17+j];
                    printf("obj_addr = %02x %02x %02x\n",obj_addr[0],obj_addr[1],obj_addr[2]);
                    unsigned char Tag = recvBuff[18+j];
                    int datalen = recvBuff[19+j];
                    unsigned char* Udata;
                    Udata = new unsigned char[datalen];
                    my_strncpy(Udata,recvBuff+20+j,datalen);
                    j = j+2+datalen+3;
                    char *data;
                    hex_to_char(&data,(char*)Udata,datalen);
                    int obj_num = 0;
                    obj_num = obj_addr[1];
                    obj_num = (obj_num<<8) + obj_addr[0];
                    emit RemoteValueControl(4,num,ValueMIN,ValueMAX,Flag,QString::number(obj_num,16),Tag,QString(data));
                }
            }

            if((Flag&0x01) == 0x00)
            {
                cout << "Perset success!!! no follow-up" << endl;
                emit UpdataDebugEdit(QString("Perset success!!! no follow-up "));
                SetIsPerset(true);
                ACK++;
                SFrameSession();
                return sure;
            }
            else
            {
                cout << "Perset success!!! hava follow-up" << endl;
                emit UpdataDebugEdit(QString("Perset success!!! hava follow-up "));
                ACK++;
                SFrameSession();
            }
        }
    }
    cout << "Perset Recv fail!!!" << endl;
    emit UpdataDebugEdit(QString("Perset Recv fail"));
    return sure;
}

bool Session::PersetCancelSession(int ValueNum)
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetPersetCancel(ValueNum) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
//    show_hex((char*)SendFrame->GetAPDU(),*(SendFrame->GetAPDU()+1));
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::PersetCancelSessionSuccess(int ValueNum)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    while(sure)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            /*
            //68 0d 00 00 00 00
            //CB 00
            //09 00
            //01 00
            //08 00         -->定值区号
            //40            -->特征标识
            */
            //68 0d 00 00 00 00 cb 00 09 00 01 00 08 00 40
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
//            cout << "Frame: ";
//            show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);
//            cout << endl;
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x09,0x00};//传送原因 COT = 7
            unsigned char T1 = C_WS_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = 0x00;
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr);

            unsigned char SN[2] = {0};
            SN[0] = recvBuff[12];
            SN[1] = recvBuff[13];
            int num =SN[1];
            num = (num<<7) + SN[0];
            printf("SN[1]=%d SN[0]=%d ,ValueNum: %d ,num: %d\n",SN[0],SN[1],ValueNum,num);
            if(num!=ValueNum)
            {
                cout << "PersetCancel RecvValueNum != SendValueNum!!!" << endl;
                emit UpdataDebugEdit(QString("PersetCancel RecvValueNum != SendValueNum!!!"));

            }

            unsigned char Flag = recvBuff[14];
            if(Flag&0x1)
            {
                if(((Flag&0x80)>>7)||((Flag&0x40)>>6))
                {
                    cout << "PersetCancel RecvFlag not right!!!follow-up!!!" << endl;
                    emit UpdataDebugEdit(QString("PersetCancel RecvFlag not right!!!follow-up!!!"));
                    emit RemoteValueControl(5,num,ValueMIN,ValueMAX,Flag,QString(""),'-',QString("取消预置失败"));
                    return false;
                }
                else
                {
                    cout << "PersetCancel seccuss !!!,hava follow-up" << endl;
                    emit UpdataDebugEdit(QString("PersetCancel seccuss !!!,hava follow-up"));
                    emit RemoteValueControl(5,num,ValueMIN,ValueMAX,Flag,QString(""),'-',QString("取消预置成功"));
                    ACK++;
                    SFrameSession();
                }
            }
            else
            {
                if(((Flag&0x80)>>7)!=0&&((Flag&0x40)>>6)!=1)
                {
                    cout << "PersetCancel RecvFlag not right!!!" << endl;
                    emit UpdataDebugEdit(QString("PersetCancel RecvFlag not right!!!"));
                    emit RemoteValueControl(5,num,ValueMIN,ValueMAX,Flag,QString(""),'-',QString("取消预置失败"));
                    return false;
                }
                else
                {
                    cout << "PersetCancel seccuss !!!,hava follow-up" << endl;
                    emit UpdataDebugEdit(QString("PersetCancel seccuss !!!,hava follow-up"));
                    SetIsPerset(false);
                    emit RemoteValueControl(5,num,ValueMIN,ValueMAX,Flag,QString(""),'-',QString("取消预置成功"));
                    return true;
                }
            }


        }
    }
    cout << " PersetCancel Recv fail!!!! " << endl;
    emit UpdataDebugEdit(QString(" PersetCancel Recv fail!!!! "));
    cout << "************************************" << endl;
    return sure;
}

bool Session::CuringSession(int ValueNum)
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetCuring(ValueNum) << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
//    show_hex((char*)SendFrame->GetAPDU(),*(SendFrame->GetAPDU()+1));
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::CuringSessionSuccess(int ValueNum)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    while(sure)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            /*
            //68 0d 00 00 00 00
            //CB 00
            //07 00
            //01 00
            //08 00         -->定值区号
            //00            -->特征标识
            */
            //68 0d 00 00 00 00 cb 00 07 00 01 00 08 00 00
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
//            cout << "Frame: ";
//            show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);
//            cout << endl;
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char T1 = C_WS_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = 0x00;
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr);

            unsigned char SN[2] = {0};
            SN[0] = recvBuff[12];
            SN[1] = recvBuff[13];
            int num =SN[1];
            num = (num<<7) + SN[0];
            printf("SN[1]=%d SN[0]=%d ,ValueNum: %d ,num: %d\n",SN[0],SN[1],ValueNum,num);
            if(num!=ValueNum)
            {
                cout << "Curing RecvValueNum != SendValueNum!!!" << endl;
                emit UpdataDebugEdit(QString("Curing RecvValueNum != SendValueNum!!!"));
            }

            unsigned char Flag = recvBuff[14];
            if(Flag&0x1)
            {
                if(((Flag&0x80)>>7)||((Flag&0x40)>>6))
                {
                    cout << "Curing RecvFlag not right!!!follow-up!!!" << endl;
                    emit UpdataDebugEdit(QString("Curing RecvFlag not right!!!follow-up!!!"));
                    emit RemoteValueControl(6,num,ValueMIN,ValueMAX,Flag,QString(""),'-',QString("参数固化失败"));
                    return false;
                }
                else
                {
                    cout << "Curing seccuss !!!,hava follow-up" << endl;
                    emit UpdataDebugEdit(QString("Curing seccuss !!!,hava follow-up"));
                    emit RemoteValueControl(6,num,ValueMIN,ValueMAX,Flag,QString(""),'-',QString("参数固化成功"));
                }
            }
            else
            {
                if(((Flag&0x80)>>7)||((Flag&0x40)>>6))
                {
                    cout << "Curing RecvFlag not right!!!" << endl;
                    emit UpdataDebugEdit(QString("Curing RecvFlag not right!!!"));
                    emit RemoteValueControl(6,num,ValueMIN,ValueMAX,Flag,QString(""),'-',QString("参数固化失败"));
                    return false;
                }
                else
                {
                    cout << "Curing seccuss !!!,not follow-up" << endl;
                    emit UpdataDebugEdit(QString("Curing seccuss !!!,not follow-up"));
                    SetIsPerset(false);
                    emit RemoteValueControl(6,num,ValueMIN,ValueMAX,Flag,QString(""),'-',QString("参数固化成功"));
                    return true;
                }
            }


        }
    }
    cout << " Curing Recv fail!!!! " << endl;
    emit UpdataDebugEdit(QString(" Curing Recv fail!!!! "));
    cout << "************************************" << endl;
    return sure;
}

bool Session::EnergyCallSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetEnergyCall() << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
//    show_hex((char*)SendFrame->GetAPDU(),*(SendFrame->GetAPDU()+1));
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::EnergyCallSessionSuccess()
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    int count = -1;
    bool Confirm = false;
    while(sure)
    {
        cout << endl;
        //消息接收
        //int n = recv(client->get_socket(),(char*)recvBuff,BUF_LEN,0);
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();
        int datalen = 0;

        if(n < 0){break;  }
        if(n>1)
        {
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            //68 0e 00 00 00 00 65 01 07 00 01 00 00 00 00 05  ->  激活确认
            updata_recv_edit(1,recvBuff);
            if(count < 0)
            {
                unsigned char C[4] = {0x00,0x00,0x00,0x00};
                my_strncpy(C,recvBuff+2,4);
                unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
                unsigned char ASDU_addr[2] = {0x01,0x00};
                unsigned char obj_addr[3] = {0x00,0x00,0x00};
                //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QCC = 0x05
                unsigned char len = recvBuff[1];
                unsigned char T1 = 0x65;
                unsigned char VSQ = 0x01;
                unsigned char QCC = 0x05;
                sure = EncodeBuff(recvBuff,len,C,T1,VSQ,COT,ASDU_addr,obj_addr);
                if(recvBuff[len+1] != QCC){sure = false;cout << "QCC is error" << endl;return sure;}
                Confirm = true;
                if(sure && Confirm)
                {
                    count ++;
                    cout << "EnergyCall zero package, count = "<< count << ";" << endl;
                    cout << "EnergyCall start success!!!!" << endl;
                    emit UpdataDebugEdit(QString("EnergyCall start success!!!!"));
                    ACK++;
                    SFrameSession();
                }
            }
            else if(count >= 0)
            {
                /*
                68 37 00 00 00 00
                cf 03 25 00 01 00
                10 64 00 00 00 60 40 00 11 64 00 00 00 00 00
                00 12 64 00 00 03 40 00 00 13 64 00 00 00 00
                00 00 14 00 00 00 40 00 00 00 15 64 00 00 00
                */
                //68 37 00 00 00 00 cf 03 25 00 01 00 10 64 00 00 00 60 40 00 11 64 00 00 00 00 00 00 12 64 00 00 03 40 00 00 13 64 00 00 00 00 00 00 14 00 00 00 40 00 00 00 15 64 00 00 00
                unsigned char C[4] = {0x00,0x00,0x00,0x00};
                my_strncpy(C,recvBuff+2,4);
                unsigned char T1 = recvBuff[6];
                if(T1 != 0x65)  // 如果不是召唤结束帧
                {
                    if(T1 == 0xce)          //<206>∶＝累计量，短浮点数
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 5;

                    }
                    else if(T1 == 0xcf)     //<207>∶＝带 CP56Time2a 时标的累计量，短浮点数
                    {
                        printf("T1 = %02x,recvBuff[6] = %02x\n",T1,recvBuff[6]);
                        datalen = 12;
                    }
                    else
                    {
                        emit UpdataDebugEdit(QString("package data of T1 error!!!!"));
                        return false;
                    }
                    unsigned char VSQ = recvBuff[7];
                    unsigned char COT[2] = {0x25,0x00};//传送原因 COT = 37
                    unsigned char ASDU_addr[2] = {0x01,0x00};
                    union ADDR
                    {
                        unsigned char arr[3];
                        int addr;
                    }obj_addr;
                    my_strncpy(obj_addr.arr,recvBuff+12,3);
                    //标识类型 TI = 0x65 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x05
                    unsigned char len = recvBuff[1];
                    printf("len = %02x,recvBuff[1] =%02x\n",len,recvBuff[1]);
                    sure = EncodeBuff(recvBuff,len,C,T1,VSQ,COT,ASDU_addr,obj_addr.arr);
                    if(sure==true)
                    {
                        unsigned char data[12];
                        printf("VSQ: %02x,SQ: %02x,CON: %d\n",VSQ,VSQ>>7,VSQ&(0x7f));
                        if(((VSQ&0x80)>>7) == 0x01)    //如果是顺序
                        {
                            cout<< endl << "*********ShunXu********" << endl;
                            for(int i = 0;i<(VSQ&(0x7f));i++)
                            {
                                printf("obj_addr.addr: %x\n",obj_addr.addr);
                                printf("obj_addr.arr: %02x %02x %02x\n",obj_addr.arr[0],obj_addr.arr[1],obj_addr.arr[2]);

                                my_strncpy(data,recvBuff+15+i*datalen,datalen);
                                EnergyCallData(count,T1,obj_addr.arr,data);
                                obj_addr.addr++;
                            }
                        }
                        else            //如果是离散
                        {
                            for(int i = 0;i<(VSQ&(0x7f));i++)
                            {
                                my_strncpy(obj_addr.arr,recvBuff+12+i*(datalen+3),3);
                                my_strncpy(data,recvBuff+15+i*(datalen+3),datalen);
                                EnergyCallData(count,T1,obj_addr.arr,data);
                            }

                        }
                        count ++;
                        cout << "EnergyCall recv "<< count-1 <<" package, count = "<< count << ";" << endl;
                        ACK++;
                        SFrameSession();
                    }
                }
                else  // 如果是召唤结束帧
                {
                     unsigned char C[4] = {0x00,0x00,0x00,0x00};
                     my_strncpy(C,recvBuff+2,4);
                     unsigned char COT[2] = {0x0a,0x00};//传送原因 COT = 10
                     unsigned char ASDU_addr[2] = {0x01,0x00};
                     unsigned char obj_addr[3] = {0x00,0x00,0x00};
                     //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x05
                     unsigned char len = recvBuff[1];
                     sure = EncodeBuff(recvBuff,len,C,T1,0x01,COT,ASDU_addr,obj_addr);
                     if(recvBuff[len+1] != 0x05){sure = false;cout << "QCC is error" << endl;return sure;}
                     if(sure)
                     {
                         cout << "EnergyCall end" << endl;
                         emit UpdataDebugEdit(QString("EnergyCall End!!!!"));
                         ACK++;
                         SFrameSession();
                         cout << "************************************" << endl;
                         return sure;
                     }
                }
            }
        }
        cout << endl;
    }
    cout << "recv error!!!"<< "package error: " <<count << endl;
    emit UpdataDebugEdit(QString("EnergyCall Recv Error!!!!!!!"));
    cout << "************************************" << endl;
    return true;
}

bool Session::EnergyCallData(int packid, unsigned char T1, unsigned char *obj_addr, unsigned char *data)
{
    QString Type;
    QString Addr;
    QString Data;
    QString QDS;
    QString Time = QString("");

    if(T1 == 0xce)          //<206>∶＝累计量，短浮点数
    {
//        show_hex((char*)obj_addr,3);
//        show_hex((char*)data,1);


        unsigned char ObjAddr[3];
        char *addr;
        my_strncpy(ObjAddr,obj_addr,3);
        unsigned char tmpa = ObjAddr[0];
        ObjAddr[0] = ObjAddr[1];
        ObjAddr[1] = tmpa;
        hex_to_char(&addr,(char*)ObjAddr,3);
        char tmp[5];
        tmp[0] = addr[0];
        tmp[1] = addr[1];
        tmp[2] = addr[2];
        tmp[3] = addr[3];
        tmp[4] = '\0';


        char *data1;
        //printf("%02x %02x %02x %02x",data[0],data[1],data[2],data[3]);
        union FLOAT
        {
            float a;
            unsigned char b[4];
        }f;
        my_strncpy(f.b,data,4);
        printf("value = %f \n" , f.a);
        hex_to_char(&data1,(char*)data,4);

        Type = QString("累计量(无时标),短浮点数");
        Addr = QString(tmp);
        Data = QString("%1(%2)").arg(QString::number(f.a)).arg(data1);
        QDS = QString("有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封),"
                              "单点遥信信息%5(0开1合)").arg(QString::number((unsigned char)(data[4]>>7)&0x1)
                                                ).arg(QString::number((unsigned char)(data[4]>>6)&0x1)
                                                      ).arg(QString::number((unsigned char)(data[4]>>5)&0x1)
                                                            ).arg(QString::number((unsigned char)(data[4]>>4)&0x1)
                                                                  ).arg(QString::number((unsigned char)data[4]&0x1));

    }
    else if(T1 == 0xcf)     //<207>∶＝带 CP56Time2a 时标的累计量，短浮点数
    {
//        cout << "-------------T=0xcf-----------------" << endl;
//        show_hex((char*)obj_addr,3);
//        show_hex((char*)data,4);
//        show_hex((char*)data+4,1);
        unsigned char ObjAddr[3];
        char *addr;
        my_strncpy(ObjAddr,obj_addr,3);
        unsigned char tmpa = ObjAddr[0];
        ObjAddr[0] = ObjAddr[1];
        ObjAddr[1] = tmpa;
        hex_to_char(&addr,(char*)ObjAddr,3);
        char tmp[5];
        tmp[0] = addr[0];
        tmp[1] = addr[1];
        tmp[2] = addr[2];
        tmp[3] = addr[3];
        tmp[4] = '\0';

//        printf("addr: %02x %02x %02x\n",obj_addr[0],obj_addr[1],obj_addr[2]);

        char *data1;
//        printf("data: %02x %02x %02x %02x\n",data[0],data[1],data[2],data[3]);
        union FLOAT
        {
            float a;
            unsigned char b[4];
        }f;
        my_strncpy(f.b,data,4);
//        printf("value = %02x %02x %02x %02x\n" , f.b[0],f.b[1],f.b[2],f.b[3]);
//        printf("value = %f \n" , f.a);
        hex_to_char(&data1,(char*)data,4);
        Type = QString("累计量(有时标),短浮点数");
        Addr = QString(tmp);
        Data = QString("%1(%2)").arg(QString::number(f.a)).arg(data1);
        QDS = QString("有效%1(0有1无),刷新%2(0刷1非),"
                              "取代%3(0未1取),封锁%4(0未1封),"
                              "溢出%5(0未1溢)").arg(QString::number((unsigned char)(data[4]>>7)&0x1)
                                                ).arg(QString::number((unsigned char)(data[4]>>6)&0x1)
                                                      ).arg(QString::number((unsigned char)(data[4]>>5)&0x1)
                                                            ).arg(QString::number((unsigned char)(data[4]>>4)&0x1)
                                                                  ).arg(QString::number((unsigned char)data[4]&0x1));
        //时标解析
        unsigned char time[7] = {0};
        my_strncpy(time,data+5,7);
        st_cp56time2a datatime;
        Uchar_to_cp56time2a(time,&datatime);
        Time = get_cp56time2a_string(&datatime);
//        cout << "--------------------------------------" << endl;
    }
    emit ShowDataEnergyCall(packid,Type,Addr,Data,QDS,Time);
    return true;
}

bool Session::UpdataVersionSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetUpdataStart() << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
//    show_hex((char*)SendFrame->GetAPDU(),*(SendFrame->GetAPDU()+1));
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::UpdataVersionTransferSession(QString VersionFile)
{
    UpdataStop = false;
    char VersionFileName[100];
    my_strncpy(VersionFileName,VersionFile.toUtf8().data(),100);

    Frame frame = WriteFileSession(VersionFileName,1);
    bool ret = WriteFileSessionEnable(frame,1);
    if(ret)
    {
        // 升级文件传输
        QFile  f(VersionFile);
        int filesize= f.size();
        int offset = 0;
        emit UpdataDebugEdit(QString("VersionFile Transfering ....."));
        emit VersionUpdata(2,VersionFile,1);
        UpdataStop = false;
        while(filesize-offset&&(!UpdataStop))
        {
            SendFrame->SetWriteData(VersionFile,offset);
//            cout << filesize << endl;
//            cout << offset << endl;
            SendFrame->updataCON(V_S,ACK);
            updata_recv_edit(0,SendFrame->GetAPDU());
            Sleep(100);
            this->FrameSend(*SendFrame);
            emit UpdataValue(offset);
            V_S++;
        }
        if(UpdataStop)
        {
            //发送中断时的帧
            emit UpdataDebugEdit(QString("VersionFile Transfer Stop!!!"));
            emit UpdataLabelEnable(3);
            emit VersionUpdata(2,VersionFile,2);
            V_S++;
            return false;
        }
        emit UpdataDebugEdit(QString("VersionFile Transfer End!!!"));
        emit VersionUpdata(2,VersionFile,3);
        if(WriteFileSessionConfirm(frame,offset,1))
        {
            emit VersionUpdata(2,VersionFile,4);
            return true;
        }
        else
        {
            emit VersionUpdata(2,VersionFile,5);
            return false;
        }
    }
    return true;
}

bool Session::UpdataVersionSessionSuccess(QString VersionFile)
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    StartLevelUp = false;
    while(sure)
    {
        cout << endl;
        //消息接收
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            /*
            //68 0e 00 00 00 00
            //d3 00
            //07 00
            //01 00
            //00 00 00      -->信息体地址
            //80            -->命令类型
            */
            //68 0e 00 00 00 00 d3 00 07 00 01 00 00 00 00 80
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
//            cout << "Frame: ";
//            show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);
//            cout << endl;
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char T1 = F_SR_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = 0x00;
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr);

            unsigned char CTYPE;
            CTYPE = recvBuff[15];
            printf("CTYPE=%02x recvBuff[15]=%02x\n",CTYPE,recvBuff[15]);
            if(CTYPE!=0x80)
            {
                cout << "CTYPE is Error!!!" << endl;
                emit UpdataDebugEdit(QString("CTYPE is Error!!!"));
                emit VersionUpdata(1,VersionFile,0);
                return false;
            }

            if(sure)
            {
                cout << "Updata Start Success!!!" << endl;
                emit UpdataDebugEdit(QString("Updata Start Success!!!"));
                emit VersionUpdata(1,VersionFile,1);
                StartLevelUp = true;
                ACK++;
                SFrameSession();
            }
            else
            {
                cout << "Updata Start Fail!!!" << endl;
                emit UpdataDebugEdit(QString("Updata Start Fail!!!"));
                emit VersionUpdata(1,VersionFile,0);
                ACK++;
                SFrameSession();
                return sure;
            }

            bool over = false;
            if(StartLevelUp)   //如果启动成功
            {
                emit VersionUpdata(2,VersionFile,0);
                over = UpdataVersionTransferSession(VersionFile);
            }

            if(over)
            {
                emit UpdataDebugEdit(QString("VersionFile Transfering success!!!"));
                emit VersionUpdata(2,VersionFile,6);
                VersionFileName = VersionFile;
                return true;
            }
            else
            {
                emit UpdataDebugEdit(QString("VersionFile Transfering fail!!!"));
                emit VersionUpdata(2,VersionFile,7);
                return false;
            }

        }
    }
    cout << " Updata Start Recv Fail!!!! " << endl;
    emit UpdataDebugEdit(QString(" Updata Start Recv Fail!!!! "));
    cout << "************************************" << endl;
    return sure;
}

bool Session::RunUpdataSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetRunUpdata() << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
//    show_hex((char*)SendFrame->GetAPDU(),*(SendFrame->GetAPDU()+1));
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::RunUpdataSessionSuccess()
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    while(sure)
    {
        cout << endl;
        //消息接收
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            /*
            //68 0e 00 00 00 00
            //d3 00
            //07 00
            //01 00
            //00 00 00      -->信息体地址
            //80            -->命令类型
            */
            //68 0e 00 00 00 00 d3 00 07 00 01 00 00 00 00 00
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
//            cout << "Frame: ";
//            show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);
//            cout << endl;
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x07,0x00};//传送原因 COT = 7
            unsigned char T1 = F_SR_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = 0x00;
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr);

            unsigned char CTYPE;
            CTYPE = recvBuff[15];
            printf("CTYPE=%02x recvBuff[15]=%02x\n",CTYPE,recvBuff[15]);
            if(CTYPE!=0x00)
            {
                cout << "CTYPE is Error!!!" << endl;
                emit UpdataDebugEdit(QString("CTYPE is Error!!!"));
                emit VersionUpdata(3,VersionFileName,0);
                ACK++;
                SFrameSession();
                return false;
            }

            if(sure)
            {
                cout << "Updata Run Success!!!" << endl;
                emit UpdataDebugEdit(QString("Updata Run Success!!!"));
                emit VersionUpdata(3,VersionFileName,1);
                ACK++;
                SFrameSession();
                return true;
            }
            else
            {
                cout << "Updata Run Fail!!!" << endl;
                emit UpdataDebugEdit(QString("Updata Run Fail!!!"));
                emit VersionUpdata(3,VersionFileName,0);
                ACK++;
                SFrameSession();
                return sure;
            }
        }
    }
    cout << " Updata Run Recv Fail!!!! " << endl;
    emit UpdataDebugEdit(QString(" Updata Run Recv Fail!!!! "));
    cout << "************************************" << endl;
    return sure;
}

bool Session::AbandonUpdataSession()
{
    cout << "-----------"<< __FUNCTION__ <<"-----------------" << endl;
    cout << "ACK: " << ACK << " V_R: " << V_R << " V_S: " << V_S << endl;
    SendFrame = new Frame('I');
    cout << "is_setFrame: " << SendFrame->SetAbandonUpdata() << endl;
    SendFrame->updataCON(V_S,ACK);
    this->FrameSend(*SendFrame);
    updata_recv_edit(0,SendFrame->GetAPDU());
//    show_hex((char*)SendFrame->GetAPDU(),*(SendFrame->GetAPDU()+1));
    cout << "---------------------------------" << endl;
    V_S++;
    return true;
}

bool Session::AbandonUpdataSessionSuccess()
{
    cout << "******************" <<__FUNCTION__ << "******************" << endl;
    unsigned char recvBuff[BUF_LEN];
    bool sure = true;
    while(sure)
    {
        cout << endl;
        //消息接收
//        int n = FrameRecv(recvBuff);

        Info Recv;
        int n = -3;
//        I_InfoBuffer->wait(Recv);
        Recv = I_TakeInfo(n);
        my_strncpy(recvBuff,Recv.getInfo(),Recv.getLen());
//        n = Recv.getLen();

        if(n < 0){break;}
        if(n>1)
        {
            /*
            //68 0e 00 00 00 00
            //d3 00
            //07 00
            //01 00
            //00 00 00      -->信息体地址
            //80            -->命令类型
            */
            //68 0e 00 00 00 00 d3 00 09 00 01 00 00 00 00 00
            int len = recvBuff[1]+2;
            cout<<"recv:";
            show_hex((char*)recvBuff,len);
            updata_recv_edit(1,recvBuff);
//            cout << "Frame: ";
//            show_hex((char*)(GetSendFrame().GetAPDU()),GetSendFrame().GetAPDU()[1]+2);
//            cout << endl;
            unsigned char C[4] = {0x00,0x00,0x00,0x00};
            my_strncpy(C,recvBuff+2,4);
            unsigned char COT[2] = {0x09,0x00};//传送原因 COT = 7
            unsigned char T1 = F_SR_NA_1;
            unsigned char ASDU_addr[2] = {0x01,0x00};
            //标识类型 TI = 0x64 可变帧长限定词 VSQ  = 0x01 召唤限定词 QOI = 0x14
            unsigned char VSQ = 0x00;
            unsigned char size = recvBuff[1];
            sure = EncodeBuff(recvBuff,size,C,T1,VSQ,COT,ASDU_addr);

            unsigned char CTYPE;
            CTYPE = recvBuff[15];
            printf("CTYPE=%02x recvBuff[15]=%02x\n",CTYPE,recvBuff[15]);
            if(CTYPE!=0x00)
            {
                cout << "CTYPE is Error!!!" << endl;
                emit UpdataDebugEdit(QString("CTYPE is Error!!!"));
                emit VersionUpdata(4,VersionFileName,0);
                ACK++;
                SFrameSession();
                return false;
            }

            if(sure)
            {
                cout << "Updata Abandon Success!!!" << endl;
                emit UpdataDebugEdit(QString("Updata Abandon Success!!!"));
                emit VersionUpdata(4,VersionFileName,1);
                ACK++;
                SFrameSession();
                return true;
            }
            else
            {
                cout << "Updata Abandon Fail!!!" << endl;
                emit UpdataDebugEdit(QString("Updata Abandon Fail!!!"));
                emit VersionUpdata(4,VersionFileName,0);
                ACK++;
                SFrameSession();
                return sure;
            }
        }
    }
    cout << " Updata Abandon Recv Fail!!!! " << endl;
    emit UpdataDebugEdit(QString(" Updata Abandon Recv Fail!!!! "));
    cout << "************************************" << endl;
    return sure;
}

bool Session::ReportDataSession()
{
    Info Recv_Info;
    while(SessionTerminal->IsConnect())
    {
        Report_InfoBuffer->wait(Recv_Info);
        unsigned char *messages;
        messages = new unsigned char[BUF_LEN];
        memset(messages,0,256);
        my_strncpy(messages,Recv_Info.getInfo(),Recv_Info.getLen());
        cout << "show Report Info" << endl;
        show_hex((char*)messages,messages[1]+2);

        unsigned char T1 = messages[6];
        char* data;
        hex_to_char(&data,(char*)messages);
        int type = -1;
        if(T1 == M_SP_NA_1 || T1 == M_DP_NA_1 || T1 == M_ME_TE_1 || T1 == M_ME_TF_1)
        {
            type = 1;
        }
        else if(T1 == M_ME_NA_1 || T1 == M_ME_NB_1 || T1 == M_ME_NC_1)
        {
            type = 2;
        }
        else if(T1 == M_FT_NA_1)
        {
            type = 3;
        }
        else if(T1 == M_IT_NB_1)
        {
            type = 4;
        }
        else if(T1 == M_IT_TC_1)
        {
            type = 5;
        }

        Report(type,QString("U"),QString(data));
    }
    return false;
}

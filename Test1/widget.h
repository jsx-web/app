#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <qbuttongroup.h> 
#include <QThread>
#include <QtConcurrent>
#include <QFuture>

#include "client.h"
#include "frame.h"
#include "session.h"
#include "readfiledialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

//friend class Client;

private:
    Ui::Widget *ui;
    ReadFileDialog *readDialog;
public:
    QButtonGroup *C_S_check;
    Client *client;
    int isconnect;
    Session *session;
    QThread* thread;

public:
    void recvEidt_updata();
    void on_Btn_Read_my_clicked(char* DirName = "iec104");

private slots:
    void on_Btn_Connect_clicked();
    void on_Btn_send_clicked();
    void on_Btn_Close_clicked();
    void on_Btn_Recv_Clear_clicked();
    void on_Btn_Debug_Clear_clicked();
    void on_Btn_TotalCall_clicked();
    void on_Btn_Init_clicked();
    void on_Btn_ClockSyn_clicked();
    void on_Btn_ClockRead_clicked();
    void on_Btn_Reset_clicked();
    void on_Btn_DirCall_clicked();
    void on_Btn_Read_clicked();
    void EnablSuccess_res(bool ret,QString name);
signals:
    void WidgetReadFile(bool end, char* packe_data);
};

#endif // WIDGET_H

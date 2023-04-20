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
public:
    QButtonGroup *C_S_check;
    Client *client;
    int isconnect;
    Session *session;

public:
    void recvEidt_updata();


private slots:
    void on_Btn_Connect_clicked();
    void on_Btn_send_clicked();
    void on_Btn_Close_clicked();
    void on_Btn_Recv_Clear_clicked();
    void on_Btn_Debug_Clear_clicked();
    void on_Btn_TotalCall_clicked();
};

#endif // WIDGET_H

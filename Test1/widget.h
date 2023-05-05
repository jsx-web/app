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
#include <QTableView>
#include <QStandardItemModel>
#include <QPropertyAnimation>
#include <QMenu>
#include <QAction>

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
friend class Session;

private:
    Ui::Widget *ui;
    ReadFileDialog *readDialog;
    bool m_bSideflag;
    QPropertyAnimation *m_propertyAnimation;
    QButtonGroup *C_S_check;
    Client *client;
    int isconnect;
    Session *session;
    QThread* thread;
    QStandardItemModel* model;

//---------实现右键菜单------
private:
     QMenu *popMenu; //菜单
     QAction *actionShowAll;
     QAction *actionEncode;
     QAction *actionTag;
public:
    void MenuInit();
private slots:
    //右键菜单响应函数
    void slotContextMenu(QPoint pos);
    //显示全部
    void slotactionShowAll();
    //解析报文
    void slotactionEncode();
    //标记此行
    void slotactionTag();
//-------------------------

public:
    void on_Btn_Read_my_clicked(char* DirName = (char*)"iec104");
    void recvEidt_updata(char *data);
    void TablaViewInit();

private slots:
    void on_Btn_Connect_clicked();
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
    void on_Btn_FileTransfer_clicked();

signals:
    void WidgetReadFile(bool end, char* packe_data);
};

#endif // WIDGET_H

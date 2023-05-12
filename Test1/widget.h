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
#include "dialog_showall.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QPropertyAnimation>
#include <QMenu>
#include <QAction>
#include <QMovie>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

friend class Session;

private:
    Ui::Widget *ui;
    Dialog_ShowAll *dialog_showall;
    bool m_bSideflag;
    static int m_bSideflagcount;
    QPropertyAnimation *m_propertyAnimation;
    QButtonGroup *C_S_check;
    Client *client;
    int isconnect;
    Session *session;
    QStandardItemModel* model;
    QStandardItemModel* dir_model;
    bool colorfalage;
    QMovie *movie;

private slots:
    void slotconTimeout();

//---------实现右键菜单------
private:
     QMenu *popMenu; //菜单
     QAction *actionShowAll;
     QAction *actionEncode;
     QAction *actionTag;
signals:
    void ShowAll(QString send, QString type, QString data);
private slots:
    //右键菜单响应函数
    void slotContextMenu(QPoint pos);
    //显示全部
    void slotactionShowAll();
    //解析报文
    void slotactionEncode();
    //标记此行
    void slotactionTag();

private slots:
    //------测试帧响应判定---------
    void slotTestConfirm();
    void slotTestConfirmTimeOut();

private slots:
    //-----------接收框显示--------
    void slotappendData(int author,char type,QString data);
    //----------召唤目录显示--------
    void slotshowDirname(char* name,int n);

private slots:
    //建立连接槽函数
    void slotConnectRet(int status,QString IP,qint16 Port);
private slots:
    //读文件进度条最大值设置
    void slotReadPorgressMax(int Max);
    //读文件Value值更新
    void slotReadValue(int Value);
    //写文件进度条最大值设置
    void slotWritePorgressMax(int Max);
    //写文件Value值更新
    void slotWriteValue(int Value);
    //读文件标签
    void slotReadLabel(int status);
    //写文件标签
    void slotWriteLabel(int status);
    //更新调试框
    void slotUpdataDebugEidt(QString msg);

public:
    void recvEidt_updata(char *data);
    void TablaViewInit();
    void ProgressBarInit();
    void WritePathInit();
    void MenuInit();
    void IconInit();
    void BtnEnable();
    void SideWidgetEnable();
private slots:
    void on_Btn_sideWidget_clicked();
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
    void on_Btn_FileTransfer_clicked();
    void on_Btn_DirSelect_clicked();
    void on_Btn_Write_clicked();
    void on_Btn_SelectFile_clicked();
    void on_Btn_RemoteControl_clicked();
    void on_Btn_ElectricalEnergyCall_clicked();
    void on_Btn_VersionUpdata_clicked();

signals:
    void WidgetReadFile(bool end, char* packe_data);
};

#endif // WIDGET_H

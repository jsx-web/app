#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QButtonGroup>
#include <QThread>
#include <QtConcurrent>
#include <QFuture>
#include <QTableView>
#include <QStandardItemModel>
#include <QPropertyAnimation>
#include <QMenu>
#include <QAction>
#include <QMovie>
#include <QMessageBox>

#include "session.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QButtonGroup *C_S_check;
    QMovie *movie;
    QStandardItemModel* recvmodel;
    QStandardItemModel* initmodel;
    QStandardItemModel* totalcallmodel;

private:
    void Init();
    void TableVeiwInit();
    void IconInit();
signals:

private slots:
    //---------建立连接槽函数------
    void slotConnectRet(int status,QString IP,qint16 Port);
    //-----------接收框显示--------
    void slotappendData(int author,char type,QString data);
    //-----------更新调试框--------
    void slotUpdataDebugEidt(QString msg);
    //--------根据model写入数据----
    void slotUpdataTableView(int modelID,int modellen,QStringList data);
    //---------总召数据显示--------
    void slotShowDataTotalCall(int packid,QString T1,QString obj_addr,QString data,QString QDS,QStringList TimeList);
    //--------时钟控制的显示------
    void slotColckSyn(unsigned char T1,unsigned char*Time,int len);


private slots:
    void on_Btn_Connect_clicked();

    void on_Btn_Close_clicked();

    void on_Btn_Init_clicked();

    void on_Btn_TotalCall_clicked();

    void on_Btn_ClockSyn_clicked();

private:
    Ui::MainWindow *ui;
    Session *session;




};

#endif // MAINWINDOW_H

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
    QStandardItemModel* recvmodel;
    QMovie *movie;
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

private slots:
    void on_Btn_Connect_clicked();

    void on_Btn_Close_clicked();

    void on_Btn_Init_clicked();

private:
    Ui::MainWindow *ui;
    Session *session;




};

#endif // MAINWINDOW_H

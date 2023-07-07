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
#include <QTime>

#include "session.h"
#include "dialog_writevalueadd.h"




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
    QButtonGroup *Write_check;
    QPushButton *Btn_LogClear;
    QMovie *movie;
    QStandardItemModel* recvmodel;
    QMenu *popMenu;
    QAction *action1;
    QAction *action2;
    QAction *action3;
    QStandardItemModel* testmodel;
    QStandardItemModel* initmodel;
    QStandardItemModel* totalcallmodel;
    QStandardItemModel* colckmodel;
    QStandardItemModel* resetmodel;
    QStandardItemModel* dirmodel;
    QStandardItemModel* remotevaluemodel;
    QStringList list;
    QStringList defaultlist;
    QStandardItemModel* writevaluemodel;
    QMenu *popMenu1;
    QAction *action1_1;
    QStandardItemModel* energycallmodel;
    QStandardItemModel* versionupdatamodel;
    bool UpdataTransfer;
    QTimer *timer_calendar;//用来显示当前日期的定时器
    int Selected_Current_Row;
    QStandardItemModel *reportmodel;


private:
    void Init();
    void TableVeiwInit();
    void MenuInit();
    void IconInit();
    void ProgressBarInit();
    void TimeLableInit();
    void DefaultListInit();
    void VisableInit();
private slots:
    void timerUpdate();
signals:
    void UpdataStop();
private slots:
    //---------建立连接槽函数------
    void slotConnectRet(int status,QString IP,qint16 Port);
    //----------测试帧接收判定槽函数----
    void slotTestConfirm();
    void slotTestConfirmSuccess();
    void slotTestConfirmTimeOut();
    //-----------接收框显示--------
    void slotAppendData(int author,QString type,QString data);
    void slotAppendTest(int author,QString type,QString data);
    //----------接收框的右击菜单----
    void slotContextMenu(QPoint pos);
    void slotAction1();
    void slotAction2();
    void slotAction3();
    //-----------更新调试框--------
    void slotUpdataDebugEidt(QString msg);
    //--------根据model写入数据----
    void slotUpdataTableView(int modelID,int modellen,QStringList data);
    //---------总召数据显示--------
    void slotShowDataTotalCall(int packid,QString T1,QString obj_addr,QString data,QString QDS,QString TimeList);
    //--------时钟控制的显示------
    void slotColckControl(int operation, char sender, QString Time);
    //--------复位的显示------
    void slotResetShow(int status, QString decs);
    //-------文件目录的显示-------
    void slotShowDirname(char* name,int n);

    //---------读文件标签---------
    void slotReadLabel(int status);
    //----读文件进度条最大值设置-----
    void slotReadPorgressMax(int Max);
    //-------读文件Value值更新-----
    void slotReadValue(int Value);

    //-------写文件标签------------
    void slotWriteLabel(int status);
    //----写文件进度条最大值设置-----
    void slotWritePorgressMax(int Max);
    //-------写文件Value值更新-----
    void slotWriteValue(int Value);

    //-------远程参数读写--------
    void slotRemoteValueControl(int operation,int ValueNum,int ValueMin,int ValueMax,
                                unsigned char Flag,QString ObjAddr,unsigned char Tag,
                                QString Value);
    void RemoteValueEncode(unsigned char Tag, QString Value, int row);
    //-------接收写参数的接收-----
    void slotDialogWriteValue(QString addr ,QString tag, QString value);
    //----------接收写参数的右击菜单----
    void slotContextMenu1(QPoint pos);
    void slotAction1_1();
    //----------电能量召唤数据显示---------
    void slotShowDataEnergyCall(int packid,QString T1,QString obj_addr,QString data,QString QDS,QString TimeList);

    //-------软件升级文件标签------------
    void slotUpdataLabel(int status);
    //----软件升级文件进度条最大值设置-----
    void slotUpdataPorgressMax(int Max);
    //-------软件升级文件Value值更新-----
    void slotUpdataValue(int Value);
    //-------文件传输完成--------------
    void slotUpdataSuccess();
    //-----------软件升级显示------------
    void slotVersionUpdata(int operation,QString VersionFile,int status);
    //-----------读文件传输确认------------
    void slotReadConfirm(int status, unsigned char *FileID, unsigned char *FileOffset, unsigned char desc);
    //----------菜单导航栏----------------
    void slotActionExit();
    void slotActionHelp();
    //---------上报数据显示---------------
    void slotReport(int type,QString format,QString data);
private slots:
    void on_Btn_Connect_clicked();

    void on_Btn_Close_clicked();

    void on_Btn_Init_clicked();

    void on_Btn_TotalCall_clicked();

    void on_Btn_ClockSyn_clicked();

    void on_Btn_ClockRead_clicked();

    void on_Btn_Reset_clicked();

    void on_Btn_DirCall_clicked();

    void on_Btn_Read_clicked();

    void on_Btn_DirSelect_clicked();

    void on_Btn_Write_clicked();

    void on_Btn_SelectFile_clicked();

    void on_Btn_ReadValueNum_clicked();

    void on_Btn_SetValueNum_clicked();

    void on_Btn_InputObjAddr_clicked();

    void on_Btn_ObjAddrClear_clicked();

    void on_Btn_ReadValue_clicked();

    void on_Btn_ValueAdd_clicked();

    void on_Btn_ValueClear_clicked();

    void on_Btn_Preset_clicked();

    void on_Btn_PresetCancel_clicked();
    
    void on_Btn_Curing_clicked();

    void on_Btn_EnergyCall_clicked();

    void on_Btn_SelectVersionFile_clicked();

    void on_Btn_UpdataVersion_clicked();

    void on_Btn_UpdataStop_clicked();

    void on_Btn_RunUpdata_clicked();

    void on_Btn_AbandonUpdata_clicked();

    void on_Btn_Init_Clear_clicked();

    void on_Btn_TotalCall_Clear_clicked();

    void on_Btn_ClockRead_Clear_clicked();

    void on_Btn_Reset_Clear_clicked();

    void on_Btn_EnergyCall_Clear_clicked();

    void on_Btn_GetTime_clicked();

    void Btn_LogClear_clicked();

    void on_Btn_DirClear_clicked();

    void on_Btn_RemoteValue_clicked();

private:
    Ui::MainWindow *ui;
    Session *session;

};

#endif // MAINWINDOW_H

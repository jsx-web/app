#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    session = nullptr;
    Init();
}

MainWindow::~MainWindow()
{
    delete session;
    delete ui;
}

void MainWindow::Init()
{
    C_S_check = new QButtonGroup();
    C_S_check->addButton(ui->radioBtn_Server,0);
    C_S_check->addButton(ui->radioBtn_Client,1);
    Write_check = new QButtonGroup();
    Write_check->addButton(ui->radioButton_seq);
    Write_check->addButton(ui->radioButton_nseq);
    UpdataTransfer = false;

    ui->DateWidget->setCurrentIndex(0);
    ui->LogWidget->setCurrentIndex(1);

    IconInit();
    TableVeiwInit();
    MenuInit();
    ProgressBarInit();
    TimeLableInit();

    Btn_LogClear = new QPushButton();
    Btn_LogClear->setFlat(true);
    Btn_LogClear->setFixedSize(20,20);
    Btn_LogClear->setIcon(QIcon(":/res/Icon/clear.svg"));
    ui->LogWidget->setTabEnabled(0, false);
    ui->LogWidget->tabBar()->setTabButton(0, QTabBar::RightSide, Btn_LogClear);
    connect(Btn_LogClear,SIGNAL(clicked(bool)),this,SLOT(Btn_LogClear_clicked()));
    connect(ui->actionExit,SIGNAL(triggered(bool)),this,SLOT(slotActionExit()));
    connect(ui->actionHelp,SIGNAL(triggered(bool)),this,SLOT(slotActionHelp()));

}

void MainWindow::TableVeiwInit()
{
    //-----接收框初始化------
    recvmodel = new QStandardItemModel();
    ui->tableView_recv->setModel(recvmodel);
    QStringList table_h_headers;
    table_h_headers << "发送者" << "帧类型" << "帧数据";
    recvmodel->setHorizontalHeaderLabels(table_h_headers);
    // 显示网格线
    ui->tableView_recv->setShowGrid(true);
    // 线的样式
    ui->tableView_recv->setGridStyle(Qt::DotLine);
    ui->tableView_recv->setColumnWidth(0,100);
    ui->tableView_recv->setColumnWidth(1,100);
    //ui->tableView_recv->setColumnWidth(2,300);
    ui->tableView_recv->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    ui->tableView_recv->setContextMenuPolicy(Qt::CustomContextMenu); //可弹出右键菜单
    //-----测试链路接收框初始化------
    testmodel = new QStandardItemModel();
    ui->tableView_TestMsg->setModel(testmodel);
    testmodel->setHorizontalHeaderLabels(table_h_headers);
    // 显示网格线
    ui->tableView_TestMsg->setShowGrid(true);
    // 线的样式
    ui->tableView_TestMsg->setGridStyle(Qt::DotLine);
    ui->tableView_TestMsg->setColumnWidth(0,100);
    ui->tableView_TestMsg->setColumnWidth(1,100);
    ui->tableView_TestMsg->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //----------初始化页的tabView初始化----------
    initmodel = new QStandardItemModel();
    ui->tableView_Init->setModel(initmodel);
    QStringList tableint_h_headers;
    tableint_h_headers << "初始化状态" << "初始化类型" ;
    initmodel->setHorizontalHeaderLabels(tableint_h_headers);
    ui->tableView_Init->setShowGrid(true);
    ui->tableView_Init->setColumnWidth(0,100);
    ui->tableView_recv->setColumnWidth(1,300);
    //ui->tableView_recv->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //---------总召唤页的tabView初始化------------TotalCall
    totalcallmodel = new QStandardItemModel();
    ui->tableView_TotalCall->setModel(totalcallmodel);
    QStringList tableTC_h_headers;
    tableTC_h_headers << "数据类型" << "地址" << "数据内容" << "品质描述" << "时标";
    totalcallmodel->setHorizontalHeaderLabels(tableTC_h_headers);
    ui->tableView_TotalCall->setShowGrid(true);
    ui->tableView_TotalCall->setColumnWidth(0,110);
    ui->tableView_TotalCall->setColumnWidth(1,100);
    ui->tableView_TotalCall->setColumnWidth(2,120);
    ui->tableView_TotalCall->setColumnWidth(3,410);
    ui->tableView_TotalCall->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //----------时钟控制页的tabView初始化----------tableView_Colck
    colckmodel = new QStandardItemModel();
    ui->tableView_Colck->setModel(colckmodel);
    QStringList tableColck_h_headers;
    tableColck_h_headers << "操作" << "发送者" << "时标";
    colckmodel->setHorizontalHeaderLabels(tableColck_h_headers);
    ui->tableView_Colck->setShowGrid(true);
    ui->tableView_Colck->setColumnWidth(0,110);
    ui->tableView_Colck->setColumnWidth(1,100);
    ui->tableView_Colck->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //--------复位页的tabView的初始化-----------tableView_Reset
    resetmodel = new QStandardItemModel();
    ui->tableView_Reset->setModel(resetmodel);
    QStringList tableReset_h_headers;
    tableReset_h_headers << "复位状态" << "复位进程";
    resetmodel->setHorizontalHeaderLabels(tableReset_h_headers);
    ui->tableView_Reset->setShowGrid(true);
    ui->tableView_Reset->setColumnWidth(0,110);
    ui->tableView_Reset->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //--------文件传输的tabView的初始化----------tableView_Dir
    dirmodel = new QStandardItemModel();
    ui->tableView_Dir->setModel(dirmodel);
    QStringList tableDir_h_headers;
    tableDir_h_headers << "目录名";
    dirmodel->setHorizontalHeaderLabels(tableDir_h_headers);
    ui->tableView_Dir->setShowGrid(true);
    ui->tableView_Dir->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //---------远程参数读写tabView的初始化--------tableView_RemoteValue
    remotevaluemodel = new QStandardItemModel();
    ui->tableView_RemoteValue->setModel(remotevaluemodel);
    QStringList tableRemoteValue_h_headers;
    tableRemoteValue_h_headers << "操作" << "定值区号" << "最小区号" << "最大区号" << "参数特征标识" << "信息体地址" << "Tag 类型" << "值";
    remotevaluemodel->setHorizontalHeaderLabels(tableRemoteValue_h_headers);
    ui->tableView_RemoteValue->setShowGrid(true);
    ui->tableView_RemoteValue->setColumnWidth(0,120);
    ui->tableView_RemoteValue->setColumnWidth(1,60);
    ui->tableView_RemoteValue->setColumnWidth(2,60);
    ui->tableView_RemoteValue->setColumnWidth(3,60);
    ui->tableView_RemoteValue->setColumnWidth(4,100);
    ui->tableView_RemoteValue->setColumnWidth(5,80);
    ui->tableView_RemoteValue->setColumnWidth(6,80);
    ui->tableView_RemoteValue->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //------参数写tabView的初始化-------------tableView_WriteValue
    writevaluemodel = new QStandardItemModel();
    ui->tableView_WriteValue->setModel(writevaluemodel);
    QStringList tableWriteValue_h_headers;
    tableWriteValue_h_headers << "地址" << "Tag" << "值";
    writevaluemodel->setHorizontalHeaderLabels(tableWriteValue_h_headers);
    ui->tableView_WriteValue->setShowGrid(true);
    ui->tableView_WriteValue->setColumnWidth(0,65);
    ui->tableView_WriteValue->setColumnWidth(1,30);
    ui->tableView_WriteValue->setColumnWidth(2,50);
    ui->tableView_WriteValue->verticalHeader()->hide();//取消行号
    ui->tableView_WriteValue->setContextMenuPolicy(Qt::CustomContextMenu); //可弹出右键菜单
    //------电能量召唤tabView的初始化----------tableView_EnergyCall
    energycallmodel = new QStandardItemModel();
    ui->tableView_EnergyCall->setModel(energycallmodel);
    QStringList tableEnergyCall_h_headers;
    tableEnergyCall_h_headers << "数据类型" << "对象地址" << "值" << "品质描述符" << "消息体对象时标";
    energycallmodel->setHorizontalHeaderLabels(tableEnergyCall_h_headers);
    ui->tableView_EnergyCall->setShowGrid(true);
    ui->tableView_EnergyCall->setColumnWidth(0,140);
    ui->tableView_EnergyCall->setColumnWidth(1,60);
    ui->tableView_EnergyCall->setColumnWidth(2,160);
    ui->tableView_EnergyCall->setColumnWidth(3,160);
    ui->tableView_EnergyCall->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //-------软件升级召唤tableView------------tableView_VersionUpdata
    versionupdatamodel = new QStandardItemModel();
    ui->tableView_VersionUpdata->setModel(versionupdatamodel);
    QStringList tableVersionUpdata_h_headers;
    tableVersionUpdata_h_headers << "操作" << "升级文件" << "操作结果";
    versionupdatamodel->setHorizontalHeaderLabels(tableVersionUpdata_h_headers);
    ui->tableView_VersionUpdata->setShowGrid(true);
    ui->tableView_VersionUpdata->setColumnWidth(0,120);
    ui->tableView_VersionUpdata->setColumnWidth(1,280);
    ui->tableView_VersionUpdata->horizontalHeader()->setStretchLastSection(true); //最后一列填满表
    //-------上报数据tableView------------tableView_Report
    reportmodel = new QStandardItemModel();
    ui->tableView_Report->setModel(reportmodel);
    QStringList tableVersionReport_h_headers;
    tableVersionReport_h_headers << "上报类型" << "格式" << "帧数据";
    reportmodel->setHorizontalHeaderLabels(tableVersionReport_h_headers);
    ui->tableView_Report->setShowGrid(true);
    ui->tableView_Report->setColumnWidth(0,120);
    ui->tableView_Report->setColumnWidth(1,120);
    ui->tableView_Report->horizontalHeader()->setStretchLastSection(true); //最后一列填满表

}

void MainWindow::MenuInit()
{
    popMenu = new QMenu(ui->tableView_recv);
    action1 = new QAction();
    action2 = new QAction();
    action3 = new QAction();
    action1->setText(QString("复制"));
    action2->setText(QString("标记"));
    action3->setText(QString("取消标记"));
    popMenu->addAction(action1);
    popMenu->addAction(action2);
    popMenu->addAction(action3);
    connect(action1,SIGNAL(triggered()),this,SLOT(slotAction1()));
    connect(action2,SIGNAL(triggered()),this,SLOT(slotAction2()));
    connect(action3,SIGNAL(triggered()),this,SLOT(slotAction3()));
    connect(ui->tableView_recv, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));

    popMenu1 = new QMenu(ui->tableView_WriteValue);
    action1_1 = new QAction();
    action1_1->setText(QString("删除"));
    popMenu1->addAction(action1_1);
    connect(action1_1,SIGNAL(triggered()),this,SLOT(slotAction1_1()));
    connect(ui->tableView_WriteValue, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu1(QPoint)));

}

void MainWindow::IconInit()
{
    ui->labelConIocn->setStyleSheet("image: url(:/res/connect/fail.svg);");
    ui->Btn_Connect->setIcon(QIcon(":/res/Icon/lianjie.svg"));
    ui->Btn_Close->setIcon(QIcon(":/res/Icon/duankai.svg"));
    ui->Btn_Init->setIcon(QIcon(":/res/Icon/init.svg"));
    ui->Btn_TotalCall->setIcon(QIcon(":/res/Icon/datacall.svg"));
    ui->Btn_ClockSyn->setIcon(QIcon(":/res/Icon/sync-time.svg"));
    ui->Btn_ClockRead->setIcon(QIcon(":/res/Icon/read-time.svg"));
    ui->Btn_Reset->setIcon(QIcon(":/res/Icon/reset.svg"));
    ui->Btn_DirCall->setIcon(QIcon(":/res/Icon/FileTransfer.svg"));
}

void MainWindow::ProgressBarInit()
{
    //---------读进度条--------
    ui->progressBar_Read->setOrientation(Qt::Horizontal);
    ui->progressBar_Read->setMinimum(0);  // 最小值
    ui->progressBar_Read->setMaximum(100);  // 最大值
    ui->progressBar_Read->setValue(0);
    double dProgress = (ui->progressBar_Read->value() - ui->progressBar_Read->minimum()) * 100.0
                    / (ui->progressBar_Read->maximum() - ui->progressBar_Read->minimum());
    ui->progressBar_Read->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(dProgress, 'f', 1)));
    ui->progressBar_Read->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
    //---------写进度条--------
    ui->progressBar_Write->setOrientation(Qt::Horizontal);
    ui->progressBar_Write->setMinimum(0);  // 最小值
    ui->progressBar_Write->setMaximum(100);  // 最大值
    ui->progressBar_Write->setValue(0);
    dProgress = (ui->progressBar_Write->value() - ui->progressBar_Write->minimum()) * 100.0
                    / (ui->progressBar_Write->maximum() - ui->progressBar_Write->minimum());
    ui->progressBar_Write->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(dProgress, 'f', 1)));
    ui->progressBar_Write->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
    //-------软件升级进度条--------
    ui->progressBar_Updata->setOrientation(Qt::Horizontal);
    ui->progressBar_Updata->setMinimum(0);  // 最小值
    ui->progressBar_Updata->setMaximum(100);  // 最大值
    ui->progressBar_Updata->setValue(0);
    dProgress = (ui->progressBar_Updata->value() - ui->progressBar_Updata->minimum()) * 100.0
                    / (ui->progressBar_Updata->maximum() - ui->progressBar_Updata->minimum());
    ui->progressBar_Updata->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(dProgress, 'f', 1)));
    ui->progressBar_Updata->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}

void MainWindow::TimeLableInit()
{

    timer_calendar = new QTimer(this);//new一个QTimer对象
    connect(timer_calendar,SIGNAL(timeout()),this,SLOT(timerUpdate()));//连接槽函数
    timer_calendar->start(1000);//每一秒溢出一次进入槽函数
}

void MainWindow::timerUpdate()
{

    QDateTime time = QDateTime::currentDateTime();

    QString str = time.toString("yyyy-MM-dd \nhh:mm:ss dddd");

    ui->label_Time->setText(str);
}


void MainWindow::slotConnectRet(int status, QString IP, qint16 Port)
{
    if(status == 0)
    {
        //连接连接失败
        QMessageBox::information(this, tr("提示"), tr("连接超市,请重试"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
        ui->labelConIocn->clear();
        ui->labelConIocn->setStyleSheet("image: url(:/res/connect/success.svg);");
    }
    else if(status == 1)
    {
        //连接成功
        ui->labelConIocn->clear();
        ui->labelConIocn->setStyleSheet("image: url(:/res/connect/success.svg);");
        ui->Edit_DeBug->append(QString("connect success!!!"));
        ui->Edit_DeBug->append(QString("IP:%1,PORT:%2").arg(IP).arg(QString::number(Port)));


        //连接连接成功
        QMessageBox::information(this, tr("提示"), tr("连接成功"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);

        //启动接收分拣
        QFuture<void> RecvDataSession =QtConcurrent::run(session,&Session::RecvData);
        cout << "recv start" << endl;

        //测试链路保活
        connect(session,SIGNAL(TestConfirm()),this,SLOT(slotTestConfirm()));
        QFuture<bool> TestSession =QtConcurrent::run(session,&Session::TestSession);
        cout << "test start" << endl;

        //上报事件监听
        QFuture<bool> ReportDataSession =QtConcurrent::run(session,&Session::ReportDataSession);
        cout << "Report start" << endl;

    }
    else if(status == -4)
    {
        QMessageBox::information(this, tr("提示"), tr("连接丢失,请重新连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
    else
    {
        cout << "connecting" << endl;
    }
}

void MainWindow::slotTestConfirm()
{

    QFuture<bool> TestSessionSuccess =QtConcurrent::run(session,&Session::TestSessionSuccess);
}

void MainWindow::slotTestConfirmSuccess()
{
    session->SetWaitRecvTest(false);
    int row = ui->tableView_TestMsg->model()->rowCount();
    if(row > 120)
    {
        testmodel->setRowCount(0);
    }
    row = ui->tableView_TestMsg->model()->rowCount();
    //发送者
    testmodel->setItem(row, 0, new QStandardItem("从站"));
    QColor *color = new QColor(0, 0, 255);
    testmodel->item(row, 0)->setTextAlignment(Qt::AlignCenter);
    testmodel->item(row, 0)->setForeground(QBrush(*color));
    //帧类型
    testmodel->setItem(row, 1, new QStandardItem((QString("测试·从"))));
    testmodel->item(row, 1)->setTextAlignment(Qt::AlignCenter);
    testmodel->item(row, 1)->setForeground(QBrush(*color));
    //帧数据
    testmodel->setItem(row, 2, new QStandardItem("68 04 83 00 00 00"));
    testmodel->item(row, 2)->setForeground(QBrush(*color));

}

void MainWindow::slotTestConfirmTimeOut()
{
    session->SessionClose();
    session->SetTestLink(false);
    ui->labelConIocn->clear();
    ui->labelConIocn->setStyleSheet("image: url(:/res/connect/fail.svg);");
}

void MainWindow::slotAppendData(int author, QString type, QString data)
{
    cout << __FUNCTION__ << endl;
    int row = ui->tableView_recv->model()->rowCount();
    QColor *color;
    //发送者
    if(author == 0)
    {
        recvmodel->setItem(row, 0, new QStandardItem("主站"));
        color = new QColor(255, 0, 0);
    }
    else if(author == 1)
    {
        recvmodel->setItem(row, 0, new QStandardItem("从站"));
        color = new QColor(0, 255, 0);
    }else
    {
        recvmodel->setItem(row, 0, new QStandardItem("其他消息"));
        color = new QColor(0, 0, 255);
    }
    recvmodel->item(row, 0)->setTextAlignment(Qt::AlignCenter);
    recvmodel->item(row, 0)->setForeground(QBrush(*color));
    //帧类型
    recvmodel->setItem(row, 1, new QStandardItem((type+QString("格式"))));
    recvmodel->item(row, 1)->setTextAlignment(Qt::AlignCenter);
    recvmodel->item(row, 1)->setForeground(QBrush(*color));
    //帧数据
    recvmodel->setItem(row, 2, new QStandardItem(data));
    recvmodel->item(row, 2)->setForeground(QBrush(*color));
    //qDebug() << "data: " << data;
    ui->tableView_recv->scrollToBottom();   //滚动条自动滚到最下
    cout << endl;
}

void MainWindow::slotAppendTest(int author, QString type, QString data)
{
//    cout << __FUNCTION__ << endl;
    int row = ui->tableView_TestMsg->model()->rowCount();
    QColor *color;
    //发送者
    if(author == 0)
    {
        testmodel->setItem(row, 0, new QStandardItem("主站"));
        color = new QColor(255, 0, 0);
    }
    else if(author == 1)
    {
        testmodel->setItem(row, 0, new QStandardItem("从站"));
        color = new QColor(0, 255, 0);
    }else
    {
        testmodel->setItem(row, 0, new QStandardItem("其他消息"));
        color = new QColor(0, 0, 255);
    }
    testmodel->item(row, 0)->setTextAlignment(Qt::AlignCenter);
    testmodel->item(row, 0)->setForeground(QBrush(*color));
    //帧类型
    testmodel->setItem(row, 1, new QStandardItem((type)));
    testmodel->item(row, 1)->setTextAlignment(Qt::AlignCenter);
    testmodel->item(row, 1)->setForeground(QBrush(*color));
    //帧数据
    testmodel->setItem(row, 2, new QStandardItem(data));
    testmodel->item(row, 2)->setForeground(QBrush(*color));
    //qDebug() << "data: " << data;
    ui->tableView_TestMsg->scrollToBottom();   //滚动条自动滚到最下
    //    cout << endl;
}

void MainWindow::slotContextMenu(QPoint pos)
{
    auto index = ui->tableView_recv->indexAt(pos);
    if (index.isValid())
    {
        popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
    }
}

void MainWindow::slotAction1()
{
    auto q_index = ui->tableView_recv->currentIndex();
    int index = q_index.row();

    QString data;
    QVariant frame = ui->tableView_recv->model()->index(index,2).data();
    data = frame.toString();

//    qDebug() << "DATA: " << data;

    QClipboard *clip = QApplication::clipboard();
    clip->setText(data);
}

void MainWindow::slotAction2()
{
    auto q_index = ui->tableView_recv->currentIndex();
    int index = q_index.row();

    recvmodel->item(index, 0)->setBackground(QColor(Qt::yellow));
    recvmodel->item(index, 1)->setBackground(QColor(Qt::yellow));
    recvmodel->item(index, 2)->setBackground(QColor(Qt::yellow));

}

void MainWindow::slotAction3()
{
    auto q_index = ui->tableView_recv->currentIndex();
    int index = q_index.row();

    recvmodel->item(index, 0)->setBackground(QColor(Qt::white));
    recvmodel->item(index, 1)->setBackground(QColor(Qt::white));
    recvmodel->item(index, 2)->setBackground(QColor(Qt::white));

}

void MainWindow::slotUpdataDebugEidt(QString msg)
{
    ui->Edit_DeBug->append(msg);
}

void MainWindow::slotUpdataTableView(int modelID, int modellen, QStringList data)
{
    cout << __FUNCTION__ << endl;
    QTableView *table;
    QStandardItemModel *model;
    if(modelID == 1)
    {
        table = ui->tableView_Init;
        model = initmodel;
    }
    else
    {
        cout << "modelID is error" << endl;
    }
    int row = table->model()->rowCount();
    for(int i=0;i<modellen;i++)
    {
        model->setItem(row, i, new QStandardItem(data[i]));
    }
    int i = 0;
    foreach(QString variable, data)//使用foreach遍历container
    {
        model->setItem(row, i++, new QStandardItem(variable));
    }
    ui->tableView_Init->scrollToBottom();   //滚动条自动滚到最下
}

void MainWindow::slotShowDataTotalCall(int packid,QString T1, QString obj_addr, QString data, QString QDS, QString Time)
{
//      ui->Edit_DeBug->append(QString("slotShowDataTotalCall"));
//      int row = ui->tableView_TotalCall->model()->rowCount();
//      totalcallmodel->setItem(row, 0, new QStandardItem(T1));
    QStringList QList;
    QList << T1 << obj_addr << data << QDS;
    int row = ui->tableView_TotalCall->model()->rowCount();
    QColor *color;
    if(packid%2 == 0)
    {
        color = new QColor(139,69,19);
    }
    else
    {
        color = new QColor(139,0,139);
    }
    if(packid == -1)
    {
        color = new QColor(255,69,0);
    }
    int i = 0;
    for(QString value:QList)
    {
        totalcallmodel->setItem(row, i, new QStandardItem(value));
        totalcallmodel->item(row, i)->setTextAlignment(Qt::AlignCenter);
        totalcallmodel->item(row, i)->setForeground(QBrush(*color));
        i++;
    }
    if(Time.isEmpty())
    {
        totalcallmodel->setItem(row, i, new QStandardItem(QString("-")));
        totalcallmodel->item(row, i)->setTextAlignment(Qt::AlignCenter);
        totalcallmodel->item(row, i)->setForeground(QBrush(*color));
    }
    else
    {
        totalcallmodel->setItem(row, i, new QStandardItem(QString(Time)));
        totalcallmodel->item(row, i)->setTextAlignment(Qt::AlignCenter);
        totalcallmodel->item(row, i)->setForeground(QBrush(*color));
    }
    ui->tableView_TotalCall->scrollToBottom();   //滚动条自动滚到最下
}

void MainWindow::slotColckControl(int operation, char sender, QString Time)
{
    int row = ui->tableView_Colck->model()->rowCount();
    QString Operation;
    if(operation == 1)
    {
        Operation = QString("时钟同步");
    }
    else if(operation == 2)
    {
        Operation = QString("时钟同步(失败)");
    }
    else if(operation == 3)
    {
        Operation = QString("时钟同步(成功)");
    }else if(operation == 4)
    {
        Operation = QString("时钟读取");
    }
    else if(operation == 5)
    {
        Operation = QString("时钟读取(失败)");
    }
    else if(operation == 6)
    {
        Operation = QString("时钟读取(成功)");
    }
    colckmodel->setItem(row, 0, new QStandardItem(Operation));
    QString Sender;
    if(sender == 'R')
    {
        Sender = QString("从站");
    }
    else if(sender == 'S')
    {
        Sender = QString("主站");
    }
    colckmodel->setItem(row, 1, new QStandardItem(Sender));
    colckmodel->setItem(row, 2, new QStandardItem(Time));
    ui->tableView_Colck->scrollToBottom();   //滚动条自动滚到最下
}

void MainWindow::slotResetShow(int status, QString decs)
{
    int row = ui->tableView_Reset->model()->rowCount();
    QString Stat;
    if(status == 1) Stat = QString("成功");
    else Stat = QString("失败");
    resetmodel->setItem(row, 0, new QStandardItem(Stat));
    resetmodel->setItem(row, 1, new QStandardItem(decs));
    ui->tableView_Reset->scrollToBottom();   //滚动条自动滚到最下
}

void MainWindow::slotShowDirname(char *name, int n)
{
    cout << __FUNCTION__ << endl;
    char *NAME = new char[n+1];
    memset(NAME,0,n+1);
    my_strncpy(NAME,name,n);
    cout << "NAME: " << NAME << endl;
    int row = ui->tableView_Dir->model()->rowCount();
    cout << "row: " <<row << endl;
    dirmodel->setItem(row, 0, new QStandardItem(NAME));
    dirmodel->item(row, 0)->setTextAlignment(Qt::AlignCenter);
    ui->tableView_Dir->scrollToBottom();
}

void MainWindow::slotReadLabel(int status)
{
    QPalette pe;
    if(status == 0)
    {
        ui->label_ReadEnable->setText(QString("未激活"));
        pe.setColor(QPalette::WindowText,Qt::black);
        ui->label_ReadEnable->setPalette(pe);
    }
    else if(status == 1)
    {
        ui->label_ReadEnable->setText(QString("已激活"));
        pe.setColor(QPalette::WindowText,Qt::red);
        ui->label_ReadEnable->setPalette(pe);
    }
    else if(status == 2)
    {
        ui->label_ReadEnable->setText(QString("已完成"));
        pe.setColor(QPalette::WindowText,Qt::green);
        ui->label_ReadEnable->setPalette(pe);
    }
}

void MainWindow::slotReadPorgressMax(int Max)
{
    ui->progressBar_Read->setMaximum(Max);  // 最大值
}

void MainWindow::slotReadValue(int Value)
{
    ui->progressBar_Read->setValue(Value);  // 当前进度
    double dProgress = (ui->progressBar_Read->value() - ui->progressBar_Read->minimum()) * 100.0
                    / (ui->progressBar_Read->maximum() - ui->progressBar_Read->minimum());
    ui->progressBar_Read->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(dProgress, 'f', 1)));
    ui->progressBar_Read->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}

void MainWindow::slotWriteLabel(int status)
{
    QPalette pe;
    if(status == 0)
    {
        ui->label_WirteEnable->setText(QString("未激活"));
        pe.setColor(QPalette::WindowText,Qt::black);
        ui->label_WirteEnable->setPalette(pe);
    }
    else if(status == 1)
    {
        ui->label_WirteEnable->setText(QString("已激活"));
        pe.setColor(QPalette::WindowText,Qt::red);
        ui->label_WirteEnable->setPalette(pe);
    }
    else if(status == 2)
    {
        ui->label_WirteEnable->setText(QString("已完成"));
        pe.setColor(QPalette::WindowText,Qt::green);
        ui->label_WirteEnable->setPalette(pe);
    }
}

void MainWindow::slotWritePorgressMax(int Max)
{
    ui->progressBar_Write->setMaximum(Max);  // 最大值
}

void MainWindow::slotWriteValue(int Value)
{
    ui->progressBar_Write->setValue(Value);  // 当前进度
    double dProgress = (ui->progressBar_Write->value() - ui->progressBar_Write->minimum()) * 100.0
                    / (ui->progressBar_Write->maximum() - ui->progressBar_Write->minimum());
    ui->progressBar_Write->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(dProgress, 'f', 1)));
    ui->progressBar_Write->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}

void MainWindow::slotRemoteValueControl(int operation, int ValueNum, int ValueMin, int ValueMax,
                                        unsigned char Flag, QString ObjAddr, unsigned char Tag,
                                        QString Value)
{
    int row = ui->tableView_RemoteValue->model()->rowCount();
    QString Operation;
    if(operation == 1) Operation = QString("读定值参数");
    else if(operation == 2) Operation = QString("切换定值参数");
    else if(operation == 3) Operation = QString("读多个/全部参数和定值");
    else if(operation == 4) Operation = QString("预置参数和定值");
    else if(operation == 5) Operation = QString("取消预置参数和定值");
    else if(operation == 6) Operation = QString("固化参数和定值");
    else Operation = QString("-");

    remotevaluemodel->setItem(row, 0, new QStandardItem(Operation));
    remotevaluemodel->setItem(row, 1, new QStandardItem(QString::number(ValueNum)));

    //定值区最大值
    if(ValueMin == -1)
        remotevaluemodel->setItem(row, 2, new QStandardItem(QString("未读取")));
    else
        remotevaluemodel->setItem(row, 2, new QStandardItem(QString::number(ValueMin)));
    //定值区最小值
    if(ValueMin == -1)
        remotevaluemodel->setItem(row, 3, new QStandardItem(QString("未读取")));
    else
        remotevaluemodel->setItem(row, 3, new QStandardItem(QString::number(ValueMax)));
    //参数特征标识
    if(Flag == '-')
    {
        remotevaluemodel->setItem(row, 4, new QStandardItem(QString("-")));
    }
    else
    {
        char* flagarr;
        hex_to_char(&flagarr,(char*)(&Flag),1);
        QString decs = QString("(S/E=%1(0固化1预置),CR=%2(0未用1取消预置),CONT=%3(0无后续1有后续))").arg(
                    QString::number((Flag&0x80)>>7)).arg(
                    QString::number((Flag&0x40)>>6)).arg(
                    QString::number(Flag&0x01));
        remotevaluemodel->setItem(row, 4, new QStandardItem(QString(flagarr)+decs));
    }
    //信息体地址
    cout << "add OBJ :" ;
    cout << ObjAddr.toUtf8().data() << endl;

    if(ObjAddr.isEmpty())
    {
        remotevaluemodel->setItem(row, 5, new QStandardItem(QString("-")));
    }
    else
    {
        int n = ObjAddr.toInt();
        QString addr = QString("%1").arg(n,4,10,QLatin1Char('0'));
        remotevaluemodel->setItem(row, 5, new QStandardItem(addr));
    }
    //Tag类型
    if(Tag == '-')
    {
        remotevaluemodel->setItem(row, 6, new QStandardItem(QString("-")));
    }
    else
    {
        char* tagarr;
        hex_to_char(&tagarr,(char*)(&Tag),1);

        remotevaluemodel->setItem(row, 6, new QStandardItem(QString::number((int)Tag)));
    }
    //值
    if(Value.isEmpty())
        remotevaluemodel->setItem(row, 7, new QStandardItem(QString("-")));
    else
        remotevaluemodel->setItem(row, 7, new QStandardItem(Value));
    //设置居中
    for(int i=0;i<8;i++)
        remotevaluemodel->item(row, i)->setTextAlignment(Qt::AlignCenter);

    ui->tableView_RemoteValue->scrollToBottom();
}

void MainWindow::slotDialogWriteValue(QString addr, QString tag, QString value)
{
    int row = ui->tableView_WriteValue->model()->rowCount();
    writevaluemodel->setItem(row, 0, new QStandardItem(addr));
    writevaluemodel->setItem(row, 1, new QStandardItem(tag));
    writevaluemodel->setItem(row, 2, new QStandardItem(value));
    for(int i=0;i<3;i++)
        writevaluemodel->item(row, i)->setTextAlignment(Qt::AlignCenter);
//    qDebug() << addr;
//    qDebug() << tag;
    //    qDebug() << value;
}

void MainWindow::slotContextMenu1(QPoint pos)
{
    auto index = ui->tableView_WriteValue->indexAt(pos);
    Selected_Current_Row = index.row();    //获取到了当前右键所选的行数
    if (index.isValid())
    {
        popMenu1->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
    }
}

void MainWindow::slotAction1_1()
{
    //弹出提示框，看是否删除数据
    QMessageBox message(QMessageBox::NoIcon, QString("提示"),
                                QString("是否删除本行数据?"),
    QMessageBox::Yes | QMessageBox::No, NULL);

    //确认删除
    if(message.exec() == QMessageBox::Yes)
    {
        if(action1_1->text() == QString("删除"))   //看选中了删除这个菜单
        {
            writevaluemodel->removeRow(Selected_Current_Row);  //删除掉了表格信息
        }
    }
}

void MainWindow::slotShowDataEnergyCall(int packid, QString T1, QString obj_addr, QString data, QString QDS, QString Time)
{
    QStringList QList;
    QList << T1 << obj_addr << data << QDS;
    int row = ui->tableView_EnergyCall->model()->rowCount();
    QColor *color;
    if(packid%2 == 0)
    {
        color = new QColor(139,69,19);
    }
    else
    {
        color = new QColor(139,0,139);
    }
    int i = 0;
    for(QString value:QList)
    {
        energycallmodel->setItem(row, i, new QStandardItem(value));
        energycallmodel->item(row, i)->setTextAlignment(Qt::AlignCenter);
        energycallmodel->item(row, i)->setForeground(QBrush(*color));
        i++;
    }
    if(Time.isEmpty())
    {
        energycallmodel->setItem(row, i, new QStandardItem(QString("-")));
        energycallmodel->item(row, i)->setTextAlignment(Qt::AlignCenter);
        energycallmodel->item(row, i)->setForeground(QBrush(*color));
    }
    else
    {
        energycallmodel->setItem(row, i, new QStandardItem(QString(Time)));
        energycallmodel->item(row, i)->setTextAlignment(Qt::AlignCenter);
        energycallmodel->item(row, i)->setForeground(QBrush(*color));
    }
    ui->tableView_EnergyCall->scrollToBottom();   //滚动条自动滚到最下
}

void MainWindow::slotUpdataLabel(int status)
{
    QPalette pe;
    if(status == 0)
    {
        ui->UpdataLabel->setText(QString("未激活"));
        pe.setColor(QPalette::WindowText,Qt::black);
        ui->UpdataLabel->setPalette(pe);
    }
    else if(status == 1)
    {
        ui->UpdataLabel->setText(QString("已激活"));
        pe.setColor(QPalette::WindowText,Qt::red);
        ui->UpdataLabel->setPalette(pe);
    }
    else if(status == 2)
    {
        ui->UpdataLabel->setText(QString("已完成"));
        pe.setColor(QPalette::WindowText,Qt::green);
        ui->UpdataLabel->setPalette(pe);
    }
    if(status == 3)
    {
        ui->UpdataLabel->setText(QString("已中断"));
        pe.setColor(QPalette::WindowText,Qt::yellow);
        ui->UpdataLabel->setPalette(pe);
    }
}

void MainWindow::slotUpdataPorgressMax(int Max)
{
    ui->progressBar_Updata->setMaximum(Max);  // 最大值
}

void MainWindow::slotUpdataValue(int Value)
{
    ui->progressBar_Updata->setValue(Value);  // 当前进度
    double dProgress = (ui->progressBar_Updata->value() - ui->progressBar_Updata->minimum()) * 100.0
                    / (ui->progressBar_Updata->maximum() - ui->progressBar_Updata->minimum());
    ui->progressBar_Updata->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(dProgress, 'f', 1)));
    ui->progressBar_Updata->setAlignment(Qt::AlignRight | Qt::AlignVCenter);  // 对齐方式
}

void MainWindow::slotUpdataSuccess()
{
    UpdataTransfer = true;
}

void MainWindow::slotVersionUpdata(int operation, QString VersionFile, int status)
{
    int row = ui->tableView_VersionUpdata->model()->rowCount();
    if(operation == 1)
    {
        versionupdatamodel->setItem(row, 0, new QStandardItem(QString("升级启动")));
    }
    else if(operation == 2)
    {
        versionupdatamodel->setItem(row, 0, new QStandardItem(QString("升级文件传输")));
    }
    else if(operation == 3)
    {
        versionupdatamodel->setItem(row, 0, new QStandardItem(QString("确认升级")));
    }
    else if(operation == 4)
    {
        versionupdatamodel->setItem(row, 0, new QStandardItem(QString("取消升级")));
    }
    else
    {
        versionupdatamodel->setItem(row, 0, new QStandardItem(QString("其他操作")));
    }

    versionupdatamodel->setItem(row, 1, new QStandardItem(VersionFile));

    if(operation!=2)
    {
        if(status == 0)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("失败")));
        }
        else if(status == 1)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("成功")));
        }
        else
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("-")));
        }
    }
    else
    {
        if(status == 0)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("传输开始")));
        }
        else if(status == 1)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("传输中")));
        }
        else if(status == 2)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("传输中断")));
        }
        else if(status == 3)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("传输结束")));
        }
        else if(status == 4)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("传输确认成功")));
        }
        else if(status == 5)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("传输确认失败")));
        }
        else if(status == 6)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("传输成功")));
        }
        else if(status == 7)
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("传输失败")));
        }
        else
        {
            versionupdatamodel->setItem(row, 2, new QStandardItem(QString("-")));
        }
    }
}

void MainWindow::slotReadConfirm(int status, unsigned char *FileID, unsigned char *FileOffset, unsigned char desc)
{
    if(status == 0)
    {
        //失败
        QMessageBox::information(this, tr("提示"), tr("文件传输失败"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
    else if(status == 1)
    {
        //文件传输确认
        QFuture<bool> ReadConfirmSession =QtConcurrent::run(session,&Session::ReadConfirmSession,FileID,FileOffset,desc);
        //成功
        QMessageBox::information(this, tr("提示"), tr("文件传输成功"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::slotActionExit()
{
    QMessageBox::StandardButton ret = QMessageBox::question(this,
        tr("提示"),
        tr("是否真的要退出？"),
        QMessageBox::Ok | QMessageBox::Cancel,
        QMessageBox::Ok);



    if(ret == QMessageBox::Ok)
    {
        qApp->closeAllWindows();
        qApp->quit();
        exit(0);
    }

}

void MainWindow::slotActionHelp()
{
    QMessageBox::information(this, tr("提示"), tr("这是一个 IEC104主站 应用程序"),
        QMessageBox::Ok | QMessageBox::Cancel,
                             QMessageBox::Ok);
}

void MainWindow::slotReport(int type, QString format, QString data)
{
    cout << __FUNCTION__ << endl;
    int row = ui->tableView_Report->model()->rowCount();
    QColor *color;
    //发送者
    if(type == 1)
    {
        reportmodel->setItem(row, 0, new QStandardItem("遥信"));
        color = new QColor(255, 0, 0);
    }
    else if(type == 2)
    {
        reportmodel->setItem(row, 0, new QStandardItem("遥测"));
        color = new QColor(255,160,122);
    }else if(type == 3)
    {
        reportmodel->setItem(row, 0, new QStandardItem("故障事件信息"));
        color = new QColor(189,183,107);
    }
    else if(type == 4)
    {
        reportmodel->setItem(row, 0, new QStandardItem("电能量数据报文"));
        color = new QColor(46,139,87);
    }
    else if(type == 5)
    {
        reportmodel->setItem(row, 0, new QStandardItem("带时标的电能量报文"));
        color = new QColor(0,139,139);
    }
    else
    {
        reportmodel->setItem(row, 0, new QStandardItem("其他"));
        color = new QColor(47,79,79);
    }
    reportmodel->item(row, 0)->setTextAlignment(Qt::AlignCenter);
    reportmodel->item(row, 0)->setForeground(QBrush(*color));
    //帧类型
    reportmodel->setItem(row, 1, new QStandardItem((format)));
    reportmodel->item(row, 1)->setTextAlignment(Qt::AlignCenter);
    reportmodel->item(row, 1)->setForeground(QBrush(*color));
    //帧数据
    reportmodel->setItem(row, 2, new QStandardItem(data));
    reportmodel->item(row, 2)->setForeground(QBrush(*color));
    //qDebug() << "data: " << data;
    ui->tableView_Report->scrollToBottom();   //滚动条自动滚到最下
}

void MainWindow::on_Btn_Connect_clicked()
{
    ui->Edit_DeBug->append(QString("start connect..........."));
    if(ui->radioBtn_Client->isChecked())
    {
        qDebug() << "本地测试";
        ui->Edit_IP->setText(QString("127.0.0.1"));
        ui->Edit_Port->setText(QString("2404"));
    }
    else if(ui->radioBtn_Server->isChecked())
    {
        qDebug() << "终端测试";
//        ui->Edit_IP->setText(QString("192.168.1.156"));
//        ui->Edit_Port->setText(QString("2404"));
    }
    QString IP = ui->Edit_IP->text();
    quint16 Port = ui->Edit_Port->text().toInt();
//        qDebug() << "IP：" << IP;
//        qDebug() << "Port: " << Port;
    session = new Session();
    if(!session->Signals_Slot)
    {
        //槽函数连接
        connect(session,SIGNAL(ConnectRet(int,QString,qint16)),this,SLOT(slotConnectRet(int,QString,qint16)));
        connect(session,SIGNAL(TestConfirmTimeOut()),this,SLOT(slotTestConfirmTimeOut()));
        connect(session,SIGNAL(TestConfirmSuccess()),this,SLOT(slotTestConfirmSuccess()));
        connect(session,SIGNAL(AppendData(int,QString,QString)),this,SLOT(slotAppendData(int,QString,QString)));
        connect(session,SIGNAL(AppendTest(int,QString,QString)),this,SLOT(slotAppendTest(int,QString,QString)));
        connect(session,SIGNAL(UpdataDebugEdit(QString)),this,SLOT(slotUpdataDebugEidt(QString)));
        connect(session,SIGNAL(UpdataTableView(int,int,QStringList)),this,SLOT(slotUpdataTableView(int,int,QStringList)));
        connect(session,SIGNAL(ShowDataTotalCall(int,QString,QString,QString,QString,QString)),this,SLOT(slotShowDataTotalCall(int,QString,QString,QString,QString,QString)));
        connect(session,SIGNAL(ColckControl(int,char,QString)),this,SLOT(slotColckControl(int,char,QString)));
        connect(session,SIGNAL(ResetShow(int,QString)),this,SLOT(slotResetShow(int,QString)));
        connect(session,SIGNAL(ShowDirname(char*,int)),this,SLOT(slotShowDirname(char*,int)));
        //-----------------------------------
        connect(session, SIGNAL(WriteLabelEnable(int)), this, SLOT(slotWriteLabel(int)));
        connect(session, SIGNAL(ReadLabelEnable(int)), this, SLOT(slotReadLabel(int)));
        connect(session,SIGNAL(ReadConfirm(int,unsigned char*,unsigned char*,unsigned char)),this,SLOT(slotReadConfirm(int,unsigned char*,unsigned char*,unsigned char)));
        connect(session, SIGNAL(UpdataLabelEnable(int)), this, SLOT(slotUpdataLabel(int)));
        connect(this,SIGNAL(UpdataStop()),session,SLOT(slotUpdataStop()));
        //-----------------------------------
        connect(session,SIGNAL(ReadMax(int)),this,SLOT(slotReadPorgressMax(int)));
        connect(session,SIGNAL(ReadValue(int)),this,SLOT(slotReadValue(int)));
        connect(session,SIGNAL(WriteMax(int)),this,SLOT(slotWritePorgressMax(int)));
        connect(session,SIGNAL(WriteValue(int)),this,SLOT(slotWriteValue(int)));
        connect(session,SIGNAL(UpdataMax(int)),this,SLOT(slotUpdataPorgressMax(int)));
        connect(session,SIGNAL(UpdataValue(int)),this,SLOT(slotUpdataValue(int)));
        //-----------------------------------
        connect(session,SIGNAL(RemoteValueControl(int,int,int,int,unsigned char,QString,unsigned char,QString)),
                this,SLOT(slotRemoteValueControl(int,int,int,int,unsigned char,QString,unsigned char,QString)));
        //-----------------------------------
        connect(session,SIGNAL(ShowDataEnergyCall(int,QString,QString,QString,QString,QString)),this,SLOT(slotShowDataEnergyCall(int,QString,QString,QString,QString,QString)));
        session->Signals_Slot = true;
        //-----------------------------------
        connect(session,SIGNAL(UpdataSuccess()),this,SLOT(slotUpdataSuccess()));
        connect(session,SIGNAL(VersionUpdata(int,QString,int)),this,SLOT(slotVersionUpdata(int,QString,int)));

    }

    QFuture<bool> ConnectSession =QtConcurrent::run(session,&Session::ConnectSession,IP,Port);

//    //加载中动画
    ui->labelConIocn->clear();
    ui->labelConIocn->setStyleSheet("image: url(:/res/connect/loading.gif);");
    movie = new QMovie(":/res/connect/loading.gif");
    ui->labelConIocn->setMovie(movie); // 1. 设置要显示的 GIF 动画图片
    movie->start(); // 2. 启动动画

    //ConnectRet.waitForFinished();
}

void MainWindow::on_Btn_Close_clicked()
{
    if(session != nullptr)
    {
        session->SessionClose();
        session->SetTestLink(false);
        if(!session->IsConnect())
        {
            ui->labelConIocn->clear();
            ui->labelConIocn->setStyleSheet("image: url(:/res/connect/fail.svg);");
            //连接断开
            QMessageBox::information(this, tr("提示"), tr("连接已断开"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("还未连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }

}

void MainWindow::on_Btn_Init_clicked()
{
    if(session != nullptr)
    {
        session->InitSession();
        QFuture<bool> InitSessionSuccess =QtConcurrent::run(session,&Session::InitSessionSuccess);
        //InitSessionSuccess.waitForFinished();
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_TotalCall_clicked()
{
    if(session != nullptr)
    {
        if(session->isInit)
        {
            session->TotalCallSession();
            QFuture<bool> TotalCallSessionSuccess =QtConcurrent::run(session,&Session::TotalCallSessionSuccess);
            //TotalCallSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_ClockSyn_clicked()
{
    if(session != nullptr)
    {
        if(session->isInit)
        {
            QDateTime datetime = QDateTime::currentDateTime(); //当前时间
            quint16 ms = datetime.time().msec();                //毫秒
            quint16 year = datetime.date().year();             //年
            quint8 month = datetime.date().month();			   //月
            quint8 day = datetime.date().day();				   //日
            quint8 hour = datetime.time().hour();			   //时
            quint8 minute = datetime.time().minute();          //分
            quint8 second = datetime.time().second();          //秒
            quint8 dayOfWeek = datetime.date().dayOfWeek();    //星期
            st_cp56time2a time;
            time.Time = 0;
            time.Compts.year = year-2000;
            time.Compts.month = month;
            time.Compts.mday = day;
            time.Compts.hour = hour;
            time.Compts.min = minute;
            time.Compts.msec = second*1000+ms;
            time.Compts.wday = dayOfWeek;
            char data[7] = {0};
            cp56time2a_to_char(data,time);
            unsigned char *udata;
            udata = new unsigned char[7];
            memset(udata,0,7);
            my_strncpy((char*)udata,data,7);

    //        unsigned char CP56Time2aNOW[7] = {0x01,0x02,0x03,0x04,0x81,0x09,0x10};
            Frame f = session->ClockSynSession(udata);
            QFuture<bool> ClockSynSessionSuccess =QtConcurrent::run(session,&Session::ClockSynSessionSuccess,udata);
            //ClockSynSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_ClockRead_clicked()
{
    if(session != nullptr)
    {
        if(session->isInit)
        {
            session->ClockReadSession();
            QFuture<bool> ClockReadSessionSuccess =QtConcurrent::run(session,&Session::ClockReadSessionSuccess);
            //ClockSynSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_Reset_clicked()
{
    if(session != nullptr)
    {
        if(session->isInit)
        {
            session->ResetSession();
            QFuture<bool> ResetSessionSuccess =QtConcurrent::run(session,&Session::ResetSessionSuccess);
            //ClockSynSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_DirCall_clicked()
{
    if(session != nullptr)
    {
        if(session->isInit)
        {
            cout << __FUNCTION__ << endl;
            QString DirNameStr = ui->Edit_DirName->text();
    //        if(DirNameStr.isEmpty())
    //        {
    //            DirNameStr = QString("iec104");
    //            ui->Edit_DirName->setText(DirNameStr);
    //        }
            char DirName[100];
            strncpy(DirName,DirNameStr.toUtf8().data(),100);
            cout << "DirName: " << DirName << endl;
            session->DirCallSession(DirName);
            //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
            QFuture<bool> DirCallSessionSuccess =QtConcurrent::run(session,&Session::DirCallSessionSuccess);
    //        ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_Read_clicked()
{
    if(session != nullptr)
    {
        if(session->isInit)
        {
            cout << __FUNCTION__ << endl;
            QString RFileStr = ui->Edit_ReadFileName->text();
            QString RDirStr = ui->Edit_StorageDir->text();
            if(RFileStr.isEmpty())
            {
                RFileStr = QString("dir.cpp");
                ui->Edit_ReadFileName->setText(RFileStr);
            }
            char RFileName[100] = {0};
            strncpy(RFileName,RFileStr.toUtf8().data(),100);
            cout << "RFileName: " <<  RFileName << endl;
            qDebug()<< "RDirStr: " << RDirStr;
            if(RDirStr.isEmpty())
            {
                ui->Edit_StorageDir->setText(PATH);
            }
            session->ReadFileSession(RFileName);
            //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
            QFuture<bool> ReadFileSessionSuccess =QtConcurrent::run(session,&Session::ReadFileSessionSuccess,RDirStr);
            //ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_DirSelect_clicked()
{
    cout << __FUNCTION__ << endl;
    QString dir = QFileDialog::getExistingDirectory(this,
            tr("Select Directory"), "/",
             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qDebug() << "DirName: " << dir ;
    ui->Edit_StorageDir->setText(dir);
    QPalette pe;
    ui->label_ReadEnable->setText(QString("未激活"));
    pe.setColor(QPalette::WindowText,Qt::black);
    ui->label_ReadEnable->setPalette(pe);
}

void MainWindow::on_Btn_Write_clicked()
{
    if(session != nullptr)
     {
        if(session->isInit)
        {
             cout << __FUNCTION__ << endl;
             QString WFileStr = ui->Edit_WirteFileName->text();
             if(WFileStr.isEmpty())
             {
                 WFileStr = QString(WRITEPATH)+QString("filetest.txt");
                 ui->Edit_WirteFileName->setText(WFileStr);
             }
             char WFileName[100] = {0};
             strncpy(WFileName,WFileStr.toUtf8().data(),100);
             cout << "WFileName: " <<  WFileName << endl;
             qDebug()<< "WFileStr: " << WFileStr;
             Frame f = session->WriteFileSession(WFileName,0);
             //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
             QFuture<bool> WriteFileSessionSuccess =QtConcurrent::run(session,&Session::WriteFileSessionSuccess,f,WFileStr);
             //ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }
}

void MainWindow::on_Btn_SelectFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Select File"),
            "D:/",
            tr("*"));
    qDebug() << "fileName: " << fileName ;
    ui->Edit_WirteFileName->setText(fileName);
    QPalette pe;
    ui->label_WirteEnable->setText(QString("未激活"));
    pe.setColor(QPalette::WindowText,Qt::black);
    ui->label_WirteEnable->setPalette(pe);
}

void MainWindow::on_Btn_ReadValueNum_clicked()
{
    if(session != nullptr)
     {
        if(session->isInit)
        {
             cout << __FUNCTION__ << endl;
             session->ReadFixedValueNumSession();
             //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
             QFuture<bool> ReadFixedValueNumSessionSuccess =QtConcurrent::run(session,&Session::ReadFixedValueNumSessionSuccess);
             //ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }
}

void MainWindow::on_Btn_SetValueNum_clicked()
{
    if(session != nullptr)
     {
        if(session->isInit)
        {
         cout << __FUNCTION__ << endl;
         QString Num = ui->Edit_ValueNum->text();
         cout << Num.toInt() << endl;
         session->SetFixedValueNumSession(Num.toInt());
         //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
         QFuture<bool> SetFixedValueNumSessionSuccess =QtConcurrent::run(session,&Session::SetFixedValueNumSessionSuccess,Num.toInt());
         //ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }
}

void MainWindow::on_Btn_InputObjAddr_clicked()
{
    QString Addr = QInputDialog::getText(this,
                                             "QInputdialog_Name",
                                             "请输入信息体地址(0x005001=01 50 00)",
                                             QLineEdit::Normal,
                                             "01 50 00"
                                             );

    if(!Addr.isEmpty())
    {
        list.append(Addr);
        QStringListModel *listmodel = new QStringListModel(list);
        ui->listView_Addr->setModel(listmodel);
    }
//    qDebug() << list;
}

void MainWindow::on_Btn_ReadValue_clicked()
{
    if(session != nullptr)
     {
        if(session->isInit)
        {
            QString Num = ui->Edit_ReadValueNum->text();
            if(Num.isEmpty()) ui->Edit_ReadValueNum->setText(QString("8"));
            Num = ui->Edit_ReadValueNum->text();
            qDebug() << Num;
            qDebug() << list;

            cout << __FUNCTION__ << endl;
            cout << Num.toInt() << endl;
            session->ReadValueSession(Num.toInt(),list);
            //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
            QFuture<bool> ReadValueSessionSuccess =QtConcurrent::run(session,&Session::ReadValueSessionSuccess,Num.toInt());
            //ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }

}

void MainWindow::on_Btn_ObjAddrClear_clicked()
{
    list.clear();
    QStringListModel *listmodel = new QStringListModel(list);
    ui->listView_Addr->setModel(listmodel);
}

void MainWindow::on_Btn_ValueAdd_clicked()
{
    Dialog_WriteValueAdd *dialog = new Dialog_WriteValueAdd();
    dialog->show();
    connect(dialog,SIGNAL(DialogWriteValue(QString,QString,QString)),this,SLOT(slotDialogWriteValue(QString,QString,QString)));
}

void MainWindow::on_Btn_ValueClear_clicked()
{
    writevaluemodel->removeRows(0,writevaluemodel->rowCount()); //删除所有行
}

void MainWindow::on_Btn_Preset_clicked()
{
    cout << __FUNCTION__ << endl;
    if(session != nullptr)
     {
        if(session->isInit)
        {
            QString Num = ui->Edit_WriteValueNum->text();
            if(Num.isEmpty()) ui->Edit_WriteValueNum->setText(QString("8"));
            Num = ui->Edit_WriteValueNum->text();

            cout << Num.toInt() << endl;

            int row = ui->tableView_WriteValue->model()->rowCount();
            cout << "ROW: " << row << endl;
            if(row == 0)
            {
                QMessageBox::warning(this, tr("提示"), tr("请添加数据"),
                    QMessageBox::Ok | QMessageBox::Cancel,
                    QMessageBox::Ok);
            }
            else
            {
                QStringList addrlist;
                QStringList taglist;
                QStringList valuelist;
                for(int i = 0; i < row; i++)
                {
                    //遍历第row行的所有信息
                    QModelIndex index = writevaluemodel->index(i,0);
                    QString addr = writevaluemodel->data(index).toString();
                    index = writevaluemodel->index(i,1);
                    QString tag = writevaluemodel->data(index).toString();
                    index = writevaluemodel->index(i,2);
                    QString value = writevaluemodel->data(index).toString();
                    addrlist<<addr;
                    taglist<<tag;
                    valuelist<<value;
                }
                int select = 1;
                if(ui->radioButton_nseq->isChecked()){select = 0;}
                session->PersetSession(Num.toInt(),select,addrlist,taglist,valuelist);
                //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
                QFuture<bool> PersetSessionSuccess =QtConcurrent::run(session,&Session::PersetSessionSuccess,Num.toInt(),select);
                //ClockReadSessionSuccess.waitForFinished();
            }
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }
}

void MainWindow::on_Btn_PresetCancel_clicked()
{
    cout << __FUNCTION__ << endl;
    if(session != nullptr)
     {
        if(session->GetIsPerset())
        {
            QString Num = ui->Edit_WriteValueNum->text();
            if(Num.isEmpty()) ui->Edit_WriteValueNum->setText(QString("8"));
            Num = ui->Edit_WriteValueNum->text();
            cout << Num.toInt() << endl;
            session->PersetCancelSession(Num.toInt());
            //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
            QFuture<bool> PersetCancelSessionSuccess =QtConcurrent::run(session,&Session::PersetCancelSessionSuccess,Num.toInt());
            //ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::information(this, tr("提示"), tr("请先进行参数预置"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }
}

void MainWindow::on_Btn_Curing_clicked()
{
    cout << __FUNCTION__ << endl;
    if(session != nullptr)
     {
        if(session->GetIsPerset())
        {
            QString Num = ui->Edit_WriteValueNum->text();
            if(Num.isEmpty()) ui->Edit_WriteValueNum->setText(QString("8"));
            Num = ui->Edit_WriteValueNum->text();
            cout << Num.toInt() << endl;
            session->CuringSession(Num.toInt());
            //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
            QFuture<bool> CuringSessionSuccess =QtConcurrent::run(session,&Session::CuringSessionSuccess,Num.toInt());
            //ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::information(this, tr("提示"), tr("请先进行参数预置"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }
}

void MainWindow::on_Btn_EnergyCall_clicked()
{
    if(session != nullptr)
     {
        if(session->isInit)
        {
            cout << __FUNCTION__ << endl;
            session->EnergyCallSession();
            //show_hex((char*)(f.GetAPDU()),f.GetAPDU()[1]+2);
            QFuture<bool> EnergyCallSessionSuccess =QtConcurrent::run(session,&Session::EnergyCallSessionSuccess);
            //ClockReadSessionSuccess.waitForFinished();
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }

}

void MainWindow::on_Btn_SelectVersionFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Select File"),
            "D:/",
            tr("*"));
    qDebug() << "fileName: " << fileName ;
    ui->Edit_VersionFile->setText(fileName);
//    QPalette pe;
//    ui->label_WirteEnable->setText(QString("未激活"));
//    pe.setColor(QPalette::WindowText,Qt::black);
//    ui->label_WirteEnable->setPalette(pe);

}

void MainWindow::on_Btn_UpdataVersion_clicked()
{
    if(session != nullptr)
     {
        if(session->isInit)
        {
            QString VersionFile = ui->Edit_VersionFile->text();
            if(VersionFile.isEmpty())
            {
                QMessageBox::information(this, tr("提示"), tr("未选择升级文件"),
                    QMessageBox::Ok | QMessageBox::Cancel,
                    QMessageBox::Ok);
                VersionFile = QString(WRITEPATH)+QString("filetest.txt");
                ui->Edit_VersionFile->setText(VersionFile);
            }
            else
            {
                UpdataTransfer = false;

                cout << __FUNCTION__ << endl;

                session->UpdataVersionSession();
                QFuture<bool> UpdataVersionSessionSuccess =QtConcurrent::run(session,&Session::UpdataVersionSessionSuccess,VersionFile);
                //ClockReadSessionSuccess.waitForFinished();
            }
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
     }
     else
     {
         QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
             QMessageBox::Ok | QMessageBox::Cancel,
             QMessageBox::Ok);
     }
}

void MainWindow::on_Btn_UpdataStop_clicked()
{
    if(session != nullptr)
    {
        if(session->GetStartLevelUp())
        {
            emit UpdataStop();
        }
        else
        {
            QMessageBox::information(this, tr("提示"), tr("升级文件未启动"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先链接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }

}

void MainWindow::on_Btn_RunUpdata_clicked()
{
    if(session != nullptr)
    {
        if(session->isInit)
        {
            if(UpdataTransfer)
            {
                cout << __FUNCTION__ << endl;
                session->RunUpdataSession();
                QFuture<bool> RunUpdataSessionSuccess =QtConcurrent::run(session,&Session::RunUpdataSessionSuccess);
                //ClockReadSessionSuccess.waitForFinished();
            }
            else
            {
                QMessageBox::information(this, tr("提示"), tr("升级文件未传输成功"),
                    QMessageBox::Ok | QMessageBox::Cancel,
                    QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_AbandonUpdata_clicked()
{
    if(session != nullptr)
    {
        if(session->isInit)
        {
            if(UpdataTransfer)
            {
                cout << __FUNCTION__ << endl;
                session->AbandonUpdataSession();
                QFuture<bool> AbandonUpdataSessionSuccess =QtConcurrent::run(session,&Session::AbandonUpdataSessionSuccess);
                //ClockReadSessionSuccess.waitForFinished();
            }
            else
            {
                QMessageBox::information(this, tr("提示"), tr("升级文件未传输成功"),
                    QMessageBox::Ok | QMessageBox::Cancel,
                    QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::warning(this, tr("提示"), tr("请先初始化"),
                QMessageBox::Ok | QMessageBox::Cancel,
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this, tr("提示"), tr("请先进行连接"),
            QMessageBox::Ok | QMessageBox::Cancel,
            QMessageBox::Ok);
    }
}

void MainWindow::on_Btn_Init_Clear_clicked()
{
    initmodel->setRowCount(0);
}

void MainWindow::on_Btn_TotalCall_Clear_clicked()
{
    totalcallmodel->setRowCount(0);
}

void MainWindow::on_Btn_ClockRead_Clear_clicked()
{
    colckmodel->setRowCount(0);
}

void MainWindow::on_Btn_Reset_Clear_clicked()
{
    resetmodel->setRowCount(0);
}

void MainWindow::on_Btn_EnergyCall_Clear_clicked()
{
    energycallmodel->setRowCount(0);
}

void MainWindow::on_Btn_GetTime_clicked()
{
    QDateTime datetime = QDateTime::currentDateTime(); //当前时间
    quint16 ms = datetime.time().msec();                //毫秒
    quint16 year = datetime.date().year();             //年
    quint8 month = datetime.date().month();			   //月
    quint8 day = datetime.date().day();				   //日
    quint8 hour = datetime.time().hour();			   //时
    quint8 minute = datetime.time().minute();          //分
    quint8 second = datetime.time().second();          //秒
    quint8 dayOfWeek = datetime.date().dayOfWeek();    //星期

    qDebug() << datetime;
    QString str = datetime.toString("yyyy-MM-dd hh:mm:ss.zzz");
    qDebug() << str;
    st_cp56time2a time;
    qDebug() << year-2000 << month << day << hour << minute << second << ms;
    time.Time = 0;
    time.Compts.year = year-2000;
    time.Compts.month = month;
    time.Compts.mday = day;
    time.Compts.hour = hour;
    time.Compts.min = minute;
    time.Compts.msec = second*1000+ms;
    time.Compts.wday = dayOfWeek;

    qDebug() << "msec: " << time.Compts.msec;
    qDebug() << "dayOfWeek" << dayOfWeek;
    qDebug() << "time: " << get_cp56time2a_string(&time);
    char data[7] = {0};
    cout << "time.Time: ";
    cout << time.Time << endl;
    printf("%014llx\n",time.Time);
    cout << "data: ";
    cp56time2a_to_char(data,time);
    unsigned char udata[7] = {0};
    my_strncpy((char*)udata,data,7);
    cout << "show_hex: ";
    show_hex((char*)udata,7);
    qDebug() << "-------------------";
    char *st_cp56time2a_ch;
    hex_to_char(&st_cp56time2a_ch,(char*)udata,7);
    QString dataStr = QString(st_cp56time2a_ch);
    int n = dataStr.length();
    while(n-2 > 0)
    {
        n = n - 2;
        dataStr.insert(n," ");
    }

    QString msg = QString("时间: \n")+str+QString("\n时标格式: \n  ")+dataStr;

    QMessageBox box;//也可以创建其它对话框，如Qdialog，可以设置窗口大小，文本显示位置等等
    box.setText(msg);
    box.setTextInteractionFlags(Qt::TextSelectableByMouse);//设置文本可选中
    box.exec();//函数用于弹出对话框，会阻塞，关闭对话框后解除阻塞
}

void MainWindow::Btn_LogClear_clicked()
{
    int index = ui->LogWidget->currentIndex();
//    cout << "index: " << index << endl;
    if(index == 1)
    {
        recvmodel->setRowCount(0);
    }
    else if(index == 2)
    {
        ui->Edit_DeBug->clear();
    }
    else if(index == 3)
    {
        testmodel->setRowCount(0);
    }
}

void MainWindow::on_Btn_DirClear_clicked()
{
    dirmodel->setRowCount(0);
}

void MainWindow::on_Btn_RemoteValue_clicked()
{
    remotevaluemodel->setRowCount(0);
}

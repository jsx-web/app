/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *action_1;
    QAction *action_2;
    QAction *actionHelp;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    QTabWidget *DateWidget;
    QWidget *tab_Connect;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioBtn_Server;
    QRadioButton *radioBtn_Client;
    QLineEdit *Edit_IP;
    QLineEdit *Edit_Port;
    QSpacerItem *horizontalSpacer;
    QLabel *labelConIocn;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *Btn_Connect;
    QPushButton *Btn_Close;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_3;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *Btn_ConnectClear;
    QTableView *tableViewConnect;
    QWidget *tab_Init;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_5;
    QPushButton *Btn_Init_Clear;
    QPushButton *Btn_Init;
    QTableView *tableView_Init;
    QWidget *tab_TollCall;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_6;
    QPushButton *Btn_TotalCall;
    QPushButton *Btn_TotalCall_Clear;
    QTableView *tableView_TotalCall;
    QWidget *tab_ColckControl;
    QVBoxLayout *verticalLayout_5;
    QWidget *widget_7;
    QPushButton *Btn_ClockRead;
    QPushButton *Btn_ClockSyn;
    QPushButton *Btn_ClockRead_Clear;
    QLabel *label_Time;
    QPushButton *Btn_GetTime;
    QTableView *tableView_Colck;
    QWidget *tab_Reset;
    QVBoxLayout *verticalLayout_6;
    QWidget *widget_8;
    QPushButton *Btn_Reset;
    QPushButton *Btn_Reset_Clear;
    QTableView *tableView_Reset;
    QWidget *tab_FileTransfer;
    QHBoxLayout *horizontalLayout_26;
    QWidget *widget_9;
    QHBoxLayout *horizontalLayout_23;
    QWidget *widget_14;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_3;
    QLineEdit *Edit_DirName;
    QPushButton *Btn_DirCall;
    QFrame *line;
    QVBoxLayout *verticalLayout;
    QWidget *widget_11;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_4;
    QLineEdit *Edit_ReadFileName;
    QLabel *label_ReadEnable;
    QWidget *widget_12;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_6;
    QLineEdit *Edit_StorageDir;
    QPushButton *Btn_DirSelect;
    QHBoxLayout *horizontalLayout_10;
    QProgressBar *progressBar_Read;
    QPushButton *Btn_Read;
    QFrame *line_2;
    QVBoxLayout *verticalLayout_7;
    QWidget *widget_13;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_7;
    QLineEdit *Edit_WirteFileName;
    QPushButton *Btn_SelectFile;
    QLabel *label_WirteEnable;
    QHBoxLayout *horizontalLayout_13;
    QProgressBar *progressBar_Write;
    QPushButton *Btn_Write;
    QVBoxLayout *verticalLayout_22;
    QHBoxLayout *horizontalLayout_7;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *Btn_DirClear;
    QTableView *tableView_Dir;
    QSpacerItem *horizontalSpacer_5;
    QWidget *tab_RemoteValue;
    QVBoxLayout *verticalLayout_10;
    QWidget *widget_15;
    QHBoxLayout *horizontalLayout_25;
    QWidget *widget_17;
    QVBoxLayout *verticalLayout_17;
    QPushButton *Btn_ReadValueNum;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_16;
    QLineEdit *Edit_ValueNum;
    QPushButton *Btn_SetValueNum;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *Btn_RemoteValue;
    QSpacerItem *horizontalSpacer_9;
    QFrame *line_3;
    QWidget *widget_10;
    QVBoxLayout *verticalLayout_13;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label;
    QLineEdit *Edit_ReadValueNum;
    QListView *listView_Addr;
    QHBoxLayout *horizontalLayout_15;
    QPushButton *Btn_InputObjAddr;
    QPushButton *Btn_ObjAddrClear;
    QPushButton *Btn_ReadValue;
    QFrame *line_4;
    QWidget *widget_16;
    QVBoxLayout *verticalLayout_15;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_5;
    QLineEdit *Edit_WriteValueNum;
    QHBoxLayout *horizontalLayout_18;
    QTableView *tableView_WriteValue;
    QVBoxLayout *verticalLayout_14;
    QPushButton *Btn_ValueAdd;
    QPushButton *Btn_ValueClear;
    QRadioButton *radioButton_nseq;
    QRadioButton *radioButton_seq;
    QHBoxLayout *horizontalLayout_17;
    QPushButton *Btn_Preset;
    QPushButton *Btn_PresetCancel;
    QPushButton *Btn_Curing;
    QFrame *line_5;
    QSpacerItem *horizontalSpacer_6;
    QTableView *tableView_RemoteValue;
    QWidget *tab_ElectricalEnergyCall;
    QVBoxLayout *verticalLayout_18;
    QWidget *widget_18;
    QPushButton *Btn_EnergyCall;
    QPushButton *Btn_EnergyCall_Clear;
    QTableView *tableView_EnergyCall;
    QWidget *tab_VersionUpdata;
    QVBoxLayout *verticalLayout_19;
    QWidget *widget_19;
    QHBoxLayout *horizontalLayout_22;
    QWidget *widget_20;
    QVBoxLayout *verticalLayout_20;
    QHBoxLayout *horizontalLayout_21;
    QHBoxLayout *horizontalLayout_19;
    QLineEdit *Edit_VersionFile;
    QPushButton *Btn_SelectVersionFile;
    QPushButton *Btn_UpdataVersion;
    QHBoxLayout *horizontalLayout_20;
    QProgressBar *progressBar_Updata;
    QLabel *UpdataLabel;
    QPushButton *Btn_UpdataStop;
    QWidget *widget_21;
    QVBoxLayout *verticalLayout_21;
    QPushButton *Btn_RunUpdata;
    QPushButton *Btn_AbandonUpdata;
    QSpacerItem *horizontalSpacer_7;
    QTableView *tableView_VersionUpdata;
    QTabWidget *LogWidget;
    QWidget *tab;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_12;
    QTableView *tableView_recv;
    QWidget *tab_4;
    QVBoxLayout *verticalLayout_11;
    QTextBrowser *Edit_DeBug;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_23;
    QTableView *tableView_TestMsg;
    QWidget *tab_5;
    QHBoxLayout *horizontalLayout_24;
    QTableView *tableView_Report;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(952, 600);
        MainWindow->setMinimumSize(QSize(950, 600));
        QIcon icon;
        icon.addFile(QStringLiteral(":/res/Icon/Icon.svg"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        action_1 = new QAction(MainWindow);
        action_1->setObjectName(QStringLiteral("action_1"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QStringLiteral("action_2"));
        actionHelp = new QAction(MainWindow);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        DateWidget = new QTabWidget(splitter);
        DateWidget->setObjectName(QStringLiteral("DateWidget"));
        DateWidget->setMinimumSize(QSize(0, 0));
        DateWidget->setMaximumSize(QSize(16777215, 16777215));
        DateWidget->setStyleSheet(QLatin1String("QTabBar::tab{width:100}\n"
"QTabBar::tab{height:50}"));
        tab_Connect = new QWidget();
        tab_Connect->setObjectName(QStringLiteral("tab_Connect"));
        verticalLayout_2 = new QVBoxLayout(tab_Connect);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        widget_3 = new QWidget(tab_Connect);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setMinimumSize(QSize(0, 0));
        widget_3->setMaximumSize(QSize(800, 50));
        horizontalLayout_4 = new QHBoxLayout(widget_3);
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(widget_3);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        radioBtn_Server = new QRadioButton(widget);
        radioBtn_Server->setObjectName(QStringLiteral("radioBtn_Server"));
        radioBtn_Server->setChecked(false);

        horizontalLayout->addWidget(radioBtn_Server);

        radioBtn_Client = new QRadioButton(widget);
        radioBtn_Client->setObjectName(QStringLiteral("radioBtn_Client"));
        radioBtn_Client->setChecked(true);

        horizontalLayout->addWidget(radioBtn_Client);


        horizontalLayout_2->addLayout(horizontalLayout);

        Edit_IP = new QLineEdit(widget);
        Edit_IP->setObjectName(QStringLiteral("Edit_IP"));

        horizontalLayout_2->addWidget(Edit_IP);

        Edit_Port = new QLineEdit(widget);
        Edit_Port->setObjectName(QStringLiteral("Edit_Port"));

        horizontalLayout_2->addWidget(Edit_Port);


        horizontalLayout_4->addWidget(widget);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        labelConIocn = new QLabel(widget_3);
        labelConIocn->setObjectName(QStringLiteral("labelConIocn"));
        labelConIocn->setMinimumSize(QSize(50, 50));
        labelConIocn->setMaximumSize(QSize(50, 50));

        horizontalLayout_4->addWidget(labelConIocn);

        widget_2 = new QWidget(widget_3);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        horizontalLayout_3 = new QHBoxLayout(widget_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        Btn_Connect = new QPushButton(widget_2);
        Btn_Connect->setObjectName(QStringLiteral("Btn_Connect"));

        horizontalLayout_3->addWidget(Btn_Connect);

        Btn_Close = new QPushButton(widget_2);
        Btn_Close->setObjectName(QStringLiteral("Btn_Close"));

        horizontalLayout_3->addWidget(Btn_Close);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        widget_4 = new QWidget(widget_2);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        horizontalLayout_5 = new QHBoxLayout(widget_4);
        horizontalLayout_5->setSpacing(0);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        Btn_ConnectClear = new QPushButton(widget_4);
        Btn_ConnectClear->setObjectName(QStringLiteral("Btn_ConnectClear"));

        horizontalLayout_5->addWidget(Btn_ConnectClear);


        horizontalLayout_3->addWidget(widget_4);


        horizontalLayout_4->addWidget(widget_2);


        verticalLayout_2->addWidget(widget_3);

        tableViewConnect = new QTableView(tab_Connect);
        tableViewConnect->setObjectName(QStringLiteral("tableViewConnect"));

        verticalLayout_2->addWidget(tableViewConnect);

        DateWidget->addTab(tab_Connect, QString());
        tab_Init = new QWidget();
        tab_Init->setObjectName(QStringLiteral("tab_Init"));
        verticalLayout_3 = new QVBoxLayout(tab_Init);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget_5 = new QWidget(tab_Init);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        widget_5->setMinimumSize(QSize(0, 45));
        widget_5->setMaximumSize(QSize(900, 16777215));
        Btn_Init_Clear = new QPushButton(widget_5);
        Btn_Init_Clear->setObjectName(QStringLiteral("Btn_Init_Clear"));
        Btn_Init_Clear->setGeometry(QRect(110, 10, 75, 30));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Btn_Init_Clear->sizePolicy().hasHeightForWidth());
        Btn_Init_Clear->setSizePolicy(sizePolicy);
        Btn_Init_Clear->setMinimumSize(QSize(75, 30));
        Btn_Init = new QPushButton(widget_5);
        Btn_Init->setObjectName(QStringLiteral("Btn_Init"));
        Btn_Init->setGeometry(QRect(10, 10, 75, 30));
        sizePolicy.setHeightForWidth(Btn_Init->sizePolicy().hasHeightForWidth());
        Btn_Init->setSizePolicy(sizePolicy);
        Btn_Init->setMinimumSize(QSize(0, 30));

        verticalLayout_3->addWidget(widget_5);

        tableView_Init = new QTableView(tab_Init);
        tableView_Init->setObjectName(QStringLiteral("tableView_Init"));

        verticalLayout_3->addWidget(tableView_Init);

        DateWidget->addTab(tab_Init, QString());
        tab_TollCall = new QWidget();
        tab_TollCall->setObjectName(QStringLiteral("tab_TollCall"));
        verticalLayout_4 = new QVBoxLayout(tab_TollCall);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        widget_6 = new QWidget(tab_TollCall);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        widget_6->setMinimumSize(QSize(0, 50));
        Btn_TotalCall = new QPushButton(widget_6);
        Btn_TotalCall->setObjectName(QStringLiteral("Btn_TotalCall"));
        Btn_TotalCall->setGeometry(QRect(10, 10, 75, 30));
        sizePolicy.setHeightForWidth(Btn_TotalCall->sizePolicy().hasHeightForWidth());
        Btn_TotalCall->setSizePolicy(sizePolicy);
        Btn_TotalCall_Clear = new QPushButton(widget_6);
        Btn_TotalCall_Clear->setObjectName(QStringLiteral("Btn_TotalCall_Clear"));
        Btn_TotalCall_Clear->setGeometry(QRect(140, 10, 75, 30));
        sizePolicy.setHeightForWidth(Btn_TotalCall_Clear->sizePolicy().hasHeightForWidth());
        Btn_TotalCall_Clear->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(widget_6);

        tableView_TotalCall = new QTableView(tab_TollCall);
        tableView_TotalCall->setObjectName(QStringLiteral("tableView_TotalCall"));

        verticalLayout_4->addWidget(tableView_TotalCall);

        DateWidget->addTab(tab_TollCall, QString());
        tab_ColckControl = new QWidget();
        tab_ColckControl->setObjectName(QStringLiteral("tab_ColckControl"));
        verticalLayout_5 = new QVBoxLayout(tab_ColckControl);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        widget_7 = new QWidget(tab_ColckControl);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        widget_7->setMinimumSize(QSize(0, 50));
        Btn_ClockRead = new QPushButton(widget_7);
        Btn_ClockRead->setObjectName(QStringLiteral("Btn_ClockRead"));
        Btn_ClockRead->setGeometry(QRect(40, 10, 75, 30));
        Btn_ClockRead->setMinimumSize(QSize(0, 30));
        Btn_ClockSyn = new QPushButton(widget_7);
        Btn_ClockSyn->setObjectName(QStringLiteral("Btn_ClockSyn"));
        Btn_ClockSyn->setGeometry(QRect(150, 10, 75, 30));
        Btn_ClockSyn->setMinimumSize(QSize(0, 30));
        Btn_ClockRead_Clear = new QPushButton(widget_7);
        Btn_ClockRead_Clear->setObjectName(QStringLiteral("Btn_ClockRead_Clear"));
        Btn_ClockRead_Clear->setGeometry(QRect(290, 10, 75, 30));
        Btn_ClockRead_Clear->setMinimumSize(QSize(0, 30));
        label_Time = new QLabel(widget_7);
        label_Time->setObjectName(QStringLiteral("label_Time"));
        label_Time->setGeometry(QRect(430, 10, 121, 31));
        Btn_GetTime = new QPushButton(widget_7);
        Btn_GetTime->setObjectName(QStringLiteral("Btn_GetTime"));
        Btn_GetTime->setGeometry(QRect(560, 10, 111, 30));
        Btn_GetTime->setMinimumSize(QSize(0, 30));

        verticalLayout_5->addWidget(widget_7);

        tableView_Colck = new QTableView(tab_ColckControl);
        tableView_Colck->setObjectName(QStringLiteral("tableView_Colck"));

        verticalLayout_5->addWidget(tableView_Colck);

        DateWidget->addTab(tab_ColckControl, QString());
        tab_Reset = new QWidget();
        tab_Reset->setObjectName(QStringLiteral("tab_Reset"));
        verticalLayout_6 = new QVBoxLayout(tab_Reset);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        widget_8 = new QWidget(tab_Reset);
        widget_8->setObjectName(QStringLiteral("widget_8"));
        widget_8->setMinimumSize(QSize(0, 50));
        Btn_Reset = new QPushButton(widget_8);
        Btn_Reset->setObjectName(QStringLiteral("Btn_Reset"));
        Btn_Reset->setGeometry(QRect(20, 10, 111, 30));
        Btn_Reset->setMinimumSize(QSize(0, 30));
        Btn_Reset_Clear = new QPushButton(widget_8);
        Btn_Reset_Clear->setObjectName(QStringLiteral("Btn_Reset_Clear"));
        Btn_Reset_Clear->setGeometry(QRect(160, 10, 75, 30));
        Btn_Reset_Clear->setMinimumSize(QSize(0, 30));

        verticalLayout_6->addWidget(widget_8);

        tableView_Reset = new QTableView(tab_Reset);
        tableView_Reset->setObjectName(QStringLiteral("tableView_Reset"));

        verticalLayout_6->addWidget(tableView_Reset);

        DateWidget->addTab(tab_Reset, QString());
        tab_FileTransfer = new QWidget();
        tab_FileTransfer->setObjectName(QStringLiteral("tab_FileTransfer"));
        horizontalLayout_26 = new QHBoxLayout(tab_FileTransfer);
        horizontalLayout_26->setSpacing(6);
        horizontalLayout_26->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_26->setObjectName(QStringLiteral("horizontalLayout_26"));
        widget_9 = new QWidget(tab_FileTransfer);
        widget_9->setObjectName(QStringLiteral("widget_9"));
        widget_9->setMinimumSize(QSize(0, 280));
        widget_9->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout_23 = new QHBoxLayout(widget_9);
        horizontalLayout_23->setSpacing(0);
        horizontalLayout_23->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        horizontalLayout_23->setContentsMargins(0, 0, 0, 0);
        widget_14 = new QWidget(widget_9);
        widget_14->setObjectName(QStringLiteral("widget_14"));
        widget_14->setMinimumSize(QSize(440, 280));
        widget_14->setMaximumSize(QSize(440, 440));
        verticalLayout_8 = new QVBoxLayout(widget_14);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(-1, 5, -1, 5);
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_3 = new QLabel(widget_14);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_6->addWidget(label_3);

        Edit_DirName = new QLineEdit(widget_14);
        Edit_DirName->setObjectName(QStringLiteral("Edit_DirName"));
        Edit_DirName->setMinimumSize(QSize(0, 20));

        horizontalLayout_6->addWidget(Edit_DirName);

        Btn_DirCall = new QPushButton(widget_14);
        Btn_DirCall->setObjectName(QStringLiteral("Btn_DirCall"));
        Btn_DirCall->setMinimumSize(QSize(0, 30));

        horizontalLayout_6->addWidget(Btn_DirCall);


        verticalLayout_8->addLayout(horizontalLayout_6);

        line = new QFrame(widget_14);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_8->addWidget(line);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget_11 = new QWidget(widget_14);
        widget_11->setObjectName(QStringLiteral("widget_11"));
        widget_11->setMinimumSize(QSize(330, 0));
        horizontalLayout_8 = new QHBoxLayout(widget_11);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_4 = new QLabel(widget_11);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_8->addWidget(label_4);

        Edit_ReadFileName = new QLineEdit(widget_11);
        Edit_ReadFileName->setObjectName(QStringLiteral("Edit_ReadFileName"));
        Edit_ReadFileName->setMinimumSize(QSize(0, 20));

        horizontalLayout_8->addWidget(Edit_ReadFileName);

        label_ReadEnable = new QLabel(widget_11);
        label_ReadEnable->setObjectName(QStringLiteral("label_ReadEnable"));

        horizontalLayout_8->addWidget(label_ReadEnable);


        verticalLayout->addWidget(widget_11);

        widget_12 = new QWidget(widget_14);
        widget_12->setObjectName(QStringLiteral("widget_12"));
        widget_12->setMinimumSize(QSize(0, 40));
        widget_12->setMaximumSize(QSize(16777215, 16777215));
        horizontalLayout_9 = new QHBoxLayout(widget_12);
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_6 = new QLabel(widget_12);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_9->addWidget(label_6);

        Edit_StorageDir = new QLineEdit(widget_12);
        Edit_StorageDir->setObjectName(QStringLiteral("Edit_StorageDir"));
        Edit_StorageDir->setMinimumSize(QSize(0, 20));

        horizontalLayout_9->addWidget(Edit_StorageDir);

        Btn_DirSelect = new QPushButton(widget_12);
        Btn_DirSelect->setObjectName(QStringLiteral("Btn_DirSelect"));
        Btn_DirSelect->setMinimumSize(QSize(0, 30));

        horizontalLayout_9->addWidget(Btn_DirSelect);


        verticalLayout->addWidget(widget_12);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        progressBar_Read = new QProgressBar(widget_14);
        progressBar_Read->setObjectName(QStringLiteral("progressBar_Read"));
        progressBar_Read->setValue(24);

        horizontalLayout_10->addWidget(progressBar_Read);

        Btn_Read = new QPushButton(widget_14);
        Btn_Read->setObjectName(QStringLiteral("Btn_Read"));
        Btn_Read->setMinimumSize(QSize(0, 30));

        horizontalLayout_10->addWidget(Btn_Read);


        verticalLayout->addLayout(horizontalLayout_10);


        verticalLayout_8->addLayout(verticalLayout);

        line_2 = new QFrame(widget_14);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout_8->addWidget(line_2);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        widget_13 = new QWidget(widget_14);
        widget_13->setObjectName(QStringLiteral("widget_13"));
        widget_13->setMinimumSize(QSize(330, 0));
        horizontalLayout_11 = new QHBoxLayout(widget_13);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        label_7 = new QLabel(widget_13);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_11->addWidget(label_7);

        Edit_WirteFileName = new QLineEdit(widget_13);
        Edit_WirteFileName->setObjectName(QStringLiteral("Edit_WirteFileName"));
        Edit_WirteFileName->setMinimumSize(QSize(0, 20));

        horizontalLayout_11->addWidget(Edit_WirteFileName);

        Btn_SelectFile = new QPushButton(widget_13);
        Btn_SelectFile->setObjectName(QStringLiteral("Btn_SelectFile"));

        horizontalLayout_11->addWidget(Btn_SelectFile);

        label_WirteEnable = new QLabel(widget_13);
        label_WirteEnable->setObjectName(QStringLiteral("label_WirteEnable"));

        horizontalLayout_11->addWidget(label_WirteEnable);


        verticalLayout_7->addWidget(widget_13);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        progressBar_Write = new QProgressBar(widget_14);
        progressBar_Write->setObjectName(QStringLiteral("progressBar_Write"));
        progressBar_Write->setValue(24);

        horizontalLayout_13->addWidget(progressBar_Write);

        Btn_Write = new QPushButton(widget_14);
        Btn_Write->setObjectName(QStringLiteral("Btn_Write"));
        Btn_Write->setMinimumSize(QSize(0, 30));

        horizontalLayout_13->addWidget(Btn_Write);


        verticalLayout_7->addLayout(horizontalLayout_13);


        verticalLayout_8->addLayout(verticalLayout_7);


        horizontalLayout_23->addWidget(widget_14);

        verticalLayout_22 = new QVBoxLayout();
        verticalLayout_22->setSpacing(6);
        verticalLayout_22->setObjectName(QStringLiteral("verticalLayout_22"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalSpacer_8 = new QSpacerItem(353, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_8);

        Btn_DirClear = new QPushButton(widget_9);
        Btn_DirClear->setObjectName(QStringLiteral("Btn_DirClear"));
        Btn_DirClear->setMinimumSize(QSize(0, 30));

        horizontalLayout_7->addWidget(Btn_DirClear);


        verticalLayout_22->addLayout(horizontalLayout_7);

        tableView_Dir = new QTableView(widget_9);
        tableView_Dir->setObjectName(QStringLiteral("tableView_Dir"));
        tableView_Dir->setMinimumSize(QSize(440, 0));
        tableView_Dir->setMaximumSize(QSize(440, 16777215));

        verticalLayout_22->addWidget(tableView_Dir);


        horizontalLayout_23->addLayout(verticalLayout_22);

        horizontalSpacer_5 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_23->addItem(horizontalSpacer_5);


        horizontalLayout_26->addWidget(widget_9);

        DateWidget->addTab(tab_FileTransfer, QString());
        tab_RemoteValue = new QWidget();
        tab_RemoteValue->setObjectName(QStringLiteral("tab_RemoteValue"));
        verticalLayout_10 = new QVBoxLayout(tab_RemoteValue);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        widget_15 = new QWidget(tab_RemoteValue);
        widget_15->setObjectName(QStringLiteral("widget_15"));
        widget_15->setEnabled(true);
        widget_15->setMinimumSize(QSize(0, 165));
        widget_15->setMaximumSize(QSize(16777215, 165));
        horizontalLayout_25 = new QHBoxLayout(widget_15);
        horizontalLayout_25->setSpacing(6);
        horizontalLayout_25->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_25->setObjectName(QStringLiteral("horizontalLayout_25"));
        horizontalLayout_25->setContentsMargins(-1, 0, -1, 0);
        widget_17 = new QWidget(widget_15);
        widget_17->setObjectName(QStringLiteral("widget_17"));
        widget_17->setMaximumSize(QSize(150, 16777215));
        verticalLayout_17 = new QVBoxLayout(widget_17);
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setContentsMargins(11, 11, 11, 11);
        verticalLayout_17->setObjectName(QStringLiteral("verticalLayout_17"));
        verticalLayout_17->setContentsMargins(-1, 0, -1, 0);
        Btn_ReadValueNum = new QPushButton(widget_17);
        Btn_ReadValueNum->setObjectName(QStringLiteral("Btn_ReadValueNum"));
        Btn_ReadValueNum->setMinimumSize(QSize(0, 30));

        verticalLayout_17->addWidget(Btn_ReadValueNum);

        verticalSpacer_2 = new QSpacerItem(20, 44, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_17->addItem(verticalSpacer_2);

        verticalLayout_16 = new QVBoxLayout();
        verticalLayout_16->setSpacing(6);
        verticalLayout_16->setObjectName(QStringLiteral("verticalLayout_16"));
        Edit_ValueNum = new QLineEdit(widget_17);
        Edit_ValueNum->setObjectName(QStringLiteral("Edit_ValueNum"));
        sizePolicy.setHeightForWidth(Edit_ValueNum->sizePolicy().hasHeightForWidth());
        Edit_ValueNum->setSizePolicy(sizePolicy);
        Edit_ValueNum->setMinimumSize(QSize(9, 20));
        Edit_ValueNum->setMaximumSize(QSize(16666666, 30));

        verticalLayout_16->addWidget(Edit_ValueNum);

        Btn_SetValueNum = new QPushButton(widget_17);
        Btn_SetValueNum->setObjectName(QStringLiteral("Btn_SetValueNum"));
        Btn_SetValueNum->setMinimumSize(QSize(0, 30));

        verticalLayout_16->addWidget(Btn_SetValueNum);


        verticalLayout_17->addLayout(verticalLayout_16);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        Btn_RemoteValue = new QPushButton(widget_17);
        Btn_RemoteValue->setObjectName(QStringLiteral("Btn_RemoteValue"));

        horizontalLayout_12->addWidget(Btn_RemoteValue);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_9);


        verticalLayout_17->addLayout(horizontalLayout_12);


        horizontalLayout_25->addWidget(widget_17);

        line_3 = new QFrame(widget_15);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        horizontalLayout_25->addWidget(line_3);

        widget_10 = new QWidget(widget_15);
        widget_10->setObjectName(QStringLiteral("widget_10"));
        widget_10->setMaximumSize(QSize(155, 16777215));
        verticalLayout_13 = new QVBoxLayout(widget_10);
        verticalLayout_13->setSpacing(6);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QStringLiteral("verticalLayout_13"));
        verticalLayout_13->setContentsMargins(-1, 0, -1, 0);
        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label = new QLabel(widget_10);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_14->addWidget(label);

        Edit_ReadValueNum = new QLineEdit(widget_10);
        Edit_ReadValueNum->setObjectName(QStringLiteral("Edit_ReadValueNum"));

        horizontalLayout_14->addWidget(Edit_ReadValueNum);


        verticalLayout_13->addLayout(horizontalLayout_14);

        listView_Addr = new QListView(widget_10);
        listView_Addr->setObjectName(QStringLiteral("listView_Addr"));
        listView_Addr->setEnabled(false);
        listView_Addr->setMinimumSize(QSize(0, 27));
        listView_Addr->setMaximumSize(QSize(153, 75));
        listView_Addr->setDragEnabled(false);

        verticalLayout_13->addWidget(listView_Addr);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(0);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        Btn_InputObjAddr = new QPushButton(widget_10);
        Btn_InputObjAddr->setObjectName(QStringLiteral("Btn_InputObjAddr"));

        horizontalLayout_15->addWidget(Btn_InputObjAddr);

        Btn_ObjAddrClear = new QPushButton(widget_10);
        Btn_ObjAddrClear->setObjectName(QStringLiteral("Btn_ObjAddrClear"));
        Btn_ObjAddrClear->setMinimumSize(QSize(20, 0));
        Btn_ObjAddrClear->setMaximumSize(QSize(39, 16777215));

        horizontalLayout_15->addWidget(Btn_ObjAddrClear);


        verticalLayout_13->addLayout(horizontalLayout_15);

        Btn_ReadValue = new QPushButton(widget_10);
        Btn_ReadValue->setObjectName(QStringLiteral("Btn_ReadValue"));

        verticalLayout_13->addWidget(Btn_ReadValue);


        horizontalLayout_25->addWidget(widget_10);

        line_4 = new QFrame(widget_15);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        horizontalLayout_25->addWidget(line_4);

        widget_16 = new QWidget(widget_15);
        widget_16->setObjectName(QStringLiteral("widget_16"));
        widget_16->setMaximumSize(QSize(234, 16777215));
        verticalLayout_15 = new QVBoxLayout(widget_16);
        verticalLayout_15->setSpacing(6);
        verticalLayout_15->setContentsMargins(11, 11, 11, 11);
        verticalLayout_15->setObjectName(QStringLiteral("verticalLayout_15"));
        verticalLayout_15->setContentsMargins(-1, 0, -1, 0);
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        label_5 = new QLabel(widget_16);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_16->addWidget(label_5);

        Edit_WriteValueNum = new QLineEdit(widget_16);
        Edit_WriteValueNum->setObjectName(QStringLiteral("Edit_WriteValueNum"));

        horizontalLayout_16->addWidget(Edit_WriteValueNum);


        verticalLayout_15->addLayout(horizontalLayout_16);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(2);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        tableView_WriteValue = new QTableView(widget_16);
        tableView_WriteValue->setObjectName(QStringLiteral("tableView_WriteValue"));
        tableView_WriteValue->setMinimumSize(QSize(150, 0));
        tableView_WriteValue->setMaximumSize(QSize(160, 16777215));

        horizontalLayout_18->addWidget(tableView_WriteValue);

        verticalLayout_14 = new QVBoxLayout();
        verticalLayout_14->setSpacing(0);
        verticalLayout_14->setObjectName(QStringLiteral("verticalLayout_14"));
        Btn_ValueAdd = new QPushButton(widget_16);
        Btn_ValueAdd->setObjectName(QStringLiteral("Btn_ValueAdd"));
        Btn_ValueAdd->setMinimumSize(QSize(0, 0));
        Btn_ValueAdd->setMaximumSize(QSize(50, 16777215));

        verticalLayout_14->addWidget(Btn_ValueAdd);

        Btn_ValueClear = new QPushButton(widget_16);
        Btn_ValueClear->setObjectName(QStringLiteral("Btn_ValueClear"));
        Btn_ValueClear->setMaximumSize(QSize(50, 16777215));

        verticalLayout_14->addWidget(Btn_ValueClear);

        radioButton_nseq = new QRadioButton(widget_16);
        radioButton_nseq->setObjectName(QStringLiteral("radioButton_nseq"));

        verticalLayout_14->addWidget(radioButton_nseq);

        radioButton_seq = new QRadioButton(widget_16);
        radioButton_seq->setObjectName(QStringLiteral("radioButton_seq"));
        radioButton_seq->setChecked(true);

        verticalLayout_14->addWidget(radioButton_seq);


        horizontalLayout_18->addLayout(verticalLayout_14);


        verticalLayout_15->addLayout(horizontalLayout_18);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        Btn_Preset = new QPushButton(widget_16);
        Btn_Preset->setObjectName(QStringLiteral("Btn_Preset"));

        horizontalLayout_17->addWidget(Btn_Preset);

        Btn_PresetCancel = new QPushButton(widget_16);
        Btn_PresetCancel->setObjectName(QStringLiteral("Btn_PresetCancel"));

        horizontalLayout_17->addWidget(Btn_PresetCancel);

        Btn_Curing = new QPushButton(widget_16);
        Btn_Curing->setObjectName(QStringLiteral("Btn_Curing"));

        horizontalLayout_17->addWidget(Btn_Curing);


        verticalLayout_15->addLayout(horizontalLayout_17);


        horizontalLayout_25->addWidget(widget_16);

        line_5 = new QFrame(widget_15);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);

        horizontalLayout_25->addWidget(line_5);

        horizontalSpacer_6 = new QSpacerItem(330, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_25->addItem(horizontalSpacer_6);


        verticalLayout_10->addWidget(widget_15);

        tableView_RemoteValue = new QTableView(tab_RemoteValue);
        tableView_RemoteValue->setObjectName(QStringLiteral("tableView_RemoteValue"));

        verticalLayout_10->addWidget(tableView_RemoteValue);

        DateWidget->addTab(tab_RemoteValue, QString());
        tab_ElectricalEnergyCall = new QWidget();
        tab_ElectricalEnergyCall->setObjectName(QStringLiteral("tab_ElectricalEnergyCall"));
        verticalLayout_18 = new QVBoxLayout(tab_ElectricalEnergyCall);
        verticalLayout_18->setSpacing(6);
        verticalLayout_18->setContentsMargins(11, 11, 11, 11);
        verticalLayout_18->setObjectName(QStringLiteral("verticalLayout_18"));
        widget_18 = new QWidget(tab_ElectricalEnergyCall);
        widget_18->setObjectName(QStringLiteral("widget_18"));
        widget_18->setMinimumSize(QSize(0, 50));
        Btn_EnergyCall = new QPushButton(widget_18);
        Btn_EnergyCall->setObjectName(QStringLiteral("Btn_EnergyCall"));
        Btn_EnergyCall->setGeometry(QRect(40, 10, 75, 30));
        Btn_EnergyCall_Clear = new QPushButton(widget_18);
        Btn_EnergyCall_Clear->setObjectName(QStringLiteral("Btn_EnergyCall_Clear"));
        Btn_EnergyCall_Clear->setGeometry(QRect(130, 10, 75, 30));
        Btn_EnergyCall_Clear->setMinimumSize(QSize(0, 30));

        verticalLayout_18->addWidget(widget_18);

        tableView_EnergyCall = new QTableView(tab_ElectricalEnergyCall);
        tableView_EnergyCall->setObjectName(QStringLiteral("tableView_EnergyCall"));

        verticalLayout_18->addWidget(tableView_EnergyCall);

        DateWidget->addTab(tab_ElectricalEnergyCall, QString());
        tab_VersionUpdata = new QWidget();
        tab_VersionUpdata->setObjectName(QStringLiteral("tab_VersionUpdata"));
        verticalLayout_19 = new QVBoxLayout(tab_VersionUpdata);
        verticalLayout_19->setSpacing(6);
        verticalLayout_19->setContentsMargins(11, 11, 11, 11);
        verticalLayout_19->setObjectName(QStringLiteral("verticalLayout_19"));
        widget_19 = new QWidget(tab_VersionUpdata);
        widget_19->setObjectName(QStringLiteral("widget_19"));
        widget_19->setMinimumSize(QSize(0, 100));
        horizontalLayout_22 = new QHBoxLayout(widget_19);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        widget_20 = new QWidget(widget_19);
        widget_20->setObjectName(QStringLiteral("widget_20"));
        widget_20->setMinimumSize(QSize(430, 0));
        widget_20->setMaximumSize(QSize(430, 16777215));
        verticalLayout_20 = new QVBoxLayout(widget_20);
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setContentsMargins(11, 11, 11, 11);
        verticalLayout_20->setObjectName(QStringLiteral("verticalLayout_20"));
        verticalLayout_20->setContentsMargins(-1, 0, -1, 0);
        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        Edit_VersionFile = new QLineEdit(widget_20);
        Edit_VersionFile->setObjectName(QStringLiteral("Edit_VersionFile"));
        Edit_VersionFile->setMinimumSize(QSize(0, 30));

        horizontalLayout_19->addWidget(Edit_VersionFile);

        Btn_SelectVersionFile = new QPushButton(widget_20);
        Btn_SelectVersionFile->setObjectName(QStringLiteral("Btn_SelectVersionFile"));
        Btn_SelectVersionFile->setMinimumSize(QSize(0, 30));

        horizontalLayout_19->addWidget(Btn_SelectVersionFile);


        horizontalLayout_21->addLayout(horizontalLayout_19);

        Btn_UpdataVersion = new QPushButton(widget_20);
        Btn_UpdataVersion->setObjectName(QStringLiteral("Btn_UpdataVersion"));
        Btn_UpdataVersion->setMinimumSize(QSize(0, 30));
        Btn_UpdataVersion->setMaximumSize(QSize(16777215, 30));

        horizontalLayout_21->addWidget(Btn_UpdataVersion);


        verticalLayout_20->addLayout(horizontalLayout_21);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        progressBar_Updata = new QProgressBar(widget_20);
        progressBar_Updata->setObjectName(QStringLiteral("progressBar_Updata"));
        progressBar_Updata->setMinimumSize(QSize(0, 25));
        progressBar_Updata->setValue(24);

        horizontalLayout_20->addWidget(progressBar_Updata);

        UpdataLabel = new QLabel(widget_20);
        UpdataLabel->setObjectName(QStringLiteral("UpdataLabel"));
        UpdataLabel->setMinimumSize(QSize(50, 40));
        UpdataLabel->setMaximumSize(QSize(50, 50));
        UpdataLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_20->addWidget(UpdataLabel);

        Btn_UpdataStop = new QPushButton(widget_20);
        Btn_UpdataStop->setObjectName(QStringLiteral("Btn_UpdataStop"));
        Btn_UpdataStop->setMinimumSize(QSize(0, 30));

        horizontalLayout_20->addWidget(Btn_UpdataStop);


        verticalLayout_20->addLayout(horizontalLayout_20);


        horizontalLayout_22->addWidget(widget_20);

        widget_21 = new QWidget(widget_19);
        widget_21->setObjectName(QStringLiteral("widget_21"));
        verticalLayout_21 = new QVBoxLayout(widget_21);
        verticalLayout_21->setSpacing(13);
        verticalLayout_21->setContentsMargins(11, 11, 11, 11);
        verticalLayout_21->setObjectName(QStringLiteral("verticalLayout_21"));
        verticalLayout_21->setContentsMargins(0, 0, 0, 0);
        Btn_RunUpdata = new QPushButton(widget_21);
        Btn_RunUpdata->setObjectName(QStringLiteral("Btn_RunUpdata"));
        Btn_RunUpdata->setMinimumSize(QSize(0, 30));

        verticalLayout_21->addWidget(Btn_RunUpdata);

        Btn_AbandonUpdata = new QPushButton(widget_21);
        Btn_AbandonUpdata->setObjectName(QStringLiteral("Btn_AbandonUpdata"));
        Btn_AbandonUpdata->setMinimumSize(QSize(0, 30));

        verticalLayout_21->addWidget(Btn_AbandonUpdata);


        horizontalLayout_22->addWidget(widget_21);

        horizontalSpacer_7 = new QSpacerItem(451, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_22->addItem(horizontalSpacer_7);


        verticalLayout_19->addWidget(widget_19);

        tableView_VersionUpdata = new QTableView(tab_VersionUpdata);
        tableView_VersionUpdata->setObjectName(QStringLiteral("tableView_VersionUpdata"));

        verticalLayout_19->addWidget(tableView_VersionUpdata);

        QIcon icon1;
        icon1.addFile(QStringLiteral(":/res/Icon/VersionUpdata.svg"), QSize(), QIcon::Normal, QIcon::Off);
        DateWidget->addTab(tab_VersionUpdata, icon1, QString());
        splitter->addWidget(DateWidget);
        LogWidget = new QTabWidget(splitter);
        LogWidget->setObjectName(QStringLiteral("LogWidget"));
        LogWidget->setIconSize(QSize(12, 12));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        LogWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_12 = new QVBoxLayout(tab_3);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        tableView_recv = new QTableView(tab_3);
        tableView_recv->setObjectName(QStringLiteral("tableView_recv"));

        verticalLayout_12->addWidget(tableView_recv);

        LogWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        verticalLayout_11 = new QVBoxLayout(tab_4);
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        Edit_DeBug = new QTextBrowser(tab_4);
        Edit_DeBug->setObjectName(QStringLiteral("Edit_DeBug"));
        Edit_DeBug->setMinimumSize(QSize(0, 0));

        verticalLayout_11->addWidget(Edit_DeBug);

        LogWidget->addTab(tab_4, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_23 = new QVBoxLayout(tab_2);
        verticalLayout_23->setSpacing(6);
        verticalLayout_23->setContentsMargins(11, 11, 11, 11);
        verticalLayout_23->setObjectName(QStringLiteral("verticalLayout_23"));
        tableView_TestMsg = new QTableView(tab_2);
        tableView_TestMsg->setObjectName(QStringLiteral("tableView_TestMsg"));

        verticalLayout_23->addWidget(tableView_TestMsg);

        LogWidget->addTab(tab_2, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        horizontalLayout_24 = new QHBoxLayout(tab_5);
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        tableView_Report = new QTableView(tab_5);
        tableView_Report->setObjectName(QStringLiteral("tableView_Report"));

        horizontalLayout_24->addWidget(tableView_Report);

        LogWidget->addTab(tab_5, QString());
        splitter->addWidget(LogWidget);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 952, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(actionExit);
        menu_2->addAction(action_1);
        menu_2->addAction(action_2);
        menu_3->addAction(actionHelp);

        retranslateUi(MainWindow);

        DateWidget->setCurrentIndex(0);
        LogWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionExit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", Q_NULLPTR));
        action_1->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        action_2->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        actionHelp->setText(QApplication::translate("MainWindow", "\345\270\256\345\212\251", Q_NULLPTR));
        radioBtn_Server->setText(QApplication::translate("MainWindow", " \347\273\210\347\253\257", Q_NULLPTR));
        radioBtn_Client->setText(QApplication::translate("MainWindow", "\346\234\254\345\234\260", Q_NULLPTR));
        Edit_IP->setText(QApplication::translate("MainWindow", "192.168.1.156", Q_NULLPTR));
        Edit_Port->setText(QApplication::translate("MainWindow", "2404", Q_NULLPTR));
        labelConIocn->setText(QString());
        Btn_Connect->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245", Q_NULLPTR));
        Btn_Close->setText(QApplication::translate("MainWindow", "\346\226\255\345\274\200", Q_NULLPTR));
        Btn_ConnectClear->setText(QApplication::translate("MainWindow", " \346\270\205\347\251\272", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_Connect), QApplication::translate("MainWindow", "\345\273\272\347\253\213\350\277\236\346\216\245", Q_NULLPTR));
        Btn_Init_Clear->setText(QApplication::translate("MainWindow", " \346\270\205\347\251\272", Q_NULLPTR));
        Btn_Init->setText(QApplication::translate("MainWindow", " \345\210\235\345\247\213\345\214\226", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_Init), QApplication::translate("MainWindow", "\345\210\235\345\247\213\345\214\226", Q_NULLPTR));
        Btn_TotalCall->setText(QApplication::translate("MainWindow", " \346\200\273\345\217\254\345\224\244", Q_NULLPTR));
        Btn_TotalCall_Clear->setText(QApplication::translate("MainWindow", " \346\270\205\347\251\272", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_TollCall), QApplication::translate("MainWindow", "\346\200\273\345\217\254\345\224\244", Q_NULLPTR));
        Btn_ClockRead->setText(QApplication::translate("MainWindow", "\346\227\266\351\222\237\350\257\273\345\217\226", Q_NULLPTR));
        Btn_ClockSyn->setText(QApplication::translate("MainWindow", "\346\227\266\351\222\237\345\220\214\346\255\245", Q_NULLPTR));
        Btn_ClockRead_Clear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        label_Time->setText(QApplication::translate("MainWindow", "\345\275\223\345\211\215\346\227\266\351\227\264\345\234\250\350\277\231\346\230\276\347\244\272", Q_NULLPTR));
        Btn_GetTime->setText(QApplication::translate("MainWindow", "  \350\216\267\345\217\226\345\275\223\345\211\215\346\227\266\351\227\264", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_ColckControl), QApplication::translate("MainWindow", " \346\227\266\351\222\237\346\216\247\345\210\266", Q_NULLPTR));
        Btn_Reset->setText(QApplication::translate("MainWindow", " \345\274\200\345\220\257\345\244\215\344\275\215", Q_NULLPTR));
        Btn_Reset_Clear->setText(QApplication::translate("MainWindow", " \346\270\205\347\251\272", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_Reset), QApplication::translate("MainWindow", "\345\244\215\344\275\215", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\347\233\256\345\275\225\345\220\215\357\274\232", Q_NULLPTR));
        Btn_DirCall->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\347\233\256\345\275\225\345\217\254\345\224\244", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\345\220\215\357\274\232", Q_NULLPTR));
        label_ReadEnable->setText(QApplication::translate("MainWindow", " \346\234\252\346\277\200\346\264\273", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", " \346\226\207\344\273\266\345\255\230\346\224\276\347\233\256\345\275\225\357\274\232", Q_NULLPTR));
        Edit_StorageDir->setText(QString());
        Btn_DirSelect->setText(QApplication::translate("MainWindow", " \351\200\211\346\213\251\347\233\256\345\275\225", Q_NULLPTR));
        Btn_Read->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\350\257\273\345\217\226", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\345\220\215\357\274\232", Q_NULLPTR));
        Btn_SelectFile->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\351\200\211\346\213\251", Q_NULLPTR));
        label_WirteEnable->setText(QApplication::translate("MainWindow", " \346\234\252\346\277\200\346\264\273", Q_NULLPTR));
        Btn_Write->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\345\206\231\345\205\245", Q_NULLPTR));
        Btn_DirClear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_FileTransfer), QApplication::translate("MainWindow", "\346\226\207\344\273\266\344\274\240\350\276\223", Q_NULLPTR));
        Btn_ReadValueNum->setText(QApplication::translate("MainWindow", "\350\257\273\345\256\232\345\200\274\345\214\272\345\217\267", Q_NULLPTR));
        Btn_SetValueNum->setText(QApplication::translate("MainWindow", "\345\210\207\346\215\242\345\256\232\345\200\274\345\214\272\345\217\267", Q_NULLPTR));
        Btn_RemoteValue->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", " \345\214\272\345\217\267\357\274\232", Q_NULLPTR));
        Btn_InputObjAddr->setText(QApplication::translate("MainWindow", "\350\276\223\345\205\245\344\277\241\346\201\257\344\275\223\345\234\260\345\235\200", Q_NULLPTR));
        Btn_ObjAddrClear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        Btn_ReadValue->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\202\346\225\260", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\345\214\272\345\217\267\357\274\232", Q_NULLPTR));
        Btn_ValueAdd->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240", Q_NULLPTR));
        Btn_ValueClear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        radioButton_nseq->setText(QApplication::translate("MainWindow", "\347\246\273\346\225\243", Q_NULLPTR));
        radioButton_seq->setText(QApplication::translate("MainWindow", "\351\241\272\345\272\217", Q_NULLPTR));
        Btn_Preset->setText(QApplication::translate("MainWindow", "\345\217\202\346\225\260\351\242\204\347\275\256", Q_NULLPTR));
        Btn_PresetCancel->setText(QApplication::translate("MainWindow", "\345\217\226\346\266\210\351\242\204\347\275\256", Q_NULLPTR));
        Btn_Curing->setText(QApplication::translate("MainWindow", "\345\217\202\346\225\260\345\233\272\345\214\226", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_RemoteValue), QApplication::translate("MainWindow", "\350\277\234\347\250\213\345\217\202\346\225\260\350\257\273\345\206\231", Q_NULLPTR));
        Btn_EnergyCall->setText(QApplication::translate("MainWindow", "\347\224\265\350\203\275\351\207\217\345\217\254\345\224\244", Q_NULLPTR));
        Btn_EnergyCall_Clear->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_ElectricalEnergyCall), QApplication::translate("MainWindow", "\347\224\265\350\203\275\351\207\217\345\217\254\345\224\244", Q_NULLPTR));
        Btn_SelectVersionFile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\215\207\347\272\247\346\226\207\344\273\266", Q_NULLPTR));
        Btn_UpdataVersion->setText(QApplication::translate("MainWindow", "\350\275\257\344\273\266\345\215\207\347\272\247\344\274\240\350\276\223", Q_NULLPTR));
        UpdataLabel->setText(QApplication::translate("MainWindow", "\346\234\252\346\277\200\346\264\273", Q_NULLPTR));
        Btn_UpdataStop->setText(QApplication::translate("MainWindow", "\344\270\255\346\226\255\345\215\207\347\272\247", Q_NULLPTR));
        Btn_RunUpdata->setText(QApplication::translate("MainWindow", "\347\241\256\350\256\244\345\215\207\347\272\247", Q_NULLPTR));
        Btn_AbandonUpdata->setText(QApplication::translate("MainWindow", "\346\224\276\345\274\203\345\215\207\347\272\247", Q_NULLPTR));
        DateWidget->setTabText(DateWidget->indexOf(tab_VersionUpdata), QApplication::translate("MainWindow", "\350\275\257\344\273\266\345\215\207\347\272\247", Q_NULLPTR));
        LogWidget->setTabText(LogWidget->indexOf(tab), QString());
        LogWidget->setTabText(LogWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\346\216\245\346\224\266\346\266\210\346\201\257", Q_NULLPTR));
        LogWidget->setTabText(LogWidget->indexOf(tab_4), QApplication::translate("MainWindow", "\350\260\203\350\257\225\346\266\210\346\201\257", Q_NULLPTR));
        LogWidget->setTabText(LogWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\346\265\213\350\257\225\351\223\276\350\267\257\346\266\210\346\201\257", Q_NULLPTR));
        LogWidget->setTabText(LogWidget->indexOf(tab_5), QApplication::translate("MainWindow", "\344\270\212\346\212\245\346\266\210\346\201\257", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\345\274\200\345\247\213", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\351\200\211\351\241\271", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("MainWindow", "\345\205\263\344\272\216", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

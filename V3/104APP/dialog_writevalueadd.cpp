#include "dialog_writevalueadd.h"
#include "ui_dialog_writevalueadd.h"

Dialog_WriteValueAdd::Dialog_WriteValueAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_WriteValueAdd)
{
    ui->setupUi(this);
}

Dialog_WriteValueAdd::~Dialog_WriteValueAdd()
{
    delete ui;
}

void Dialog_WriteValueAdd::on_buttonBox_accepted()
{
    QString Addr = ui->Edit_Addr->text();
    QString Str = QString("%1").arg(Addr.toInt(), 6, 10, QLatin1Char('0'));
    QString Tag = ui->Edit_Tag->text();
    Tag = QString("%1").arg(Tag.toInt(), 2, 16, QLatin1Char('0'));
    QString Value = ui->Edit_Value->text();
    Value = QString("%1").arg(Value.toInt(), 2, 16, QLatin1Char('0'));
    emit DialogWriteValue(Str,Tag,Value);
}

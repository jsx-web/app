#ifndef DIALOG_WRITEVALUEADD_H
#define DIALOG_WRITEVALUEADD_H

#include <QDialog>

namespace Ui {
class Dialog_WriteValueAdd;
}

class Dialog_WriteValueAdd : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_WriteValueAdd(QWidget *parent = 0);
    ~Dialog_WriteValueAdd();

signals:
    void DialogWriteValue(QString addr,QString Tag ,QString Value);
private slots:
    void on_buttonBox_accepted();
private:
    Ui::Dialog_WriteValueAdd *ui;
};

#endif // DIALOG_WRITEVALUEADD_H

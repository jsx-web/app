#ifndef READFILEDIALOG_H
#define READFILEDIALOG_H

#include <QDialog>
#include "config.h"

namespace Ui {
class ReadFileDialog;
}

class ReadFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReadFileDialog(QWidget *parent = 0);
    ~ReadFileDialog();

private slots:
    void on_Btn_Enable_clicked();

private:
    Ui::ReadFileDialog *ui;
    
private slots:
    void WidgetReadFile_res(bool end, char *pack_date);
signals:
    void EnablSuccess(bool ret,QString name);
};

#endif // READFILEDIALOG_H

#ifndef DIALOG_SHOWALL_H
#define DIALOG_SHOWALL_H

#include <QDialog>

namespace Ui {
class Dialog_ShowAll;
}

class Dialog_ShowAll : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ShowAll(QWidget *parent = 0);
    ~Dialog_ShowAll();

private:
    Ui::Dialog_ShowAll *ui;
};

#endif // DIALOG_SHOWALL_H

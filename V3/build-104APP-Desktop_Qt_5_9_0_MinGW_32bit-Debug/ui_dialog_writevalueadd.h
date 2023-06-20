/********************************************************************************
** Form generated from reading UI file 'dialog_writevalueadd.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_WRITEVALUEADD_H
#define UI_DIALOG_WRITEVALUEADD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog_WriteValueAdd
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *Edit_Addr;
    QLabel *label_2;
    QLineEdit *Edit_Tag;
    QLabel *label_3;
    QLineEdit *Edit_Value;

    void setupUi(QDialog *Dialog_WriteValueAdd)
    {
        if (Dialog_WriteValueAdd->objectName().isEmpty())
            Dialog_WriteValueAdd->setObjectName(QStringLiteral("Dialog_WriteValueAdd"));
        Dialog_WriteValueAdd->resize(262, 177);
        buttonBox = new QDialogButtonBox(Dialog_WriteValueAdd);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(40, 130, 161, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(Dialog_WriteValueAdd);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 50, 241, 74));
        formLayout = new QFormLayout(layoutWidget);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        Edit_Addr = new QLineEdit(layoutWidget);
        Edit_Addr->setObjectName(QStringLiteral("Edit_Addr"));

        formLayout->setWidget(0, QFormLayout::FieldRole, Edit_Addr);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        Edit_Tag = new QLineEdit(layoutWidget);
        Edit_Tag->setObjectName(QStringLiteral("Edit_Tag"));

        formLayout->setWidget(1, QFormLayout::FieldRole, Edit_Tag);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        Edit_Value = new QLineEdit(layoutWidget);
        Edit_Value->setObjectName(QStringLiteral("Edit_Value"));

        formLayout->setWidget(2, QFormLayout::FieldRole, Edit_Value);


        retranslateUi(Dialog_WriteValueAdd);
        QObject::connect(buttonBox, SIGNAL(accepted()), Dialog_WriteValueAdd, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Dialog_WriteValueAdd, SLOT(reject()));

        QMetaObject::connectSlotsByName(Dialog_WriteValueAdd);
    } // setupUi

    void retranslateUi(QDialog *Dialog_WriteValueAdd)
    {
        Dialog_WriteValueAdd->setWindowTitle(QApplication::translate("Dialog_WriteValueAdd", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog_WriteValueAdd", "\345\234\260\345\235\200(0x000000)\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("Dialog_WriteValueAdd", "Tag(\345\215\201\350\277\233\345\210\266)\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("Dialog_WriteValueAdd", "\345\200\274(\345\215\201\350\277\233\345\210\266)\357\274\232", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog_WriteValueAdd: public Ui_Dialog_WriteValueAdd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_WRITEVALUEADD_H

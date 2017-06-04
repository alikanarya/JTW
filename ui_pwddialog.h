/********************************************************************************
** Form generated from reading UI file 'pwddialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PWDDIALOG_H
#define UI_PWDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_pwdDialog
{
public:
    QLineEdit *lineEdit;
    QPushButton *acceptButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *pwdDialog)
    {
        if (pwdDialog->objectName().isEmpty())
            pwdDialog->setObjectName(QStringLiteral("pwdDialog"));
        pwdDialog->setWindowModality(Qt::WindowModal);
        pwdDialog->resize(220, 85);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        pwdDialog->setWindowIcon(icon);
        lineEdit = new QLineEdit(pwdDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 10, 200, 25));
        QFont font;
        font.setPointSize(10);
        lineEdit->setFont(font);
        lineEdit->setEchoMode(QLineEdit::Password);
        acceptButton = new QPushButton(pwdDialog);
        acceptButton->setObjectName(QStringLiteral("acceptButton"));
        acceptButton->setGeometry(QRect(10, 50, 80, 25));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        acceptButton->setFont(font1);
        cancelButton = new QPushButton(pwdDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(130, 50, 80, 25));
        cancelButton->setFont(font1);

        retranslateUi(pwdDialog);
        QObject::connect(acceptButton, SIGNAL(clicked()), pwdDialog, SLOT(acceptButton()));
        QObject::connect(cancelButton, SIGNAL(clicked()), pwdDialog, SLOT(cancelButton()));
        QObject::connect(lineEdit, SIGNAL(returnPressed()), pwdDialog, SLOT(acceptButton()));

        QMetaObject::connectSlotsByName(pwdDialog);
    } // setupUi

    void retranslateUi(QDialog *pwdDialog)
    {
        pwdDialog->setWindowTitle(QApplication::translate("pwdDialog", "\305\236ifre Giri\305\237i", 0));
        acceptButton->setText(QApplication::translate("pwdDialog", "Tamam", 0));
        cancelButton->setText(QApplication::translate("pwdDialog", "\304\260ptal", 0));
    } // retranslateUi

};

namespace Ui {
    class pwdDialog: public Ui_pwdDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PWDDIALOG_H

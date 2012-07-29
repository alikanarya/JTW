/********************************************************************************
** Form generated from reading UI file 'pwddialog.ui'
**
** Created: Sat 28. Jul 01:23:03 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PWDDIALOG_H
#define UI_PWDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

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
            pwdDialog->setObjectName(QString::fromUtf8("pwdDialog"));
        pwdDialog->setWindowModality(Qt::WindowModal);
        pwdDialog->resize(220, 85);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        pwdDialog->setWindowIcon(icon);
        lineEdit = new QLineEdit(pwdDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(10, 10, 200, 25));
        QFont font;
        font.setPointSize(10);
        lineEdit->setFont(font);
        lineEdit->setEchoMode(QLineEdit::Password);
        acceptButton = new QPushButton(pwdDialog);
        acceptButton->setObjectName(QString::fromUtf8("acceptButton"));
        acceptButton->setGeometry(QRect(10, 50, 80, 25));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        acceptButton->setFont(font1);
        cancelButton = new QPushButton(pwdDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
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
        pwdDialog->setWindowTitle(QApplication::translate("pwdDialog", "\305\236ifre Giri\305\237i", 0, QApplication::UnicodeUTF8));
        acceptButton->setText(QApplication::translate("pwdDialog", "Tamam", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("pwdDialog", "\304\260ptal", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class pwdDialog: public Ui_pwdDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PWDDIALOG_H

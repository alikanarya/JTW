/********************************************************************************
** Form generated from reading UI file 'exitdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXITDIALOG_H
#define UI_EXITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_exitDialog
{
public:
    QLabel *label;
    QPushButton *acceptButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *exitDialog)
    {
        if (exitDialog->objectName().isEmpty())
            exitDialog->setObjectName(QStringLiteral("exitDialog"));
        exitDialog->setWindowModality(Qt::WindowModal);
        exitDialog->resize(242, 90);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitDialog->setWindowIcon(icon);
        label = new QLabel(exitDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 199, 31));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(14);
        font.setBold(true);
        font.setItalic(false);
        font.setUnderline(true);
        font.setWeight(75);
        font.setStrikeOut(false);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        acceptButton = new QPushButton(exitDialog);
        acceptButton->setObjectName(QStringLiteral("acceptButton"));
        acceptButton->setGeometry(QRect(20, 50, 80, 25));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        acceptButton->setFont(font1);
        cancelButton = new QPushButton(exitDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(140, 50, 80, 25));
        cancelButton->setFont(font1);

        retranslateUi(exitDialog);
        QObject::connect(acceptButton, SIGNAL(clicked()), exitDialog, SLOT(acceptButton()));
        QObject::connect(cancelButton, SIGNAL(clicked()), exitDialog, SLOT(cancelButton()));

        QMetaObject::connectSlotsByName(exitDialog);
    } // setupUi

    void retranslateUi(QDialog *exitDialog)
    {
        exitDialog->setWindowTitle(QApplication::translate("exitDialog", "\303\207\304\261k\304\261\305\237 Onay\304\261", 0));
        label->setText(QApplication::translate("exitDialog", "Program\304\261 Kapat?", 0));
        acceptButton->setText(QApplication::translate("exitDialog", "Evet", 0));
        cancelButton->setText(QApplication::translate("exitDialog", "Hay\304\261r", 0));
    } // retranslateUi

};

namespace Ui {
    class exitDialog: public Ui_exitDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXITDIALOG_H

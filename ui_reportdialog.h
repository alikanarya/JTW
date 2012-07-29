/********************************************************************************
** Form generated from reading UI file 'reportdialog.ui'
**
** Created: Sat 28. Jul 01:23:03 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTDIALOG_H
#define UI_REPORTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_reportDialog
{
public:
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *reportDialog)
    {
        if (reportDialog->objectName().isEmpty())
            reportDialog->setObjectName(QString::fromUtf8("reportDialog"));
        reportDialog->setWindowModality(Qt::WindowModal);
        reportDialog->resize(600, 360);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(reportDialog->sizePolicy().hasHeightForWidth());
        reportDialog->setSizePolicy(sizePolicy);
        reportDialog->setMinimumSize(QSize(600, 360));
        reportDialog->setMaximumSize(QSize(600, 360));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        reportDialog->setWindowIcon(icon);
        plainTextEdit = new QPlainTextEdit(reportDialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 10, 581, 341));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        plainTextEdit->setFont(font);
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 127);"));

        retranslateUi(reportDialog);

        QMetaObject::connectSlotsByName(reportDialog);
    } // setupUi

    void retranslateUi(QDialog *reportDialog)
    {
        reportDialog->setWindowTitle(QApplication::translate("reportDialog", "Rapor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class reportDialog: public Ui_reportDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTDIALOG_H

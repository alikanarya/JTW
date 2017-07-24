/********************************************************************************
** Form generated from reading UI file 'reportdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTDIALOG_H
#define UI_REPORTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_reportDialog
{
public:
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *reportDialog)
    {
        if (reportDialog->objectName().isEmpty())
            reportDialog->setObjectName(QStringLiteral("reportDialog"));
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
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        reportDialog->setWindowIcon(icon);
        plainTextEdit = new QPlainTextEdit(reportDialog);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 10, 581, 341));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        plainTextEdit->setFont(font);
        plainTextEdit->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 195);"));
        plainTextEdit->setReadOnly(true);

        retranslateUi(reportDialog);

        QMetaObject::connectSlotsByName(reportDialog);
    } // setupUi

    void retranslateUi(QDialog *reportDialog)
    {
        reportDialog->setWindowTitle(QApplication::translate("reportDialog", "Rapor", 0));
    } // retranslateUi

};

namespace Ui {
    class reportDialog: public Ui_reportDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTDIALOG_H

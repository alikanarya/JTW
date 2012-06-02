/********************************************************************************
** Form generated from reading UI file 'zctrldialog.ui'
**
** Created: Sat 2. Jun 13:54:58 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZCTRLDIALOG_H
#define UI_ZCTRLDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_zctrlDialog
{
public:

    void setupUi(QDialog *zctrlDialog)
    {
        if (zctrlDialog->objectName().isEmpty())
            zctrlDialog->setObjectName(QString::fromUtf8("zctrlDialog"));
        zctrlDialog->setWindowModality(Qt::NonModal);
        zctrlDialog->resize(400, 300);

        retranslateUi(zctrlDialog);

        QMetaObject::connectSlotsByName(zctrlDialog);
    } // setupUi

    void retranslateUi(QDialog *zctrlDialog)
    {
        zctrlDialog->setWindowTitle(QApplication::translate("zctrlDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class zctrlDialog: public Ui_zctrlDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZCTRLDIALOG_H

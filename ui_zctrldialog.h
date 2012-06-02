/********************************************************************************
** Form generated from reading UI file 'zctrldialog.ui'
**
** Created: Sat 2. Jun 18:52:58 2012
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
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_zctrlDialog
{
public:
    QCheckBox *zctrlBox;

    void setupUi(QDialog *zctrlDialog)
    {
        if (zctrlDialog->objectName().isEmpty())
            zctrlDialog->setObjectName(QString::fromUtf8("zctrlDialog"));
        zctrlDialog->setWindowModality(Qt::WindowModal);
        zctrlDialog->resize(400, 300);
        zctrlBox = new QCheckBox(zctrlDialog);
        zctrlBox->setObjectName(QString::fromUtf8("zctrlBox"));
        zctrlBox->setGeometry(QRect(20, 30, 131, 17));
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        zctrlBox->setFont(font);

        retranslateUi(zctrlDialog);
        QObject::connect(zctrlBox, SIGNAL(stateChanged(int)), zctrlDialog, SLOT(zctrlBox()));

        QMetaObject::connectSlotsByName(zctrlDialog);
    } // setupUi

    void retranslateUi(QDialog *zctrlDialog)
    {
        zctrlDialog->setWindowTitle(QApplication::translate("zctrlDialog", "Y\303\274kseklik Kontrol\303\274", 0, QApplication::UnicodeUTF8));
        zctrlBox->setText(QApplication::translate("zctrlDialog", "Y\303\274kseklik Kontrol\303\274", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class zctrlDialog: public Ui_zctrlDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZCTRLDIALOG_H

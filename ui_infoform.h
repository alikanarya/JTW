/********************************************************************************
** Form generated from reading UI file 'infoform.ui'
**
** Created: Fri 28. Sep 19:43:05 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOFORM_H
#define UI_INFOFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_infoForm
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;

    void setupUi(QWidget *infoForm)
    {
        if (infoForm->objectName().isEmpty())
            infoForm->setObjectName(QString::fromUtf8("infoForm"));
        infoForm->setWindowModality(Qt::WindowModal);
        infoForm->resize(400, 200);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(infoForm->sizePolicy().hasHeightForWidth());
        infoForm->setSizePolicy(sizePolicy);
        infoForm->setMinimumSize(QSize(400, 200));
        infoForm->setMaximumSize(QSize(400, 200));
        infoForm->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        infoForm->setWindowIcon(icon);
        label = new QLabel(infoForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(5, 30, 391, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Pristina"));
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(85, 0, 255);"));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(infoForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(5, 60, 391, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Pristina"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 255);"));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(infoForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(5, 90, 392, 32));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(0, 170, 255);"));
        label_3->setAlignment(Qt::AlignCenter);
        label_4 = new QLabel(infoForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(5, 120, 392, 32));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        label_4->setFont(font2);
        label_4->setAlignment(Qt::AlignCenter);
        label_5 = new QLabel(infoForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 176, 361, 20));
        label_5->setFont(font2);

        retranslateUi(infoForm);

        QMetaObject::connectSlotsByName(infoForm);
    } // setupUi

    void retranslateUi(QWidget *infoForm)
    {
        infoForm->setWindowTitle(QApplication::translate("infoForm", "Hakk\304\261nda", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("infoForm", "JTW", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("infoForm", "Joint Tracker for Welding", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("infoForm", "Kaynak i\303\247in Birle\305\237me Yeri Takip\303\247isi", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("infoForm", "ver 1.9.34, \302\251Orun Elektronik, 2012", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("infoForm", "T\303\274m haklar\304\261 sakl\304\261d\304\261r. All rights reserved.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class infoForm: public Ui_infoForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOFORM_H

/********************************************************************************
** Form generated from reading UI file 'analyzedialog.ui'
**
** Created: Sun 27. Mar 01:57:43 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYZEDIALOG_H
#define UI_ANALYZEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_analyzeDialog
{
public:
    QLabel *labelMono;
    QLabel *labelAnalyze;
    QPlainTextEdit *plainTextEdit;
    QLabel *label;
    QPushButton *acceptButton;
    QPushButton *cancelButton;
    QPushButton *cancelButton_2;

    void setupUi(QDialog *analyzeDialog)
    {
        if (analyzeDialog->objectName().isEmpty())
            analyzeDialog->setObjectName(QString::fromUtf8("analyzeDialog"));
        analyzeDialog->setWindowModality(Qt::WindowModal);
        analyzeDialog->resize(670, 529);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        analyzeDialog->setWindowIcon(icon);
        labelMono = new QLabel(analyzeDialog);
        labelMono->setObjectName(QString::fromUtf8("labelMono"));
        labelMono->setGeometry(QRect(10, 10, 320, 100));
        labelMono->setFrameShape(QFrame::Box);
        labelAnalyze = new QLabel(analyzeDialog);
        labelAnalyze->setObjectName(QString::fromUtf8("labelAnalyze"));
        labelAnalyze->setGeometry(QRect(340, 10, 320, 100));
        labelAnalyze->setFrameShape(QFrame::Box);
        plainTextEdit = new QPlainTextEdit(analyzeDialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 120, 651, 331));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        plainTextEdit->setFont(font);
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit->setReadOnly(true);
        label = new QLabel(analyzeDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 460, 651, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        label->setAlignment(Qt::AlignCenter);
        acceptButton = new QPushButton(analyzeDialog);
        acceptButton->setObjectName(QString::fromUtf8("acceptButton"));
        acceptButton->setGeometry(QRect(220, 490, 80, 25));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        acceptButton->setFont(font2);
        acceptButton->setStyleSheet(QString::fromUtf8("color: rgb(0, 255, 127);"));
        cancelButton = new QPushButton(analyzeDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(360, 490, 80, 25));
        cancelButton->setFont(font2);
        cancelButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        cancelButton_2 = new QPushButton(analyzeDialog);
        cancelButton_2->setObjectName(QString::fromUtf8("cancelButton_2"));
        cancelButton_2->setGeometry(QRect(580, 490, 80, 25));
        cancelButton_2->setFont(font2);
        cancelButton_2->setStyleSheet(QString::fromUtf8(""));

        retranslateUi(analyzeDialog);
        QObject::connect(acceptButton, SIGNAL(clicked()), analyzeDialog, SLOT(acceptButton()));
        QObject::connect(cancelButton, SIGNAL(clicked()), analyzeDialog, SLOT(cancelButton()));
        QObject::connect(cancelButton_2, SIGNAL(clicked()), analyzeDialog, SLOT(sampleButton()));

        QMetaObject::connectSlotsByName(analyzeDialog);
    } // setupUi

    void retranslateUi(QDialog *analyzeDialog)
    {
        analyzeDialog->setWindowTitle(QApplication::translate("analyzeDialog", "Analiz Sonucu", 0, QApplication::UnicodeUTF8));
        labelMono->setText(QString());
        labelAnalyze->setText(QString());
        label->setText(QApplication::translate("analyzeDialog", "ONAYLIYOR MUSUNUZ ?", 0, QApplication::UnicodeUTF8));
        acceptButton->setText(QApplication::translate("analyzeDialog", "EVET", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("analyzeDialog", "HAYIR", 0, QApplication::UnicodeUTF8));
        cancelButton_2->setText(QApplication::translate("analyzeDialog", "\303\226RNEK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class analyzeDialog: public Ui_analyzeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYZEDIALOG_H

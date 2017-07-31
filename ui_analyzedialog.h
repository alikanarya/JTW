/********************************************************************************
** Form generated from reading UI file 'analyzedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANALYZEDIALOG_H
#define UI_ANALYZEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

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
            analyzeDialog->setObjectName(QStringLiteral("analyzeDialog"));
        analyzeDialog->setWindowModality(Qt::WindowModal);
        analyzeDialog->resize(670, 760);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        analyzeDialog->setWindowIcon(icon);
        labelMono = new QLabel(analyzeDialog);
        labelMono->setObjectName(QStringLiteral("labelMono"));
        labelMono->setGeometry(QRect(10, 10, 320, 320));
        labelMono->setFrameShape(QFrame::Box);
        labelMono->setScaledContents(true);
        labelAnalyze = new QLabel(analyzeDialog);
        labelAnalyze->setObjectName(QStringLiteral("labelAnalyze"));
        labelAnalyze->setGeometry(QRect(340, 10, 320, 320));
        labelAnalyze->setFrameShape(QFrame::Box);
        labelAnalyze->setScaledContents(true);
        labelAnalyze->setAlignment(Qt::AlignCenter);
        plainTextEdit = new QPlainTextEdit(analyzeDialog);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 340, 651, 331));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        plainTextEdit->setFont(font);
        plainTextEdit->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 195);"));
        plainTextEdit->setReadOnly(true);
        label = new QLabel(analyzeDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 680, 651, 20));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        label->setAlignment(Qt::AlignCenter);
        acceptButton = new QPushButton(analyzeDialog);
        acceptButton->setObjectName(QStringLiteral("acceptButton"));
        acceptButton->setGeometry(QRect(213, 710, 100, 40));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        acceptButton->setFont(font2);
        cancelButton = new QPushButton(analyzeDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(353, 710, 100, 40));
        cancelButton->setFont(font2);
        cancelButton_2 = new QPushButton(analyzeDialog);
        cancelButton_2->setObjectName(QStringLiteral("cancelButton_2"));
        cancelButton_2->setEnabled(false);
        cancelButton_2->setGeometry(QRect(580, 710, 80, 25));
        QFont font3;
        font3.setFamily(QStringLiteral("Arial"));
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        cancelButton_2->setFont(font3);
        cancelButton_2->setStyleSheet(QStringLiteral(""));

        retranslateUi(analyzeDialog);
        QObject::connect(acceptButton, SIGNAL(clicked()), analyzeDialog, SLOT(acceptButton()));
        QObject::connect(cancelButton, SIGNAL(clicked()), analyzeDialog, SLOT(cancelButton()));
        QObject::connect(cancelButton_2, SIGNAL(clicked()), analyzeDialog, SLOT(sampleButton()));

        QMetaObject::connectSlotsByName(analyzeDialog);
    } // setupUi

    void retranslateUi(QDialog *analyzeDialog)
    {
        analyzeDialog->setWindowTitle(QApplication::translate("analyzeDialog", "Analiz Sonucu", 0));
        labelMono->setText(QString());
        labelAnalyze->setText(QString());
        label->setText(QApplication::translate("analyzeDialog", "ONAYLIYOR MUSUNUZ ?", 0));
        acceptButton->setText(QApplication::translate("analyzeDialog", "EVET", 0));
        cancelButton->setText(QApplication::translate("analyzeDialog", "HAYIR", 0));
        cancelButton_2->setText(QApplication::translate("analyzeDialog", "\303\226RNEK", 0));
    } // retranslateUi

};

namespace Ui {
    class analyzeDialog: public Ui_analyzeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANALYZEDIALOG_H

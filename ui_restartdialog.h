/********************************************************************************
** Form generated from reading UI file 'restartdialog.ui'
**
** Created: Sat 28. Jul 01:23:03 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTARTDIALOG_H
#define UI_RESTARTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_restartDialog
{
public:
    QPushButton *cancelButton;
    QPushButton *acceptButton;
    QTextEdit *textEdit;

    void setupUi(QDialog *restartDialog)
    {
        if (restartDialog->objectName().isEmpty())
            restartDialog->setObjectName(QString::fromUtf8("restartDialog"));
        restartDialog->setWindowModality(Qt::WindowModal);
        restartDialog->resize(391, 223);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        restartDialog->setWindowIcon(icon);
        cancelButton = new QPushButton(restartDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(240, 190, 80, 25));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        cancelButton->setFont(font);
        acceptButton = new QPushButton(restartDialog);
        acceptButton->setObjectName(QString::fromUtf8("acceptButton"));
        acceptButton->setGeometry(QRect(70, 190, 80, 25));
        acceptButton->setFont(font);
        textEdit = new QTextEdit(restartDialog);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, 10, 371, 161));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        textEdit->setFont(font1);
        textEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(236, 233, 216);"));
        textEdit->setFrameShape(QFrame::NoFrame);

        retranslateUi(restartDialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), restartDialog, SLOT(cancelButton()));
        QObject::connect(acceptButton, SIGNAL(clicked()), restartDialog, SLOT(acceptButton()));

        QMetaObject::connectSlotsByName(restartDialog);
    } // setupUi

    void retranslateUi(QDialog *restartDialog)
    {
        restartDialog->setWindowTitle(QApplication::translate("restartDialog", "Dialog", 0));
        cancelButton->setText(QApplication::translate("restartDialog", "\304\260ptal", 0));
        acceptButton->setText(QApplication::translate("restartDialog", "Kabul", 0));
/*
        restartDialog->setWindowTitle(QApplication::translate("restartDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("restartDialog", "\304\260ptal", 0, QApplication::UnicodeUTF8));
        acceptButton->setText(QApplication::translate("restartDialog", "Kabul", 0, QApplication::UnicodeUTF8));
*/
        textEdit->setHtml(QApplication::translate("restartDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">KABUL SE\303\207\304\260LD\304\260\304\236\304\260NDE</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">PROGRAM OTOMAT\304\260K OLARAK YEN\304\260DEN BA\305\236LATILACAK VE</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">YEN\304\260 G\304\260R\304"
                        "\260LEN AYARLAR \304\260LE \303\207ALI\305\236ACAKTIR.</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600;\"></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">BU \304\260\305\236LEM\304\260 \303\234RET\304\260M ANINDA YAPMAYINIZ.</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">\304\260\305\236LEM\304\260N YARIM KALMASINA NEDEN OLACAKTIR.</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600;\"></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin"
                        "-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ff0000;\">EM\304\260N M\304\260S\304\260N\304\260Z !!!</span></p></body></html>", 0));
//                      "-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ff0000;\">EM\304\260N M\304\260S\304\260N\304\260Z !!!</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class restartDialog: public Ui_restartDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTARTDIALOG_H

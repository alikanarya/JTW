/********************************************************************************
** Form generated from reading UI file 'licensedialog.ui'
**
** Created: Tue Dec 21 01:13:39 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LICENSEDIALOG_H
#define UI_LICENSEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_licenseDialog
{
public:
    QLineEdit *lineEdit;
    QLabel *label;
    QTextBrowser *textBrowser;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton;

    void setupUi(QDialog *licenseDialog)
    {
        if (licenseDialog->objectName().isEmpty())
            licenseDialog->setObjectName(QString::fromUtf8("licenseDialog"));
        licenseDialog->setWindowModality(Qt::WindowModal);
        licenseDialog->resize(494, 396);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(licenseDialog->sizePolicy().hasHeightForWidth());
        licenseDialog->setSizePolicy(sizePolicy);
        licenseDialog->setMinimumSize(QSize(494, 396));
        licenseDialog->setMaximumSize(QSize(494, 396));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        licenseDialog->setWindowIcon(icon);
        lineEdit = new QLineEdit(licenseDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(160, 100, 281, 31));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        lineEdit->setFont(font);
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setReadOnly(true);
        label = new QLabel(licenseDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(50, 110, 91, 16));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        textBrowser = new QTextBrowser(licenseDialog);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(20, 140, 451, 211));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Arial"));
        font2.setPointSize(10);
        textBrowser->setFont(font2);
        textBrowser->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit = new QPlainTextEdit(licenseDialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(20, 10, 451, 81));
        plainTextEdit->setFont(font1);
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit->setReadOnly(true);
        pushButton = new QPushButton(licenseDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(210, 360, 80, 25));
        pushButton->setFont(font1);

        retranslateUi(licenseDialog);
        QObject::connect(pushButton, SIGNAL(clicked()), licenseDialog, SLOT(closeButton()));

        QMetaObject::connectSlotsByName(licenseDialog);
    } // setupUi

    void retranslateUi(QDialog *licenseDialog)
    {
        licenseDialog->setWindowTitle(QApplication::translate("licenseDialog", "L\304\260SANS UYARISI", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("licenseDialog", "48-48-50-50-49-53-52-67-52-48-69-67", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("licenseDialog", "\303\234r\303\274n Anahtar\304\261", 0, QApplication::UnicodeUTF8));
        textBrowser->setHtml(QApplication::translate("licenseDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">* Her bilgisayar i\303\247in ayr\304\261 lisans dosyas\304\261 al\304\261nmal\304\261d\304\261r.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">* Lisans kodu, bilgisayara ba\304\237l\304\261 verilir.</sp"
                        "an></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">* Bilgisayar konfig\303\274rasyonu veya yaz\304\261l\304\261m\304\261 de\304\237i\305\237ince, mevcut lisans dosyas\304\261 kullan\304\261lmaz hale gelebilir. Bu durumda yukar\304\261daki \303\274r\303\274n anahtar\304\261n\304\261 Aykurt Elektri\304\237e bildirerek lisans dosyas\304\261 talep ediniz.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell"
                        " Dlg 2';\">Detayl\304\261 bilgi i\303\247in Aykurt Elektrik ile temas kurunuz.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-weight:600;\">Program Kapat\304\261lacakt\304\261r !</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("licenseDialog", "Tamam", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class licenseDialog: public Ui_licenseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICENSEDIALOG_H

/********************************************************************************
** Form generated from reading UI file 'licensedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LICENSEDIALOG_H
#define UI_LICENSEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

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
            licenseDialog->setObjectName(QStringLiteral("licenseDialog"));
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
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        licenseDialog->setWindowIcon(icon);
        lineEdit = new QLineEdit(licenseDialog);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(160, 100, 281, 31));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        lineEdit->setFont(font);
        lineEdit->setAlignment(Qt::AlignCenter);
        lineEdit->setReadOnly(true);
        label = new QLabel(licenseDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 110, 91, 16));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        textBrowser = new QTextBrowser(licenseDialog);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(20, 140, 451, 211));
        QFont font2;
        font2.setFamily(QStringLiteral("Arial"));
        font2.setPointSize(10);
        textBrowser->setFont(font2);
        textBrowser->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        plainTextEdit = new QPlainTextEdit(licenseDialog);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(20, 10, 451, 81));
        plainTextEdit->setFont(font1);
        plainTextEdit->setStyleSheet(QStringLiteral("background-color: rgb(170, 255, 127);"));
        plainTextEdit->setReadOnly(true);
        pushButton = new QPushButton(licenseDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(210, 360, 80, 25));
        pushButton->setFont(font1);

        retranslateUi(licenseDialog);
        QObject::connect(pushButton, SIGNAL(clicked()), licenseDialog, SLOT(closeButton()));

        QMetaObject::connectSlotsByName(licenseDialog);
    } // setupUi

    void retranslateUi(QDialog *licenseDialog)
    {
        licenseDialog->setWindowTitle(QApplication::translate("licenseDialog", "L\304\260SANS UYARISI", 0));
        lineEdit->setText(QApplication::translate("licenseDialog", "48-48-50-50-49-53-52-67-52-48-69-67", 0));
        label->setText(QApplication::translate("licenseDialog", "\303\234r\303\274n Anahtar\304\261", 0));
        textBrowser->setHtml(QApplication::translate("licenseDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">* Her bilgisayar i\303\247in ayr\304\261 lisans dosyas\304\261 al\304\261nmal\304\261d\304\261r.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">* Lisans kodu, bilgisayara ba\304\237l\304\261 verili"
                        "r.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">* Bilgisayar konfig\303\274rasyonu veya yaz\304\261l\304\261m\304\261 de\304\237i\305\237ince, mevcut lisans dosyas\304\261 kullan\304\261lmaz hale gelebilir. Bu durumda yukar\304\261daki \303\274r\303\274n anahtar\304\261n\304\261 Orun Elektroni\304\237e bildirerek lisans dosyas\304\261 talep ediniz.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" fon"
                        "t-family:'MS Shell Dlg 2';\">Detayl\304\261 bilgi i\303\247in Orun Elektronik ile temas kurunuz.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-weight:600;\">Program Kapat\304\261lacakt\304\261r !</span></p></body></html>", 0));
        pushButton->setText(QApplication::translate("licenseDialog", "Tamam", 0));
    } // retranslateUi

};

namespace Ui {
    class licenseDialog: public Ui_licenseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LICENSEDIALOG_H

/********************************************************************************
** Form generated from reading UI file 'sampledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAMPLEDIALOG_H
#define UI_SAMPLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_sampleDialog
{
public:
    QLabel *labelMonoCorrect;
    QLabel *labelAnalyzeCorrect;
    QLabel *labelMonoWrong;
    QLabel *labelAnalyzeWrong;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;

    void setupUi(QDialog *sampleDialog)
    {
        if (sampleDialog->objectName().isEmpty())
            sampleDialog->setObjectName(QStringLiteral("sampleDialog"));
        sampleDialog->setWindowModality(Qt::WindowModal);
        sampleDialog->resize(720, 294);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        sampleDialog->setWindowIcon(icon);
        labelMonoCorrect = new QLabel(sampleDialog);
        labelMonoCorrect->setObjectName(QStringLiteral("labelMonoCorrect"));
        labelMonoCorrect->setGeometry(QRect(10, 40, 320, 100));
        labelMonoCorrect->setFrameShape(QFrame::Box);
        labelMonoCorrect->setPixmap(QPixmap(QString::fromUtf8(":/pixmaps/resources/good_mono.jpg")));
        labelAnalyzeCorrect = new QLabel(sampleDialog);
        labelAnalyzeCorrect->setObjectName(QStringLiteral("labelAnalyzeCorrect"));
        labelAnalyzeCorrect->setGeometry(QRect(10, 150, 320, 100));
        labelAnalyzeCorrect->setFrameShape(QFrame::Box);
        labelAnalyzeCorrect->setPixmap(QPixmap(QString::fromUtf8(":/pixmaps/resources/good_corner.jpg")));
        labelMonoWrong = new QLabel(sampleDialog);
        labelMonoWrong->setObjectName(QStringLiteral("labelMonoWrong"));
        labelMonoWrong->setGeometry(QRect(390, 40, 320, 100));
        labelMonoWrong->setFrameShape(QFrame::Box);
        labelMonoWrong->setPixmap(QPixmap(QString::fromUtf8(":/pixmaps/resources/bad_mono.jpg")));
        labelAnalyzeWrong = new QLabel(sampleDialog);
        labelAnalyzeWrong->setObjectName(QStringLiteral("labelAnalyzeWrong"));
        labelAnalyzeWrong->setGeometry(QRect(390, 150, 320, 100));
        labelAnalyzeWrong->setFrameShape(QFrame::Box);
        labelAnalyzeWrong->setPixmap(QPixmap(QString::fromUtf8(":/pixmaps/resources/bad_corner.jpg")));
        pushButton = new QPushButton(sampleDialog);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(320, 260, 80, 25));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        pushButton->setFont(font);
        label = new QLabel(sampleDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 320, 25));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        label->setStyleSheet(QLatin1String("color: rgb(85, 255, 127);\n"
"background-color: rgb(255, 255, 255);"));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(sampleDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(390, 10, 320, 25));
        label_2->setFont(font1);
        label_2->setStyleSheet(QLatin1String("background-color: rgb(255, 255, 255);\n"
"color: rgb(255, 0, 0);"));
        label_2->setAlignment(Qt::AlignCenter);

        retranslateUi(sampleDialog);
        QObject::connect(pushButton, SIGNAL(clicked()), sampleDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(sampleDialog);
    } // setupUi

    void retranslateUi(QDialog *sampleDialog)
    {
        sampleDialog->setWindowTitle(QApplication::translate("sampleDialog", "\303\226rnek G\303\266r\303\274nt\303\274ler", 0));
        labelMonoCorrect->setText(QString());
        labelAnalyzeCorrect->setText(QString());
        labelMonoWrong->setText(QString());
        labelAnalyzeWrong->setText(QString());
        pushButton->setText(QApplication::translate("sampleDialog", "TAMAM", 0));
        label->setText(QApplication::translate("sampleDialog", "DO\304\236RU G\303\226R\303\234NT\303\234", 0));
        label_2->setText(QApplication::translate("sampleDialog", "YANLI\305\236 G\303\226R\303\234NT\303\234", 0));
    } // retranslateUi

};

namespace Ui {
    class sampleDialog: public Ui_sampleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAMPLEDIALOG_H

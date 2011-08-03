/********************************************************************************
** Form generated from reading UI file 'helpdialog.ui'
**
** Created: Sun 27. Mar 01:57:44 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPDIALOG_H
#define UI_HELPDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_helpDialog
{
public:
    QPushButton *infoButton;
    QPushButton *analyzeButton;
    QPushButton *playButton;
    QPushButton *trackButton;
    QPushButton *controlButton;
    QPushButton *screenShotButton;
    QPushButton *stopButton;
    QPushButton *guideButton;
    QPushButton *settingsButton;
    QPlainTextEdit *plainTextEdit;
    QPlainTextEdit *plainTextEdit_2;
    QPlainTextEdit *plainTextEdit_3;
    QPlainTextEdit *plainTextEdit_4;
    QPlainTextEdit *plainTextEdit_5;
    QPlainTextEdit *plainTextEdit_6;
    QPlainTextEdit *plainTextEdit_7;
    QPlainTextEdit *plainTextEdit_8;
    QPlainTextEdit *plainTextEdit_9;
    QPlainTextEdit *plainTextEdit_10;
    QPushButton *helpButton;
    QPushButton *exitButton;
    QPushButton *emergencyButton;
    QPlainTextEdit *plainTextEdit_11;
    QPlainTextEdit *plainTextEdit_12;

    void setupUi(QDialog *helpDialog)
    {
        if (helpDialog->objectName().isEmpty())
            helpDialog->setObjectName(QString::fromUtf8("helpDialog"));
        helpDialog->setWindowModality(Qt::WindowModal);
        helpDialog->resize(1310, 735);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        helpDialog->setWindowIcon(icon);
        infoButton = new QPushButton(helpDialog);
        infoButton->setObjectName(QString::fromUtf8("infoButton"));
        infoButton->setGeometry(QRect(0, 618, 52, 48));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/info-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        infoButton->setIcon(icon1);
        infoButton->setIconSize(QSize(40, 40));
        infoButton->setFlat(true);
        analyzeButton = new QPushButton(helpDialog);
        analyzeButton->setObjectName(QString::fromUtf8("analyzeButton"));
        analyzeButton->setGeometry(QRect(2, 260, 60, 56));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/calculator-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        analyzeButton->setIcon(icon2);
        analyzeButton->setIconSize(QSize(48, 48));
        analyzeButton->setFlat(true);
        playButton = new QPushButton(helpDialog);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setGeometry(QRect(2, 12, 60, 56));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/Play-Normal-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        playButton->setIcon(icon3);
        playButton->setIconSize(QSize(48, 48));
        playButton->setFlat(true);
        trackButton = new QPushButton(helpDialog);
        trackButton->setObjectName(QString::fromUtf8("trackButton"));
        trackButton->setEnabled(true);
        trackButton->setGeometry(QRect(0, 352, 60, 56));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/Phone-Book-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        trackButton->setIcon(icon4);
        trackButton->setIconSize(QSize(48, 48));
        trackButton->setFlat(true);
        controlButton = new QPushButton(helpDialog);
        controlButton->setObjectName(QString::fromUtf8("controlButton"));
        controlButton->setEnabled(true);
        controlButton->setGeometry(QRect(0, 414, 60, 56));
        controlButton->setIcon(icon);
        controlButton->setIconSize(QSize(48, 48));
        controlButton->setFlat(true);
        screenShotButton = new QPushButton(helpDialog);
        screenShotButton->setObjectName(QString::fromUtf8("screenShotButton"));
        screenShotButton->setEnabled(true);
        screenShotButton->setGeometry(QRect(2, 136, 60, 56));
        screenShotButton->setLayoutDirection(Qt::LeftToRight);
        screenShotButton->setAutoFillBackground(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resources/Image-capture-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        screenShotButton->setIcon(icon5);
        screenShotButton->setIconSize(QSize(48, 48));
        screenShotButton->setFlat(true);
        stopButton = new QPushButton(helpDialog);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setEnabled(true);
        stopButton->setGeometry(QRect(2, 74, 60, 56));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resources/Stop-Normal-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon6);
        stopButton->setIconSize(QSize(48, 48));
        stopButton->setFlat(true);
        guideButton = new QPushButton(helpDialog);
        guideButton->setObjectName(QString::fromUtf8("guideButton"));
        guideButton->setGeometry(QRect(2, 198, 60, 56));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resources/target.png"), QSize(), QIcon::Normal, QIcon::Off);
        guideButton->setIcon(icon7);
        guideButton->setIconSize(QSize(48, 48));
        guideButton->setFlat(true);
        settingsButton = new QPushButton(helpDialog);
        settingsButton->setObjectName(QString::fromUtf8("settingsButton"));
        settingsButton->setGeometry(QRect(0, 555, 60, 56));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resources/settings-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsButton->setIcon(icon8);
        settingsButton->setIconSize(QSize(48, 48));
        settingsButton->setFlat(true);
        plainTextEdit = new QPlainTextEdit(helpDialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(70, 12, 570, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        plainTextEdit->setFont(font);
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit->setFrameShape(QFrame::Box);
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setTabChangesFocus(false);
        plainTextEdit->setReadOnly(true);
        plainTextEdit->setBackgroundVisible(false);
        plainTextEdit_2 = new QPlainTextEdit(helpDialog);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        plainTextEdit_2->setGeometry(QRect(70, 74, 570, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        plainTextEdit_2->setFont(font1);
        plainTextEdit_2->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_2->setFrameShape(QFrame::Box);
        plainTextEdit_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_2->setTabChangesFocus(false);
        plainTextEdit_2->setReadOnly(true);
        plainTextEdit_2->setBackgroundVisible(false);
        plainTextEdit_3 = new QPlainTextEdit(helpDialog);
        plainTextEdit_3->setObjectName(QString::fromUtf8("plainTextEdit_3"));
        plainTextEdit_3->setGeometry(QRect(70, 136, 570, 50));
        plainTextEdit_3->setFont(font);
        plainTextEdit_3->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_3->setFrameShape(QFrame::Box);
        plainTextEdit_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_3->setTabChangesFocus(false);
        plainTextEdit_3->setReadOnly(true);
        plainTextEdit_3->setBackgroundVisible(false);
        plainTextEdit_4 = new QPlainTextEdit(helpDialog);
        plainTextEdit_4->setObjectName(QString::fromUtf8("plainTextEdit_4"));
        plainTextEdit_4->setGeometry(QRect(70, 198, 570, 50));
        plainTextEdit_4->setFont(font);
        plainTextEdit_4->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_4->setFrameShape(QFrame::Box);
        plainTextEdit_4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_4->setTabChangesFocus(false);
        plainTextEdit_4->setReadOnly(true);
        plainTextEdit_4->setBackgroundVisible(false);
        plainTextEdit_5 = new QPlainTextEdit(helpDialog);
        plainTextEdit_5->setObjectName(QString::fromUtf8("plainTextEdit_5"));
        plainTextEdit_5->setGeometry(QRect(70, 260, 570, 80));
        plainTextEdit_5->setFont(font);
        plainTextEdit_5->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_5->setFrameShape(QFrame::Box);
        plainTextEdit_5->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_5->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_5->setTabChangesFocus(false);
        plainTextEdit_5->setReadOnly(true);
        plainTextEdit_5->setBackgroundVisible(false);
        plainTextEdit_6 = new QPlainTextEdit(helpDialog);
        plainTextEdit_6->setObjectName(QString::fromUtf8("plainTextEdit_6"));
        plainTextEdit_6->setGeometry(QRect(70, 352, 570, 50));
        plainTextEdit_6->setFont(font);
        plainTextEdit_6->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_6->setFrameShape(QFrame::Box);
        plainTextEdit_6->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_6->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_6->setTabChangesFocus(false);
        plainTextEdit_6->setReadOnly(true);
        plainTextEdit_6->setBackgroundVisible(false);
        plainTextEdit_7 = new QPlainTextEdit(helpDialog);
        plainTextEdit_7->setObjectName(QString::fromUtf8("plainTextEdit_7"));
        plainTextEdit_7->setGeometry(QRect(70, 414, 570, 130));
        plainTextEdit_7->setFont(font);
        plainTextEdit_7->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_7->setFrameShape(QFrame::Box);
        plainTextEdit_7->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_7->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_7->setTabChangesFocus(false);
        plainTextEdit_7->setReadOnly(true);
        plainTextEdit_7->setBackgroundVisible(false);
        plainTextEdit_8 = new QPlainTextEdit(helpDialog);
        plainTextEdit_8->setObjectName(QString::fromUtf8("plainTextEdit_8"));
        plainTextEdit_8->setGeometry(QRect(70, 556, 570, 50));
        plainTextEdit_8->setFont(font);
        plainTextEdit_8->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_8->setFrameShape(QFrame::Box);
        plainTextEdit_8->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_8->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_8->setTabChangesFocus(false);
        plainTextEdit_8->setReadOnly(true);
        plainTextEdit_8->setBackgroundVisible(false);
        plainTextEdit_9 = new QPlainTextEdit(helpDialog);
        plainTextEdit_9->setObjectName(QString::fromUtf8("plainTextEdit_9"));
        plainTextEdit_9->setGeometry(QRect(70, 618, 570, 50));
        plainTextEdit_9->setFont(font);
        plainTextEdit_9->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_9->setFrameShape(QFrame::Box);
        plainTextEdit_9->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_9->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_9->setTabChangesFocus(false);
        plainTextEdit_9->setReadOnly(true);
        plainTextEdit_9->setBackgroundVisible(false);
        plainTextEdit_10 = new QPlainTextEdit(helpDialog);
        plainTextEdit_10->setObjectName(QString::fromUtf8("plainTextEdit_10"));
        plainTextEdit_10->setGeometry(QRect(70, 680, 570, 50));
        plainTextEdit_10->setFont(font);
        plainTextEdit_10->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_10->setFrameShape(QFrame::Box);
        plainTextEdit_10->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_10->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_10->setTabChangesFocus(false);
        plainTextEdit_10->setReadOnly(true);
        plainTextEdit_10->setBackgroundVisible(false);
        helpButton = new QPushButton(helpDialog);
        helpButton->setObjectName(QString::fromUtf8("helpButton"));
        helpButton->setGeometry(QRect(0, 680, 48, 48));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/resources/Button-Help-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        helpButton->setIcon(icon9);
        helpButton->setIconSize(QSize(40, 40));
        helpButton->setFlat(true);
        exitButton = new QPushButton(helpDialog);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(670, 74, 48, 48));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/resources/exit-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitButton->setIcon(icon10);
        exitButton->setIconSize(QSize(48, 48));
        exitButton->setFlat(true);
        emergencyButton = new QPushButton(helpDialog);
        emergencyButton->setObjectName(QString::fromUtf8("emergencyButton"));
        emergencyButton->setGeometry(QRect(660, 0, 70, 70));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/resources/Emergency-Stop-Enabled-Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        emergencyButton->setIcon(icon11);
        emergencyButton->setIconSize(QSize(64, 64));
        emergencyButton->setFlat(true);
        plainTextEdit_11 = new QPlainTextEdit(helpDialog);
        plainTextEdit_11->setObjectName(QString::fromUtf8("plainTextEdit_11"));
        plainTextEdit_11->setGeometry(QRect(730, 12, 570, 51));
        plainTextEdit_11->setFont(font);
        plainTextEdit_11->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_11->setFrameShape(QFrame::Box);
        plainTextEdit_11->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_11->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_11->setTabChangesFocus(false);
        plainTextEdit_11->setReadOnly(true);
        plainTextEdit_11->setBackgroundVisible(false);
        plainTextEdit_12 = new QPlainTextEdit(helpDialog);
        plainTextEdit_12->setObjectName(QString::fromUtf8("plainTextEdit_12"));
        plainTextEdit_12->setGeometry(QRect(730, 74, 570, 51));
        plainTextEdit_12->setFont(font);
        plainTextEdit_12->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit_12->setFrameShape(QFrame::Box);
        plainTextEdit_12->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_12->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit_12->setTabChangesFocus(false);
        plainTextEdit_12->setReadOnly(true);
        plainTextEdit_12->setBackgroundVisible(false);

        retranslateUi(helpDialog);

        QMetaObject::connectSlotsByName(helpDialog);
    } // setupUi

    void retranslateUi(QDialog *helpDialog)
    {
        helpDialog->setWindowTitle(QApplication::translate("helpDialog", "Yard\304\261m Ekran\304\261", 0, QApplication::UnicodeUTF8));
        infoButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        analyzeButton->setToolTip(QApplication::translate("helpDialog", "Analiz", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        analyzeButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        playButton->setToolTip(QApplication::translate("helpDialog", "Kamera", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        playButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        trackButton->setToolTip(QApplication::translate("helpDialog", "\304\260z Takibi", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        trackButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        controlButton->setToolTip(QApplication::translate("helpDialog", "Kaynak", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        controlButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        screenShotButton->setToolTip(QApplication::translate("helpDialog", "Resim Yakala", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        screenShotButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        stopButton->setToolTip(QApplication::translate("helpDialog", "Kamera Durdur", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stopButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        guideButton->setToolTip(QApplication::translate("helpDialog", "Rehber", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        guideButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        settingsButton->setToolTip(QApplication::translate("helpDialog", "Ayarlar", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        settingsButton->setText(QString());
        plainTextEdit->setDocumentTitle(QString());
        plainTextEdit->setPlainText(QApplication::translate("helpDialog", "Kamera Butonu: Kamera g\303\266r\303\274nt\303\274s\303\274n\303\274 g\303\266sterir", 0, QApplication::UnicodeUTF8));
        plainTextEdit_2->setDocumentTitle(QString());
        plainTextEdit_2->setPlainText(QApplication::translate("helpDialog", "Kamera Durdur Butonu: Kamera g\303\266r\303\274nt\303\274s\303\274n\303\274 durdurur\n"
"\304\260z Takibi ve Kaynak yap\304\261lamaz", 0, QApplication::UnicodeUTF8));
        plainTextEdit_3->setDocumentTitle(QString());
        plainTextEdit_3->setPlainText(QApplication::translate("helpDialog", "Resim Yakala Butonu: Anl\304\261k g\303\266r\303\274nt\303\274y\303\274 bilgisayara resim olarak kay\304\261t eder", 0, QApplication::UnicodeUTF8));
        plainTextEdit_4->setDocumentTitle(QString());
        plainTextEdit_4->setPlainText(QApplication::translate("helpDialog", "Rehber Butonu: Rehberi ekranda g\303\266sterir veya saklar", 0, QApplication::UnicodeUTF8));
        plainTextEdit_5->setDocumentTitle(QString());
        plainTextEdit_5->setPlainText(QApplication::translate("helpDialog", "Analiz Butonu: K\303\266\305\237e ve Merkez noktas\304\261 tespiti yapar ve operat\303\266rden onay ister\n"
"Onay verilirse Kaynak yap\304\261labilir, verilmezse yap\304\261lamaz\n"
"Sembol aktif ise, Kaynak yap\304\261labilmesi i\303\247in operat\303\266r onay\304\261 beklendi\304\237ini belirtir\n"
"Sembol pasif ise, operat\303\266r onay\304\261 al\304\261nm\304\261\305\237 demektir, Kaynak yap\304\261labilir", 0, QApplication::UnicodeUTF8));
        plainTextEdit_6->setDocumentTitle(QString());
        plainTextEdit_6->setPlainText(QApplication::translate("helpDialog", "\304\260z Takibi Butonu: Merkez noktas\304\261n\304\261 takip eder ve buldu\304\237u sapmalar\304\261 ekranda grafik olarak g\303\266sterir\n"
"Kaynak yap\304\261lmas\304\261 i\303\247in devrede olmal\304\261d\304\261r", 0, QApplication::UnicodeUTF8));
        plainTextEdit_7->setDocumentTitle(QString());
        plainTextEdit_7->setPlainText(QApplication::translate("helpDialog", "Kaynak Butonu: Bulunan sapmalara g\303\266re PLC'ye komut g\303\266ndererek kaynak yap\304\261lmas\304\261n\304\261 sa\304\237lar\n"
"(Kaynak ba\305\237l\304\261\304\237\304\261n\304\261 kontrol eder, kaynak aktivasyonu operat\303\266r taraf\304\261ndan manuel yap\304\261l\304\261r)\n"
"- Kamera g\303\266r\303\274nt\303\274s\303\274 durdu\304\237unda/ula\305\237\304\261lamad\304\261\304\237\304\261nda veya\n"
"- PLC'ye ula\305\237\304\261lamad\304\261\304\237\304\261nda veya\n"
"- Operat\303\266r, analiz sonu\303\247lar\304\261na onay vermediyse veya\n"
"- \304\260z takibi yap\304\261lm\304\261yorsa\n"
"KAYNAK YAPILAMAZ", 0, QApplication::UnicodeUTF8));
        plainTextEdit_8->setDocumentTitle(QString());
        plainTextEdit_8->setPlainText(QApplication::translate("helpDialog", "Ayarlar Butonu: Teknik servisin gerekti\304\237inde yapmas\304\261 gereken ayarlar\304\261 g\303\266sterir", 0, QApplication::UnicodeUTF8));
        plainTextEdit_9->setDocumentTitle(QString());
        plainTextEdit_9->setPlainText(QApplication::translate("helpDialog", "\304\260nfo Butonu: Program hakk\304\261nda bilgi", 0, QApplication::UnicodeUTF8));
        plainTextEdit_10->setDocumentTitle(QString());
        plainTextEdit_10->setPlainText(QApplication::translate("helpDialog", "Yard\304\261m Butonu: Bu ekran", 0, QApplication::UnicodeUTF8));
        helpButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        exitButton->setToolTip(QApplication::translate("helpDialog", "\303\207\304\261k\304\261\305\237", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        exitButton->setText(QString());
        emergencyButton->setText(QString());
        plainTextEdit_11->setDocumentTitle(QString());
        plainTextEdit_11->setPlainText(QApplication::translate("helpDialog", "Acil Stop: Acil bir durum oldu\304\237unda bas\304\261n\304\261z", 0, QApplication::UnicodeUTF8));
        plainTextEdit_12->setDocumentTitle(QString());
        plainTextEdit_12->setPlainText(QApplication::translate("helpDialog", "\303\207\304\261k\304\261\305\237 Butonu: Programdan \303\247\304\261k\304\261\305\237", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class helpDialog: public Ui_helpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPDIALOG_H

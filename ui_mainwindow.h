/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Nov 12 20:34:01 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *imageFrame;
    QPushButton *screenShotButton;
    QLabel *labelTime;
    QPushButton *playButton;
    QPushButton *stopButton;
    QPlainTextEdit *plainTextEdit;
    QPushButton *analyzeButton;
    QFrame *guideFrame;
    QFrame *guideLineVert;
    QFrame *guideLineHorz;
    QFrame *guideTolRight;
    QFrame *guideTolLeft;
    QFrame *frame;
    QPushButton *guideButton;
    QGraphicsView *trackView;
    QPushButton *settingsButton;
    QPushButton *trackButton;
    QPushButton *controlButton;
    QPushButton *plcStatus;
    QPushButton *infoButton;
    QPushButton *exitButton;
    QLabel *label_3;
    QLabel *labelLogoCustomer;
    QPushButton *emergencyButton;
    QPushButton *cameraStatus;
    QFrame *frame_2;
    QTextBrowser *textBrowser;
    QLabel *label;
    QLineEdit *editControlDelay;
    QLabel *label_2;
    QLabel *label_4;
    QPushButton *clearMsgBoxButton;
    QPushButton *helpButton;
    QPushButton *reportButton;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::ApplicationModal);
        MainWindow->setEnabled(true);
        MainWindow->resize(1300, 768);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMaximumSize(QSize(2000, 2000));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        imageFrame = new QLabel(centralWidget);
        imageFrame->setObjectName(QString::fromUtf8("imageFrame"));
        imageFrame->setGeometry(QRect(10, 70, 644, 484));
        imageFrame->setFrameShape(QFrame::Box);
        imageFrame->setLineWidth(2);
        imageFrame->setScaledContents(false);
        screenShotButton = new QPushButton(centralWidget);
        screenShotButton->setObjectName(QString::fromUtf8("screenShotButton"));
        screenShotButton->setEnabled(true);
        screenShotButton->setGeometry(QRect(130, 10, 48, 48));
        screenShotButton->setLayoutDirection(Qt::LeftToRight);
        screenShotButton->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resources/Image-capture-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        screenShotButton->setIcon(icon1);
        screenShotButton->setIconSize(QSize(48, 48));
        screenShotButton->setFlat(true);
        labelTime = new QLabel(centralWidget);
        labelTime->setObjectName(QString::fromUtf8("labelTime"));
        labelTime->setGeometry(QRect(661, 320, 104, 25));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        labelTime->setFont(font);
        labelTime->setFrameShape(QFrame::Box);
        labelTime->setLineWidth(2);
        labelTime->setAlignment(Qt::AlignCenter);
        playButton = new QPushButton(centralWidget);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setGeometry(QRect(10, 10, 48, 48));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resources/Play-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/resources/Play-Normal-icon.png"), QSize(), QIcon::Normal, QIcon::On);
        playButton->setIcon(icon2);
        playButton->setIconSize(QSize(48, 48));
        playButton->setFlat(true);
        stopButton = new QPushButton(centralWidget);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setEnabled(true);
        stopButton->setGeometry(QRect(70, 10, 48, 48));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resources/Stop-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon3);
        stopButton->setIconSize(QSize(48, 48));
        stopButton->setFlat(true);
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 580, 644, 145));
        QFont font1;
        font1.setPointSize(10);
        plainTextEdit->setFont(font1);
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(170, 255, 127);"));
        plainTextEdit->setFrameShape(QFrame::Panel);
        plainTextEdit->setLineWidth(2);
        plainTextEdit->setReadOnly(true);
        analyzeButton = new QPushButton(centralWidget);
        analyzeButton->setObjectName(QString::fromUtf8("analyzeButton"));
        analyzeButton->setGeometry(QRect(300, 10, 48, 48));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resources/calculator-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        analyzeButton->setIcon(icon4);
        analyzeButton->setIconSize(QSize(48, 48));
        analyzeButton->setFlat(true);
        guideFrame = new QFrame(centralWidget);
        guideFrame->setObjectName(QString::fromUtf8("guideFrame"));
        guideFrame->setEnabled(true);
        guideFrame->setGeometry(QRect(170, 72, 324, 480));
        guideFrame->setAutoFillBackground(false);
        guideFrame->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0);"));
        guideFrame->setFrameShape(QFrame::NoFrame);
        guideFrame->setFrameShadow(QFrame::Plain);
        guideFrame->setLineWidth(2);
        guideFrame->setMidLineWidth(0);
        guideLineVert = new QFrame(guideFrame);
        guideLineVert->setObjectName(QString::fromUtf8("guideLineVert"));
        guideLineVert->setEnabled(true);
        guideLineVert->setGeometry(QRect(161, 0, 2, 480));
        guideLineVert->setFrameShadow(QFrame::Plain);
        guideLineVert->setLineWidth(2);
        guideLineVert->setFrameShape(QFrame::VLine);
        guideLineHorz = new QFrame(guideFrame);
        guideLineHorz->setObjectName(QString::fromUtf8("guideLineHorz"));
        guideLineHorz->setGeometry(QRect(2, 239, 320, 2));
        guideLineHorz->setFrameShadow(QFrame::Plain);
        guideLineHorz->setLineWidth(2);
        guideLineHorz->setFrameShape(QFrame::HLine);
        guideTolRight = new QFrame(guideFrame);
        guideTolRight->setObjectName(QString::fromUtf8("guideTolRight"));
        guideTolRight->setGeometry(QRect(172, 190, 1, 100));
        guideTolRight->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        guideTolRight->setFrameShadow(QFrame::Plain);
        guideTolRight->setLineWidth(1);
        guideTolRight->setFrameShape(QFrame::VLine);
        guideTolLeft = new QFrame(guideFrame);
        guideTolLeft->setObjectName(QString::fromUtf8("guideTolLeft"));
        guideTolLeft->setGeometry(QRect(151, 190, 1, 100));
        guideTolLeft->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        guideTolLeft->setFrameShadow(QFrame::Plain);
        guideTolLeft->setLineWidth(1);
        guideTolLeft->setFrameShape(QFrame::VLine);
        frame = new QFrame(guideFrame);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 188, 324, 104));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(2);
        guideButton = new QPushButton(centralWidget);
        guideButton->setObjectName(QString::fromUtf8("guideButton"));
        guideButton->setGeometry(QRect(190, 10, 48, 48));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resources/target.png"), QSize(), QIcon::Normal, QIcon::Off);
        guideButton->setIcon(icon5);
        guideButton->setIconSize(QSize(48, 48));
        guideButton->setFlat(true);
        trackView = new QGraphicsView(centralWidget);
        trackView->setObjectName(QString::fromUtf8("trackView"));
        trackView->setGeometry(QRect(661, 70, 104, 240));
        trackView->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(236, 233, 216);"));
        trackView->setFrameShape(QFrame::Box);
        trackView->setFrameShadow(QFrame::Plain);
        trackView->setLineWidth(2);
        trackView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        trackView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        trackView->setSceneRect(QRectF(0, 0, 101, 480));
        trackView->setTransformationAnchor(QGraphicsView::NoAnchor);
        settingsButton = new QPushButton(centralWidget);
        settingsButton->setObjectName(QString::fromUtf8("settingsButton"));
        settingsButton->setGeometry(QRect(590, 10, 48, 48));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resources/settings-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsButton->setIcon(icon6);
        settingsButton->setIconSize(QSize(48, 48));
        settingsButton->setFlat(true);
        trackButton = new QPushButton(centralWidget);
        trackButton->setObjectName(QString::fromUtf8("trackButton"));
        trackButton->setEnabled(false);
        trackButton->setGeometry(QRect(360, 10, 48, 48));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resources/Phone-Book-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        trackButton->setIcon(icon7);
        trackButton->setIconSize(QSize(48, 48));
        trackButton->setFlat(true);
        controlButton = new QPushButton(centralWidget);
        controlButton->setObjectName(QString::fromUtf8("controlButton"));
        controlButton->setEnabled(false);
        controlButton->setGeometry(QRect(420, 10, 48, 48));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resources/Weather-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        controlButton->setIcon(icon8);
        controlButton->setIconSize(QSize(48, 48));
        controlButton->setFlat(true);
        plcStatus = new QPushButton(centralWidget);
        plcStatus->setObjectName(QString::fromUtf8("plcStatus"));
        plcStatus->setGeometry(QRect(1057, 680, 48, 48));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/resources/s7_200-Disabled-Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        plcStatus->setIcon(icon9);
        plcStatus->setIconSize(QSize(48, 48));
        plcStatus->setFlat(true);
        infoButton = new QPushButton(centralWidget);
        infoButton->setObjectName(QString::fromUtf8("infoButton"));
        infoButton->setGeometry(QRect(650, 10, 48, 48));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/resources/info-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        infoButton->setIcon(icon10);
        infoButton->setIconSize(QSize(40, 40));
        infoButton->setFlat(true);
        exitButton = new QPushButton(centralWidget);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(1245, 680, 48, 48));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/resources/exit-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitButton->setIcon(icon11);
        exitButton->setIconSize(QSize(48, 48));
        exitButton->setFlat(true);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(810, 10, 481, 48));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Pristina"));
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setWeight(75);
        label_3->setFont(font2);
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(7, 65, 255);"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        labelLogoCustomer = new QLabel(centralWidget);
        labelLogoCustomer->setObjectName(QString::fromUtf8("labelLogoCustomer"));
        labelLogoCustomer->setGeometry(QRect(1226, 99, 66, 66));
        labelLogoCustomer->setFrameShape(QFrame::Box);
        labelLogoCustomer->setLineWidth(2);
        labelLogoCustomer->setPixmap(QPixmap(QString::fromUtf8(":/resources/NokselLogo.bmp")));
        emergencyButton = new QPushButton(centralWidget);
        emergencyButton->setObjectName(QString::fromUtf8("emergencyButton"));
        emergencyButton->setGeometry(QRect(1140, 669, 70, 70));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/resources/Emergency-Stop-Disabled-Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon12.addFile(QString::fromUtf8(":/resources/Emergency-Stop-Enabled-Icon.png"), QSize(), QIcon::Normal, QIcon::On);
        emergencyButton->setIcon(icon12);
        emergencyButton->setIconSize(QSize(64, 64));
        emergencyButton->setFlat(true);
        cameraStatus = new QPushButton(centralWidget);
        cameraStatus->setObjectName(QString::fromUtf8("cameraStatus"));
        cameraStatus->setGeometry(QRect(996, 680, 48, 48));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/resources/Camera-Disabled-Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        cameraStatus->setIcon(icon13);
        cameraStatus->setIconSize(QSize(48, 48));
        cameraStatus->setFlat(true);
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(773, 99, 310, 211));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 127);"));
        frame_2->setFrameShape(QFrame::Box);
        frame_2->setFrameShadow(QFrame::Plain);
        frame_2->setLineWidth(2);
        textBrowser = new QTextBrowser(frame_2);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(2, 70, 306, 131));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Arial"));
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        textBrowser->setFont(font3);
        textBrowser->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 127);"));
        textBrowser->setFrameShape(QFrame::NoFrame);
        textBrowser->setFrameShadow(QFrame::Plain);
        textBrowser->setLineWidth(2);
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(2, 2, 306, 30));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Arial"));
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setWeight(75);
        label->setFont(font4);
        label->setAlignment(Qt::AlignCenter);
        editControlDelay = new QLineEdit(frame_2);
        editControlDelay->setObjectName(QString::fromUtf8("editControlDelay"));
        editControlDelay->setGeometry(QRect(116, 30, 80, 25));
        editControlDelay->setFont(font);
        editControlDelay->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);\n"
"background-color: rgb(255, 255, 255);"));
        editControlDelay->setMaxLength(5);
        editControlDelay->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 560, 306, 20));
        label_2->setFont(font3);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(1030, 50, 261, 21));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        QFont font5;
        font5.setFamily(QString::fromUtf8("Pristina"));
        font5.setPointSize(12);
        font5.setBold(true);
        font5.setWeight(75);
        label_4->setFont(font5);
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        clearMsgBoxButton = new QPushButton(centralWidget);
        clearMsgBoxButton->setObjectName(QString::fromUtf8("clearMsgBoxButton"));
        clearMsgBoxButton->setGeometry(QRect(574, 555, 80, 25));
        clearMsgBoxButton->setFont(font3);
        helpButton = new QPushButton(centralWidget);
        helpButton->setObjectName(QString::fromUtf8("helpButton"));
        helpButton->setGeometry(QRect(710, 10, 48, 48));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/resources/Button-Help-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        helpButton->setIcon(icon14);
        helpButton->setIconSize(QSize(40, 40));
        helpButton->setFlat(true);
        reportButton = new QPushButton(centralWidget);
        reportButton->setObjectName(QString::fromUtf8("reportButton"));
        reportButton->setGeometry(QRect(530, 10, 48, 48));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/resources/report-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        reportButton->setIcon(icon15);
        reportButton->setIconSize(QSize(40, 40));
        reportButton->setFlat(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1300, 20));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setStyleSheet(QString::fromUtf8("background-color: rgb(212, 236, 192);\n"
"color: rgb(0, 0, 255);"));
        statusBar->setSizeGripEnabled(false);
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(stopButton, SIGNAL(clicked()), MainWindow, SLOT(stopButton()));
        QObject::connect(playButton, SIGNAL(clicked()), MainWindow, SLOT(playButton()));
        QObject::connect(screenShotButton, SIGNAL(clicked()), MainWindow, SLOT(getImageFromCam()));
        QObject::connect(analyzeButton, SIGNAL(clicked()), MainWindow, SLOT(analyzeButton()));
        QObject::connect(guideButton, SIGNAL(clicked()), MainWindow, SLOT(guideButton()));
        QObject::connect(settingsButton, SIGNAL(clicked()), MainWindow, SLOT(showSettingsForm()));
        QObject::connect(trackButton, SIGNAL(clicked()), MainWindow, SLOT(trackButton()));
        QObject::connect(controlButton, SIGNAL(clicked()), MainWindow, SLOT(controlButton()));
        QObject::connect(infoButton, SIGNAL(clicked()), MainWindow, SLOT(infoButton()));
        QObject::connect(exitButton, SIGNAL(clicked()), MainWindow, SLOT(exitButton()));
        QObject::connect(emergencyButton, SIGNAL(clicked()), MainWindow, SLOT(emergencyButton()));
        QObject::connect(editControlDelay, SIGNAL(returnPressed()), MainWindow, SLOT(getControlDelay()));
        QObject::connect(clearMsgBoxButton, SIGNAL(clicked()), MainWindow, SLOT(clearMsgBoxButton()));
        QObject::connect(helpButton, SIGNAL(clicked()), MainWindow, SLOT(helpButton()));
        QObject::connect(reportButton, SIGNAL(clicked()), MainWindow, SLOT(showReport()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "JTW - Kaynak i\303\247in Birle\305\237me Yeri Takibi", 0, QApplication::UnicodeUTF8));
        imageFrame->setText(QString());
#ifndef QT_NO_TOOLTIP
        screenShotButton->setToolTip(QApplication::translate("MainWindow", "Resim Yakala", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        screenShotButton->setText(QString());
        labelTime->setText(QApplication::translate("MainWindow", "Saat", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        playButton->setToolTip(QApplication::translate("MainWindow", "Kamera", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        playButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        stopButton->setToolTip(QApplication::translate("MainWindow", "Kamera Durdur", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stopButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        analyzeButton->setToolTip(QApplication::translate("MainWindow", "Analiz", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        analyzeButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        guideButton->setToolTip(QApplication::translate("MainWindow", "Rehber", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        guideButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        settingsButton->setToolTip(QApplication::translate("MainWindow", "Ayarlar", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        settingsButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        trackButton->setToolTip(QApplication::translate("MainWindow", "\304\260z Takibi", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        trackButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        controlButton->setToolTip(QApplication::translate("MainWindow", "Kaynak", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        controlButton->setText(QString());
        plcStatus->setText(QString());
        infoButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        exitButton->setToolTip(QApplication::translate("MainWindow", "\303\207\304\261k\304\261\305\237", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        exitButton->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "JTW - Kaynak i\303\247in Birle\305\237me Yeri Takip\303\247isi", 0, QApplication::UnicodeUTF8));
        labelLogoCustomer->setText(QString());
#ifndef QT_NO_TOOLTIP
        emergencyButton->setToolTip(QApplication::translate("MainWindow", "Acil Stop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        emergencyButton->setText(QString());
        cameraStatus->setText(QString());
        textBrowser->setHtml(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Arial'; font-size:10pt; font-weight:600; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">Boru besleme h\304\261z\304\261d\304\261r.</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">Kameran\304\261n g\303\266rd\303\274\304\237\303\274 alana</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">kayna"
                        "k ba\305\237l\304\261\304\237\304\261n\304\261n gelme zaman\304\261n\304\261 ifade eder.</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">Birimi mili saniyedir (1 sn = 1000 ms).</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">Do\304\237ru zamanda komutun verilebilmesi i\303\247in \303\266nemlidir.</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">Hassas \303\266l\303\247\303\274m yap\304\261n\304\261z.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "KAYNAK GEC\304\260KMES\304\260", 0, QApplication::UnicodeUTF8));
        editControlDelay->setInputMask(QString());
        editControlDelay->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "Bilgi Penceresi", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "\302\251Aykurt Elektrik Hendek/Sakarya, 2010", 0, QApplication::UnicodeUTF8));
        clearMsgBoxButton->setText(QApplication::translate("MainWindow", "Temizle", 0, QApplication::UnicodeUTF8));
        helpButton->setText(QString());
        reportButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

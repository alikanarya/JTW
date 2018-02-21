/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

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
    QLabel *label_4;
    QPushButton *helpButton;
    QPushButton *reportButton;
    QPushButton *cmdStatus;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QLabel *labelDistanceTag;
    QLabel *labelDistance;
    QLineEdit *testEdit;
    QLabel *labelDistanceTag2;
    QPushButton *testButton;
    QLineEdit *timeEdit;
    QLabel *labelTimeTag;
    QPushButton *videoButton;
    QPushButton *setupButton;
    QPushButton *guideAlignButton;
    QPushButton *passOneButton;
    QPushButton *passTwoButton;
    QPushButton *targetDriftLeft;
    QPushButton *targetDriftRight;
    QPushButton *targetDriftCenter;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::ApplicationModal);
        MainWindow->setEnabled(true);
        MainWindow->resize(794, 750);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(794, 750));
        MainWindow->setMaximumSize(QSize(794, 750));
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        imageFrame = new QLabel(centralWidget);
        imageFrame->setObjectName(QStringLiteral("imageFrame"));
        imageFrame->setGeometry(QRect(75, 38, 644, 484));
        imageFrame->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        imageFrame->setFrameShape(QFrame::Box);
        imageFrame->setLineWidth(2);
        imageFrame->setScaledContents(false);
        imageFrame->setAlignment(Qt::AlignCenter);
        screenShotButton = new QPushButton(centralWidget);
        screenShotButton->setObjectName(QStringLiteral("screenShotButton"));
        screenShotButton->setEnabled(true);
        screenShotButton->setGeometry(QRect(20, 158, 32, 32));
        screenShotButton->setLayoutDirection(Qt::LeftToRight);
        screenShotButton->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/resources/Image-capture-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        screenShotButton->setIcon(icon1);
        screenShotButton->setIconSize(QSize(32, 32));
        screenShotButton->setFlat(true);
        labelTime = new QLabel(centralWidget);
        labelTime->setObjectName(QStringLiteral("labelTime"));
        labelTime->setEnabled(false);
        labelTime->setGeometry(QRect(560, 558, 61, 23));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        labelTime->setFont(font);
        labelTime->setFrameShape(QFrame::NoFrame);
        labelTime->setLineWidth(2);
        labelTime->setAlignment(Qt::AlignCenter);
        playButton = new QPushButton(centralWidget);
        playButton->setObjectName(QStringLiteral("playButton"));
        playButton->setGeometry(QRect(20, 318, 32, 32));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/resources/Play-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QStringLiteral(":/resources/Play-Normal-icon.png"), QSize(), QIcon::Normal, QIcon::On);
        playButton->setIcon(icon2);
        playButton->setIconSize(QSize(32, 32));
        playButton->setFlat(true);
        stopButton = new QPushButton(centralWidget);
        stopButton->setObjectName(QStringLiteral("stopButton"));
        stopButton->setEnabled(true);
        stopButton->setGeometry(QRect(20, 358, 32, 32));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/resources/Stop-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon3);
        stopButton->setIconSize(QSize(32, 32));
        stopButton->setFlat(true);
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(4, 523, 230, 160));
        QFont font1;
        font1.setPointSize(7);
        plainTextEdit->setFont(font1);
        plainTextEdit->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 195);"));
        plainTextEdit->setFrameShape(QFrame::Box);
        plainTextEdit->setFrameShadow(QFrame::Sunken);
        plainTextEdit->setLineWidth(1);
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setReadOnly(true);
        analyzeButton = new QPushButton(centralWidget);
        analyzeButton->setObjectName(QStringLiteral("analyzeButton"));
        analyzeButton->setGeometry(QRect(20, 38, 32, 32));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/resources/calculator-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        analyzeButton->setIcon(icon4);
        analyzeButton->setIconSize(QSize(32, 32));
        analyzeButton->setFlat(true);
        guideFrame = new QFrame(centralWidget);
        guideFrame->setObjectName(QStringLiteral("guideFrame"));
        guideFrame->setEnabled(true);
        guideFrame->setGeometry(QRect(235, 40, 324, 480));
        guideFrame->setAutoFillBackground(false);
        guideFrame->setStyleSheet(QStringLiteral("color: rgb(0, 255, 0);"));
        guideFrame->setFrameShape(QFrame::NoFrame);
        guideFrame->setFrameShadow(QFrame::Plain);
        guideFrame->setLineWidth(2);
        guideFrame->setMidLineWidth(0);
        guideLineVert = new QFrame(guideFrame);
        guideLineVert->setObjectName(QStringLiteral("guideLineVert"));
        guideLineVert->setEnabled(true);
        guideLineVert->setGeometry(QRect(161, 0, 2, 480));
        guideLineVert->setFrameShadow(QFrame::Plain);
        guideLineVert->setLineWidth(2);
        guideLineVert->setFrameShape(QFrame::VLine);
        guideLineHorz = new QFrame(guideFrame);
        guideLineHorz->setObjectName(QStringLiteral("guideLineHorz"));
        guideLineHorz->setGeometry(QRect(2, 239, 320, 2));
        guideLineHorz->setFrameShadow(QFrame::Plain);
        guideLineHorz->setLineWidth(2);
        guideLineHorz->setFrameShape(QFrame::HLine);
        frame = new QFrame(guideFrame);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 188, 324, 104));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(2);
        guideButton = new QPushButton(centralWidget);
        guideButton->setObjectName(QStringLiteral("guideButton"));
        guideButton->setGeometry(QRect(20, 238, 32, 32));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/resources/target.png"), QSize(), QIcon::Normal, QIcon::Off);
        guideButton->setIcon(icon5);
        guideButton->setIconSize(QSize(32, 32));
        guideButton->setFlat(true);
        trackView = new QGraphicsView(centralWidget);
        trackView->setObjectName(QStringLiteral("trackView"));
        trackView->setGeometry(QRect(235, 523, 324, 160));
        trackView->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"background-color: rgb(236, 233, 216);"));
        trackView->setFrameShape(QFrame::Box);
        trackView->setFrameShadow(QFrame::Plain);
        trackView->setLineWidth(1);
        trackView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        trackView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        trackView->setSceneRect(QRectF(0, 0, 320, 160));
        trackView->setTransformationAnchor(QGraphicsView::NoAnchor);
        settingsButton = new QPushButton(centralWidget);
        settingsButton->setObjectName(QStringLiteral("settingsButton"));
        settingsButton->setGeometry(QRect(20, 438, 32, 32));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/resources/settings-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsButton->setIcon(icon6);
        settingsButton->setIconSize(QSize(32, 32));
        settingsButton->setFlat(true);
        trackButton = new QPushButton(centralWidget);
        trackButton->setObjectName(QStringLiteral("trackButton"));
        trackButton->setEnabled(false);
        trackButton->setGeometry(QRect(20, 78, 32, 32));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/resources/Phone-Book-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        trackButton->setIcon(icon7);
        trackButton->setIconSize(QSize(32, 32));
        trackButton->setFlat(true);
        controlButton = new QPushButton(centralWidget);
        controlButton->setObjectName(QStringLiteral("controlButton"));
        controlButton->setEnabled(false);
        controlButton->setGeometry(QRect(20, 118, 32, 32));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/resources/Weather-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        controlButton->setIcon(icon8);
        controlButton->setIconSize(QSize(32, 32));
        controlButton->setFlat(true);
        plcStatus = new QPushButton(centralWidget);
        plcStatus->setObjectName(QStringLiteral("plcStatus"));
        plcStatus->setGeometry(QRect(740, 488, 32, 32));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/resources/s7_200-Disabled-Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        plcStatus->setIcon(icon9);
        plcStatus->setIconSize(QSize(32, 32));
        plcStatus->setFlat(true);
        infoButton = new QPushButton(centralWidget);
        infoButton->setObjectName(QStringLiteral("infoButton"));
        infoButton->setGeometry(QRect(740, 78, 32, 32));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/resources/info-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        infoButton->setIcon(icon10);
        infoButton->setIconSize(QSize(32, 32));
        infoButton->setFlat(true);
        exitButton = new QPushButton(centralWidget);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(740, 38, 32, 32));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/resources/exit-48.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitButton->setIcon(icon11);
        exitButton->setIconSize(QSize(32, 32));
        exitButton->setFlat(true);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(75, 8, 241, 25));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setFamily(QStringLiteral("Pristina"));
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setWeight(75);
        label_3->setFont(font2);
        label_3->setStyleSheet(QStringLiteral("color: rgb(7, 65, 255);"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelLogoCustomer = new QLabel(centralWidget);
        labelLogoCustomer->setObjectName(QStringLiteral("labelLogoCustomer"));
        labelLogoCustomer->setEnabled(true);
        labelLogoCustomer->setGeometry(QRect(347, 3, 101, 32));
        labelLogoCustomer->setFrameShape(QFrame::StyledPanel);
        labelLogoCustomer->setLineWidth(1);
        labelLogoCustomer->setPixmap(QPixmap(QString::fromUtf8(":/resources/aygazlogo.jpg")));
        labelLogoCustomer->setScaledContents(true);
        emergencyButton = new QPushButton(centralWidget);
        emergencyButton->setObjectName(QStringLiteral("emergencyButton"));
        emergencyButton->setEnabled(false);
        emergencyButton->setGeometry(QRect(740, 408, 32, 32));
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/resources/Emergency-Stop-Disabled-Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon12.addFile(QStringLiteral(":/resources/Emergency-Stop-Enabled-Icon.png"), QSize(), QIcon::Normal, QIcon::On);
        emergencyButton->setIcon(icon12);
        emergencyButton->setIconSize(QSize(32, 32));
        emergencyButton->setFlat(true);
        cameraStatus = new QPushButton(centralWidget);
        cameraStatus->setObjectName(QStringLiteral("cameraStatus"));
        cameraStatus->setGeometry(QRect(740, 448, 32, 32));
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/resources/Camera-Disabled-Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        cameraStatus->setIcon(icon13);
        cameraStatus->setIconSize(QSize(32, 32));
        cameraStatus->setFlat(true);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(565, 8, 151, 25));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        QFont font3;
        font3.setFamily(QStringLiteral("Pristina"));
        font3.setPointSize(12);
        font3.setBold(true);
        font3.setWeight(75);
        label_4->setFont(font3);
        label_4->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        helpButton = new QPushButton(centralWidget);
        helpButton->setObjectName(QStringLiteral("helpButton"));
        helpButton->setGeometry(QRect(740, 118, 32, 32));
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/resources/Button-Help-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        helpButton->setIcon(icon14);
        helpButton->setIconSize(QSize(32, 32));
        helpButton->setFlat(true);
        reportButton = new QPushButton(centralWidget);
        reportButton->setObjectName(QStringLiteral("reportButton"));
        reportButton->setGeometry(QRect(20, 198, 32, 32));
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/resources/report-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        reportButton->setIcon(icon15);
        reportButton->setIconSize(QSize(32, 32));
        reportButton->setFlat(true);
        cmdStatus = new QPushButton(centralWidget);
        cmdStatus->setObjectName(QStringLiteral("cmdStatus"));
        cmdStatus->setGeometry(QRect(570, 528, 32, 32));
        cmdStatus->setAutoFillBackground(false);
        cmdStatus->setStyleSheet(QStringLiteral(""));
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/resources/backward.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdStatus->setIcon(icon16);
        cmdStatus->setIconSize(QSize(32, 32));
        cmdStatus->setFlat(true);
        leftButton = new QPushButton(centralWidget);
        leftButton->setObjectName(QStringLiteral("leftButton"));
        leftButton->setGeometry(QRect(646, 528, 32, 32));
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/resources/left.png"), QSize(), QIcon::Normal, QIcon::Off);
        leftButton->setIcon(icon17);
        leftButton->setIconSize(QSize(32, 32));
        leftButton->setFlat(true);
        rightButton = new QPushButton(centralWidget);
        rightButton->setObjectName(QStringLiteral("rightButton"));
        rightButton->setGeometry(QRect(686, 528, 32, 32));
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/resources/right.png"), QSize(), QIcon::Normal, QIcon::Off);
        rightButton->setIcon(icon18);
        rightButton->setIconSize(QSize(32, 32));
        rightButton->setFlat(true);
        labelDistanceTag = new QLabel(centralWidget);
        labelDistanceTag->setObjectName(QStringLiteral("labelDistanceTag"));
        labelDistanceTag->setGeometry(QRect(570, 638, 43, 44));
        QFont font4;
        font4.setPointSize(11);
        font4.setBold(true);
        font4.setWeight(75);
        labelDistanceTag->setFont(font4);
        labelDistanceTag->setStyleSheet(QStringLiteral("color: rgb(170, 0, 0);"));
        labelDistanceTag->setFrameShape(QFrame::NoFrame);
        labelDistanceTag->setLineWidth(2);
        labelDistanceTag->setPixmap(QPixmap(QString::fromUtf8(":/pixmaps/resources/distance.png")));
        labelDistanceTag->setScaledContents(true);
        labelDistanceTag->setAlignment(Qt::AlignCenter);
        labelDistance = new QLabel(centralWidget);
        labelDistance->setObjectName(QStringLiteral("labelDistance"));
        labelDistance->setGeometry(QRect(613, 639, 61, 23));
        labelDistance->setFont(font);
        labelDistance->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));
        labelDistance->setFrameShape(QFrame::NoFrame);
        labelDistance->setLineWidth(2);
        labelDistance->setAlignment(Qt::AlignCenter);
        testEdit = new QLineEdit(centralWidget);
        testEdit->setObjectName(QStringLiteral("testEdit"));
        testEdit->setEnabled(true);
        testEdit->setGeometry(QRect(570, 610, 51, 20));
        QFont font5;
        font5.setPointSize(9);
        font5.setBold(true);
        font5.setWeight(75);
        testEdit->setFont(font5);
        labelDistanceTag2 = new QLabel(centralWidget);
        labelDistanceTag2->setObjectName(QStringLiteral("labelDistanceTag2"));
        labelDistanceTag2->setGeometry(QRect(613, 657, 61, 23));
        labelDistanceTag2->setFont(font);
        labelDistanceTag2->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        labelDistanceTag2->setFrameShape(QFrame::NoFrame);
        labelDistanceTag2->setLineWidth(2);
        labelDistanceTag2->setAlignment(Qt::AlignCenter);
        testButton = new QPushButton(centralWidget);
        testButton->setObjectName(QStringLiteral("testButton"));
        testButton->setEnabled(true);
        testButton->setGeometry(QRect(570, 588, 41, 23));
        timeEdit = new QLineEdit(centralWidget);
        timeEdit->setObjectName(QStringLiteral("timeEdit"));
        timeEdit->setGeometry(QRect(740, 557, 41, 20));
        labelTimeTag = new QLabel(centralWidget);
        labelTimeTag->setObjectName(QStringLiteral("labelTimeTag"));
        labelTimeTag->setGeometry(QRect(620, 558, 121, 23));
        QFont font6;
        font6.setPointSize(8);
        font6.setBold(true);
        font6.setWeight(75);
        labelTimeTag->setFont(font6);
        labelTimeTag->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        labelTimeTag->setFrameShape(QFrame::NoFrame);
        labelTimeTag->setLineWidth(2);
        labelTimeTag->setAlignment(Qt::AlignCenter);
        videoButton = new QPushButton(centralWidget);
        videoButton->setObjectName(QStringLiteral("videoButton"));
        videoButton->setEnabled(true);
        videoButton->setGeometry(QRect(20, 398, 32, 32));
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/resources/Record-Disabled-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        videoButton->setIcon(icon19);
        videoButton->setIconSize(QSize(32, 32));
        videoButton->setFlat(true);
        setupButton = new QPushButton(centralWidget);
        setupButton->setObjectName(QStringLiteral("setupButton"));
        setupButton->setGeometry(QRect(20, 478, 32, 32));
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/resources/height-icon-ON.png"), QSize(), QIcon::Normal, QIcon::Off);
        setupButton->setIcon(icon20);
        setupButton->setIconSize(QSize(32, 32));
        setupButton->setFlat(true);
        guideAlignButton = new QPushButton(centralWidget);
        guideAlignButton->setObjectName(QStringLiteral("guideAlignButton"));
        guideAlignButton->setGeometry(QRect(20, 278, 32, 32));
        QIcon icon21;
        icon21.addFile(QStringLiteral(":/resources/target_Align.png"), QSize(), QIcon::Normal, QIcon::Off);
        guideAlignButton->setIcon(icon21);
        guideAlignButton->setIconSize(QSize(32, 32));
        guideAlignButton->setFlat(true);
        passOneButton = new QPushButton(centralWidget);
        passOneButton->setObjectName(QStringLiteral("passOneButton"));
        passOneButton->setGeometry(QRect(727, 218, 60, 60));
        passOneButton->setFont(font);
        passOneButton->setAutoFillBackground(false);
        passOneButton->setCheckable(false);
        passOneButton->setChecked(false);
        passOneButton->setFlat(false);
        passTwoButton = new QPushButton(centralWidget);
        passTwoButton->setObjectName(QStringLiteral("passTwoButton"));
        passTwoButton->setGeometry(QRect(727, 306, 60, 60));
        passTwoButton->setFont(font);
        passTwoButton->setCheckable(false);
        targetDriftLeft = new QPushButton(centralWidget);
        targetDriftLeft->setObjectName(QStringLiteral("targetDriftLeft"));
        targetDriftLeft->setGeometry(QRect(720, 280, 24, 24));
        QIcon icon22;
        icon22.addFile(QStringLiteral(":/resources/left-arrow-red.png"), QSize(), QIcon::Normal, QIcon::Off);
        targetDriftLeft->setIcon(icon22);
        targetDriftLeft->setIconSize(QSize(32, 32));
        targetDriftLeft->setFlat(true);
        targetDriftRight = new QPushButton(centralWidget);
        targetDriftRight->setObjectName(QStringLiteral("targetDriftRight"));
        targetDriftRight->setGeometry(QRect(770, 280, 24, 24));
        QIcon icon23;
        icon23.addFile(QStringLiteral(":/resources/right-arrow-red.png"), QSize(), QIcon::Normal, QIcon::Off);
        targetDriftRight->setIcon(icon23);
        targetDriftRight->setIconSize(QSize(32, 32));
        targetDriftRight->setFlat(true);
        targetDriftCenter = new QPushButton(centralWidget);
        targetDriftCenter->setObjectName(QStringLiteral("targetDriftCenter"));
        targetDriftCenter->setGeometry(QRect(746, 280, 24, 24));
        QIcon icon24;
        icon24.addFile(QStringLiteral(":/resources/reload.png"), QSize(), QIcon::Normal, QIcon::Off);
        targetDriftCenter->setIcon(icon24);
        targetDriftCenter->setIconSize(QSize(32, 32));
        targetDriftCenter->setFlat(true);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 794, 21));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setStyleSheet(QLatin1String("background-color: rgb(212, 236, 192);\n"
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
        QObject::connect(helpButton, SIGNAL(clicked()), MainWindow, SLOT(helpButton()));
        QObject::connect(reportButton, SIGNAL(clicked()), MainWindow, SLOT(showReport()));
        QObject::connect(leftButton, SIGNAL(clicked()), MainWindow, SLOT(target2Left()));
        QObject::connect(rightButton, SIGNAL(clicked()), MainWindow, SLOT(target2Right()));
        QObject::connect(testEdit, SIGNAL(returnPressed()), MainWindow, SLOT(testEdit()));
        QObject::connect(testButton, SIGNAL(clicked()), MainWindow, SLOT(testButton()));
        QObject::connect(timeEdit, SIGNAL(returnPressed()), MainWindow, SLOT(timeEdit()));
        QObject::connect(videoButton, SIGNAL(clicked()), MainWindow, SLOT(videoButton()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "JTW - Kaynak i\303\247in Birle\305\237me Yeri Takip\303\247isi", 0));
        imageFrame->setText(QString());
#ifndef QT_NO_TOOLTIP
        screenShotButton->setToolTip(QApplication::translate("MainWindow", "Resim Yakala", 0));
#endif // QT_NO_TOOLTIP
        screenShotButton->setText(QString());
        labelTime->setText(QApplication::translate("MainWindow", "Saat", 0));
#ifndef QT_NO_TOOLTIP
        playButton->setToolTip(QApplication::translate("MainWindow", "Kamera", 0));
#endif // QT_NO_TOOLTIP
        playButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        stopButton->setToolTip(QApplication::translate("MainWindow", "Kamera Durdur", 0));
#endif // QT_NO_TOOLTIP
        stopButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        analyzeButton->setToolTip(QApplication::translate("MainWindow", "Analiz", 0));
#endif // QT_NO_TOOLTIP
        analyzeButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        guideButton->setToolTip(QApplication::translate("MainWindow", "Rehber", 0));
#endif // QT_NO_TOOLTIP
        guideButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        settingsButton->setToolTip(QApplication::translate("MainWindow", "Ayarlar", 0));
#endif // QT_NO_TOOLTIP
        settingsButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        trackButton->setToolTip(QApplication::translate("MainWindow", "\304\260z Takibi", 0));
#endif // QT_NO_TOOLTIP
        trackButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        controlButton->setToolTip(QApplication::translate("MainWindow", "Kaynak", 0));
#endif // QT_NO_TOOLTIP
        controlButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        plcStatus->setToolTip(QApplication::translate("MainWindow", "PLC Ba\304\237lant\304\261 Durumu", 0));
#endif // QT_NO_TOOLTIP
        plcStatus->setText(QString());
#ifndef QT_NO_TOOLTIP
        infoButton->setToolTip(QApplication::translate("MainWindow", "Bilgi", 0));
#endif // QT_NO_TOOLTIP
        infoButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        exitButton->setToolTip(QApplication::translate("MainWindow", "\303\207\304\261k\304\261\305\237", 0));
#endif // QT_NO_TOOLTIP
        exitButton->setText(QString());
        label_3->setText(QApplication::translate("MainWindow", "JTW - Kaynak Takibi", 0));
        labelLogoCustomer->setText(QString());
#ifndef QT_NO_TOOLTIP
        emergencyButton->setToolTip(QApplication::translate("MainWindow", "Acil Stop", 0));
#endif // QT_NO_TOOLTIP
        emergencyButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        cameraStatus->setToolTip(QApplication::translate("MainWindow", "Kamera Ba\304\237lant\304\261 Durumu", 0));
#endif // QT_NO_TOOLTIP
        cameraStatus->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "\302\251Orun Elektronik", 0));
#ifndef QT_NO_TOOLTIP
        helpButton->setToolTip(QApplication::translate("MainWindow", "Yard\304\261m", 0));
#endif // QT_NO_TOOLTIP
        helpButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        reportButton->setToolTip(QApplication::translate("MainWindow", "Rapor", 0));
#endif // QT_NO_TOOLTIP
        reportButton->setText(QString());
        cmdStatus->setText(QString());
#ifndef QT_NO_TOOLTIP
        leftButton->setToolTip(QApplication::translate("MainWindow", "Rehber Sola", 0));
#endif // QT_NO_TOOLTIP
        leftButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        rightButton->setToolTip(QApplication::translate("MainWindow", "Rehber Sa\304\237a", 0));
#endif // QT_NO_TOOLTIP
        rightButton->setText(QString());
        labelDistanceTag->setText(QString());
        labelDistance->setText(QApplication::translate("MainWindow", "000.0", 0));
        testEdit->setInputMask(QString());
        labelDistanceTag2->setText(QApplication::translate("MainWindow", "(mm)", 0));
        testButton->setText(QApplication::translate("MainWindow", "Test", 0));
        labelTimeTag->setText(QApplication::translate("MainWindow", "Stop Zaman\304\261 (sn)", 0));
#ifndef QT_NO_TOOLTIP
        videoButton->setToolTip(QApplication::translate("MainWindow", "Video Kaydet", 0));
#endif // QT_NO_TOOLTIP
        videoButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        setupButton->setToolTip(QApplication::translate("MainWindow", "Ayarlar", 0));
#endif // QT_NO_TOOLTIP
        setupButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        guideAlignButton->setToolTip(QApplication::translate("MainWindow", "Rehber Hizala", 0));
#endif // QT_NO_TOOLTIP
        guideAlignButton->setText(QString());
        passOneButton->setText(QApplication::translate("MainWindow", "1.\n"
"PASO", 0));
        passTwoButton->setText(QApplication::translate("MainWindow", "2.\n"
"PASO", 0));
#ifndef QT_NO_TOOLTIP
        targetDriftLeft->setToolTip(QApplication::translate("MainWindow", "Rehber Sola", 0));
#endif // QT_NO_TOOLTIP
        targetDriftLeft->setText(QString());
#ifndef QT_NO_TOOLTIP
        targetDriftRight->setToolTip(QApplication::translate("MainWindow", "Rehber Sa\304\237a", 0));
#endif // QT_NO_TOOLTIP
        targetDriftRight->setText(QString());
#ifndef QT_NO_TOOLTIP
        targetDriftCenter->setToolTip(QApplication::translate("MainWindow", "Merkez", 0));
#endif // QT_NO_TOOLTIP
        targetDriftCenter->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

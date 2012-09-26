/********************************************************************************
** Form generated from reading UI file 'setupform.ui'
**
** Created: Wed 26. Sep 13:45:50 2012
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPFORM_H
#define UI_SETUPFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_setupForm
{
public:
    QLabel *labelTarget;
    QLabel *labelMono;
    QLabel *labelEdge;
    QLabel *labelHough;
    QLabel *labelAnalyze;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *captureButton;
    QCheckBox *checkSaveTarget;
    QCheckBox *checkSaveMono;
    QCheckBox *checkSaveEdge;
    QCheckBox *checkSaveHough;
    QCheckBox *checkSaveAnalyze;
    QCheckBox *checkSaveMonoMatrix;
    QCheckBox *checkSaveEdgeMatrix;
    QCheckBox *checkSaveHoughMatrix;
    QPushButton *saveButton;
    QCheckBox *checkSaveHoughLines;
    QCheckBox *checkSaveHoughSpace;
    QLineEdit *editHoughLineNo;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *editHoughThreshold;
    QLabel *labelVoteAvg;
    QLabel *labelPrimaryLine;
    QLineEdit *editHoughThetaStep;
    QLabel *label_8;
    QLineEdit *editHoughThetaMin;
    QLabel *label_9;
    QLineEdit *editHoughThetaMax;
    QPlainTextEdit *plainTextEdit;
    QLineEdit *editVoidThreshold;
    QLabel *label_10;
    QPushButton *saveExitButton;
    QPushButton *exitButton;
    QPushButton *clearButton;
    QCheckBox *checkSubImage;
    QLabel *label_11;
    QLineEdit *editHoughThetaMinSub;
    QLabel *label_12;
    QLineEdit *editHoughThetaStepSub;
    QLineEdit *editHoughThetaMaxSub;
    QFrame *line;
    QFrame *line_2;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *editIPI;
    QLineEdit *editFPS;
    QComboBox *subTypeBox;
    QCheckBox *zctrlBox;
    QCheckBox *hardControlStartBox;
    QCheckBox *widthControlBox;
    QLabel *label_15;
    QCheckBox *readMachineStatusBox;
    QCheckBox *readDistanceBox;
    QCheckBox *dynamicAlgoBox;
    QCheckBox *readWeldSeamBox;

    void setupUi(QWidget *setupForm)
    {
        if (setupForm->objectName().isEmpty())
            setupForm->setObjectName(QString::fromUtf8("setupForm"));
        setupForm->setWindowModality(Qt::WindowModal);
        setupForm->resize(1019, 800);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(setupForm->sizePolicy().hasHeightForWidth());
        setupForm->setSizePolicy(sizePolicy);
        setupForm->setMinimumSize(QSize(1019, 800));
        setupForm->setMaximumSize(QSize(1019, 800));
        setupForm->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        setupForm->setWindowIcon(icon);
        labelTarget = new QLabel(setupForm);
        labelTarget->setObjectName(QString::fromUtf8("labelTarget"));
        labelTarget->setGeometry(QRect(20, 50, 320, 100));
        labelTarget->setFrameShape(QFrame::Box);
        labelMono = new QLabel(setupForm);
        labelMono->setObjectName(QString::fromUtf8("labelMono"));
        labelMono->setGeometry(QRect(20, 200, 320, 100));
        labelMono->setFrameShape(QFrame::Box);
        labelEdge = new QLabel(setupForm);
        labelEdge->setObjectName(QString::fromUtf8("labelEdge"));
        labelEdge->setGeometry(QRect(20, 350, 320, 100));
        labelEdge->setFrameShape(QFrame::Box);
        labelHough = new QLabel(setupForm);
        labelHough->setObjectName(QString::fromUtf8("labelHough"));
        labelHough->setGeometry(QRect(20, 500, 320, 100));
        labelHough->setFrameShape(QFrame::Box);
        labelAnalyze = new QLabel(setupForm);
        labelAnalyze->setObjectName(QString::fromUtf8("labelAnalyze"));
        labelAnalyze->setGeometry(QRect(20, 650, 320, 100));
        labelAnalyze->setFrameShape(QFrame::Box);
        label = new QLabel(setupForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 81, 16));
        label_2 = new QLabel(setupForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 180, 111, 16));
        label_3 = new QLabel(setupForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 330, 141, 16));
        label_4 = new QLabel(setupForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 480, 81, 16));
        label_5 = new QLabel(setupForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 630, 91, 16));
        captureButton = new QPushButton(setupForm);
        captureButton->setObjectName(QString::fromUtf8("captureButton"));
        captureButton->setGeometry(QRect(130, 20, 91, 25));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        captureButton->setFont(font);
        checkSaveTarget = new QCheckBox(setupForm);
        checkSaveTarget->setObjectName(QString::fromUtf8("checkSaveTarget"));
        checkSaveTarget->setGeometry(QRect(20, 160, 71, 17));
        checkSaveMono = new QCheckBox(setupForm);
        checkSaveMono->setObjectName(QString::fromUtf8("checkSaveMono"));
        checkSaveMono->setGeometry(QRect(20, 310, 71, 17));
        checkSaveEdge = new QCheckBox(setupForm);
        checkSaveEdge->setObjectName(QString::fromUtf8("checkSaveEdge"));
        checkSaveEdge->setGeometry(QRect(20, 460, 71, 17));
        checkSaveHough = new QCheckBox(setupForm);
        checkSaveHough->setObjectName(QString::fromUtf8("checkSaveHough"));
        checkSaveHough->setGeometry(QRect(20, 610, 71, 17));
        checkSaveAnalyze = new QCheckBox(setupForm);
        checkSaveAnalyze->setObjectName(QString::fromUtf8("checkSaveAnalyze"));
        checkSaveAnalyze->setGeometry(QRect(20, 760, 71, 17));
        checkSaveMonoMatrix = new QCheckBox(setupForm);
        checkSaveMonoMatrix->setObjectName(QString::fromUtf8("checkSaveMonoMatrix"));
        checkSaveMonoMatrix->setGeometry(QRect(70, 310, 81, 17));
        checkSaveEdgeMatrix = new QCheckBox(setupForm);
        checkSaveEdgeMatrix->setObjectName(QString::fromUtf8("checkSaveEdgeMatrix"));
        checkSaveEdgeMatrix->setGeometry(QRect(70, 460, 81, 17));
        checkSaveHoughMatrix = new QCheckBox(setupForm);
        checkSaveHoughMatrix->setObjectName(QString::fromUtf8("checkSaveHoughMatrix"));
        checkSaveHoughMatrix->setGeometry(QRect(70, 610, 81, 17));
        saveButton = new QPushButton(setupForm);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(230, 20, 111, 25));
        saveButton->setFont(font);
        checkSaveHoughLines = new QCheckBox(setupForm);
        checkSaveHoughLines->setObjectName(QString::fromUtf8("checkSaveHoughLines"));
        checkSaveHoughLines->setGeometry(QRect(130, 610, 101, 17));
        checkSaveHoughSpace = new QCheckBox(setupForm);
        checkSaveHoughSpace->setObjectName(QString::fromUtf8("checkSaveHoughSpace"));
        checkSaveHoughSpace->setGeometry(QRect(220, 610, 81, 17));
        editHoughLineNo = new QLineEdit(setupForm);
        editHoughLineNo->setObjectName(QString::fromUtf8("editHoughLineNo"));
        editHoughLineNo->setGeometry(QRect(480, 200, 42, 20));
        editHoughLineNo->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(setupForm);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(360, 200, 111, 21));
        label_6->setFont(font);
        label_6->setFrameShape(QFrame::Box);
        label_6->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label_7 = new QLabel(setupForm);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(360, 230, 111, 21));
        label_7->setFont(font);
        label_7->setFrameShape(QFrame::Box);
        label_7->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThreshold = new QLineEdit(setupForm);
        editHoughThreshold->setObjectName(QString::fromUtf8("editHoughThreshold"));
        editHoughThreshold->setGeometry(QRect(480, 230, 42, 20));
        editHoughThreshold->setAlignment(Qt::AlignCenter);
        labelVoteAvg = new QLabel(setupForm);
        labelVoteAvg->setObjectName(QString::fromUtf8("labelVoteAvg"));
        labelVoteAvg->setGeometry(QRect(530, 230, 42, 20));
        labelVoteAvg->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        labelVoteAvg->setFrameShape(QFrame::Box);
        labelVoteAvg->setAlignment(Qt::AlignCenter);
        labelPrimaryLine = new QLabel(setupForm);
        labelPrimaryLine->setObjectName(QString::fromUtf8("labelPrimaryLine"));
        labelPrimaryLine->setEnabled(true);
        labelPrimaryLine->setGeometry(QRect(200, 480, 140, 16));
        labelPrimaryLine->setFont(font);
        labelPrimaryLine->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        labelPrimaryLine->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editHoughThetaStep = new QLineEdit(setupForm);
        editHoughThetaStep->setObjectName(QString::fromUtf8("editHoughThetaStep"));
        editHoughThetaStep->setGeometry(QRect(480, 290, 42, 20));
        editHoughThetaStep->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(setupForm);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(360, 290, 111, 21));
        label_8->setFont(font);
        label_8->setFrameShape(QFrame::Box);
        label_8->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaMin = new QLineEdit(setupForm);
        editHoughThetaMin->setObjectName(QString::fromUtf8("editHoughThetaMin"));
        editHoughThetaMin->setGeometry(QRect(480, 260, 42, 20));
        editHoughThetaMin->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(setupForm);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(360, 260, 111, 21));
        label_9->setFont(font);
        label_9->setFrameShape(QFrame::Box);
        label_9->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaMax = new QLineEdit(setupForm);
        editHoughThetaMax->setObjectName(QString::fromUtf8("editHoughThetaMax"));
        editHoughThetaMax->setGeometry(QRect(530, 260, 42, 20));
        editHoughThetaMax->setAlignment(Qt::AlignCenter);
        plainTextEdit = new QPlainTextEdit(setupForm);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(360, 510, 321, 241));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Tahoma"));
        plainTextEdit->setFont(font1);
        editVoidThreshold = new QLineEdit(setupForm);
        editVoidThreshold->setObjectName(QString::fromUtf8("editVoidThreshold"));
        editVoidThreshold->setGeometry(QRect(480, 320, 42, 20));
        editVoidThreshold->setAlignment(Qt::AlignCenter);
        label_10 = new QLabel(setupForm);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(360, 320, 111, 21));
        label_10->setFont(font);
        label_10->setFrameShape(QFrame::Box);
        label_10->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        saveExitButton = new QPushButton(setupForm);
        saveExitButton->setObjectName(QString::fromUtf8("saveExitButton"));
        saveExitButton->setGeometry(QRect(800, 730, 85, 25));
        saveExitButton->setFont(font);
        saveExitButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        exitButton = new QPushButton(setupForm);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(917, 730, 85, 25));
        exitButton->setFont(font);
        exitButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        clearButton = new QPushButton(setupForm);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(800, 700, 85, 25));
        clearButton->setFont(font);
        checkSubImage = new QCheckBox(setupForm);
        checkSubImage->setObjectName(QString::fromUtf8("checkSubImage"));
        checkSubImage->setGeometry(QRect(360, 360, 181, 17));
        label_11 = new QLabel(setupForm);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(360, 410, 71, 21));
        label_11->setFont(font);
        label_11->setFrameShape(QFrame::Box);
        label_11->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaMinSub = new QLineEdit(setupForm);
        editHoughThetaMinSub->setObjectName(QString::fromUtf8("editHoughThetaMinSub"));
        editHoughThetaMinSub->setGeometry(QRect(450, 380, 42, 20));
        editHoughThetaMinSub->setAlignment(Qt::AlignCenter);
        label_12 = new QLabel(setupForm);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(360, 380, 71, 21));
        label_12->setFont(font);
        label_12->setFrameShape(QFrame::Box);
        label_12->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaStepSub = new QLineEdit(setupForm);
        editHoughThetaStepSub->setObjectName(QString::fromUtf8("editHoughThetaStepSub"));
        editHoughThetaStepSub->setGeometry(QRect(450, 410, 42, 20));
        editHoughThetaStepSub->setAlignment(Qt::AlignCenter);
        editHoughThetaMaxSub = new QLineEdit(setupForm);
        editHoughThetaMaxSub->setObjectName(QString::fromUtf8("editHoughThetaMaxSub"));
        editHoughThetaMaxSub->setGeometry(QRect(500, 380, 42, 20));
        editHoughThetaMaxSub->setAlignment(Qt::AlignCenter);
        line = new QFrame(setupForm);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(350, 350, 311, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(setupForm);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(349, 430, 311, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_13 = new QLabel(setupForm);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(750, 30, 111, 21));
        label_13->setFont(font);
        label_13->setFrameShape(QFrame::Box);
        label_13->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label_14 = new QLabel(setupForm);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(750, 60, 111, 21));
        label_14->setFont(font);
        label_14->setFrameShape(QFrame::Box);
        label_14->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editIPI = new QLineEdit(setupForm);
        editIPI->setObjectName(QString::fromUtf8("editIPI"));
        editIPI->setGeometry(QRect(870, 60, 42, 20));
        editIPI->setMaxLength(2);
        editIPI->setAlignment(Qt::AlignCenter);
        editFPS = new QLineEdit(setupForm);
        editFPS->setObjectName(QString::fromUtf8("editFPS"));
        editFPS->setGeometry(QRect(870, 30, 42, 20));
        editFPS->setAlignment(Qt::AlignCenter);
        subTypeBox = new QComboBox(setupForm);
        subTypeBox->setObjectName(QString::fromUtf8("subTypeBox"));
        subTypeBox->setGeometry(QRect(550, 380, 111, 22));
        zctrlBox = new QCheckBox(setupForm);
        zctrlBox->setObjectName(QString::fromUtf8("zctrlBox"));
        zctrlBox->setEnabled(false);
        zctrlBox->setGeometry(QRect(750, 90, 131, 17));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        zctrlBox->setFont(font2);
        hardControlStartBox = new QCheckBox(setupForm);
        hardControlStartBox->setObjectName(QString::fromUtf8("hardControlStartBox"));
        hardControlStartBox->setGeometry(QRect(750, 150, 161, 17));
        hardControlStartBox->setFont(font2);
        widthControlBox = new QCheckBox(setupForm);
        widthControlBox->setObjectName(QString::fromUtf8("widthControlBox"));
        widthControlBox->setGeometry(QRect(750, 120, 161, 17));
        widthControlBox->setFont(font2);
        label_15 = new QLabel(setupForm);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(750, 220, 231, 201));
        label_15->setFont(font);
        label_15->setFrameShape(QFrame::Box);
        label_15->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        readMachineStatusBox = new QCheckBox(setupForm);
        readMachineStatusBox->setObjectName(QString::fromUtf8("readMachineStatusBox"));
        readMachineStatusBox->setGeometry(QRect(760, 250, 201, 17));
        readMachineStatusBox->setFont(font2);
        readDistanceBox = new QCheckBox(setupForm);
        readDistanceBox->setObjectName(QString::fromUtf8("readDistanceBox"));
        readDistanceBox->setGeometry(QRect(760, 270, 201, 17));
        readDistanceBox->setFont(font2);
        dynamicAlgoBox = new QCheckBox(setupForm);
        dynamicAlgoBox->setObjectName(QString::fromUtf8("dynamicAlgoBox"));
        dynamicAlgoBox->setGeometry(QRect(750, 180, 161, 17));
        dynamicAlgoBox->setFont(font2);
        readWeldSeamBox = new QCheckBox(setupForm);
        readWeldSeamBox->setObjectName(QString::fromUtf8("readWeldSeamBox"));
        readWeldSeamBox->setGeometry(QRect(760, 290, 201, 17));
        readWeldSeamBox->setFont(font2);

        retranslateUi(setupForm);
        QObject::connect(captureButton, SIGNAL(clicked()), setupForm, SLOT(captureButton()));
        QObject::connect(saveButton, SIGNAL(clicked()), setupForm, SLOT(saveButton()));
        QObject::connect(clearButton, SIGNAL(clicked()), setupForm, SLOT(clearButton()));
        QObject::connect(exitButton, SIGNAL(clicked()), setupForm, SLOT(exitButton()));
        QObject::connect(saveExitButton, SIGNAL(clicked()), setupForm, SLOT(saveExitButton()));
        QObject::connect(checkSubImage, SIGNAL(clicked()), setupForm, SLOT(subImageCheck()));
        QObject::connect(subTypeBox, SIGNAL(currentIndexChanged(int)), setupForm, SLOT(subType()));
        QObject::connect(zctrlBox, SIGNAL(stateChanged(int)), setupForm, SLOT(zctrlBox()));
        QObject::connect(hardControlStartBox, SIGNAL(stateChanged(int)), setupForm, SLOT(hardControlStartBox()));
        QObject::connect(widthControlBox, SIGNAL(stateChanged(int)), setupForm, SLOT(widthControl()));
        QObject::connect(readMachineStatusBox, SIGNAL(stateChanged(int)), setupForm, SLOT(readMachineStatusBox()));
        QObject::connect(readDistanceBox, SIGNAL(stateChanged(int)), setupForm, SLOT(readDistanceBox()));
        QObject::connect(dynamicAlgoBox, SIGNAL(stateChanged(int)), setupForm, SLOT(dynamicAlgoBox()));
        QObject::connect(readWeldSeamBox, SIGNAL(stateChanged(int)), setupForm, SLOT(readWeldSeamBox()));

        QMetaObject::connectSlotsByName(setupForm);
    } // setupUi

    void retranslateUi(QWidget *setupForm)
    {
        setupForm->setWindowTitle(QApplication::translate("setupForm", "Kurulum Ayarlar\304\261", 0, QApplication::UnicodeUTF8));
        labelTarget->setText(QString());
        labelMono->setText(QString());
        labelEdge->setText(QString());
        labelHough->setText(QString());
        labelAnalyze->setText(QString());
        label->setText(QApplication::translate("setupForm", "Hedef Alan \304\260maj\304\261", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("setupForm", "Mono \304\260maj", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("setupForm", "Tespit Edilen Kenarlar", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("setupForm", "\303\207izgi Tespiti", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("setupForm", "K\303\266\305\237e Tespiti", 0, QApplication::UnicodeUTF8));
        captureButton->setText(QApplication::translate("setupForm", "Yakala", 0, QApplication::UnicodeUTF8));
        checkSaveTarget->setText(QApplication::translate("setupForm", "\304\260maj", 0, QApplication::UnicodeUTF8));
        checkSaveMono->setText(QApplication::translate("setupForm", "\304\260maj", 0, QApplication::UnicodeUTF8));
        checkSaveEdge->setText(QApplication::translate("setupForm", "\304\260maj", 0, QApplication::UnicodeUTF8));
        checkSaveHough->setText(QApplication::translate("setupForm", "\304\260maj", 0, QApplication::UnicodeUTF8));
        checkSaveAnalyze->setText(QApplication::translate("setupForm", "\304\260maj", 0, QApplication::UnicodeUTF8));
        checkSaveMonoMatrix->setText(QApplication::translate("setupForm", "Matrix", 0, QApplication::UnicodeUTF8));
        checkSaveEdgeMatrix->setText(QApplication::translate("setupForm", "Matrix", 0, QApplication::UnicodeUTF8));
        checkSaveHoughMatrix->setText(QApplication::translate("setupForm", "Matrix", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("setupForm", "Se\303\247ilileri Kaydet", 0, QApplication::UnicodeUTF8));
        checkSaveHoughLines->setText(QApplication::translate("setupForm", "\303\207izgi Matrixi", 0, QApplication::UnicodeUTF8));
        checkSaveHoughSpace->setText(QApplication::translate("setupForm", "HSpace", 0, QApplication::UnicodeUTF8));
        editHoughLineNo->setInputMask(QApplication::translate("setupForm", "99; ", 0, QApplication::UnicodeUTF8));
        editHoughLineNo->setText(QApplication::translate("setupForm", "2", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("setupForm", "\303\207izgi Say\304\261s\304\261", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("setupForm", "E\305\237ik", 0, QApplication::UnicodeUTF8));
        editHoughThreshold->setInputMask(QApplication::translate("setupForm", "999; ", 0, QApplication::UnicodeUTF8));
        editHoughThreshold->setText(QApplication::translate("setupForm", "200", 0, QApplication::UnicodeUTF8));
        labelVoteAvg->setText(QApplication::translate("setupForm", "Avg", 0, QApplication::UnicodeUTF8));
        labelPrimaryLine->setText(QApplication::translate("setupForm", "\303\207\304\260ZG\304\260 BULUNAMIYOR!", 0, QApplication::UnicodeUTF8));
        editHoughThetaStep->setInputMask(QApplication::translate("setupForm", "9.9; ", 0, QApplication::UnicodeUTF8));
        editHoughThetaStep->setText(QApplication::translate("setupForm", "1.0", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("setupForm", "A\303\247\304\261 \303\207\303\266z.", 0, QApplication::UnicodeUTF8));
        editHoughThetaMin->setInputMask(QApplication::translate("setupForm", "999; ", 0, QApplication::UnicodeUTF8));
        editHoughThetaMin->setText(QApplication::translate("setupForm", "80", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("setupForm", "A\303\247\304\261 Aral\304\261\304\237\304\261", 0, QApplication::UnicodeUTF8));
        editHoughThetaMax->setInputMask(QApplication::translate("setupForm", "999; ", 0, QApplication::UnicodeUTF8));
        editHoughThetaMax->setText(QApplication::translate("setupForm", "100", 0, QApplication::UnicodeUTF8));
        plainTextEdit->setDocumentTitle(QString());
        plainTextEdit->setPlainText(QString());
        editVoidThreshold->setInputMask(QApplication::translate("setupForm", "99; ", 0, QApplication::UnicodeUTF8));
        editVoidThreshold->setText(QApplication::translate("setupForm", "30", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("setupForm", "Void E\305\237i\304\237i", 0, QApplication::UnicodeUTF8));
        saveExitButton->setText(QApplication::translate("setupForm", "Kaydet ve \303\207\304\261k", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("setupForm", "\303\207\304\261k", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("setupForm", "Temizle", 0, QApplication::UnicodeUTF8));
        checkSubImage->setText(QApplication::translate("setupForm", "Alt \304\260maj Algoritmas\304\261", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("setupForm", "A\303\247\304\261 \303\207\303\266z.", 0, QApplication::UnicodeUTF8));
        editHoughThetaMinSub->setInputMask(QApplication::translate("setupForm", "999; ", 0, QApplication::UnicodeUTF8));
        editHoughThetaMinSub->setText(QApplication::translate("setupForm", "80", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("setupForm", "A\303\247\304\261 Aral\304\261\304\237\304\261", 0, QApplication::UnicodeUTF8));
        editHoughThetaStepSub->setInputMask(QApplication::translate("setupForm", "9.9; ", 0, QApplication::UnicodeUTF8));
        editHoughThetaStepSub->setText(QApplication::translate("setupForm", "1.0", 0, QApplication::UnicodeUTF8));
        editHoughThetaMaxSub->setInputMask(QApplication::translate("setupForm", "999; ", 0, QApplication::UnicodeUTF8));
        editHoughThetaMaxSub->setText(QApplication::translate("setupForm", "100", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("setupForm", "FPS", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("setupForm", "\304\260.\304\260\305\237leme Aral\304\261\304\237\304\261", 0, QApplication::UnicodeUTF8));
        editIPI->setInputMask(QApplication::translate("setupForm", "99; ", 0, QApplication::UnicodeUTF8));
        editIPI->setText(QApplication::translate("setupForm", "3", 0, QApplication::UnicodeUTF8));
        editFPS->setInputMask(QApplication::translate("setupForm", "99; ", 0, QApplication::UnicodeUTF8));
        editFPS->setText(QApplication::translate("setupForm", "10", 0, QApplication::UnicodeUTF8));
        subTypeBox->clear();
        subTypeBox->insertItems(0, QStringList()
         << QApplication::translate("setupForm", "Bo\305\237luk Algo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("setupForm", "Doluluk Algo", 0, QApplication::UnicodeUTF8)
        );
        zctrlBox->setText(QApplication::translate("setupForm", "Y\303\274kseklik Kontrol\303\274", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        hardControlStartBox->setToolTip(QApplication::translate("setupForm", "Makineden Kaynak Ba\305\237lang\304\261c\304\261", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        hardControlStartBox->setText(QApplication::translate("setupForm", "Hard Kaynak Ba\305\237lang\304\261c\304\261", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        widthControlBox->setToolTip(QApplication::translate("setupForm", "Kaynak ba\305\237lang\304\261c\304\261ndaki geni\305\237li\304\237in %80 ile %120 si aras\304\261n\304\261 ge\303\247erli say", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        widthControlBox->setText(QApplication::translate("setupForm", "Geni\305\237lik Kontrol\303\274", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("setupForm", "PLC B\304\260LG\304\260", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        readMachineStatusBox->setToolTip(QApplication::translate("setupForm", "DB2.DB0.x", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        readMachineStatusBox->setText(QApplication::translate("setupForm", "Makine \303\207al\304\261\305\237ma Durumu Bilgisi", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        readDistanceBox->setToolTip(QApplication::translate("setupForm", "DBX.DBW2", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        readDistanceBox->setText(QApplication::translate("setupForm", "Mesafe Bilgisi", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        dynamicAlgoBox->setToolTip(QApplication::translate("setupForm", "\304\260KA ya Otomaik Ge\303\247", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        dynamicAlgoBox->setText(QApplication::translate("setupForm", "Dinamik Algoritma", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        readWeldSeamBox->setToolTip(QApplication::translate("setupForm", "DB2.DB0.x", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        readWeldSeamBox->setText(QApplication::translate("setupForm", "Kaynak \304\260zi  Bilgisi", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class setupForm: public Ui_setupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPFORM_H

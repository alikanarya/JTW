/********************************************************************************
** Form generated from reading UI file 'setupform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPFORM_H
#define UI_SETUPFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

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
    QLineEdit *editHoughThetaMinSub;
    QLineEdit *editHoughThetaStepSub;
    QLineEdit *editHoughThetaMaxSub;
    QFrame *line;
    QFrame *line_2;
    QLabel *label_13;
    QLabel *label_14;
    QLineEdit *editIPI;
    QLineEdit *editFPS;
    QCheckBox *zctrlBox;
    QCheckBox *hardControlStartBox;
    QCheckBox *widthControlBox;
    QLabel *label_15;
    QCheckBox *readMachineStatusBox;
    QCheckBox *readDistanceBox;
    QCheckBox *dynamicAlgoBox;
    QCheckBox *readWeldSeamBox;
    QCheckBox *timeControlBox;
    QCheckBox *lineDetectionBox;
    QLineEdit *editLineScore;
    QLabel *label_16;
    QPushButton *captureButton_2;
    QCheckBox *cannyThinningBox;
    QSlider *brightnessSlider;
    QLabel *labelBrightness;
    QSlider *contrastSlider;
    QLabel *labelContrast;
    QSlider *gammaSlider;
    QLabel *labelGamma;
    QPushButton *brightnessReset;
    QPushButton *contrastReset;
    QPushButton *gammaReset;
    QSlider *fileSlider;
    QCheckBox *checkProcessing;
    QLabel *labelGaussSize;
    QSlider *gaussSizeSlider;
    QSlider *gaussSDevSlider;
    QLabel *labelGaussSDev;
    QComboBox *algorithmBox;
    QRadioButton *radioLaser;
    QRadioButton *radioWoLaser;
    QComboBox *edgeDetectionBox;
    QComboBox *detecAlgoBox;
    QSlider *mainEdgesSlider;
    QButtonGroup *laserGroup;

    void setupUi(QWidget *setupForm)
    {
        if (setupForm->objectName().isEmpty())
            setupForm->setObjectName(QStringLiteral("setupForm"));
        setupForm->setWindowModality(Qt::WindowModal);
        setupForm->resize(1680, 850);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(setupForm->sizePolicy().hasHeightForWidth());
        setupForm->setSizePolicy(sizePolicy);
        setupForm->setMinimumSize(QSize(1019, 635));
        setupForm->setMaximumSize(QSize(1680, 850));
        setupForm->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        setupForm->setWindowIcon(icon);
        labelTarget = new QLabel(setupForm);
        labelTarget->setObjectName(QStringLiteral("labelTarget"));
        labelTarget->setGeometry(QRect(20, 100, 320, 320));
        labelTarget->setFrameShape(QFrame::Box);
        labelTarget->setScaledContents(true);
        labelMono = new QLabel(setupForm);
        labelMono->setObjectName(QStringLiteral("labelMono"));
        labelMono->setGeometry(QRect(350, 100, 320, 320));
        labelMono->setFrameShape(QFrame::Box);
        labelMono->setScaledContents(true);
        labelEdge = new QLabel(setupForm);
        labelEdge->setObjectName(QStringLiteral("labelEdge"));
        labelEdge->setGeometry(QRect(680, 100, 320, 320));
        labelEdge->setFrameShape(QFrame::Box);
        labelEdge->setScaledContents(true);
        labelHough = new QLabel(setupForm);
        labelHough->setObjectName(QStringLiteral("labelHough"));
        labelHough->setGeometry(QRect(1011, 100, 320, 320));
        labelHough->setFrameShape(QFrame::Box);
        labelHough->setScaledContents(true);
        labelAnalyze = new QLabel(setupForm);
        labelAnalyze->setObjectName(QStringLiteral("labelAnalyze"));
        labelAnalyze->setGeometry(QRect(1340, 100, 320, 320));
        labelAnalyze->setFrameShape(QFrame::Box);
        labelAnalyze->setScaledContents(true);
        label = new QLabel(setupForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 80, 101, 16));
        label_2 = new QLabel(setupForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(350, 80, 111, 16));
        label_3 = new QLabel(setupForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(680, 80, 141, 16));
        label_4 = new QLabel(setupForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(1011, 80, 81, 16));
        label_5 = new QLabel(setupForm);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(1340, 80, 91, 16));
        captureButton = new QPushButton(setupForm);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setGeometry(QRect(470, 50, 91, 25));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        captureButton->setFont(font);
        checkSaveTarget = new QCheckBox(setupForm);
        checkSaveTarget->setObjectName(QStringLiteral("checkSaveTarget"));
        checkSaveTarget->setGeometry(QRect(269, 430, 71, 17));
        checkSaveTarget->setLayoutDirection(Qt::RightToLeft);
        checkSaveMono = new QCheckBox(setupForm);
        checkSaveMono->setObjectName(QStringLiteral("checkSaveMono"));
        checkSaveMono->setGeometry(QRect(350, 430, 71, 17));
        checkSaveEdge = new QCheckBox(setupForm);
        checkSaveEdge->setObjectName(QStringLiteral("checkSaveEdge"));
        checkSaveEdge->setGeometry(QRect(680, 430, 71, 17));
        checkSaveHough = new QCheckBox(setupForm);
        checkSaveHough->setObjectName(QStringLiteral("checkSaveHough"));
        checkSaveHough->setGeometry(QRect(1010, 430, 71, 17));
        checkSaveAnalyze = new QCheckBox(setupForm);
        checkSaveAnalyze->setObjectName(QStringLiteral("checkSaveAnalyze"));
        checkSaveAnalyze->setGeometry(QRect(1340, 430, 71, 17));
        checkSaveMonoMatrix = new QCheckBox(setupForm);
        checkSaveMonoMatrix->setObjectName(QStringLiteral("checkSaveMonoMatrix"));
        checkSaveMonoMatrix->setGeometry(QRect(410, 430, 81, 17));
        checkSaveEdgeMatrix = new QCheckBox(setupForm);
        checkSaveEdgeMatrix->setObjectName(QStringLiteral("checkSaveEdgeMatrix"));
        checkSaveEdgeMatrix->setGeometry(QRect(740, 430, 81, 17));
        checkSaveHoughMatrix = new QCheckBox(setupForm);
        checkSaveHoughMatrix->setObjectName(QStringLiteral("checkSaveHoughMatrix"));
        checkSaveHoughMatrix->setGeometry(QRect(1070, 430, 81, 17));
        saveButton = new QPushButton(setupForm);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(20, 426, 131, 25));
        saveButton->setFont(font);
        checkSaveHoughLines = new QCheckBox(setupForm);
        checkSaveHoughLines->setObjectName(QStringLiteral("checkSaveHoughLines"));
        checkSaveHoughLines->setGeometry(QRect(1140, 430, 101, 17));
        checkSaveHoughSpace = new QCheckBox(setupForm);
        checkSaveHoughSpace->setObjectName(QStringLiteral("checkSaveHoughSpace"));
        checkSaveHoughSpace->setGeometry(QRect(1240, 430, 81, 17));
        editHoughLineNo = new QLineEdit(setupForm);
        editHoughLineNo->setObjectName(QStringLiteral("editHoughLineNo"));
        editHoughLineNo->setGeometry(QRect(1510, 520, 42, 20));
        editHoughLineNo->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(setupForm);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(1390, 520, 111, 21));
        label_6->setFont(font);
        label_6->setFrameShape(QFrame::Box);
        label_6->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label_7 = new QLabel(setupForm);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(1390, 550, 111, 21));
        label_7->setFont(font);
        label_7->setFrameShape(QFrame::Box);
        label_7->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThreshold = new QLineEdit(setupForm);
        editHoughThreshold->setObjectName(QStringLiteral("editHoughThreshold"));
        editHoughThreshold->setGeometry(QRect(1510, 550, 42, 20));
        editHoughThreshold->setAlignment(Qt::AlignCenter);
        labelVoteAvg = new QLabel(setupForm);
        labelVoteAvg->setObjectName(QStringLiteral("labelVoteAvg"));
        labelVoteAvg->setGeometry(QRect(1560, 550, 42, 20));
        labelVoteAvg->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelVoteAvg->setFrameShape(QFrame::Box);
        labelVoteAvg->setAlignment(Qt::AlignCenter);
        labelPrimaryLine = new QLabel(setupForm);
        labelPrimaryLine->setObjectName(QStringLiteral("labelPrimaryLine"));
        labelPrimaryLine->setEnabled(true);
        labelPrimaryLine->setGeometry(QRect(1160, 80, 171, 20));
        labelPrimaryLine->setFont(font);
        labelPrimaryLine->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelPrimaryLine->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editHoughThetaStep = new QLineEdit(setupForm);
        editHoughThetaStep->setObjectName(QStringLiteral("editHoughThetaStep"));
        editHoughThetaStep->setGeometry(QRect(1510, 610, 42, 20));
        editHoughThetaStep->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(setupForm);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(1390, 610, 111, 21));
        label_8->setFont(font);
        label_8->setFrameShape(QFrame::Box);
        label_8->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaMin = new QLineEdit(setupForm);
        editHoughThetaMin->setObjectName(QStringLiteral("editHoughThetaMin"));
        editHoughThetaMin->setGeometry(QRect(1510, 580, 42, 20));
        editHoughThetaMin->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(setupForm);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(1390, 580, 111, 21));
        label_9->setFont(font);
        label_9->setFrameShape(QFrame::Box);
        label_9->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaMax = new QLineEdit(setupForm);
        editHoughThetaMax->setObjectName(QStringLiteral("editHoughThetaMax"));
        editHoughThetaMax->setGeometry(QRect(1560, 580, 42, 20));
        editHoughThetaMax->setAlignment(Qt::AlignCenter);
        plainTextEdit = new QPlainTextEdit(setupForm);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(610, 540, 581, 191));
        QFont font1;
        font1.setFamily(QStringLiteral("Tahoma"));
        plainTextEdit->setFont(font1);
        editVoidThreshold = new QLineEdit(setupForm);
        editVoidThreshold->setObjectName(QStringLiteral("editVoidThreshold"));
        editVoidThreshold->setGeometry(QRect(1510, 640, 42, 20));
        editVoidThreshold->setAlignment(Qt::AlignCenter);
        label_10 = new QLabel(setupForm);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(1390, 640, 111, 21));
        label_10->setFont(font);
        label_10->setFrameShape(QFrame::Box);
        label_10->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        saveExitButton = new QPushButton(setupForm);
        saveExitButton->setObjectName(QStringLiteral("saveExitButton"));
        saveExitButton->setGeometry(QRect(360, 810, 171, 25));
        saveExitButton->setFont(font);
        saveExitButton->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        exitButton = new QPushButton(setupForm);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(360, 780, 85, 25));
        exitButton->setFont(font);
        exitButton->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        clearButton = new QPushButton(setupForm);
        clearButton->setObjectName(QStringLiteral("clearButton"));
        clearButton->setGeometry(QRect(360, 750, 85, 25));
        clearButton->setFont(font);
        checkSubImage = new QCheckBox(setupForm);
        checkSubImage->setObjectName(QStringLiteral("checkSubImage"));
        checkSubImage->setEnabled(false);
        checkSubImage->setGeometry(QRect(1210, 770, 101, 17));
        editHoughThetaMinSub = new QLineEdit(setupForm);
        editHoughThetaMinSub->setObjectName(QStringLiteral("editHoughThetaMinSub"));
        editHoughThetaMinSub->setEnabled(false);
        editHoughThetaMinSub->setGeometry(QRect(1210, 790, 42, 20));
        editHoughThetaMinSub->setAlignment(Qt::AlignCenter);
        editHoughThetaStepSub = new QLineEdit(setupForm);
        editHoughThetaStepSub->setObjectName(QStringLiteral("editHoughThetaStepSub"));
        editHoughThetaStepSub->setEnabled(false);
        editHoughThetaStepSub->setGeometry(QRect(1310, 790, 42, 20));
        editHoughThetaStepSub->setAlignment(Qt::AlignCenter);
        editHoughThetaMaxSub = new QLineEdit(setupForm);
        editHoughThetaMaxSub->setObjectName(QStringLiteral("editHoughThetaMaxSub"));
        editHoughThetaMaxSub->setEnabled(false);
        editHoughThetaMaxSub->setGeometry(QRect(1260, 790, 42, 20));
        editHoughThetaMaxSub->setAlignment(Qt::AlignCenter);
        line = new QFrame(setupForm);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(630, 520, 311, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(setupForm);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(30, 600, 311, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_13 = new QLabel(setupForm);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(360, 480, 111, 21));
        label_13->setFont(font);
        label_13->setFrameShape(QFrame::Box);
        label_13->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label_14 = new QLabel(setupForm);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(360, 510, 111, 21));
        label_14->setFont(font);
        label_14->setFrameShape(QFrame::Box);
        label_14->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editIPI = new QLineEdit(setupForm);
        editIPI->setObjectName(QStringLiteral("editIPI"));
        editIPI->setGeometry(QRect(480, 510, 42, 20));
        editIPI->setMaxLength(2);
        editIPI->setAlignment(Qt::AlignCenter);
        editFPS = new QLineEdit(setupForm);
        editFPS->setObjectName(QStringLiteral("editFPS"));
        editFPS->setGeometry(QRect(480, 480, 42, 20));
        editFPS->setAlignment(Qt::AlignCenter);
        zctrlBox = new QCheckBox(setupForm);
        zctrlBox->setObjectName(QStringLiteral("zctrlBox"));
        zctrlBox->setEnabled(false);
        zctrlBox->setGeometry(QRect(360, 540, 131, 17));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        zctrlBox->setFont(font2);
        hardControlStartBox = new QCheckBox(setupForm);
        hardControlStartBox->setObjectName(QStringLiteral("hardControlStartBox"));
        hardControlStartBox->setGeometry(QRect(360, 600, 161, 17));
        hardControlStartBox->setFont(font2);
        widthControlBox = new QCheckBox(setupForm);
        widthControlBox->setObjectName(QStringLiteral("widthControlBox"));
        widthControlBox->setGeometry(QRect(360, 570, 161, 17));
        widthControlBox->setFont(font2);
        label_15 = new QLabel(setupForm);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(1390, 720, 231, 101));
        label_15->setFont(font);
        label_15->setFrameShape(QFrame::Box);
        label_15->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        readMachineStatusBox = new QCheckBox(setupForm);
        readMachineStatusBox->setObjectName(QStringLiteral("readMachineStatusBox"));
        readMachineStatusBox->setGeometry(QRect(1400, 750, 201, 17));
        readMachineStatusBox->setFont(font2);
        readDistanceBox = new QCheckBox(setupForm);
        readDistanceBox->setObjectName(QStringLiteral("readDistanceBox"));
        readDistanceBox->setGeometry(QRect(1400, 770, 201, 17));
        readDistanceBox->setFont(font2);
        dynamicAlgoBox = new QCheckBox(setupForm);
        dynamicAlgoBox->setObjectName(QStringLiteral("dynamicAlgoBox"));
        dynamicAlgoBox->setGeometry(QRect(360, 630, 161, 17));
        dynamicAlgoBox->setFont(font2);
        readWeldSeamBox = new QCheckBox(setupForm);
        readWeldSeamBox->setObjectName(QStringLiteral("readWeldSeamBox"));
        readWeldSeamBox->setGeometry(QRect(1400, 790, 201, 17));
        readWeldSeamBox->setFont(font2);
        timeControlBox = new QCheckBox(setupForm);
        timeControlBox->setObjectName(QStringLiteral("timeControlBox"));
        timeControlBox->setGeometry(QRect(360, 660, 161, 17));
        timeControlBox->setFont(font2);
        lineDetectionBox = new QCheckBox(setupForm);
        lineDetectionBox->setObjectName(QStringLiteral("lineDetectionBox"));
        lineDetectionBox->setGeometry(QRect(360, 690, 161, 17));
        lineDetectionBox->setFont(font2);
        editLineScore = new QLineEdit(setupForm);
        editLineScore->setObjectName(QStringLiteral("editLineScore"));
        editLineScore->setGeometry(QRect(480, 688, 42, 20));
        editLineScore->setAlignment(Qt::AlignCenter);
        label_16 = new QLabel(setupForm);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(530, 690, 31, 16));
        captureButton_2 = new QPushButton(setupForm);
        captureButton_2->setObjectName(QStringLiteral("captureButton_2"));
        captureButton_2->setGeometry(QRect(20, 53, 101, 25));
        captureButton_2->setFont(font);
        cannyThinningBox = new QCheckBox(setupForm);
        cannyThinningBox->setObjectName(QStringLiteral("cannyThinningBox"));
        cannyThinningBox->setGeometry(QRect(1390, 680, 161, 17));
        cannyThinningBox->setFont(font2);
        brightnessSlider = new QSlider(setupForm);
        brightnessSlider->setObjectName(QStringLiteral("brightnessSlider"));
        brightnessSlider->setGeometry(QRect(30, 480, 160, 22));
        brightnessSlider->setAutoFillBackground(false);
        brightnessSlider->setMinimum(-30);
        brightnessSlider->setMaximum(30);
        brightnessSlider->setSingleStep(5);
        brightnessSlider->setTracking(true);
        brightnessSlider->setOrientation(Qt::Horizontal);
        brightnessSlider->setInvertedAppearance(false);
        brightnessSlider->setInvertedControls(false);
        brightnessSlider->setTickPosition(QSlider::TicksBothSides);
        brightnessSlider->setTickInterval(10);
        labelBrightness = new QLabel(setupForm);
        labelBrightness->setObjectName(QStringLiteral("labelBrightness"));
        labelBrightness->setGeometry(QRect(210, 480, 42, 20));
        labelBrightness->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelBrightness->setFrameShape(QFrame::Box);
        labelBrightness->setAlignment(Qt::AlignCenter);
        contrastSlider = new QSlider(setupForm);
        contrastSlider->setObjectName(QStringLiteral("contrastSlider"));
        contrastSlider->setGeometry(QRect(30, 510, 160, 22));
        contrastSlider->setMinimum(0);
        contrastSlider->setMaximum(200);
        contrastSlider->setSingleStep(10);
        contrastSlider->setValue(100);
        contrastSlider->setTracking(true);
        contrastSlider->setOrientation(Qt::Horizontal);
        contrastSlider->setTickPosition(QSlider::TicksBothSides);
        contrastSlider->setTickInterval(50);
        labelContrast = new QLabel(setupForm);
        labelContrast->setObjectName(QStringLiteral("labelContrast"));
        labelContrast->setGeometry(QRect(210, 510, 42, 20));
        labelContrast->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelContrast->setFrameShape(QFrame::Box);
        labelContrast->setAlignment(Qt::AlignCenter);
        gammaSlider = new QSlider(setupForm);
        gammaSlider->setObjectName(QStringLiteral("gammaSlider"));
        gammaSlider->setGeometry(QRect(30, 540, 160, 22));
        gammaSlider->setMinimum(0);
        gammaSlider->setMaximum(200);
        gammaSlider->setSingleStep(10);
        gammaSlider->setValue(100);
        gammaSlider->setTracking(true);
        gammaSlider->setOrientation(Qt::Horizontal);
        gammaSlider->setTickPosition(QSlider::TicksBothSides);
        gammaSlider->setTickInterval(50);
        labelGamma = new QLabel(setupForm);
        labelGamma->setObjectName(QStringLiteral("labelGamma"));
        labelGamma->setGeometry(QRect(210, 540, 42, 20));
        labelGamma->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelGamma->setFrameShape(QFrame::Box);
        labelGamma->setAlignment(Qt::AlignCenter);
        brightnessReset = new QPushButton(setupForm);
        brightnessReset->setObjectName(QStringLiteral("brightnessReset"));
        brightnessReset->setGeometry(QRect(260, 480, 42, 22));
        brightnessReset->setFont(font);
        contrastReset = new QPushButton(setupForm);
        contrastReset->setObjectName(QStringLiteral("contrastReset"));
        contrastReset->setGeometry(QRect(260, 510, 42, 22));
        contrastReset->setFont(font);
        gammaReset = new QPushButton(setupForm);
        gammaReset->setObjectName(QStringLiteral("gammaReset"));
        gammaReset->setGeometry(QRect(260, 540, 42, 22));
        gammaReset->setFont(font);
        fileSlider = new QSlider(setupForm);
        fileSlider->setObjectName(QStringLiteral("fileSlider"));
        fileSlider->setGeometry(QRect(140, 60, 191, 22));
        fileSlider->setOrientation(Qt::Horizontal);
        checkProcessing = new QCheckBox(setupForm);
        checkProcessing->setObjectName(QStringLiteral("checkProcessing"));
        checkProcessing->setGeometry(QRect(140, 20, 101, 17));
        labelGaussSize = new QLabel(setupForm);
        labelGaussSize->setObjectName(QStringLiteral("labelGaussSize"));
        labelGaussSize->setGeometry(QRect(800, 470, 42, 20));
        labelGaussSize->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelGaussSize->setFrameShape(QFrame::Box);
        labelGaussSize->setAlignment(Qt::AlignCenter);
        gaussSizeSlider = new QSlider(setupForm);
        gaussSizeSlider->setObjectName(QStringLiteral("gaussSizeSlider"));
        gaussSizeSlider->setGeometry(QRect(620, 470, 160, 22));
        gaussSizeSlider->setAutoFillBackground(false);
        gaussSizeSlider->setMinimum(1);
        gaussSizeSlider->setMaximum(5);
        gaussSizeSlider->setSingleStep(1);
        gaussSizeSlider->setValue(1);
        gaussSizeSlider->setSliderPosition(1);
        gaussSizeSlider->setTracking(true);
        gaussSizeSlider->setOrientation(Qt::Horizontal);
        gaussSizeSlider->setInvertedAppearance(false);
        gaussSizeSlider->setInvertedControls(false);
        gaussSizeSlider->setTickPosition(QSlider::TicksBothSides);
        gaussSizeSlider->setTickInterval(1);
        gaussSDevSlider = new QSlider(setupForm);
        gaussSDevSlider->setObjectName(QStringLiteral("gaussSDevSlider"));
        gaussSDevSlider->setGeometry(QRect(620, 500, 160, 22));
        gaussSDevSlider->setAutoFillBackground(false);
        gaussSDevSlider->setMinimum(1);
        gaussSDevSlider->setMaximum(20);
        gaussSDevSlider->setSingleStep(1);
        gaussSDevSlider->setValue(10);
        gaussSDevSlider->setSliderPosition(10);
        gaussSDevSlider->setTracking(true);
        gaussSDevSlider->setOrientation(Qt::Horizontal);
        gaussSDevSlider->setInvertedAppearance(false);
        gaussSDevSlider->setInvertedControls(false);
        gaussSDevSlider->setTickPosition(QSlider::TicksBothSides);
        gaussSDevSlider->setTickInterval(5);
        labelGaussSDev = new QLabel(setupForm);
        labelGaussSDev->setObjectName(QStringLiteral("labelGaussSDev"));
        labelGaussSDev->setGeometry(QRect(800, 500, 42, 20));
        labelGaussSDev->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelGaussSDev->setFrameShape(QFrame::Box);
        labelGaussSDev->setAlignment(Qt::AlignCenter);
        algorithmBox = new QComboBox(setupForm);
        algorithmBox->setObjectName(QStringLiteral("algorithmBox"));
        algorithmBox->setGeometry(QRect(220, 570, 111, 22));
        radioLaser = new QRadioButton(setupForm);
        laserGroup = new QButtonGroup(setupForm);
        laserGroup->setObjectName(QStringLiteral("laserGroup"));
        laserGroup->addButton(radioLaser);
        radioLaser->setObjectName(QStringLiteral("radioLaser"));
        radioLaser->setGeometry(QRect(1050, 460, 103, 21));
        radioLaser->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        radioLaser->setChecked(true);
        radioWoLaser = new QRadioButton(setupForm);
        laserGroup->addButton(radioWoLaser);
        radioWoLaser->setObjectName(QStringLiteral("radioWoLaser"));
        radioWoLaser->setGeometry(QRect(1170, 460, 111, 21));
        radioWoLaser->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        edgeDetectionBox = new QComboBox(setupForm);
        edgeDetectionBox->setObjectName(QStringLiteral("edgeDetectionBox"));
        edgeDetectionBox->setGeometry(QRect(880, 430, 111, 22));
        detecAlgoBox = new QComboBox(setupForm);
        detecAlgoBox->setObjectName(QStringLiteral("detecAlgoBox"));
        detecAlgoBox->setGeometry(QRect(1010, 490, 201, 22));
        mainEdgesSlider = new QSlider(setupForm);
        mainEdgesSlider->setObjectName(QStringLiteral("mainEdgesSlider"));
        mainEdgesSlider->setGeometry(QRect(1390, 490, 160, 22));
        mainEdgesSlider->setAutoFillBackground(false);
        mainEdgesSlider->setMinimum(1);
        mainEdgesSlider->setMaximum(5);
        mainEdgesSlider->setSingleStep(1);
        mainEdgesSlider->setValue(1);
        mainEdgesSlider->setSliderPosition(1);
        mainEdgesSlider->setTracking(true);
        mainEdgesSlider->setOrientation(Qt::Horizontal);
        mainEdgesSlider->setInvertedAppearance(false);
        mainEdgesSlider->setInvertedControls(false);
        mainEdgesSlider->setTickPosition(QSlider::TicksBothSides);
        mainEdgesSlider->setTickInterval(1);

        retranslateUi(setupForm);
        QObject::connect(captureButton, SIGNAL(clicked()), setupForm, SLOT(captureButton()));
        QObject::connect(saveButton, SIGNAL(clicked()), setupForm, SLOT(saveButton()));
        QObject::connect(clearButton, SIGNAL(clicked()), setupForm, SLOT(clearButton()));
        QObject::connect(exitButton, SIGNAL(clicked()), setupForm, SLOT(exitButton()));
        QObject::connect(saveExitButton, SIGNAL(clicked()), setupForm, SLOT(saveExitButton()));
        QObject::connect(checkSubImage, SIGNAL(clicked()), setupForm, SLOT(subImageCheck()));
        QObject::connect(zctrlBox, SIGNAL(stateChanged(int)), setupForm, SLOT(zctrlBox()));
        QObject::connect(hardControlStartBox, SIGNAL(stateChanged(int)), setupForm, SLOT(hardControlStartBox()));
        QObject::connect(widthControlBox, SIGNAL(stateChanged(int)), setupForm, SLOT(widthControl()));
        QObject::connect(readMachineStatusBox, SIGNAL(stateChanged(int)), setupForm, SLOT(readMachineStatusBox()));
        QObject::connect(readDistanceBox, SIGNAL(stateChanged(int)), setupForm, SLOT(readDistanceBox()));
        QObject::connect(dynamicAlgoBox, SIGNAL(stateChanged(int)), setupForm, SLOT(dynamicAlgoBox()));
        QObject::connect(readWeldSeamBox, SIGNAL(stateChanged(int)), setupForm, SLOT(readWeldSeamBox()));
        QObject::connect(timeControlBox, SIGNAL(stateChanged(int)), setupForm, SLOT(timeControlBox()));
        QObject::connect(lineDetectionBox, SIGNAL(stateChanged(int)), setupForm, SLOT(lineDetectionBox()));
        QObject::connect(editLineScore, SIGNAL(returnPressed()), setupForm, SLOT(editLineScore()));

        QMetaObject::connectSlotsByName(setupForm);
    } // setupUi

    void retranslateUi(QWidget *setupForm)
    {
        setupForm->setWindowTitle(QApplication::translate("setupForm", "Kurulum Ayarlar\304\261", 0));
        labelTarget->setText(QString());
        labelMono->setText(QString());
        labelEdge->setText(QString());
        labelHough->setText(QString());
        labelAnalyze->setText(QString());
        label->setText(QApplication::translate("setupForm", "Hedef Alan \304\260maj\304\261", 0));
        label_2->setText(QApplication::translate("setupForm", "Mono \304\260maj", 0));
        label_3->setText(QApplication::translate("setupForm", "Tespit Edilen Kenarlar", 0));
        label_4->setText(QApplication::translate("setupForm", "\303\207izgi Tespiti", 0));
        label_5->setText(QApplication::translate("setupForm", "K\303\266\305\237e Tespiti", 0));
        captureButton->setText(QApplication::translate("setupForm", "Yakala", 0));
        checkSaveTarget->setText(QApplication::translate("setupForm", "\304\260maj", 0));
        checkSaveMono->setText(QApplication::translate("setupForm", "\304\260maj", 0));
        checkSaveEdge->setText(QApplication::translate("setupForm", "\304\260maj", 0));
        checkSaveHough->setText(QApplication::translate("setupForm", "\304\260maj", 0));
        checkSaveAnalyze->setText(QApplication::translate("setupForm", "\304\260maj", 0));
        checkSaveMonoMatrix->setText(QApplication::translate("setupForm", "Matrix", 0));
        checkSaveEdgeMatrix->setText(QApplication::translate("setupForm", "Matrix", 0));
        checkSaveHoughMatrix->setText(QApplication::translate("setupForm", "Matrix", 0));
        saveButton->setText(QApplication::translate("setupForm", "Se\303\247ilileri Kaydet", 0));
        checkSaveHoughLines->setText(QApplication::translate("setupForm", "\303\207izgi Matrixi", 0));
        checkSaveHoughSpace->setText(QApplication::translate("setupForm", "HSpace", 0));
        editHoughLineNo->setInputMask(QApplication::translate("setupForm", "999", 0));
        editHoughLineNo->setText(QApplication::translate("setupForm", "2", 0));
        label_6->setText(QApplication::translate("setupForm", "\303\207izgi Say\304\261s\304\261", 0));
        label_7->setText(QApplication::translate("setupForm", "E\305\237ik", 0));
        editHoughThreshold->setInputMask(QApplication::translate("setupForm", "999", 0));
        editHoughThreshold->setText(QApplication::translate("setupForm", "200", 0));
        labelVoteAvg->setText(QApplication::translate("setupForm", "Avg", 0));
        labelPrimaryLine->setText(QApplication::translate("setupForm", "\303\207\304\260ZG\304\260 BULUNAMIYOR!", 0));
        editHoughThetaStep->setInputMask(QApplication::translate("setupForm", "9.9", 0));
        editHoughThetaStep->setText(QApplication::translate("setupForm", "1.0", 0));
        label_8->setText(QApplication::translate("setupForm", "A\303\247\304\261 \303\207\303\266z.", 0));
        editHoughThetaMin->setInputMask(QApplication::translate("setupForm", "999", 0));
        editHoughThetaMin->setText(QApplication::translate("setupForm", "87", 0));
        label_9->setText(QApplication::translate("setupForm", "A\303\247\304\261 Aral\304\261\304\237\304\261", 0));
        editHoughThetaMax->setInputMask(QApplication::translate("setupForm", "999", 0));
        editHoughThetaMax->setText(QApplication::translate("setupForm", "93", 0));
        plainTextEdit->setDocumentTitle(QString());
        plainTextEdit->setPlainText(QString());
        editVoidThreshold->setInputMask(QApplication::translate("setupForm", "99", 0));
        editVoidThreshold->setText(QApplication::translate("setupForm", "30", 0));
        label_10->setText(QApplication::translate("setupForm", "Void E\305\237i\304\237i", 0));
        saveExitButton->setText(QApplication::translate("setupForm", "Kaydet ve \303\207\304\261k", 0));
        exitButton->setText(QApplication::translate("setupForm", "\303\207\304\261k", 0));
        clearButton->setText(QApplication::translate("setupForm", "Temizle", 0));
        checkSubImage->setText(QApplication::translate("setupForm", "Alt \304\260maj", 0));
        editHoughThetaMinSub->setInputMask(QApplication::translate("setupForm", "999", 0));
        editHoughThetaMinSub->setText(QApplication::translate("setupForm", "80", 0));
        editHoughThetaStepSub->setInputMask(QApplication::translate("setupForm", "9.9", 0));
        editHoughThetaStepSub->setText(QApplication::translate("setupForm", "1.0", 0));
        editHoughThetaMaxSub->setInputMask(QApplication::translate("setupForm", "999", 0));
        editHoughThetaMaxSub->setText(QApplication::translate("setupForm", "100", 0));
        label_13->setText(QApplication::translate("setupForm", "FPS", 0));
        label_14->setText(QApplication::translate("setupForm", "\304\260.\304\260\305\237leme Aral\304\261\304\237\304\261", 0));
        editIPI->setInputMask(QApplication::translate("setupForm", "99", 0));
        editIPI->setText(QApplication::translate("setupForm", "3", 0));
        editFPS->setInputMask(QApplication::translate("setupForm", "99", 0));
        editFPS->setText(QApplication::translate("setupForm", "10", 0));
        zctrlBox->setText(QApplication::translate("setupForm", "Y\303\274kseklik Kontrol\303\274", 0));
#ifndef QT_NO_TOOLTIP
        hardControlStartBox->setToolTip(QApplication::translate("setupForm", "Makineden Kaynak Ba\305\237lang\304\261c\304\261", 0));
#endif // QT_NO_TOOLTIP
        hardControlStartBox->setText(QApplication::translate("setupForm", "Hard Kaynak Ba\305\237lang\304\261c\304\261", 0));
#ifndef QT_NO_TOOLTIP
        widthControlBox->setToolTip(QApplication::translate("setupForm", "Kaynak ba\305\237lang\304\261c\304\261ndaki geni\305\237li\304\237in %80 ile %120 si aras\304\261n\304\261 ge\303\247erli say", 0));
#endif // QT_NO_TOOLTIP
        widthControlBox->setText(QApplication::translate("setupForm", "Geni\305\237lik Kontrol\303\274", 0));
        label_15->setText(QApplication::translate("setupForm", "PLC B\304\260LG\304\260", 0));
#ifndef QT_NO_TOOLTIP
        readMachineStatusBox->setToolTip(QApplication::translate("setupForm", "DB2.DB0.x", 0));
#endif // QT_NO_TOOLTIP
        readMachineStatusBox->setText(QApplication::translate("setupForm", "Makine \303\207al\304\261\305\237ma Durumu Bilgisi", 0));
#ifndef QT_NO_TOOLTIP
        readDistanceBox->setToolTip(QApplication::translate("setupForm", "DBX.DBW2", 0));
#endif // QT_NO_TOOLTIP
        readDistanceBox->setText(QApplication::translate("setupForm", "Mesafe Bilgisi", 0));
#ifndef QT_NO_TOOLTIP
        dynamicAlgoBox->setToolTip(QApplication::translate("setupForm", "\304\260KA ya Otomaik Ge\303\247", 0));
#endif // QT_NO_TOOLTIP
        dynamicAlgoBox->setText(QApplication::translate("setupForm", "Dinamik Algoritma", 0));
#ifndef QT_NO_TOOLTIP
        readWeldSeamBox->setToolTip(QApplication::translate("setupForm", "DB2.DB0.x", 0));
#endif // QT_NO_TOOLTIP
        readWeldSeamBox->setText(QApplication::translate("setupForm", "Kaynak \304\260zi  Bilgisi", 0));
#ifndef QT_NO_TOOLTIP
        timeControlBox->setToolTip(QApplication::translate("setupForm", "\304\260KA ya Otomaik Ge\303\247", 0));
#endif // QT_NO_TOOLTIP
        timeControlBox->setText(QApplication::translate("setupForm", "Zaman Kontrol\303\274", 0));
#ifndef QT_NO_TOOLTIP
        lineDetectionBox->setToolTip(QApplication::translate("setupForm", "\304\260KA ya Otomaik Ge\303\247", 0));
#endif // QT_NO_TOOLTIP
        lineDetectionBox->setText(QApplication::translate("setupForm", "\303\207izgi Tespiti", 0));
        editLineScore->setInputMask(QApplication::translate("setupForm", "99", 0));
        editLineScore->setText(QApplication::translate("setupForm", "0", 0));
        label_16->setText(QApplication::translate("setupForm", "(%)", 0));
        captureButton_2->setText(QApplication::translate("setupForm", "Resim Y\303\274kle", 0));
#ifndef QT_NO_TOOLTIP
        cannyThinningBox->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        cannyThinningBox->setText(QApplication::translate("setupForm", "Canny \304\260nceltme", 0));
        labelBrightness->setText(QApplication::translate("setupForm", "0", 0));
        labelContrast->setText(QApplication::translate("setupForm", "0", 0));
        labelGamma->setText(QApplication::translate("setupForm", "0", 0));
        brightnessReset->setText(QApplication::translate("setupForm", "R", 0));
        contrastReset->setText(QApplication::translate("setupForm", "R", 0));
        gammaReset->setText(QApplication::translate("setupForm", "R", 0));
        checkProcessing->setText(QApplication::translate("setupForm", "Analiz Aktif", 0));
        labelGaussSize->setText(QApplication::translate("setupForm", "0", 0));
        labelGaussSDev->setText(QApplication::translate("setupForm", "0", 0));
        algorithmBox->clear();
        algorithmBox->insertItems(0, QStringList()
         << QApplication::translate("setupForm", "Canny", 0)
         << QApplication::translate("setupForm", "Hough Transform", 0)
        );
        radioLaser->setText(QApplication::translate("setupForm", "Lazerli Takip", 0));
        radioWoLaser->setText(QApplication::translate("setupForm", "Lazersiz Takip", 0));
        edgeDetectionBox->clear();
        edgeDetectionBox->insertItems(0, QStringList()
         << QApplication::translate("setupForm", "Yok", 0)
         << QApplication::translate("setupForm", "Sobel", 0)
         << QApplication::translate("setupForm", "Canny", 0)
        );
        detecAlgoBox->clear();
        detecAlgoBox->insertItems(0, QStringList()
         << QApplication::translate("setupForm", "Yok", 0)
         << QApplication::translate("setupForm", "detectLongestSolidLines", 0)
         << QApplication::translate("setupForm", "detectMainEdges", 0)
         << QApplication::translate("setupForm", "detectPrimaryVoid", 0)
         << QApplication::translate("setupForm", "detectThinJointCenter", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class setupForm: public Ui_setupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPFORM_H

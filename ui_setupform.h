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

    void setupUi(QWidget *setupForm)
    {
        if (setupForm->objectName().isEmpty())
            setupForm->setObjectName(QStringLiteral("setupForm"));
        setupForm->setWindowModality(Qt::WindowModal);
        setupForm->resize(1349, 800);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(setupForm->sizePolicy().hasHeightForWidth());
        setupForm->setSizePolicy(sizePolicy);
        setupForm->setMinimumSize(QSize(1019, 635));
        setupForm->setMaximumSize(QSize(1500, 800));
        setupForm->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        setupForm->setWindowIcon(icon);
        labelTarget = new QLabel(setupForm);
        labelTarget->setObjectName(QStringLiteral("labelTarget"));
        labelTarget->setGeometry(QRect(20, 50, 320, 320));
        labelTarget->setFrameShape(QFrame::Box);
        labelTarget->setScaledContents(false);
        labelMono = new QLabel(setupForm);
        labelMono->setObjectName(QStringLiteral("labelMono"));
        labelMono->setGeometry(QRect(350, 50, 320, 320));
        labelMono->setFrameShape(QFrame::Box);
        labelEdge = new QLabel(setupForm);
        labelEdge->setObjectName(QStringLiteral("labelEdge"));
        labelEdge->setGeometry(QRect(680, 50, 320, 320));
        labelEdge->setFrameShape(QFrame::Box);
        labelHough = new QLabel(setupForm);
        labelHough->setObjectName(QStringLiteral("labelHough"));
        labelHough->setGeometry(QRect(1011, 50, 320, 320));
        labelHough->setFrameShape(QFrame::Box);
        labelAnalyze = new QLabel(setupForm);
        labelAnalyze->setObjectName(QStringLiteral("labelAnalyze"));
        labelAnalyze->setGeometry(QRect(1010, 420, 320, 320));
        labelAnalyze->setFrameShape(QFrame::Box);
        label = new QLabel(setupForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 101, 16));
        label_2 = new QLabel(setupForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(350, 30, 111, 16));
        label_3 = new QLabel(setupForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(680, 30, 141, 16));
        label_4 = new QLabel(setupForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(1011, 30, 81, 16));
        label_5 = new QLabel(setupForm);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(1010, 400, 91, 16));
        captureButton = new QPushButton(setupForm);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setGeometry(QRect(20, 3, 91, 25));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        captureButton->setFont(font);
        checkSaveTarget = new QCheckBox(setupForm);
        checkSaveTarget->setObjectName(QStringLiteral("checkSaveTarget"));
        checkSaveTarget->setGeometry(QRect(20, 380, 71, 17));
        checkSaveMono = new QCheckBox(setupForm);
        checkSaveMono->setObjectName(QStringLiteral("checkSaveMono"));
        checkSaveMono->setGeometry(QRect(350, 380, 71, 17));
        checkSaveEdge = new QCheckBox(setupForm);
        checkSaveEdge->setObjectName(QStringLiteral("checkSaveEdge"));
        checkSaveEdge->setGeometry(QRect(680, 380, 71, 17));
        checkSaveHough = new QCheckBox(setupForm);
        checkSaveHough->setObjectName(QStringLiteral("checkSaveHough"));
        checkSaveHough->setGeometry(QRect(1010, 380, 71, 17));
        checkSaveAnalyze = new QCheckBox(setupForm);
        checkSaveAnalyze->setObjectName(QStringLiteral("checkSaveAnalyze"));
        checkSaveAnalyze->setGeometry(QRect(1010, 750, 71, 17));
        checkSaveMonoMatrix = new QCheckBox(setupForm);
        checkSaveMonoMatrix->setObjectName(QStringLiteral("checkSaveMonoMatrix"));
        checkSaveMonoMatrix->setGeometry(QRect(410, 380, 81, 17));
        checkSaveEdgeMatrix = new QCheckBox(setupForm);
        checkSaveEdgeMatrix->setObjectName(QStringLiteral("checkSaveEdgeMatrix"));
        checkSaveEdgeMatrix->setGeometry(QRect(740, 380, 81, 17));
        checkSaveHoughMatrix = new QCheckBox(setupForm);
        checkSaveHoughMatrix->setObjectName(QStringLiteral("checkSaveHoughMatrix"));
        checkSaveHoughMatrix->setGeometry(QRect(1070, 380, 81, 17));
        saveButton = new QPushButton(setupForm);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(120, 3, 131, 25));
        saveButton->setFont(font);
        checkSaveHoughLines = new QCheckBox(setupForm);
        checkSaveHoughLines->setObjectName(QStringLiteral("checkSaveHoughLines"));
        checkSaveHoughLines->setGeometry(QRect(1140, 380, 101, 17));
        checkSaveHoughSpace = new QCheckBox(setupForm);
        checkSaveHoughSpace->setObjectName(QStringLiteral("checkSaveHoughSpace"));
        checkSaveHoughSpace->setGeometry(QRect(1240, 380, 81, 17));
        editHoughLineNo = new QLineEdit(setupForm);
        editHoughLineNo->setObjectName(QStringLiteral("editHoughLineNo"));
        editHoughLineNo->setGeometry(QRect(140, 550, 42, 20));
        editHoughLineNo->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(setupForm);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 550, 111, 21));
        label_6->setFont(font);
        label_6->setFrameShape(QFrame::Box);
        label_6->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label_7 = new QLabel(setupForm);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 580, 111, 21));
        label_7->setFont(font);
        label_7->setFrameShape(QFrame::Box);
        label_7->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThreshold = new QLineEdit(setupForm);
        editHoughThreshold->setObjectName(QStringLiteral("editHoughThreshold"));
        editHoughThreshold->setGeometry(QRect(140, 580, 42, 20));
        editHoughThreshold->setAlignment(Qt::AlignCenter);
        labelVoteAvg = new QLabel(setupForm);
        labelVoteAvg->setObjectName(QStringLiteral("labelVoteAvg"));
        labelVoteAvg->setGeometry(QRect(190, 580, 42, 20));
        labelVoteAvg->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelVoteAvg->setFrameShape(QFrame::Box);
        labelVoteAvg->setAlignment(Qt::AlignCenter);
        labelPrimaryLine = new QLabel(setupForm);
        labelPrimaryLine->setObjectName(QStringLiteral("labelPrimaryLine"));
        labelPrimaryLine->setEnabled(true);
        labelPrimaryLine->setGeometry(QRect(1160, 30, 171, 20));
        labelPrimaryLine->setFont(font);
        labelPrimaryLine->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelPrimaryLine->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        editHoughThetaStep = new QLineEdit(setupForm);
        editHoughThetaStep->setObjectName(QStringLiteral("editHoughThetaStep"));
        editHoughThetaStep->setGeometry(QRect(140, 640, 42, 20));
        editHoughThetaStep->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(setupForm);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 640, 111, 21));
        label_8->setFont(font);
        label_8->setFrameShape(QFrame::Box);
        label_8->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaMin = new QLineEdit(setupForm);
        editHoughThetaMin->setObjectName(QStringLiteral("editHoughThetaMin"));
        editHoughThetaMin->setGeometry(QRect(140, 610, 42, 20));
        editHoughThetaMin->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(setupForm);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 610, 111, 21));
        label_9->setFont(font);
        label_9->setFrameShape(QFrame::Box);
        label_9->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaMax = new QLineEdit(setupForm);
        editHoughThetaMax->setObjectName(QStringLiteral("editHoughThetaMax"));
        editHoughThetaMax->setGeometry(QRect(190, 610, 42, 20));
        editHoughThetaMax->setAlignment(Qt::AlignCenter);
        plainTextEdit = new QPlainTextEdit(setupForm);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(620, 420, 321, 191));
        QFont font1;
        font1.setFamily(QStringLiteral("Tahoma"));
        plainTextEdit->setFont(font1);
        editVoidThreshold = new QLineEdit(setupForm);
        editVoidThreshold->setObjectName(QStringLiteral("editVoidThreshold"));
        editVoidThreshold->setGeometry(QRect(140, 670, 42, 20));
        editVoidThreshold->setAlignment(Qt::AlignCenter);
        label_10 = new QLabel(setupForm);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(20, 670, 111, 21));
        label_10->setFont(font);
        label_10->setFrameShape(QFrame::Box);
        label_10->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        saveExitButton = new QPushButton(setupForm);
        saveExitButton->setObjectName(QStringLiteral("saveExitButton"));
        saveExitButton->setGeometry(QRect(360, 760, 171, 25));
        saveExitButton->setFont(font);
        saveExitButton->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        exitButton = new QPushButton(setupForm);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(360, 730, 85, 25));
        exitButton->setFont(font);
        exitButton->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        clearButton = new QPushButton(setupForm);
        clearButton->setObjectName(QStringLiteral("clearButton"));
        clearButton->setGeometry(QRect(360, 700, 85, 25));
        clearButton->setFont(font);
        checkSubImage = new QCheckBox(setupForm);
        checkSubImage->setObjectName(QStringLiteral("checkSubImage"));
        checkSubImage->setGeometry(QRect(20, 710, 181, 17));
        label_11 = new QLabel(setupForm);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(20, 760, 71, 21));
        label_11->setFont(font);
        label_11->setFrameShape(QFrame::Box);
        label_11->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaMinSub = new QLineEdit(setupForm);
        editHoughThetaMinSub->setObjectName(QStringLiteral("editHoughThetaMinSub"));
        editHoughThetaMinSub->setGeometry(QRect(110, 730, 42, 20));
        editHoughThetaMinSub->setAlignment(Qt::AlignCenter);
        label_12 = new QLabel(setupForm);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(20, 730, 71, 21));
        label_12->setFont(font);
        label_12->setFrameShape(QFrame::Box);
        label_12->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editHoughThetaStepSub = new QLineEdit(setupForm);
        editHoughThetaStepSub->setObjectName(QStringLiteral("editHoughThetaStepSub"));
        editHoughThetaStepSub->setGeometry(QRect(110, 760, 42, 20));
        editHoughThetaStepSub->setAlignment(Qt::AlignCenter);
        editHoughThetaMaxSub = new QLineEdit(setupForm);
        editHoughThetaMaxSub->setObjectName(QStringLiteral("editHoughThetaMaxSub"));
        editHoughThetaMaxSub->setGeometry(QRect(160, 730, 42, 20));
        editHoughThetaMaxSub->setAlignment(Qt::AlignCenter);
        line = new QFrame(setupForm);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(10, 700, 311, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(setupForm);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(9, 780, 311, 16));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_13 = new QLabel(setupForm);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(360, 430, 111, 21));
        label_13->setFont(font);
        label_13->setFrameShape(QFrame::Box);
        label_13->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label_14 = new QLabel(setupForm);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(360, 460, 111, 21));
        label_14->setFont(font);
        label_14->setFrameShape(QFrame::Box);
        label_14->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        editIPI = new QLineEdit(setupForm);
        editIPI->setObjectName(QStringLiteral("editIPI"));
        editIPI->setGeometry(QRect(480, 460, 42, 20));
        editIPI->setMaxLength(2);
        editIPI->setAlignment(Qt::AlignCenter);
        editFPS = new QLineEdit(setupForm);
        editFPS->setObjectName(QStringLiteral("editFPS"));
        editFPS->setGeometry(QRect(480, 430, 42, 20));
        editFPS->setAlignment(Qt::AlignCenter);
        subTypeBox = new QComboBox(setupForm);
        subTypeBox->setObjectName(QStringLiteral("subTypeBox"));
        subTypeBox->setGeometry(QRect(210, 730, 111, 22));
        zctrlBox = new QCheckBox(setupForm);
        zctrlBox->setObjectName(QStringLiteral("zctrlBox"));
        zctrlBox->setEnabled(false);
        zctrlBox->setGeometry(QRect(360, 490, 131, 17));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        zctrlBox->setFont(font2);
        hardControlStartBox = new QCheckBox(setupForm);
        hardControlStartBox->setObjectName(QStringLiteral("hardControlStartBox"));
        hardControlStartBox->setGeometry(QRect(360, 550, 161, 17));
        hardControlStartBox->setFont(font2);
        widthControlBox = new QCheckBox(setupForm);
        widthControlBox->setObjectName(QStringLiteral("widthControlBox"));
        widthControlBox->setGeometry(QRect(360, 520, 161, 17));
        widthControlBox->setFont(font2);
        label_15 = new QLabel(setupForm);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(620, 680, 231, 101));
        label_15->setFont(font);
        label_15->setFrameShape(QFrame::Box);
        label_15->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        readMachineStatusBox = new QCheckBox(setupForm);
        readMachineStatusBox->setObjectName(QStringLiteral("readMachineStatusBox"));
        readMachineStatusBox->setGeometry(QRect(630, 710, 201, 17));
        readMachineStatusBox->setFont(font2);
        readDistanceBox = new QCheckBox(setupForm);
        readDistanceBox->setObjectName(QStringLiteral("readDistanceBox"));
        readDistanceBox->setGeometry(QRect(630, 730, 201, 17));
        readDistanceBox->setFont(font2);
        dynamicAlgoBox = new QCheckBox(setupForm);
        dynamicAlgoBox->setObjectName(QStringLiteral("dynamicAlgoBox"));
        dynamicAlgoBox->setGeometry(QRect(360, 580, 161, 17));
        dynamicAlgoBox->setFont(font2);
        readWeldSeamBox = new QCheckBox(setupForm);
        readWeldSeamBox->setObjectName(QStringLiteral("readWeldSeamBox"));
        readWeldSeamBox->setGeometry(QRect(630, 750, 201, 17));
        readWeldSeamBox->setFont(font2);
        timeControlBox = new QCheckBox(setupForm);
        timeControlBox->setObjectName(QStringLiteral("timeControlBox"));
        timeControlBox->setGeometry(QRect(360, 610, 161, 17));
        timeControlBox->setFont(font2);
        lineDetectionBox = new QCheckBox(setupForm);
        lineDetectionBox->setObjectName(QStringLiteral("lineDetectionBox"));
        lineDetectionBox->setGeometry(QRect(360, 640, 161, 17));
        lineDetectionBox->setFont(font2);
        editLineScore = new QLineEdit(setupForm);
        editLineScore->setObjectName(QStringLiteral("editLineScore"));
        editLineScore->setGeometry(QRect(480, 638, 42, 20));
        editLineScore->setAlignment(Qt::AlignCenter);
        label_16 = new QLabel(setupForm);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(530, 640, 31, 16));
        captureButton_2 = new QPushButton(setupForm);
        captureButton_2->setObjectName(QStringLiteral("captureButton_2"));
        captureButton_2->setGeometry(QRect(260, 3, 101, 25));
        captureButton_2->setFont(font);
        cannyThinningBox = new QCheckBox(setupForm);
        cannyThinningBox->setObjectName(QStringLiteral("cannyThinningBox"));
        cannyThinningBox->setGeometry(QRect(360, 670, 161, 17));
        cannyThinningBox->setFont(font2);
        brightnessSlider = new QSlider(setupForm);
        brightnessSlider->setObjectName(QStringLiteral("brightnessSlider"));
        brightnessSlider->setGeometry(QRect(30, 430, 160, 22));
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
        labelBrightness->setGeometry(QRect(210, 430, 42, 20));
        labelBrightness->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelBrightness->setFrameShape(QFrame::Box);
        labelBrightness->setAlignment(Qt::AlignCenter);
        contrastSlider = new QSlider(setupForm);
        contrastSlider->setObjectName(QStringLiteral("contrastSlider"));
        contrastSlider->setGeometry(QRect(30, 460, 160, 22));
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
        labelContrast->setGeometry(QRect(210, 460, 42, 20));
        labelContrast->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelContrast->setFrameShape(QFrame::Box);
        labelContrast->setAlignment(Qt::AlignCenter);
        gammaSlider = new QSlider(setupForm);
        gammaSlider->setObjectName(QStringLiteral("gammaSlider"));
        gammaSlider->setGeometry(QRect(30, 490, 160, 22));
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
        labelGamma->setGeometry(QRect(210, 490, 42, 20));
        labelGamma->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        labelGamma->setFrameShape(QFrame::Box);
        labelGamma->setAlignment(Qt::AlignCenter);
        brightnessReset = new QPushButton(setupForm);
        brightnessReset->setObjectName(QStringLiteral("brightnessReset"));
        brightnessReset->setGeometry(QRect(260, 430, 42, 22));
        brightnessReset->setFont(font);
        contrastReset = new QPushButton(setupForm);
        contrastReset->setObjectName(QStringLiteral("contrastReset"));
        contrastReset->setGeometry(QRect(260, 460, 42, 22));
        contrastReset->setFont(font);
        gammaReset = new QPushButton(setupForm);
        gammaReset->setObjectName(QStringLiteral("gammaReset"));
        gammaReset->setGeometry(QRect(260, 490, 42, 22));
        gammaReset->setFont(font);

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
        editHoughLineNo->setInputMask(QApplication::translate("setupForm", "99", 0));
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
        editHoughThetaMin->setText(QApplication::translate("setupForm", "80", 0));
        label_9->setText(QApplication::translate("setupForm", "A\303\247\304\261 Aral\304\261\304\237\304\261", 0));
        editHoughThetaMax->setInputMask(QApplication::translate("setupForm", "999", 0));
        editHoughThetaMax->setText(QApplication::translate("setupForm", "100", 0));
        plainTextEdit->setDocumentTitle(QString());
        plainTextEdit->setPlainText(QString());
        editVoidThreshold->setInputMask(QApplication::translate("setupForm", "99", 0));
        editVoidThreshold->setText(QApplication::translate("setupForm", "30", 0));
        label_10->setText(QApplication::translate("setupForm", "Void E\305\237i\304\237i", 0));
        saveExitButton->setText(QApplication::translate("setupForm", "Kaydet ve \303\207\304\261k", 0));
        exitButton->setText(QApplication::translate("setupForm", "\303\207\304\261k", 0));
        clearButton->setText(QApplication::translate("setupForm", "Temizle", 0));
        checkSubImage->setText(QApplication::translate("setupForm", "Alt \304\260maj Algoritmas\304\261", 0));
        label_11->setText(QApplication::translate("setupForm", "A\303\247\304\261 \303\207\303\266z.", 0));
        editHoughThetaMinSub->setInputMask(QApplication::translate("setupForm", "999", 0));
        editHoughThetaMinSub->setText(QApplication::translate("setupForm", "80", 0));
        label_12->setText(QApplication::translate("setupForm", "A\303\247\304\261 Aral\304\261\304\237\304\261", 0));
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
        subTypeBox->clear();
        subTypeBox->insertItems(0, QStringList()
         << QApplication::translate("setupForm", "Bo\305\237luk Algo", 0)
         << QApplication::translate("setupForm", "Doluluk Algo", 0)
        );
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
        cannyThinningBox->setToolTip(QApplication::translate("setupForm", "\304\260KA ya Otomaik Ge\303\247", 0));
#endif // QT_NO_TOOLTIP
        cannyThinningBox->setText(QApplication::translate("setupForm", "Canny \304\260nceltme", 0));
        labelBrightness->setText(QApplication::translate("setupForm", "0", 0));
        labelContrast->setText(QApplication::translate("setupForm", "0", 0));
        labelGamma->setText(QApplication::translate("setupForm", "0", 0));
        brightnessReset->setText(QApplication::translate("setupForm", "R", 0));
        contrastReset->setText(QApplication::translate("setupForm", "R", 0));
        gammaReset->setText(QApplication::translate("setupForm", "R", 0));
    } // retranslateUi

};

namespace Ui {
    class setupForm: public Ui_setupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPFORM_H

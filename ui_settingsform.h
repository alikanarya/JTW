/********************************************************************************
** Form generated from reading UI file 'settingsform.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSFORM_H
#define UI_SETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_settingsForm
{
public:
    QPushButton *setupButton;
    QLabel *label;
    QSlider *errorLimitSlider;
    QLabel *labelErrorLimit;
    QPushButton *saveExitButton;
    QPushButton *exitButton;
    QTableWidget *table;
    QSlider *yResIndexSlider;
    QLabel *labelyRes;
    QLabel *label_2;
    QCheckBox *checkPLConBoot;
    QCheckBox *checkCamonBoot;
    QFrame *line;
    QFrame *line_2;
    QRadioButton *radio200;
    QRadioButton *radio300;
    QLabel *label_3;
    QLineEdit *editDBNo;
    QLineEdit *editControlDelay;
    QLabel *label_4;
    QSlider *targetSlider;
    QLabel *label_5;
    QLabel *labelTarget;
    QLabel *label_6;
    QLabel *labelTargetVert;
    QSlider *targetVertSlider;
    QLabel *labelErrorScale;
    QSlider *errorScaleSlider;
    QLabel *label_7;
    QCheckBox *checkErrorStopVisible;
    QLabel *label_8;
    QLineEdit *editTitle;
    QCheckBox *checkPLCSIM;
    QCheckBox *checkTargetAlignButtons;
    QRadioButton *radioCamJpeg;
    QRadioButton *radioCamStream;
    QPushButton *apiCheckFocus;
    QPushButton *apiFocusStatus;
    QPlainTextEdit *plainTextEdit;
    QPushButton *apiAutoFocus;
    QPushButton *apiAutoFocus_2;
    QSlider *manFocusSlider;
    QButtonGroup *radioGroup;
    QButtonGroup *camStreamGroup;

    void setupUi(QWidget *settingsForm)
    {
        if (settingsForm->objectName().isEmpty())
            settingsForm->setObjectName(QStringLiteral("settingsForm"));
        settingsForm->setWindowModality(Qt::WindowModal);
        settingsForm->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(settingsForm->sizePolicy().hasHeightForWidth());
        settingsForm->setSizePolicy(sizePolicy);
        settingsForm->setMinimumSize(QSize(800, 600));
        settingsForm->setMaximumSize(QSize(800, 600));
        QIcon icon;
        icon.addFile(QStringLiteral(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsForm->setWindowIcon(icon);
        settingsForm->setWindowOpacity(1);
        setupButton = new QPushButton(settingsForm);
        setupButton->setObjectName(QStringLiteral("setupButton"));
        setupButton->setGeometry(QRect(10, 560, 80, 25));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        setupButton->setFont(font);
        setupButton->setFocusPolicy(Qt::NoFocus);
        label = new QLabel(settingsForm);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 53, 111, 16));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        label->setFont(font1);
        errorLimitSlider = new QSlider(settingsForm);
        errorLimitSlider->setObjectName(QStringLiteral("errorLimitSlider"));
        errorLimitSlider->setGeometry(QRect(170, 50, 100, 25));
        errorLimitSlider->setMinimum(2);
        errorLimitSlider->setMaximum(50);
        errorLimitSlider->setPageStep(5);
        errorLimitSlider->setOrientation(Qt::Horizontal);
        labelErrorLimit = new QLabel(settingsForm);
        labelErrorLimit->setObjectName(QStringLiteral("labelErrorLimit"));
        labelErrorLimit->setGeometry(QRect(129, 54, 22, 21));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        labelErrorLimit->setFont(font2);
        labelErrorLimit->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        saveExitButton = new QPushButton(settingsForm);
        saveExitButton->setObjectName(QStringLiteral("saveExitButton"));
        saveExitButton->setGeometry(QRect(160, 560, 131, 25));
        saveExitButton->setFont(font);
        saveExitButton->setFocusPolicy(Qt::NoFocus);
        saveExitButton->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        exitButton = new QPushButton(settingsForm);
        exitButton->setObjectName(QStringLiteral("exitButton"));
        exitButton->setGeometry(QRect(360, 560, 80, 25));
        exitButton->setFont(font);
        exitButton->setFocusPolicy(Qt::NoFocus);
        exitButton->setStyleSheet(QStringLiteral(""));
        table = new QTableWidget(settingsForm);
        if (table->columnCount() < 1)
            table->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setBackground(QColor(0, 0, 0));
        table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (table->rowCount() < 4)
            table->setRowCount(4);
        QFont font3;
        font3.setPointSize(8);
        font3.setBold(true);
        font3.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font3);
        __qtablewidgetitem1->setBackground(QColor(0, 0, 0));
        table->setVerticalHeaderItem(0, __qtablewidgetitem1);
        QFont font4;
        font4.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font4.setPointSize(8);
        font4.setBold(true);
        font4.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font4);
        table->setVerticalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font3);
        table->setVerticalHeaderItem(2, __qtablewidgetitem3);
        QFont font5;
        font5.setBold(true);
        font5.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font5);
        table->setVerticalHeaderItem(3, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table->setItem(0, 0, __qtablewidgetitem5);
        table->setObjectName(QStringLiteral("table"));
        table->setGeometry(QRect(10, 360, 491, 111));
        table->setFrameShape(QFrame::Box);
        table->setFrameShadow(QFrame::Plain);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setAutoScroll(false);
        table->setShowGrid(true);
        table->setGridStyle(Qt::SolidLine);
        table->setCornerButtonEnabled(true);
        table->setRowCount(4);
        table->setColumnCount(1);
        table->horizontalHeader()->setVisible(false);
        table->horizontalHeader()->setDefaultSectionSize(450);
        table->horizontalHeader()->setHighlightSections(true);
        table->horizontalHeader()->setMinimumSectionSize(450);
        table->verticalHeader()->setVisible(false);
        table->verticalHeader()->setCascadingSectionResizes(false);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->verticalHeader()->setHighlightSections(true);
        table->verticalHeader()->setMinimumSectionSize(25);
        yResIndexSlider = new QSlider(settingsForm);
        yResIndexSlider->setObjectName(QStringLiteral("yResIndexSlider"));
        yResIndexSlider->setGeometry(QRect(170, 478, 100, 25));
        yResIndexSlider->setMinimum(0);
        yResIndexSlider->setMaximum(8);
        yResIndexSlider->setSingleStep(1);
        yResIndexSlider->setPageStep(1);
        yResIndexSlider->setSliderPosition(0);
        yResIndexSlider->setOrientation(Qt::Horizontal);
        labelyRes = new QLabel(settingsForm);
        labelyRes->setObjectName(QStringLiteral("labelyRes"));
        labelyRes->setGeometry(QRect(140, 481, 31, 16));
        labelyRes->setFont(font2);
        labelyRes->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));
        label_2 = new QLabel(settingsForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 480, 91, 16));
        label_2->setFont(font1);
        checkPLConBoot = new QCheckBox(settingsForm);
        checkPLConBoot->setObjectName(QStringLiteral("checkPLConBoot"));
        checkPLConBoot->setGeometry(QRect(10, 220, 431, 17));
        checkPLConBoot->setFont(font1);
        checkCamonBoot = new QCheckBox(settingsForm);
        checkCamonBoot->setObjectName(QStringLiteral("checkCamonBoot"));
        checkCamonBoot->setGeometry(QRect(10, 200, 421, 17));
        checkCamonBoot->setFont(font1);
        line = new QFrame(settingsForm);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 540, 450, 16));
        line->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        line->setFrameShadow(QFrame::Plain);
        line->setLineWidth(2);
        line->setFrameShape(QFrame::HLine);
        line_2 = new QFrame(settingsForm);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(0, 290, 450, 16));
        line_2->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setLineWidth(2);
        line_2->setFrameShape(QFrame::HLine);
        radio200 = new QRadioButton(settingsForm);
        radioGroup = new QButtonGroup(settingsForm);
        radioGroup->setObjectName(QStringLiteral("radioGroup"));
        radioGroup->addButton(radio200);
        radio200->setObjectName(QStringLiteral("radio200"));
        radio200->setGeometry(QRect(10, 310, 82, 17));
        radio200->setFont(font);
        radio300 = new QRadioButton(settingsForm);
        radioGroup->addButton(radio300);
        radio300->setObjectName(QStringLiteral("radio300"));
        radio300->setGeometry(QRect(10, 330, 82, 17));
        radio300->setFont(font);
        label_3 = new QLabel(settingsForm);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(122, 330, 61, 16));
        label_3->setFont(font);
        editDBNo = new QLineEdit(settingsForm);
        editDBNo->setObjectName(QStringLiteral("editDBNo"));
        editDBNo->setGeometry(QRect(200, 327, 51, 20));
        editControlDelay = new QLineEdit(settingsForm);
        editControlDelay->setObjectName(QStringLiteral("editControlDelay"));
        editControlDelay->setEnabled(false);
        editControlDelay->setGeometry(QRect(310, 520, 80, 20));
        editControlDelay->setFont(font3);
        editControlDelay->setFocusPolicy(Qt::StrongFocus);
        editControlDelay->setStyleSheet(QLatin1String("color: rgb(0, 0, 255);\n"
"background-color: rgb(255, 0, 0);"));
        editControlDelay->setMaxLength(5);
        editControlDelay->setAlignment(Qt::AlignCenter);
        editControlDelay->setReadOnly(true);
        label_4 = new QLabel(settingsForm);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setEnabled(false);
        label_4->setGeometry(QRect(280, 500, 121, 20));
        QFont font6;
        font6.setFamily(QStringLiteral("Arial"));
        font6.setPointSize(8);
        font6.setBold(false);
        font6.setWeight(50);
        label_4->setFont(font6);
        targetSlider = new QSlider(settingsForm);
        targetSlider->setObjectName(QStringLiteral("targetSlider"));
        targetSlider->setGeometry(QRect(170, 110, 240, 25));
        targetSlider->setMinimum(100);
        targetSlider->setMaximum(320);
        targetSlider->setSingleStep(2);
        targetSlider->setPageStep(5);
        targetSlider->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(settingsForm);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 113, 91, 16));
        label_5->setFont(font1);
        labelTarget = new QLabel(settingsForm);
        labelTarget->setObjectName(QStringLiteral("labelTarget"));
        labelTarget->setGeometry(QRect(129, 110, 33, 21));
        labelTarget->setFont(font2);
        labelTarget->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));
        label_6 = new QLabel(settingsForm);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 143, 91, 16));
        label_6->setFont(font1);
        labelTargetVert = new QLabel(settingsForm);
        labelTargetVert->setObjectName(QStringLiteral("labelTargetVert"));
        labelTargetVert->setGeometry(QRect(129, 142, 33, 21));
        labelTargetVert->setFont(font2);
        labelTargetVert->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));
        targetVertSlider = new QSlider(settingsForm);
        targetVertSlider->setObjectName(QStringLiteral("targetVertSlider"));
        targetVertSlider->setGeometry(QRect(170, 140, 240, 25));
        targetVertSlider->setMinimum(100);
        targetVertSlider->setMaximum(320);
        targetVertSlider->setSingleStep(1);
        targetVertSlider->setPageStep(5);
        targetVertSlider->setValue(100);
        targetVertSlider->setOrientation(Qt::Horizontal);
        labelErrorScale = new QLabel(settingsForm);
        labelErrorScale->setObjectName(QStringLiteral("labelErrorScale"));
        labelErrorScale->setGeometry(QRect(129, 82, 33, 21));
        labelErrorScale->setFont(font2);
        labelErrorScale->setStyleSheet(QStringLiteral("color: rgb(0, 255, 0);"));
        errorScaleSlider = new QSlider(settingsForm);
        errorScaleSlider->setObjectName(QStringLiteral("errorScaleSlider"));
        errorScaleSlider->setGeometry(QRect(170, 80, 100, 25));
        errorScaleSlider->setMinimum(0);
        errorScaleSlider->setMaximum(100);
        errorScaleSlider->setPageStep(5);
        errorScaleSlider->setValue(100);
        errorScaleSlider->setOrientation(Qt::Horizontal);
        label_7 = new QLabel(settingsForm);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 83, 111, 16));
        label_7->setFont(font1);
        checkErrorStopVisible = new QCheckBox(settingsForm);
        checkErrorStopVisible->setObjectName(QStringLiteral("checkErrorStopVisible"));
        checkErrorStopVisible->setGeometry(QRect(280, 83, 171, 17));
        checkErrorStopVisible->setFont(font1);
        label_8 = new QLabel(settingsForm);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 23, 91, 16));
        label_8->setFont(font1);
        editTitle = new QLineEdit(settingsForm);
        editTitle->setObjectName(QStringLiteral("editTitle"));
        editTitle->setGeometry(QRect(170, 20, 231, 20));
        editTitle->setMaxLength(20);
        checkPLCSIM = new QCheckBox(settingsForm);
        checkPLCSIM->setObjectName(QStringLiteral("checkPLCSIM"));
        checkPLCSIM->setGeometry(QRect(270, 330, 171, 17));
        checkPLCSIM->setFont(font1);
        checkTargetAlignButtons = new QCheckBox(settingsForm);
        checkTargetAlignButtons->setObjectName(QStringLiteral("checkTargetAlignButtons"));
        checkTargetAlignButtons->setGeometry(QRect(170, 170, 281, 17));
        checkTargetAlignButtons->setFont(font1);
        radioCamJpeg = new QRadioButton(settingsForm);
        camStreamGroup = new QButtonGroup(settingsForm);
        camStreamGroup->setObjectName(QStringLiteral("camStreamGroup"));
        camStreamGroup->addButton(radioCamJpeg);
        radioCamJpeg->setObjectName(QStringLiteral("radioCamJpeg"));
        radioCamJpeg->setGeometry(QRect(280, 250, 82, 17));
        radioCamJpeg->setFont(font);
        radioCamStream = new QRadioButton(settingsForm);
        camStreamGroup->addButton(radioCamStream);
        radioCamStream->setObjectName(QStringLiteral("radioCamStream"));
        radioCamStream->setGeometry(QRect(170, 250, 111, 17));
        radioCamStream->setFont(font);
        apiCheckFocus = new QPushButton(settingsForm);
        apiCheckFocus->setObjectName(QStringLiteral("apiCheckFocus"));
        apiCheckFocus->setGeometry(QRect(510, 10, 131, 29));
        apiFocusStatus = new QPushButton(settingsForm);
        apiFocusStatus->setObjectName(QStringLiteral("apiFocusStatus"));
        apiFocusStatus->setGeometry(QRect(510, 50, 131, 29));
        plainTextEdit = new QPlainTextEdit(settingsForm);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(510, 360, 281, 231));
        QFont font7;
        font7.setPointSize(7);
        plainTextEdit->setFont(font7);
        plainTextEdit->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 195);"));
        plainTextEdit->setFrameShape(QFrame::Box);
        plainTextEdit->setFrameShadow(QFrame::Sunken);
        plainTextEdit->setLineWidth(1);
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setReadOnly(true);
        apiAutoFocus = new QPushButton(settingsForm);
        apiAutoFocus->setObjectName(QStringLiteral("apiAutoFocus"));
        apiAutoFocus->setGeometry(QRect(510, 90, 131, 29));
        apiAutoFocus_2 = new QPushButton(settingsForm);
        apiAutoFocus_2->setObjectName(QStringLiteral("apiAutoFocus_2"));
        apiAutoFocus_2->setGeometry(QRect(510, 130, 131, 29));
        manFocusSlider = new QSlider(settingsForm);
        manFocusSlider->setObjectName(QStringLiteral("manFocusSlider"));
        manFocusSlider->setGeometry(QRect(510, 170, 240, 25));
        manFocusSlider->setMinimum(0);
        manFocusSlider->setMaximum(100);
        manFocusSlider->setSingleStep(1);
        manFocusSlider->setPageStep(5);
        manFocusSlider->setValue(100);
        manFocusSlider->setOrientation(Qt::Horizontal);

        retranslateUi(settingsForm);
        QObject::connect(setupButton, SIGNAL(clicked()), settingsForm, SLOT(showSetupForm()));
        QObject::connect(errorLimitSlider, SIGNAL(sliderMoved(int)), settingsForm, SLOT(errorLimitSlider()));
        QObject::connect(saveExitButton, SIGNAL(clicked()), settingsForm, SLOT(saveExitButton()));
        QObject::connect(exitButton, SIGNAL(clicked()), settingsForm, SLOT(exitButton()));
        QObject::connect(yResIndexSlider, SIGNAL(sliderMoved(int)), settingsForm, SLOT(yResIndexSlider()));
        QObject::connect(checkPLConBoot, SIGNAL(stateChanged(int)), settingsForm, SLOT(checkPLConBoot()));
        QObject::connect(checkCamonBoot, SIGNAL(clicked()), settingsForm, SLOT(checkCamonBoot()));
        QObject::connect(radio200, SIGNAL(clicked()), settingsForm, SLOT(changePLCtype()));
        QObject::connect(radio300, SIGNAL(clicked()), settingsForm, SLOT(changePLCtype()));
        QObject::connect(editControlDelay, SIGNAL(returnPressed()), settingsForm, SLOT(getControlDelay()));
        QObject::connect(targetSlider, SIGNAL(sliderMoved(int)), settingsForm, SLOT(targetSlider()));
        QObject::connect(targetVertSlider, SIGNAL(sliderMoved(int)), settingsForm, SLOT(targetVertSlider()));
        QObject::connect(checkErrorStopVisible, SIGNAL(clicked()), settingsForm, SLOT(checkErrorStopLimitVisible()));
        QObject::connect(errorScaleSlider, SIGNAL(sliderMoved(int)), settingsForm, SLOT(errorScaleSlider()));
        QObject::connect(editTitle, SIGNAL(returnPressed()), settingsForm, SLOT(setTitle()));
        QObject::connect(radioCamStream, SIGNAL(clicked()), settingsForm, SLOT(changeCamStreamType()));
        QObject::connect(radioCamJpeg, SIGNAL(clicked()), settingsForm, SLOT(changeCamStreamType()));

        QMetaObject::connectSlotsByName(settingsForm);
    } // setupUi

    void retranslateUi(QWidget *settingsForm)
    {
        settingsForm->setWindowTitle(QApplication::translate("settingsForm", "Ayarlar", 0));
        setupButton->setText(QApplication::translate("settingsForm", "Kurulum", 0));
        label->setText(QApplication::translate("settingsForm", "Komut Start", 0));
        labelErrorLimit->setText(QApplication::translate("settingsForm", "10", 0));
        saveExitButton->setText(QApplication::translate("settingsForm", "Kaydet ve \303\207\304\261k", 0));
        exitButton->setText(QApplication::translate("settingsForm", "\303\207\304\261k", 0));
        QTableWidgetItem *___qtablewidgetitem = table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("settingsForm", "Deger", 0));
        QTableWidgetItem *___qtablewidgetitem1 = table->verticalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("settingsForm", "Kamera JPEG", 0));
        QTableWidgetItem *___qtablewidgetitem2 = table->verticalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("settingsForm", "Kamera Stream", 0));
        QTableWidgetItem *___qtablewidgetitem3 = table->verticalHeaderItem(2);
        ___qtablewidgetitem3->setText(QApplication::translate("settingsForm", "PLC IP Adresi", 0));
        QTableWidgetItem *___qtablewidgetitem4 = table->verticalHeaderItem(3);
        ___qtablewidgetitem4->setText(QApplication::translate("settingsForm", "PLC Byte No", 0));
#ifndef QT_NO_TOOLTIP
        ___qtablewidgetitem4->setToolTip(QApplication::translate("settingsForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ff0000;\">PLC Datablo\304\237una yap\304\261lmas\304\261 gereken girdiler</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ff0000;\">Bit X.0: Eklem SA\304\236DA</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ff0000;\">Bit X.1: Eklem SOLDA</span></p>\n"
"<p style=\" m"
                        "argin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ff0000;\">Bit X.2: Stop</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; color:#ff0000;\">Bit X.3: Acil Stop</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600; color:#ff0000;\"></p></body></html>", 0));
#endif // QT_NO_TOOLTIP

        const bool __sortingEnabled = table->isSortingEnabled();
        table->setSortingEnabled(false);
        table->setSortingEnabled(__sortingEnabled);

        labelyRes->setText(QApplication::translate("settingsForm", "10", 0));
        label_2->setText(QApplication::translate("settingsForm", "Takip Trendi", 0));
        checkPLConBoot->setText(QApplication::translate("settingsForm", "Program A\303\247\304\261ld\304\261\304\237\304\261nda PLC'ye Otomatik Ba\304\237lans\304\261n", 0));
        checkCamonBoot->setText(QApplication::translate("settingsForm", "Program A\303\247\304\261ld\304\261\304\237\304\261nda Kamera'ya Otomatik Ba\304\237lans\304\261n", 0));
        radio200->setText(QApplication::translate("settingsForm", "S7-200", 0));
        radio300->setText(QApplication::translate("settingsForm", "S7-300", 0));
        label_3->setText(QApplication::translate("settingsForm", "DB No:", 0));
        editDBNo->setInputMask(QApplication::translate("settingsForm", "999", 0));
#ifndef QT_NO_TOOLTIP
        editControlDelay->setToolTip(QApplication::translate("settingsForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Boru besleme h\304\261z\304\261d\304\261r.</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Kameran\304\261n g\303\266rd\303\274\304\237\303\274 alana</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font"
                        "-weight:600;\">kaynak ba\305\237l\304\261\304\237\304\261n\304\261n gelme zaman\304\261n\304\261 ifade eder.</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Birimi mili saniyedir (1 sn = 1000 ms).</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Do\304\237ru zamanda komutun verilebilmesi i\303\247in \303\266nemlidir.</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Hassas \303\266l\303\247\303\274m yap\304\261n\304\261z.</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        editControlDelay->setInputMask(QString());
        editControlDelay->setText(QString());
        label_4->setText(QApplication::translate("settingsForm", "Kontrol Gecikmesi", 0));
        label_5->setText(QApplication::translate("settingsForm", "Hedef Yatay", 0));
        labelTarget->setText(QApplication::translate("settingsForm", "100", 0));
        label_6->setText(QApplication::translate("settingsForm", "Hedef Dikey", 0));
        labelTargetVert->setText(QApplication::translate("settingsForm", "50", 0));
        labelErrorScale->setText(QApplication::translate("settingsForm", "100", 0));
        label_7->setText(QApplication::translate("settingsForm", "Stop Y\303\274zdesi", 0));
        checkErrorStopVisible->setText(QApplication::translate("settingsForm", "Ekranda G\303\266r\303\274ns\303\274n", 0));
        label_8->setText(QApplication::translate("settingsForm", "Ba\305\237l\304\261k", 0));
#ifndef QT_NO_TOOLTIP
        editTitle->setToolTip(QApplication::translate("settingsForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ff0000;\">Aktif olabilmesi i\303\247in program\304\261 yeniden ba\305\237latman\304\261z gerekir.</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        editTitle->setInputMask(QString());
        editTitle->setText(QApplication::translate("settingsForm", "KAYNAK TAK\304\260B\304\260", 0));
        checkPLCSIM->setText(QApplication::translate("settingsForm", "PLC S\304\260M\303\234LASYON", 0));
        checkTargetAlignButtons->setText(QApplication::translate("settingsForm", "Hedef Kayd\304\261rma D\303\274\304\237meleri Aktif", 0));
        radioCamJpeg->setText(QApplication::translate("settingsForm", "JPEG", 0));
        radioCamStream->setText(QApplication::translate("settingsForm", "STREAM", 0));
        apiCheckFocus->setText(QApplication::translate("settingsForm", "Odak Kontrol\303\274", 0));
        apiFocusStatus->setText(QApplication::translate("settingsForm", "Odak De\304\237erleri", 0));
        apiAutoFocus->setText(QApplication::translate("settingsForm", "Otomatik Fokus", 0));
        apiAutoFocus_2->setText(QApplication::translate("settingsForm", "Otomatik Fokus", 0));
    } // retranslateUi

};

namespace Ui {
    class settingsForm: public Ui_settingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSFORM_H

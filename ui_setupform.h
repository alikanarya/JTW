/********************************************************************************
** Form generated from reading UI file 'setupform.ui'
**
** Created: Mon 20. Dec 16:09:54 2010
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

    void setupUi(QWidget *setupForm)
    {
        if (setupForm->objectName().isEmpty())
            setupForm->setObjectName(QString::fromUtf8("setupForm"));
        setupForm->setWindowModality(Qt::WindowModal);
        setupForm->resize(1019, 497);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(setupForm->sizePolicy().hasHeightForWidth());
        setupForm->setSizePolicy(sizePolicy);
        setupForm->setMinimumSize(QSize(1019, 497));
        setupForm->setMaximumSize(QSize(1019, 497));
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
        labelMono->setGeometry(QRect(350, 50, 320, 100));
        labelMono->setFrameShape(QFrame::Box);
        labelEdge = new QLabel(setupForm);
        labelEdge->setObjectName(QString::fromUtf8("labelEdge"));
        labelEdge->setGeometry(QRect(680, 50, 320, 100));
        labelEdge->setFrameShape(QFrame::Box);
        labelHough = new QLabel(setupForm);
        labelHough->setObjectName(QString::fromUtf8("labelHough"));
        labelHough->setGeometry(QRect(20, 200, 320, 100));
        labelHough->setFrameShape(QFrame::Box);
        labelAnalyze = new QLabel(setupForm);
        labelAnalyze->setObjectName(QString::fromUtf8("labelAnalyze"));
        labelAnalyze->setGeometry(QRect(20, 350, 320, 100));
        labelAnalyze->setFrameShape(QFrame::Box);
        label = new QLabel(setupForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 81, 16));
        label_2 = new QLabel(setupForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(350, 30, 111, 16));
        label_3 = new QLabel(setupForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(680, 30, 141, 16));
        label_4 = new QLabel(setupForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 180, 81, 16));
        label_5 = new QLabel(setupForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 330, 91, 16));
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
        checkSaveMono->setGeometry(QRect(350, 160, 71, 17));
        checkSaveEdge = new QCheckBox(setupForm);
        checkSaveEdge->setObjectName(QString::fromUtf8("checkSaveEdge"));
        checkSaveEdge->setGeometry(QRect(680, 160, 71, 17));
        checkSaveHough = new QCheckBox(setupForm);
        checkSaveHough->setObjectName(QString::fromUtf8("checkSaveHough"));
        checkSaveHough->setGeometry(QRect(20, 310, 71, 17));
        checkSaveAnalyze = new QCheckBox(setupForm);
        checkSaveAnalyze->setObjectName(QString::fromUtf8("checkSaveAnalyze"));
        checkSaveAnalyze->setGeometry(QRect(20, 460, 71, 17));
        checkSaveMonoMatrix = new QCheckBox(setupForm);
        checkSaveMonoMatrix->setObjectName(QString::fromUtf8("checkSaveMonoMatrix"));
        checkSaveMonoMatrix->setGeometry(QRect(400, 160, 81, 17));
        checkSaveEdgeMatrix = new QCheckBox(setupForm);
        checkSaveEdgeMatrix->setObjectName(QString::fromUtf8("checkSaveEdgeMatrix"));
        checkSaveEdgeMatrix->setGeometry(QRect(730, 160, 81, 17));
        checkSaveHoughMatrix = new QCheckBox(setupForm);
        checkSaveHoughMatrix->setObjectName(QString::fromUtf8("checkSaveHoughMatrix"));
        checkSaveHoughMatrix->setGeometry(QRect(70, 310, 81, 17));
        saveButton = new QPushButton(setupForm);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(230, 20, 111, 25));
        saveButton->setFont(font);
        checkSaveHoughLines = new QCheckBox(setupForm);
        checkSaveHoughLines->setObjectName(QString::fromUtf8("checkSaveHoughLines"));
        checkSaveHoughLines->setGeometry(QRect(130, 310, 101, 17));
        checkSaveHoughSpace = new QCheckBox(setupForm);
        checkSaveHoughSpace->setObjectName(QString::fromUtf8("checkSaveHoughSpace"));
        checkSaveHoughSpace->setGeometry(QRect(220, 310, 81, 17));
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
        labelPrimaryLine->setGeometry(QRect(200, 180, 140, 16));
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
        plainTextEdit->setGeometry(QRect(680, 190, 321, 241));
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
        saveExitButton->setGeometry(QRect(800, 470, 80, 25));
        saveExitButton->setFont(font);
        saveExitButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        exitButton = new QPushButton(setupForm);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(920, 470, 80, 25));
        exitButton->setFont(font);
        exitButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        clearButton = new QPushButton(setupForm);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        clearButton->setGeometry(QRect(800, 440, 80, 25));
        clearButton->setFont(font);
        label_9->raise();
        label_7->raise();
        label_6->raise();
        label_8->raise();
        label_10->raise();
        labelTarget->raise();
        labelMono->raise();
        labelEdge->raise();
        labelHough->raise();
        labelAnalyze->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        label_5->raise();
        captureButton->raise();
        checkSaveTarget->raise();
        checkSaveMono->raise();
        checkSaveEdge->raise();
        checkSaveHough->raise();
        checkSaveAnalyze->raise();
        checkSaveMonoMatrix->raise();
        checkSaveEdgeMatrix->raise();
        checkSaveHoughMatrix->raise();
        saveButton->raise();
        checkSaveHoughLines->raise();
        checkSaveHoughSpace->raise();
        labelVoteAvg->raise();
        labelPrimaryLine->raise();
        plainTextEdit->raise();
        saveExitButton->raise();
        exitButton->raise();
        clearButton->raise();
        editHoughThetaMax->raise();
        editHoughThetaMin->raise();
        editHoughThreshold->raise();
        editHoughThetaStep->raise();
        editVoidThreshold->raise();
        editHoughLineNo->raise();

        retranslateUi(setupForm);
        QObject::connect(captureButton, SIGNAL(clicked()), setupForm, SLOT(captureButton()));
        QObject::connect(saveButton, SIGNAL(clicked()), setupForm, SLOT(saveButton()));
        QObject::connect(clearButton, SIGNAL(clicked()), setupForm, SLOT(clearButton()));
        QObject::connect(exitButton, SIGNAL(clicked()), setupForm, SLOT(exitButton()));
        QObject::connect(saveExitButton, SIGNAL(clicked()), setupForm, SLOT(saveExitButton()));

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
        saveExitButton->setText(QApplication::translate("setupForm", "Kaydet & \303\207\304\261k", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("setupForm", "\303\207\304\261k", 0, QApplication::UnicodeUTF8));
        clearButton->setText(QApplication::translate("setupForm", "Temizle", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class setupForm: public Ui_setupForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPFORM_H

/********************************************************************************
** Form generated from reading UI file 'settingsform.ui'
**
** Created: Sun Jan 2 12:30:02 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSFORM_H
#define UI_SETTINGSFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

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
    QButtonGroup *radioGroup;

    void setupUi(QWidget *settingsForm)
    {
        if (settingsForm->objectName().isEmpty())
            settingsForm->setObjectName(QString::fromUtf8("settingsForm"));
        settingsForm->setWindowModality(Qt::WindowModal);
        settingsForm->resize(400, 500);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(settingsForm->sizePolicy().hasHeightForWidth());
        settingsForm->setSizePolicy(sizePolicy);
        settingsForm->setMinimumSize(QSize(400, 500));
        settingsForm->setMaximumSize(QSize(400, 500));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resources/Weather-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsForm->setWindowIcon(icon);
        settingsForm->setWindowOpacity(1);
        setupButton = new QPushButton(settingsForm);
        setupButton->setObjectName(QString::fromUtf8("setupButton"));
        setupButton->setGeometry(QRect(10, 470, 80, 25));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        setupButton->setFont(font);
        setupButton->setFocusPolicy(Qt::NoFocus);
        label = new QLabel(settingsForm);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 111, 16));
        label->setFont(font);
        errorLimitSlider = new QSlider(settingsForm);
        errorLimitSlider->setObjectName(QString::fromUtf8("errorLimitSlider"));
        errorLimitSlider->setGeometry(QRect(152, 17, 100, 25));
        errorLimitSlider->setMinimum(2);
        errorLimitSlider->setMaximum(50);
        errorLimitSlider->setPageStep(5);
        errorLimitSlider->setOrientation(Qt::Horizontal);
        labelErrorLimit = new QLabel(settingsForm);
        labelErrorLimit->setObjectName(QString::fromUtf8("labelErrorLimit"));
        labelErrorLimit->setGeometry(QRect(270, 21, 30, 16));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        labelErrorLimit->setFont(font1);
        labelErrorLimit->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        saveExitButton = new QPushButton(settingsForm);
        saveExitButton->setObjectName(QString::fromUtf8("saveExitButton"));
        saveExitButton->setGeometry(QRect(160, 420, 100, 25));
        saveExitButton->setFont(font);
        saveExitButton->setFocusPolicy(Qt::NoFocus);
        saveExitButton->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        exitButton = new QPushButton(settingsForm);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(310, 470, 80, 25));
        exitButton->setFont(font);
        exitButton->setFocusPolicy(Qt::NoFocus);
        exitButton->setStyleSheet(QString::fromUtf8(""));
        table = new QTableWidget(settingsForm);
        if (table->columnCount() < 1)
            table->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setBackground(QColor(0, 0, 0));
        table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (table->rowCount() < 10)
            table->setRowCount(10);
        QFont font2;
        font2.setPointSize(8);
        font2.setBold(true);
        font2.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font2);
        __qtablewidgetitem1->setBackground(QColor(0, 0, 0));
        table->setVerticalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setFont(font2);
        table->setVerticalHeaderItem(1, __qtablewidgetitem2);
        QFont font3;
        font3.setBold(true);
        font3.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setFont(font3);
        table->setVerticalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setFont(font3);
        table->setVerticalHeaderItem(3, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setFont(font3);
        table->setVerticalHeaderItem(4, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setFont(font3);
        table->setVerticalHeaderItem(5, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setFont(font3);
        table->setVerticalHeaderItem(6, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        __qtablewidgetitem8->setFont(font3);
        table->setVerticalHeaderItem(7, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        __qtablewidgetitem9->setFont(font3);
        table->setVerticalHeaderItem(8, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        __qtablewidgetitem10->setFont(font3);
        table->setVerticalHeaderItem(9, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        table->setItem(0, 0, __qtablewidgetitem11);
        table->setObjectName(QString::fromUtf8("table"));
        table->setGeometry(QRect(10, 220, 381, 151));
        table->setFrameShape(QFrame::Box);
        table->setFrameShadow(QFrame::Plain);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setAutoScroll(false);
        table->setShowGrid(true);
        table->setGridStyle(Qt::SolidLine);
        table->setCornerButtonEnabled(true);
        table->setRowCount(10);
        table->setColumnCount(1);
        table->horizontalHeader()->setVisible(false);
        table->horizontalHeader()->setDefaultSectionSize(200);
        table->horizontalHeader()->setHighlightSections(true);
        table->horizontalHeader()->setMinimumSectionSize(200);
        table->verticalHeader()->setVisible(true);
        table->verticalHeader()->setCascadingSectionResizes(false);
        table->verticalHeader()->setDefaultSectionSize(25);
        table->verticalHeader()->setHighlightSections(true);
        table->verticalHeader()->setMinimumSectionSize(25);
        yResIndexSlider = new QSlider(settingsForm);
        yResIndexSlider->setObjectName(QString::fromUtf8("yResIndexSlider"));
        yResIndexSlider->setGeometry(QRect(152, 388, 100, 25));
        yResIndexSlider->setMinimum(0);
        yResIndexSlider->setMaximum(8);
        yResIndexSlider->setSingleStep(1);
        yResIndexSlider->setPageStep(1);
        yResIndexSlider->setSliderPosition(0);
        yResIndexSlider->setOrientation(Qt::Horizontal);
        labelyRes = new QLabel(settingsForm);
        labelyRes->setObjectName(QString::fromUtf8("labelyRes"));
        labelyRes->setGeometry(QRect(270, 391, 31, 16));
        labelyRes->setFont(font1);
        labelyRes->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_2 = new QLabel(settingsForm);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 390, 91, 16));
        label_2->setFont(font);
        checkPLConBoot = new QCheckBox(settingsForm);
        checkPLConBoot->setObjectName(QString::fromUtf8("checkPLConBoot"));
        checkPLConBoot->setGeometry(QRect(10, 130, 361, 17));
        checkPLConBoot->setFont(font);
        checkCamonBoot = new QCheckBox(settingsForm);
        checkCamonBoot->setObjectName(QString::fromUtf8("checkCamonBoot"));
        checkCamonBoot->setGeometry(QRect(10, 110, 381, 17));
        checkCamonBoot->setFont(font);
        line = new QFrame(settingsForm);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 450, 400, 16));
        line->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        line->setFrameShadow(QFrame::Plain);
        line->setLineWidth(2);
        line->setFrameShape(QFrame::HLine);
        line_2 = new QFrame(settingsForm);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(0, 150, 400, 16));
        line_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        line_2->setFrameShadow(QFrame::Plain);
        line_2->setLineWidth(2);
        line_2->setFrameShape(QFrame::HLine);
        radio200 = new QRadioButton(settingsForm);
        radioGroup = new QButtonGroup(settingsForm);
        radioGroup->setObjectName(QString::fromUtf8("radioGroup"));
        radioGroup->addButton(radio200);
        radio200->setObjectName(QString::fromUtf8("radio200"));
        radio200->setGeometry(QRect(40, 170, 82, 17));
        radio200->setFont(font);
        radio300 = new QRadioButton(settingsForm);
        radioGroup->addButton(radio300);
        radio300->setObjectName(QString::fromUtf8("radio300"));
        radio300->setGeometry(QRect(40, 190, 82, 17));
        radio300->setFont(font);
        label_3 = new QLabel(settingsForm);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(152, 190, 46, 13));
        label_3->setFont(font);
        editDBNo = new QLineEdit(settingsForm);
        editDBNo->setObjectName(QString::fromUtf8("editDBNo"));
        editDBNo->setGeometry(QRect(204, 187, 51, 20));
        editControlDelay = new QLineEdit(settingsForm);
        editControlDelay->setObjectName(QString::fromUtf8("editControlDelay"));
        editControlDelay->setEnabled(false);
        editControlDelay->setGeometry(QRect(310, 20, 80, 20));
        editControlDelay->setFont(font2);
        editControlDelay->setFocusPolicy(Qt::StrongFocus);
        editControlDelay->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);\n"
"background-color: rgb(255, 0, 0);"));
        editControlDelay->setMaxLength(5);
        editControlDelay->setAlignment(Qt::AlignCenter);
        editControlDelay->setReadOnly(true);
        label_4 = new QLabel(settingsForm);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setEnabled(false);
        label_4->setGeometry(QRect(310, 0, 91, 16));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Arial"));
        font4.setPointSize(8);
        font4.setBold(false);
        font4.setWeight(50);
        label_4->setFont(font4);
        targetSlider = new QSlider(settingsForm);
        targetSlider->setObjectName(QString::fromUtf8("targetSlider"));
        targetSlider->setGeometry(QRect(152, 50, 240, 25));
        targetSlider->setMinimum(100);
        targetSlider->setMaximum(320);
        targetSlider->setSingleStep(2);
        targetSlider->setPageStep(5);
        targetSlider->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(settingsForm);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 53, 111, 16));
        label_5->setFont(font);
        labelTarget = new QLabel(settingsForm);
        labelTarget->setObjectName(QString::fromUtf8("labelTarget"));
        labelTarget->setGeometry(QRect(110, 53, 30, 16));
        labelTarget->setFont(font1);
        labelTarget->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));

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

        QMetaObject::connectSlotsByName(settingsForm);
    } // setupUi

    void retranslateUi(QWidget *settingsForm)
    {
        settingsForm->setWindowTitle(QApplication::translate("settingsForm", "Ayarlar", 0, QApplication::UnicodeUTF8));
        setupButton->setText(QApplication::translate("settingsForm", "Kurulum", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("settingsForm", "Kontrol Tolerans\304\261", 0, QApplication::UnicodeUTF8));
        labelErrorLimit->setText(QApplication::translate("settingsForm", "10", 0, QApplication::UnicodeUTF8));
        saveExitButton->setText(QApplication::translate("settingsForm", "Kaydet & \303\207\304\261k", 0, QApplication::UnicodeUTF8));
        exitButton->setText(QApplication::translate("settingsForm", "\303\207\304\261k", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("settingsForm", "Value", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = table->verticalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("settingsForm", "Kamera IP Adresi", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = table->verticalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("settingsForm", "PLC IP Adresi", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = table->verticalHeaderItem(2);
        ___qtablewidgetitem3->setText(QApplication::translate("settingsForm", "PLC Sa\304\237 Hareket Byte", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = table->verticalHeaderItem(3);
        ___qtablewidgetitem4->setText(QApplication::translate("settingsForm", "PLC Sa\304\237 Hareket Bit", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = table->verticalHeaderItem(4);
        ___qtablewidgetitem5->setText(QApplication::translate("settingsForm", "PLC Sol Hareket Byte", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = table->verticalHeaderItem(5);
        ___qtablewidgetitem6->setText(QApplication::translate("settingsForm", "PLC Sol Hareket Bit", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = table->verticalHeaderItem(6);
        ___qtablewidgetitem7->setText(QApplication::translate("settingsForm", "PLC Kumanda Stop Byte", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = table->verticalHeaderItem(7);
        ___qtablewidgetitem8->setText(QApplication::translate("settingsForm", "PLC Kumanda Stop Bit", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = table->verticalHeaderItem(8);
        ___qtablewidgetitem9->setText(QApplication::translate("settingsForm", "PLC Acil Stop Byte", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = table->verticalHeaderItem(9);
        ___qtablewidgetitem10->setText(QApplication::translate("settingsForm", "PLC Acil Stop Bit", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = table->isSortingEnabled();
        table->setSortingEnabled(false);
        table->setSortingEnabled(__sortingEnabled);

        labelyRes->setText(QApplication::translate("settingsForm", "10", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("settingsForm", "Takip Trendi", 0, QApplication::UnicodeUTF8));
        checkPLConBoot->setText(QApplication::translate("settingsForm", "Program A\303\247\304\261ld\304\261\304\237\304\261nda PLC'ye Otomatik Ba\304\237lans\304\261n m\304\261?", 0, QApplication::UnicodeUTF8));
        checkCamonBoot->setText(QApplication::translate("settingsForm", "Program A\303\247\304\261ld\304\261\304\237\304\261nda Kamera'ya Otomatik Ba\304\237lans\304\261n m\304\261?", 0, QApplication::UnicodeUTF8));
        radio200->setText(QApplication::translate("settingsForm", "S7-200", 0, QApplication::UnicodeUTF8));
        radio300->setText(QApplication::translate("settingsForm", "S7-300", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("settingsForm", "DB No:", 0, QApplication::UnicodeUTF8));
        editDBNo->setInputMask(QApplication::translate("settingsForm", "999; ", 0, QApplication::UnicodeUTF8));
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
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Hassas \303\266l\303\247\303\274m yap\304\261n\304\261z.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        editControlDelay->setInputMask(QString());
        editControlDelay->setText(QString());
        label_4->setText(QApplication::translate("settingsForm", "Kontrol Gecikmesi", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("settingsForm", "Hedef B\303\266lge", 0, QApplication::UnicodeUTF8));
        labelTarget->setText(QApplication::translate("settingsForm", "100", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class settingsForm: public Ui_settingsForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSFORM_H

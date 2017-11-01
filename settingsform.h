#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QTableWidgetItem>
#include <QTimer>

#include "restartdialog.h"
#include "ui_restartdialog.h"
#include "../_Modules/Getimage/getimage.h"

namespace Ui {
    class settingsForm;
}

class settingsForm : public QDialog{
    Q_OBJECT

public:

    QTableWidgetItem *itemR0C0; // camera ip
    QTableWidgetItem *itemR1C0; // plc ip
    QTableWidgetItem *itemR2C0; // plc right v-mem byte
    QTableWidgetItem *itemR3C0; // plc right bit byte
    QTableWidgetItem *itemR4C0; // plc left v-mem byte
    QTableWidgetItem *itemR5C0; // plc left bit byte
    QTableWidgetItem *itemR6C0; // plc stop v-mem byte
    QTableWidgetItem *itemR7C0; // plc stop bit byte
    QTableWidgetItem *itemR8C0; // plc emergency v-mem byte
    QTableWidgetItem *itemR9C0; // plc emergency bit byte

    QTextCursor text_cursor;

    getImage *camApi;
    QTimer *timerAutoFocus;
    bool timerLock = false;
    bool camFocusingActionState = false;
    bool checkFocusLaplacian = false;

    explicit settingsForm(QWidget *parent = 0);
    void getParameters();       // get parameters from mainwindow
    void commitChanges();       // transfer paramteres to mainwindow
    ~settingsForm();

public slots:
    void showSetupForm();       // show system setup form
    void errorLimitSlider();    // get error limit of deviation in pixels from slider >> command start
    void errorScaleSlider();    // get error scale to calculate stop limit value
    void saveExitButton();      // display restart dialog to commit changes
    void exitButton();          // exit without changing parameters
    void yResIndexSlider();     // returns index of yRes array to get yRes value
    void checkPLConBoot();      // is plc be connected on app. start
    void checkCamonBoot();      // is camara be played on app. start
    void checkErrorStopLimitVisible();
    void saveExit();            // commit changes & restart app.
    void targetSlider();        // target area horizontal size adjustment
    void targetVertSlider();    // target area vertical size adjustment
    void calFocusValue();
    void focusValueCalculated(double val);

private:
    Ui::settingsForm *ui;

private slots:
    void changePLCtype();
    void changeCamStreamType();
    void getControlDelay();     // get control delay from gui
    void setTitle();

    void on_checkPLCSIM_clicked();
    void on_checkTargetAlignButtons_clicked();

    void on_apiCheckFocus_clicked();
    void focusState(bool state);                    // true; in focus, false; out of focus
    void on_apiFocusStatus_clicked();
    void focusingActionState(bool);                 // true; focusing in action, false; stable
    void on_apiAutoFocus_clicked();
    void on_manFocusSlider_sliderReleased();
    void on_manFocusSlider_sliderMoved(int position);
    void requestCompleted();
    void checkAutoFocusingState();
    void on_focusCheck_clicked();
    void on_autoFocusCheck_clicked();
    void on_testButton_clicked();
    void on_calcFocusValueButton_clicked();
    void on_algoCheckFocus_clicked();
    void on_algoAutoFocusLocal_clicked();
    void on_algoAutoFocus2Step_clicked();
    void on_editGoodValue_returnPressed();
    void on_editBypassLocalValue_returnPressed();
};

#endif // SETTINGSFORM_H

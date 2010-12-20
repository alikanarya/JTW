#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QDialog>
#include <QTableWidgetItem>

#include "restartdialog.h"
#include "ui_restartdialog.h"

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

    explicit settingsForm(QWidget *parent = 0);
    void getParameters();       // get parameters from mainwindow
    void commitChanges();       // transfer paramteres to mainwindow
    ~settingsForm();

public slots:
    void showSetupForm();       // show system setup form
    void errorLimitSlider();    // get error limit of deviation in pixels from slider
    void saveExitButton();      // display restart dialog to commit changes
    void exitButton();          // exit without changing parameters
    void yResIndexSlider();     // returns index of yRes array to get yRes value
    void checkPLConBoot();      // is plc be connected on app. start
    void checkCamonBoot();      // is camara be played on app. start
    void saveExit();            // commit changes & restart app.

private:
    Ui::settingsForm *ui;

private slots:
    void changePLCtype();
    void getControlDelay();     // get control delay from gui

};

#endif // SETTINGSFORM_H

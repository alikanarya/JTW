#include "exitdialog.h"
#include "ui_exitdialog.h"

#include "mainwindow.h"

extern MainWindow *w;

exitDialog::exitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exitDialog){
    ui->setupUi(this);

    // only title on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}

void exitDialog::acceptButton(){
    if (w->timerControlEnabled) {
        w->timerControl->stop();
        //w->threadPLCControl->wait();

        if (!w->threadPLCControl->isRunning()){
            w->threadPLCControl->commandState = _CMD_STOP;
            w->threadPLCControl->start();
        }
    }

    w->writeSettings();
    qApp->quit();
    this->close();
}

void exitDialog::cancelButton(){
    this->close();
}

exitDialog::~exitDialog(){
    delete ui;
}

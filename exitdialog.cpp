#include "exitdialog.h"
#include "ui_exitdialog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;

exitDialog::exitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exitDialog){
    ui->setupUi(this);

    // only title on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    //flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}

void exitDialog::acceptButton(){
    if (w->timerControlEnabled) {
        w->timerControl->stop();

        if (w->threadPLCControl->isRunning()){
            // wait for last thread to end if running
            QTimer::singleShot(500, this, SLOT(wait2Stop()));
        } else {
            // send stop signal to plc
            w->threadPLCControl->commandState = _CMD_STOP;
            w->threadPLCControl->start();

            // wait for stop signal to end
            QTimer::singleShot(500, this, SLOT(exitAction()));
        }
    } else
        exitAction();
}

void exitDialog::wait2Stop(){
    if (w->threadPLCControl->isRunning()){

        // terminate last thread after waiting
        w->threadPLCControl->terminate();

        // wait for the termination of alive thread
        QTimer::singleShot(500, this, SLOT(wait2Terminate()));

    } else {

        // if last thread end, send stop signal
        w->threadPLCControl->commandState = _CMD_STOP;
        w->threadPLCControl->start();

        // wait for stop signal to end
        QTimer::singleShot(500, this, SLOT(exitAction()));
    }
}

void exitDialog::wait2Terminate(){
    if (!w->threadPLCControl->isRunning()){

        // after waiting of termination send stop signal
        w->threadPLCControl->commandState = _CMD_STOP;
        w->threadPLCControl->start();

        // wait for stop signal to end
        QTimer::singleShot(500, this, SLOT(exitAction()));
    } else
        // exit in either case
        exitAction();
}

void exitDialog::exitAction(){
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

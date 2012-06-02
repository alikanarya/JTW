#include "zctrldialog.h"
#include "ui_zctrldialog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;

zctrlDialog::zctrlDialog(QWidget *parent) : QDialog(parent), ui(new Ui::zctrlDialog){

    // only title & close button on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::WindowSystemMenuHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);
}

void zctrlDialog::zctrlBox(){
    w->zControlActive = ui->zctrlBox->isChecked();
}


zctrlDialog::~zctrlDialog(){

    if (w->zControlActive){
        w->ui->zControlButton->setIcon(w->zIconEnabled);
    } else {
        w->ui->zControlButton->setIcon(w->zIconDisabled);
    }

    delete ui;
}

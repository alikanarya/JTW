#include "zctrldialog.h"
#include "ui_zctrldialog.h"

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

zctrlDialog::~zctrlDialog(){
    delete ui;
}

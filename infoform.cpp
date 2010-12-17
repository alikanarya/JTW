#include "infoform.h"
#include "ui_infoform.h"

infoForm::infoForm(QWidget *parent) : QDialog(parent), ui(new Ui::infoForm){

    // only title & close button on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::WindowSystemMenuHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);
}

infoForm::~infoForm(){
    delete ui;
}

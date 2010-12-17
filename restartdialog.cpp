#include "restartdialog.h"
#include "ui_restartdialog.h"

restartDialog::restartDialog(QWidget *parent) : QDialog(parent), ui(new Ui::restartDialog){
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);
}

void restartDialog::cancelButton(){
    this->close();
}

void restartDialog::acceptButton(){
    emit acceptRestart();
}

restartDialog::~restartDialog(){
    delete ui;
}

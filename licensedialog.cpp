#include "licensedialog.h"
#include "ui_licensedialog.h"

#include "mainwindow.h"

extern MainWindow *w;

licenseDialog::licenseDialog(protect checkLicense, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::licenseDialog){

    // only title & close button on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);

    ui->plainTextEdit->appendPlainText(checkLicense.statusMessage);
    if (checkLicense.privateKeyList.size() != 0)
        ui->lineEdit->setText(checkLicense.privateKeyList[0]);
}

void licenseDialog::closeButton(){
    qApp->quit();
    this->close();
}

licenseDialog::~licenseDialog(){
    delete ui;
}

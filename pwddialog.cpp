#include "pwddialog.h"
#include "ui_pwddialog.h"

#include "mainwindow.h"

extern MainWindow *w;

pwdDialog::pwdDialog(int _flag, QWidget *parent) : QDialog(parent), ui(new Ui::pwdDialog){
    flag = _flag;

    // only title & close button on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::WindowSystemMenuHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);
}

void pwdDialog::acceptButton(){
    pwd = ui->lineEdit->text();

    if (flag == 0){
//        if(true){
        if(QString::compare(pwd, _PWD_SETTINGS, Qt::CaseSensitive) == 0){
            w->settingsPWDOK = true;
            w->showSettingsForm();
            this->close();
        } else {
            w->settingsPWDOK = false;
            displayWarning();
        }
    } else if (flag == 1){
//        if(true){
        if(QString::compare(pwd, _PWD_SETUP, Qt::CaseSensitive) == 0){
            w->setupPWDOK = true;
            w->showSetupForm();
            this->close();
        } else {
            w->setupPWDOK = false;
            displayWarning();
        }
    }
}

void pwdDialog::cancelButton(){
    this->close();
}

void pwdDialog::displayWarning(){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Yanlýþ Þifre!");
    msgBox.setInformativeText("Büyük/Küçük harf duyarlýdýr.");
    msgBox.exec();
}

pwdDialog::~pwdDialog(){
    delete ui;
}

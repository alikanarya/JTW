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

        if ( keys.contains(Qt::Key_Control) && keys.contains(Qt::Key_Shift) ) {

            w->settingsPWDOK = true;
            w->showSettingsForm();
            this->close();

        } else {
            if(QString::compare(pwd, _PWD_SETTINGS, Qt::CaseSensitive) == 0){
                w->settingsPWDOK = true;
                w->showSettingsForm();
                this->close();
            } else {
                w->settingsPWDOK = false;
                displayWarning();
            }
        }

    } else if (flag == 1){

        if ( keys.contains(Qt::Key_Control) && keys.contains(Qt::Key_Shift) ) {
            w->setupPWDOK = true;
            w->showSetupForm();
            this->close();
        } else {
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
}

void pwdDialog::keyPressEvent(QKeyEvent *event){

    keys+= event->key();

    //if(event->key() == Qt::Key_Control::Key_Escape) { qDebug() << "You pressed ESC";  }
}

void pwdDialog::keyReleaseEvent(QKeyEvent *event) {
}

void pwdDialog::cancelButton(){

    this->close();
}


void pwdDialog::displayWarning(){

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    QString str1 = "Yanlýþ Þifre!";
    QString str2 = "Büyük/Küçük harf duyarlýdýr.";
    msgBox.setText(str1);
    msgBox.setInformativeText(str2);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}


pwdDialog::~pwdDialog(){

    delete ui;
}

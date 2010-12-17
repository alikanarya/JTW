#include "sampledialog.h"
#include "ui_sampledialog.h"

sampleDialog::sampleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sampleDialog){
    // only title & close button on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::WindowSystemMenuHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);
}

sampleDialog::~sampleDialog(){
    delete ui;
}

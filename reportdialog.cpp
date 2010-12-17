#include "reportdialog.h"
#include "ui_reportdialog.h"

reportDialog::reportDialog(QString message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reportDialog)
{
    // only title & close button on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::WindowSystemMenuHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);
    ui->plainTextEdit->appendPlainText(message);
}

reportDialog::~reportDialog()
{
    delete ui;
}

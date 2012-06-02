#ifndef ZCTRLDIALOG_H
#define ZCTRLDIALOG_H

#include <QDialog>

namespace Ui {
    class zctrlDialog;
}

class zctrlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit zctrlDialog(QWidget *parent = 0);
    ~zctrlDialog();

private:
    Ui::zctrlDialog *ui;

private slots:

    void zctrlBox();
};

#endif // ZCTRLDIALOG_H

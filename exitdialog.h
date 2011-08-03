#ifndef EXITDIALOG_H
#define EXITDIALOG_H

#include <QDialog>

namespace Ui {
    class exitDialog;
}

class exitDialog : public QDialog{
    Q_OBJECT

public:
    explicit exitDialog(QWidget *parent = 0);
    ~exitDialog();

public slots:
    void acceptButton();
    void cancelButton();

    void wait2Stop();
    void wait2Terminate();
    void exitAction();

private:
    Ui::exitDialog *ui;
};

#endif // EXITDIALOG_H

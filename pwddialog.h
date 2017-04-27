#ifndef PWDDIALOG_H
#define PWDDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui {
    class pwdDialog;
}

class pwdDialog : public QDialog {
    Q_OBJECT

public:
    int flag;
    QString pwd;

    explicit pwdDialog(int flag, QWidget *parent = 0);
    ~pwdDialog();

private:
    Ui::pwdDialog *ui;

    void displayWarning();

private slots:
    void acceptButton();
    void cancelButton();

};

#endif // PWDDIALOG_H

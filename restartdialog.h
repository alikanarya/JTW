#ifndef RESTARTDIALOG_H
#define RESTARTDIALOG_H

#include <QDialog>

namespace Ui {
    class restartDialog;
}

class restartDialog : public QDialog{
    Q_OBJECT

public:

    explicit restartDialog(QWidget *parent = 0);
    ~restartDialog();

signals:
    void acceptRestart();

private:
    Ui::restartDialog *ui;

private slots:
    void cancelButton();
    void acceptButton();

};

#endif // RESTARTDIALOG_H

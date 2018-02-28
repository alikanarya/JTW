#ifndef PWDDIALOG_H
#define PWDDIALOG_H

#include <QtWidgets/QDialog>
#include <QSet>

namespace Ui {
    class pwdDialog;
}

class pwdDialog : public QDialog {
    Q_OBJECT

public:
    int flag;
    QString pwd;
    bool m_bFirstRelease;
//    QSet<Qt::Key> keysPressed;
    QSet<int> keys;

    explicit pwdDialog(int flag, QWidget *parent = 0);
    ~pwdDialog();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::pwdDialog *ui;

    void displayWarning();

private slots:
    void acceptButton();
    void cancelButton();

};

#endif // PWDDIALOG_H

#ifndef INFOFORM_H
#define INFOFORM_H

#include <QDialog>

namespace Ui {
    class infoForm;
}

class infoForm : public QDialog{
    Q_OBJECT

public:
    explicit infoForm(QWidget *parent = 0);
    ~infoForm();

private:
    Ui::infoForm *ui;
};

#endif // INFOFORM_H

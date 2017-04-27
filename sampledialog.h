#ifndef SAMPLEDIALOG_H
#define SAMPLEDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui {
    class sampleDialog;
}

class sampleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit sampleDialog(QWidget *parent = 0);
    ~sampleDialog();

private:
    Ui::sampleDialog *ui;
};

#endif // SAMPLEDIALOG_H

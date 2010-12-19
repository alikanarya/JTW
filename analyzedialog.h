#ifndef ANALYZEDIALOG_H
#define ANALYZEDIALOG_H

#include <QDialog>

#include "../_Modules/Imgprocess/imgprocess.h"

namespace Ui {
    class analyzeDialog;
}

class analyzeDialog : public QDialog{
    Q_OBJECT

public:
    explicit analyzeDialog(imgProcess *iprocess, int processElapsed, QWidget *parent = 0);
    ~analyzeDialog();

public slots:
    void acceptButton();
    void cancelButton();
    void sampleButton();

private:
    Ui::analyzeDialog *ui;
};

#endif // ANALYZEDIALOG_H

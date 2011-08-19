#ifndef SETUPFORM_H
#define SETUPFORM_H

#include "../_Modules/Imgprocess/imgprocess.h"
#include <QDialog>
#include <QTime>

namespace Ui {
    class setupForm;
}

class setupForm : public QDialog{
    Q_OBJECT

public:

    int startTime, endTime;     // start/end time of image processing
    int processElapsed;         // elapsed time for image processing
    bool captured;

    QString captureTimeStr;     // capture(start) time of processing (for filename)

    // file name vars
    QString fileBaseTarget;
    QString fileBaseMono;
    QString fileBaseEdge;
    QString fileBaseHough;
    QString fileBaseCorner;
    QString fileExt;
    QString fileName;
    QString savePath;

    QImage target;              // original image
    QImage *edge;               // edge detection image
    QImage *hough;              // hough transformed image (with red hough lines)
    QImage *leftImage;          // edge image with red primary line
    QImage *rightImage;          // edge image with red primary line

    // image processing vars
    int thetaMin, thetaMax;
    float thetaStep;
    int thetaMinSub, thetaMaxSub;
    float thetaStepSub;
    int houghLineNo;
    int voteThreshold, voteAvg;
    int voidThreshold;

    imgProcess *iprocess;       // image processing class
    imgProcess *iprocessLeft, *iprocessRight;

    explicit setupForm(QWidget *parent = 0);    // constructor
    void getParameters();       // get parameters from mainwindow
    ~setupForm();               // destructor

public slots:
    void captureButton();       // capture image and process
    bool saveButton();          // save selected images and datas
    void clearButton();         // clear plain text edit frame
    void saveExitButton();      // transfer paramteres to mainwindow
    void exitButton();          // exit without changing parameters
    void subImageCheck();       // sub image processing algorithm selection

    void processClassical();
    void processExtSubImage();
    void processExtSubImageTest();

private:
    Ui::setupForm *ui;
};

#endif // SETUPFORM_H

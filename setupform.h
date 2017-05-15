#ifndef SETUPFORM_H
#define SETUPFORM_H

#include "../_Modules/Imgprocess/imgprocess.h"
#include <QtWidgets/QDialog>
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
    QImage *rightImage;         // edge image with red primary line

    // image processing vars
    int thetaMin, thetaMax;
    float thetaStep;
    int thetaMinSub, thetaMaxSub;
    float thetaStepSub;
    int houghLineNo;
    int voteThreshold, voteAvg;
    int voidThreshold;
    int tCenterX;
    //bool cannyThinning;
    //int gaussianSize = 5;
    //float stdDev = 1.4;
    int algorithmType = 0;

    imgProcess *iprocess;       // image processing class
    imgProcess *iprocessLeft, *iprocessRight;
    bool iprocessInitSwitch, iprocessLeftInitSwitch, iprocessRightInitSwitch;
    bool imageLoadedFromFile = false;

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
    void subType();

    // image processing fns
    void processStandardHT();           // standard hough transform
    void processSubImageVoidness();     // sub image based on VOID areas
    void processSubImageSolidness();    // sub image based on SOLID lines
    void processSolidnessCanny();       // SOLID line analysis with canny edges

    void update();

private:
    Ui::setupForm *ui;

private slots:
    void zctrlBox();
    void widthControl();
    void hardControlStartBox();
    void readMachineStatusBox();
    void readDistanceBox();
    void dynamicAlgoBox();
    void readWeldSeamBox();
    void timeControlBox();
    void lineDetectionBox();
    void editLineScore();


    void on_captureButton_2_clicked();
    void on_brightnessSlider_sliderReleased();
    void on_brightnessSlider_sliderMoved(int position);
    void on_contrastSlider_sliderReleased();
    void on_contrastSlider_sliderMoved(int position);
    void on_gammaSlider_sliderReleased();
    void on_gammaSlider_sliderMoved(int position);
    void on_brightnessReset_clicked();
    void on_contrastReset_clicked();
    void on_gammaReset_clicked();
    void on_fileSlider_sliderMoved(int position);
    void on_gaussSizeSlider_sliderMoved(int position);
    void on_gaussSDevSlider_sliderMoved(int position);
    void on_cannyThinningBox_clicked();
    void on_algorithmBox_currentIndexChanged(int index);
};

#endif // SETUPFORM_H

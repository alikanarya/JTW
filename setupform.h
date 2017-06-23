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
    QString path, edgePath;

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
    int mainEdgesNumber = 1;
    //int centerX,centerY;
    bool matrixFlag = false;
    bool averaging = false;

    //bool cannyThinning;
    //int gaussianSize = 5;
    //float stdDev = 1.4;
    bool thinJointAlgoActive;
    int algorithmType = 0;
    int edgeDetectionState = 0; // 0:none, 1: sobel, 2: canny
    int lineDetectAlgos = 0; // 0:none, 1: detectLongestSolidLines, 2: detectMainEdges, 3: detectPrimaryVoid
    bool subImageProcessingSwitch;
    bool algoPrerequestsOk = false;
    bool DEBUG = false;

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

    // image processing fns
    void edgeDetection(imgProcess *iprocess);
    void processImage();
    void Algo1(imgProcess *iprocess);   // LASER: mono/edge(matrixFlag) > houghTr > detectLongestSolidLines
    void Algo2(imgProcess *iprocess);   // LASER: mono/edge(matrixFlag) > houghTr > detectPrimaryVoid
    void Algo3(imgProcess *iprocess);   // woLASER: edge > houghTr > detectMainEdges
    void Algo4(imgProcess *iprocess);   // woLASER: value > detectThinJointCenter
    void Algo5(imgProcess *iprocess);   // woLASER: value > contrast matrix > houghTr > calcAvgDistAndAngleOfMajors
    void Algo6(imgProcess *iprocess);   // woLASER: canny1 > houghTr > detectMainEdges > thickenEdgeMap > scoreLineCrossing

    bool saveAlgo1();
    bool saveAlgo2();
    bool saveAlgo3();
    bool saveAlgo4();
    bool saveAlgo5();
    bool saveAlgo6();

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
    void on_radioLaser_clicked();
    void on_radioWoLaser_clicked();
    void on_edgeDetectionBox_currentIndexChanged(int index);
    void on_detecAlgoBox_currentIndexChanged(int index);
    void on_mainEdgesSlider_valueChanged(int value);
    void on_saveButton_clicked();
};

#endif // SETUPFORM_H

#ifndef SETUPFORM_H
#define SETUPFORM_H

#include "../_Modules/Imgprocess/imgprocess.h"
//#include <QtWidgets/QDialog>
#include <QTime>
#include <QtWidgets>
#include "imgtools.h"

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
    int thetaMinHorLine, thetaMaxHorLine;   // for horizontal laser line
    int thetaMinVerLine, thetaMaxVerLine;   // for vertical edge lines w/o laser
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
    int brightnessVal = 0;
    int contrastVal = 100;
    int gammaVal = 100;
    bool applyCameraEnhancements = false;

    //bool cannyThinning;
    int gaussianSize = 5;
    float stdDev = 1.4;
    bool thinJointAlgoActive;
    int algorithmType = 0;
    int edgeDetectionState = 0; // 0:none, 1: sobel, 2: canny4, 3: canny1
    int lineDetectAlgos = 0; // 0:none, 1: detectLongestSolidLines, 2: detectMainEdges, 3: detectPrimaryVoid
    bool subImageProcessingSwitch;
    bool algoPrerequestsOk = false;
    bool DEBUG = false;
    bool saveAnalysis = false;
    int hdDistance, hdAngle, hdVoteValue, solidLineLength;

    int maFilterKernelSize = 11;
    int histogramAngleThreshold = 10;
    bool colorMatrix = true; // true=colored, false=gray
    double lenRateThr = 0.50;
    double bandWidthMin = 0.20;
    double bandCenterMax = 0.10;
    int histAreaNo = 1;

    bool twoPassWelding;
    bool autoDetect2ndPass;

    imgProcess *iprocess;       // image processing class
    imgProcess *iprocessLeft, *iprocessRight;
    bool iprocessInitSwitch, iprocessLeftInitSwitch, iprocessRightInitSwitch;
    bool imageLoadedFromFile = false;

    QGraphicsScene *scene1;      // to show graph
    QRect sceneRect1;
    QPen penAxis;
    QPen *penRed;
    QPen *penBlue;
    QPen *penGreen;
    QPen penX;
    QGraphicsScene *scene2;      // to show graph
    QRect sceneRect2;
    QGraphicsScene *scene3;      // to show graph


    explicit setupForm(QWidget *parent = 0);    // constructor
    void getParameters();       // get parameters from mainwindow
    void clearGraph(QGraphicsView *graph);
    void drawGraph(QGraphicsView *graph, QPen *pen, int *array, int size, QPoint yRange, bool scaleMin = true);
    void drawGraphHist(QGraphicsView *graph, QPen *pen, int *array, int size, QPoint yRange, bool scaleMin = true); // with extreme points
    void drawGraphHist2(imgProcess *ipro, QGraphicsView *graph, QPen *pen, int *array, int size, QPoint yRange, bool scaleMin = true);// with extreme filtered points
    void drawGraphList(QGraphicsView *graph, QPen *pen, QList<range> list, int *yVals, QPoint xRange, QPoint yRange);
    void drawGraphXY(QGraphicsView *graph,int *xAarray,int *yAarray, int size);

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
    void Algo7(imgProcess *iprocess);   // woLASER: edge > houghTr > detectScanHorizontal
    void histMultAreas();

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
    void on_mainEdgesSlider_valueChanged(int value);
    void on_saveButton_clicked();
    void on_cameraEnhancementsBox_stateChanged(int arg1);
    void on_editVideoDuration_returnPressed();
    void on_captureNext_clicked();
    void on_capturePrev_clicked();
    void on_alignGuideBox_clicked();
    void on_debugModeBox_clicked();
    void on_imgParametersButton_clicked();
    void on_testButton_clicked();
    void on_maFilterSizeSlider_sliderMoved(int position);
    void on_maFilterSizeSlider_sliderReleased();
    void on_histogramAnalysisButton_clicked();
    void on_histAngleSlider_sliderMoved(int position);
    void on_histAngleSlider_sliderReleased();
    void on_radioColored_clicked();
    void on_radioGray_clicked();
    void on_lenRateThrSlider_sliderMoved(int position);
    void on_lenRateThrSlider_sliderReleased();
    void on_bandWidthMinSlider_sliderMoved(int position);
    void on_bandWidthMinSlider_sliderReleased();
    void on_bandCenterMaxSlider_sliderMoved(int position);
    void on_bandCenterMaxSlider_sliderPressed();
    void on_bandCenterMaxSlider_sliderReleased();
    void on_twoPassWeldingBox_clicked(bool checked);
    void on_autoDetect2ndPassBox_clicked();
    void on_histAreaNoSlider_sliderMoved(int position);
    void on_histAreaNoSlider_sliderReleased();
};

#endif // SETUPFORM_H

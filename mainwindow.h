﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define INIFILENAME         "settings.ini"
#define _URL_CAM            "http://10.0.0.5/snap.jpg"
#define _URL_CAM_STREAM     "rtsp://admin:admin@192.168.3.3:554/cam/realmonitor?channel=1&subtype=0"
#define _CAM_STREAM_TYPE    1
#define _URL_PLC            "10.0.1.4"
#define _FPS                25
#define _IPROCESS_INT       1
#define _FRAME_WIDTH        320
#define _FRAME_HEIGHT       160
#define _THETA_MIN_HOR      85
#define _THETA_MAX_HOR      95
#define _THETA_MIN_VER      0
#define _THETA_MAX_VER      10
#define _THETA_STEP         1.0
#define _THETA_MIN_SUB      80
#define _THETA_MAX_SUB      100
#define _THETA_STEP_SUB     0.5
#define _HOUGH_LINE_NO      5
#define _VOTE_THRESHOLD     10
#define _VOID_THRESHOLD     10
#define _PLC_TYPE           1
#define _DB_NO              1
#define _BYTE_NO            0
#define _ERROR_LIMIT        5
#define _ERROR_SCALE        1.0
#define _YRES_ARRAY_INDEX   8
#define _PLC_CONN_ONBOOT    true
#define _PLCSIM             false
#define _PLAY_CAM_ONBOOT    true
#define _SUB_IPROCESS       true
#define _SUB_TYPE           0
#define _CONTROL_DELAY      0
#define _THIN_JOINT         false
#define _Z_CONTROL          false
#define _UP_TOL             1.0
#define _DOWN_TOL           1.0
#define _HARD_START         false
#define _MACHINE_NO         1
#define _WIDTH_CONTROL      false
#define _READ_MACHINE_STAT  false
#define _READ_DISTANCE      false
#define _READ_WELD_SEAM     false
#define _DYNAMIC_ALGO       false
#define _TIME_CONTROL       false
#define _TIME_LIMIT         30
#define _LINE_DETECT        false
#define _LINE_SCORE         33
#define _BRIGHTNESS         0
#define _CONTRAST           100
#define _GAMMA              100
#define _GAUSS_SIZE         5
#define _GAUSS_DEV          1.4
#define _CANNY_THIN         true
#define _EDGE_ALGO          0
#define _ALGO_TYPE          0
#define _MAIN_EDGE_NO       1
#define _CAM_ENHANCE        false
#define _ALIGN              false
#define _FOCUS_CHECK        false
#define _AUTO_FOCUS         false
#define _MAF_KERNEL         11
#define _HIST_ANGLE         10
#define _COLOR_MATRIX       true    // true=colored, false=gray
#define _LEN_RATIO          0.5
#define _BAND_WIDTH_MIN     0.2
#define _BAND_CENTER_MAX    0.1
#define _HIST_AREA_NO       1
#define _HIST_LO            2
#define _HIST_HI            7
#define _HIST_BUFFER_SIZE   3
#define _MAIN_EDGE_AREA_NO  5
#define _2_PASS_WELD        false
#define _DETECT_2ND_PASS    false
#define _CMD_BUFFER_SIZE    3
#define _CMD_BUFFER_SIZE    3
#define _ERROR_MAX_FACTOR   3
#define _PASS1_TIME         30000
#define _PASSTR_TIME        10000
#define _PASS2_TIME         40000
#define _TIME_CONTROL_2PASS true


#define _PWD_SETTINGS       "orn"
#define _PWD_SETUP          "ryhn"

#define _CMD_CENTER         0
#define _CMD_RIGHT          1
#define _CMD_LEFT           2
#define _CMD_STOP           3
#define _CMD_EMERGENCY_ACT  4
#define _CMD_EMERGENCY_PSV  5
#define _CMD_RESET          6
#define _CMD_CHECK          7

#define _CMD_Z_CENTER       10
#define _CMD_Z_UP           11
#define _CMD_Z_DOWN         12

#define _MAINTITLE "JTW - Kaynak Takibi :: "
#define _TITLE "DIŞ KAYNAK"

#define PROTOCOL_S7200  123
#define PROTOCOL_S7300  122


//const int yResArray[9] = {1, 2, 4, 6, 8, 10, 12, 16, 20}; // for height 240
//const int yResArray[9] = {12, 12, 12, 12, 12, 12, 12, 16, 20};
//const int yResArray[9] = {1, 1, 2, 4, 5, 8, 10, 16, 20}; // for height 160
const int yResArray[9] = {10, 10, 10, 10, 10, 10, 10, 16, 20};

#include <QMainWindow>
namespace Ui {
    class MainWindow;
}
#include "imgtools.h"

#include "settingsform.h"
#include "ui_settingsform.h"

#include "setupform.h"
#include "ui_setupform.h"

#include "infoform.h"
#include "ui_infoform.h"

#include "pwddialog.h"
#include "ui_pwddialog.h"

#include "reportdialog.h"
#include "ui_reportdialog.h"

#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <QNetworkInterface>

#include "../_Modules/Getimage/getimage.h"
#include "../_Modules/PlayStream/threadplaystream.h"
#include "../_Modules/Imgprocess/imgprocess.h"
#include "messages.h"
#include "../_Modules/S7/s7.h"
#include "../_Modules/Getsystemtime/getsystemtime.h"
#include "plccontrolthread.h"
#include "../_Modules/Protect/protect.h"
#include "videosavethread.h"
#include "autofocusthread.h"
#include "imgprocessthread.h"

#include <plcqtlib.h>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>
#include <fstream>
using namespace std;
//#include <QMediaPlayer>
//#include <QVideoWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    bool restartApp = false;
    QRect rectScreen;                   // screen geometry
    QString title;                      // window title
    bool settingsPWDOK;                 // to switch pwd dialog or settings form
    bool setupPWDOK;                    // to switch pwd dialog or setup form

    QSettings *settings;                // settings: to read/write ini file
    QString statusMessage;              // general status message
    QString seperator = "   ||   ";

    plcControlThread *threadPLCControl;

    QUrl urlCam;                        // camera url
    QUrl urlCamStream;                  // camera url
    int camStreamType = 0;              // 0: stream (rtsp), 1: jpeg
    bool playCamonBoot;                 // play camera on app. start
    bool trackOnBoot = false;           // start tracking on app. start
    getImage *imageGetter;              // camera image request & retrieve class
    getImage *cameraChecker;
    getImage *camApi;
    networkData *lastData, *prevData;   // current and prev image data vars
    threadPlayStream *playStream;
    int imageCaptureTime = 0;

    bool camFocusState = false;         // true; in focus, false; out of focus
    bool camFocusingActionState = false;// true; focusing in action, false; stable
    bool camDoAutoFocus = false;
    bool timerLock = false;
    QTimer *timerAutoFocus;
    bool focusCheckBeforeControl = false;
    bool autoFocusBeforeControl = false;
    bool autoFocusAfterFocusCheck = false;
    int autoFocusPassNo = 0;
    int autoFocusPassLimit = 2;
    double camFocusPos = 0.5;

    autoFocusThread *AF;
    bool doAutoFocus_Algo = false;
    int autoFocusAlgo_scheme = 0;   //0: single step, 1: deep search, 2: 2 steps, 3:
    bool autoFocusAlgo2Step = false;
    bool autoFocusAlgo2Step_Auto = false;
    bool autoFocusAlgoLocal = false;
    bool autoFocusAlgoLocal_Start = false;
    bool extraIteration = false;
    bool extraIterationForce = false;
    double sampleStart, sampleEnd, sigma;
    double sampleStart0, sampleEnd0, sampleStartPrev, sampleEndPrev;
    int sampleNo;
    float **fftArray;
    QList<double> focusValListY;
    QList<double> focusValListX;
    double focusValX_offset;
    double bestFocusPos = 0.5;
    bool focusValCalc = false;
    int focusValCalcNo = 0;
    int focusValCalcLimit = 5;
    double focusVal = 0;
    double focusVal0 = 0;
    int focusROI = 0;   //0:full image 1:target frame >1:// target frame portion
    int focusValueAlgo = 0; //0:fft 1:fuzzy entropy 2:laplacian
    int laplacianGoodValue = 200;
    int laplacianBypassLocalAlgoValue = 100;
    double localFocusHalfDist = 0.1;

    cv::Mat imgDataCV;

    bool testFlag = false;
    bool testFlag2 = false;
    int testInt1;
    float testFloat1,testFloat2 = 0;

    bool cameraDownStatus = true;
    bool camReconnectLock = false;

    bool analyzeAction = false;
    int analyzeStartTime, analyzeEndTime;

    videoSaveThread *threadVideoSave;
    QImage imageFile;
    QImage imageFileChanged;
    int brightnessVal = 0;
    int contrastVal = 100;
    int gammaVal = 100;
    bool applyCameraEnhancements = false;
    int camImageWidth = 480;
    int camImageHeight = 640;
    bool getCamImageProperties = false;

    //QMediaPlayer *player;
    //QVideoWidget *vw;


    // icons
    QIcon plcOnlineIcon, plcOfflineIcon;
    QIcon cameraOnlineIcon, cameraOfflineIcon;
    QIcon trackOnIcon, trackOffIcon;
    QIcon controlOnIcon, controlOffIcon;
    QIcon playOnIcon, playOffIcon;
    QIcon stopOnIcon, stopOffIcon;
    QIcon emergencyOnIcon, emergencyOffIcon;
    QIcon calculatorOnIcon, calculatorOffIcon;
    QIcon cmd2LeftIcon, cmd2RightIcon;
    QIcon zIconDisabled, zIconEnabled;
    QIcon videoSaveDisabled, videoSaveEnabled;

    // deviation trend vars
    QGraphicsScene *scene;      // to show devitation trend
    QPen penAxis;               // pen for dev. trend axis
    QPen penLimit;              // pen for dev. trend erorr limit lines
    QPen penStopLimit;          // pen for dev. trend erorr stop limit lines
    QPen penTrack;              // pen for dev. trend
    QPen penTrackError;         // pen for dev. trend
    QList<int> deviationData;   // dev. data list
    int yRes;                   // y coor resolution between 2 successive data in pixels
    int yResIndex;              // yRes array index to get yRes value
    int deviationDataSize;      // list size determined by yRes & trend Y axis pixels
    int eCodeDev;               // deviation error code = 200
    int error;
    int lastErrorFreeData = 0;

    // ui control vars
    bool play, pause, showGuide, trackOn, controlOn;
    bool controlDelayValid;
    bool controlInitiated;
    bool jointWidthControlActive;

    // image framing vars
    int imageWidth, imageHeight;
    int frameWidth, frameHeight;
    int frameWidthCam, frameHeightCam;
    float frameWidthRatioMax = 0.7;
    float frameHeightRatioMax = 0.7;
    int frameWidthMax, frameHeightMax;
    float aspectRatio, aspectRatioGUI;
    bool imgOrientation = false; //false: landscape;aspectRatio>1.33, true: portrait;aspectRatio<=1.33
    float mapFactorWidth, mapFactorHeight, mapFactorX;
    int mapWidth, mapHeight;
    int offsetXmin, offsetXmax;

    int offsetX, offsetY;               // upper left corner coor. of target image, for qlabel
    float offsetXCam, offsetYCam;               // upper left corner coor. of target image, for cam iamge
    int centerX;                        // image center x coor.
    QRect imageFrameRect;
    int offsetXpos;
    QRect guideFrameRect, gfBoxRect, gfLineHorRect, gfLineVerRect, gfTolLeftRect, gfTolRightRect;
    QRect sceneRect;
    int sceneCenterX;
    bool alignGuide2TrackCenter = false;

    // image processing parameters
    imgProcess *iprocess, *iprocessLeft, *iprocessRight;    // image processing objects
    bool iprocessInitSwitch;
    QImage targetArea;                  // target area image
    int iprocessInterval;
    int thetaMin, thetaMax;
    int thetaMinHorLine, thetaMaxHorLine;   // for horizontal laser line
    int thetaMinVerLine, thetaMaxVerLine;   // for vertical edge lines w/o laser
    float thetaStep;
    int thetaMinSub, thetaMaxSub;
    float thetaStepSub;
    int houghLineNo;
    int voteThreshold, voidThreshold;
    int errorLimit, errorLimitNeg, errorLimitOrginal;          // deviation start limit from center in pixels
    int errorLimitDiff;
    int errorStopLimit, errorStopLimitNeg;  // deviation stop limit from center in pixels
    int errorLimitCam, errorLimitNegCam;          // deviation start limit from center in pixels
    int errorStopLimitCam, errorStopLimitNegCam;  // deviation stop limit from center in pixels
    int errorMaxCam, errorMaxFactor = 3;    // if error is too much, it is a detection error
    bool subImageProcessingSwitch;          // sub image processing
    int subImageProcessingType;                       // 0: Void Based, 1: Solid Based
    bool thinJointAlgoActive;
    bool errorStopLimitLineVisible;
    float errorStopScale;
    int initialJointWidth;
    int maxJointWidth, minJointWidth;
    int jointWidth;
    int algorithmType = 0;
    int edgeDetectionState = 0; // 0:none, 1: sobel, 2: canny4, 3: canny1
    int areaNumber = 5; // main edge region number

    bool cannyThinning;
    int gaussianSize = 5;
    float stdDev = 1.4;
    int mainEdgesNumber = 1;
    bool matrixFlag = false;
    bool averaging = false;

    imgProcessThread *iProcessThread;

    int mainLineDistance, mainLineAngle, mainLineVoteValue, solidLineLength = 0;

    // histogram vars
    int maFilterKernelSize = 11;
    int histogramAngleThreshold = 10;
    bool colorMatrix = true; // true=colored, false=gray
    double lenRateThr = 0.50;
    double bandWidthMin = 0.20;
    double bandCenterMax = 0.1;
    int histAreaNo = 1;
    QList<int> histBuffer;
    int histBufferSize = 3;
    double histLoRate = 0.22;
    double histHiRate = 0.78;
    int histLo = 1;
    int histHi = 4;
    int histStatePrev = 0, histState = 0; // 0 no band, 1:transition, 2:full band
    const QString SS_ON = "background-color: lime";
    const QString SS_OFF = "background-color: #F0F0F0";
    const QString SS_TR = "background-color: yellow"; // transition


    // Z-Control
    bool zControlActive;
    float distanceUpTol;
    float distanceDownTol;
    float distance;
    int distanceRaw;
    float distanceTarget;
    float distanceUpStart,      // start action
          distanceUpStop,       // stop action
          distanceDownStart,    // start action
          distanceDownStop;     // stop action
    float zStartStopRate;

    // ZM-Control   FIXED PULSE FOR TORCH HEIGHT
    bool zmControlActive = false;
    int zmControlTime = 100;    // mili second

    // 2 PASS WELDING
    bool twoPassWelding;
    bool autoDetect2ndPass;
    int algorithmTypePass1 = 4; // Algo6() LINE DETECTION WITH MAIN EDGES
    int algorithmTypePass2 = 1; // Algo3() MAIN EDGES
    //int algorithmTypePass2 = 6; // Algo8() MAIN EDGES WITH AREAS
    bool pass1detected = false;
    bool pass2detected = false;
    bool passCompleted = false;
    int pass1_offsetXpos;
    int pass1_offsetXCam;
    int pass2_offsetXpos;
    int pass2_offsetXCam;
    int pass1_offsetXposDelta = 0;
    int pass1_offsetXCamDelta = 0;
    int pass2_offsetXposDelta = 0;
    int pass2_offsetXCamDelta = 0;
    bool timeControlTwoPass;
    int pass1TotalTime = 5000;
    int pass1TimerTime = 5000;
    int passTrTimerTime = 2000;
    int pass2TimerTime = 3000;
    QTimer timerPass1st;
    QTimer timerPassTr;
    QTimer timerPass2nd;


    // plc vars
    struct{
       PlcBYTE bitCommands;
       PlcBYTE bitDatas;
       PlcWORD distance;
    }DB;

    PlcQtLib *plc;

    bool PLCSIM = false;
    QUrl urlPLC;                            // plc url
    int plcType;                            // selection for S7-200, S7-300, etc
    bool connectRequested;                  // plc connection request
    bool connectRequestedonBoot;            // plc connection request on app. start
    bool plcInteractPrev = false;
    bool plcConnected = false;
    int cmdState,
        cmdStatePrev,                       // to send cmd plc if cmd is changed
        cmdStatePrev2,                      // to make it histeryzisis between start/stop
        cmdZState,
        cmdZStatePrev,                      // to send cmd plc if cmd is changed
        cmdZStatePrev2;                     // to make it histeryzisis between start/stop
    QList<int> cmdBuffer;
    QList<int> cmdBufferTime;
    int cmdBufferMaxSize = 3;
    bool cmdSended;
    bool goX;
    bool goZ;
    int DB_NO;
    int BYTE_NO;
    int controlDelay;
    int controlThreadCount, controlThreadCountSize;
    bool controlPause;
    bool readMachineStatus;
    bool readDistance;
    bool readWeldSeam;
    bool weldSeamExists;

    //hard makine kontrolü için
    bool hardControlStart;
    bool mak_aktif_now, mak_aktif_old;
    int machineNo;

    bool dynamicAlgo;

    // time control: to stop the control after some amount of time after the beginning
    bool timeControl;
    int timeLimit;
    bool permTime;
    bool startTimeControlCount;
    int timeControlCounter;

    bool lineDetection;
    int lineScoreLimit;


    bool captureVideo;
//    QList<QImage *> videoList;
    //QList<QImage> videoListx;
    QImage *videoList;
    int videoFrameCount;
    int videoFrameSize = 25;
    int videoDuration = 60; // second
    int videoPeriod = 500;  // milisecond, snapshot interval
    QTimer videoTimer, videoStopTimer;
    int videoMode = 0;  // 0:old time mode, 1: fps mode
    bool autoVideo = false;

    /*
    int right_VMEM_BYTE, right_BITofBYTE;
    int left_VMEM_BYTE, left_BITofBYTE;
    int stop_VMEM_BYTE, stop_BITofBYTE;
    int emergency_VMEM_BYTE, emergency_BITofBYTE;
    */

    struct weldData {
        int state;
        int time;
    };

    QList<weldData> weldCommands;
    int weldCommandsSize;

    // time vars
    getSystemTime timeSystem;                   // system time class
    int firstTimeTick, secondTimeTick;          // succesive system time instances
    int tickDiff;                               // difference between 2 system times
    int frameInterval;                          // image play interval in msec
    QTimer *timer, *timerSn;                    // 1msec & 1sn timers
    QTimer *timerControl;                       // to check plc live state
    QTimer *timerCommand;                       // to manage delayed weld commands
    bool timerControlEnabled;
    int timerControlInterval;
    int msecCount;                              // counter to calc. msec in 1sn
    QDateTime currentDateTime;                  // current date & time

    int fpsTarget;                      // wanted fps
    int fpsReal;                        // shown image number
    int fpsRealLast = 0;                // shown image number
    int fpsRequest;                     // img request fps because of timing problems
    unsigned int frameNo;               // image requestid given by imagegetter
    unsigned int lateFrame;             // total late image no
    unsigned int timeDelayTotal;        // total time delay of images in 1 sn
    int timeDelay;                      // current image time delay
    int timeDelayAvg;                   // average delay in 1 sn
    int imgGetDelay = 0;
    int imgGetDuration = 0;

    // file vars
    QString fileBase;
    QString fileExt;
    QString fileName;
    QString savePath;
    QString folderName;

    // image file open vars
    QString loadedFileName = "";
    QString loadedFileNamewPath = "";
    QDir fileOpenDir;
    QStringList filesInDirList;
    QStringList fileFilters  = (QStringList() << "*.jpg" << "*.png");
    int filesInDirListIndex = 0;

    // report vars
    int controlStartTime, controlElapsed;
    int errorAvg, errorTotal, processCount;
    int errorMax;
    QString logFileName;
    QList<QString> fileData;

    // permisions & alarms
    bool permPLC;                       // permision to access plc; given by plcCheck()
    bool permOperator;                  // op. permision about analyzed image; given by analyzeDialog();
    bool alarmCameraDownLock;
    bool alarmCameraOnlineLock;
    bool emergencyStop, emergencyBlink;
    bool permWeld;
    bool detectionError;

    Ui::MainWindow *ui;                 // mainwindow

    // functions ____________________________________________________________________________________
    explicit MainWindow(QWidget *parent = 0);       // constructor

    int timeDifference(int first, int last);        // time difference of msec portions
    int timeDifference(QTime first, QTime second);  // time difference between 2 QTime in msec upon 00:00
    QString timeString();
    int calcTotalMsec(int hour, int min, int second, int msec);     // calc. total msec of time values

    void addAxis();                                 // draws dev. trend axis & frame box
    void clearTrack();                              // clears dev. trend & re-paint the axis
    void drawTrack();                               // draws dev. trend
    QImage* takeTargetAreaImage();                  // get target image from current image as a new object
    void updatePassButtons();

    void edgeDetection(imgProcess *iprocess);
    void processImage(bool deleteObject = true);                            // detect corners and plc command
    void Algo1(imgProcess *iprocess);   // LASER: mono/edge(matrixFlag) > houghTr > detectLongestSolidLines
    void Algo2(imgProcess *iprocess);   // LASER: mono/edge(matrixFlag) > houghTr > detectPrimaryVoid
    void Algo3(imgProcess *iprocess);   // woLASER: edge > houghTr > detectMainEdges
    void Algo4(imgProcess *iprocess);   // woLASER: value > detectThinJointCenter
    void Algo5(imgProcess *iprocess);   // woLASER: value > contrast matrix > houghTr > calcAvgDistAndAngleOfMajors
    void Algo6(imgProcess *iprocess);   // woLASER: canny1 > houghTr > detectMainEdges > thickenEdgeMap > scoreLineCrossing
    QString calcImageParametes(QImage img, bool info=false);

    void repaintGuide();                            // update guide
    void repaintDevTrend();                         // update deviation trend
    void calcZParameters();

    void readSettings();                            // read settings from ini file
    void writeSettings();                           // write settings to ini file
    void checker();
    int getBitofByte(unsigned char byte, int bitNo);

    void doAutoFocus();
    void doAutoFocusAlgo(double start, double end, int sampleNo, int depth);
    void doAutoFocusAlgo_2StepStart();
    void doAutoFocusAlgo_2Step(double start1, double end1, int sampleNo1, int sampleNo2, bool start_end_2nd_auto = true, double start2 = 0, double end2 = 0);
    void doAutoFocusAlgo_Local_Start(double halfDistance);
    void doAutoFocusAlgo_Local();
    void startControl();
    float* fourierTransform(QImage *img, bool save=false);
    double* calcFittingPrms(QList<double> x, QList<double> y, QList<double> refY, bool &stat, bool ref = false);

    ~MainWindow();                                  // destructor

public slots:
    void showSettingsForm();                        // show settings GUI
    void showSetupForm();                           // show system setup form
    void plcControl();
    void plcConnection(bool stat);
    void plcCheck();
    void plcReadings();
    void plcCommands(int time);
    void target2Left();
    void target2Right();
    void showInfo();
    void showSetupError();
    void videoButton();
    void saveFinished();
    void stopButton();                              // stop video resets
    void initPlcTimer();                            // 2sec first connect(plc) time delay to start plc control timer

    void testButton();

    void playCam();
    void imgGetterDelay();
    void makeNetworkRequest();
    void camConnected();
    void camNotConnected();
    void killCamStreamThread();

    void checkFocusState();
    void focusState(bool state);                    // true; in focus, false; out of focus
    void checkFocusStatus();
    void checkAutoFocusingState();
    void focusingActionState(bool);                 // true; focusing in action, false; stable
    void setFocusPos(float);
    void apiRequestCompleted();
    void testSlot();
    void calcFocusValue(int algo, int roi, int number);
    void calcFocusValueSlot();
    void focusValueCalculatedSlot(double val);
    double calcFocusValueLaplacian(int frame);
    double calcFocusValueLaplacian(QImage img);
    void getFFT();
    void getFuzzyEntropy();
    void iterationFinished();
    double findCurveFitting(QList<double> x1, QList<double> y1, int iterNo);

    void imageProcessingCompleted(int time);
    void histAnalysisCompleted();

    void passTrSlot();
    void pass2StartSlot();

    void on_passOneButton_clicked();
    void on_passTwoButton_clicked();

signals:
    void cameraDown();
    void focusValueCalculated(double val);

protected:
    void closeEvent(QCloseEvent*);                  // exit from application

private slots:
    // gui controls
    void getImageFromCam();                         // saves current image from camera
    void getImageFromStream(int captureTime);
    void playButton();                              // play video inits
    void analyzeButton();                           // analyze current image to check if corners detected or not
    void guideButton();                             // show/hide guide
    void trackButton();                             // track on/off
    void controlButton();                           // plc control on/off
    void emergencyButton();
    void infoButton();                              // show info GUI
    void exitButton();                              // show exit gui
    void clearMsgBoxButton();                       // clears plan text edit widget (info) of the gui
    void helpButton();
    void showLicenseDialog();
    void showReport();
    void thinJointButton();
    void videoSlot();
    void videoTimerStopSlot();

    // process controls
    void update();                                  // 1msec timer actions
    void updateSn();                                // 1sn timer actions
    void startTimer();                              // 2sn bood delay timer
    void processCommands();

    void testEdit();
    void timeEdit();

    void on_setupButton_clicked();

    void on_guideAlignButton_clicked();


    void on_targetDriftLeft_clicked();

    void on_targetDriftCenter_clicked();

    void on_targetDriftRight_clicked();

    void on_pass1timeEdit_returnPressed();

    void on_pass2timeEdit_returnPressed();

    void on_torchUpEdit_returnPressed();

private:
    protect lic;

    bool writeReport();
    QMutex mMutex;

};

#endif // MAINWINDOW_H

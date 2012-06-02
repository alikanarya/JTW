#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define INIFILENAME         "settings.ini"
#define _URL_CAM            "http://10.0.0.5/snap.jpg"
#define _URL_PLC            "10.0.1.4"
#define _FPS                25
#define _IPROCESS_INT       1
#define _FRAME_WIDTH        320
#define _FRAME_HEIGHT       160
#define _THETA_MIN          80
#define _THETA_MAX          100
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
#define _PLAY_CAM_ONBOOT    true
#define _SUB_IPROCESS       true
#define _SUB_TYPE           0
#define _CONTROL_DELAY      0
#define _THIN_JOINT         false
#define _Z_CONTROL          false
#define _UP_TOL             1.0
#define _DOWN_TOL           1.0

#define _PWD_SETTINGS       "nokts"
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

#define _MAINTITLE "JTW - Kaynak için Birleþme Yeri Takipçisi :: "
#define _TITLE "DIÞ KAYNAK"

//const int yResArray[9] = {1, 2, 4, 6, 8, 10, 12, 16, 20}; // for height 240
//const int yResArray[9] = {12, 12, 12, 12, 12, 12, 12, 16, 20};
//const int yResArray[9] = {1, 1, 2, 4, 5, 8, 10, 16, 20}; // for height 160
const int yResArray[9] = {10, 10, 10, 10, 10, 10, 10, 16, 20};

#include <QMainWindow>
namespace Ui {
    class MainWindow;
}

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

#include "zctrldialog.h"
#include "ui_zctrldialog.h"

#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <QtGui>
#include <QNetworkInterface>

#include "../_Modules/Getimage/getimage.h"
#include "../_Modules/Imgprocess/imgprocess.h"
#include "messages.h"
#include "../_Modules/S7/s7.h"
#include "../_Modules/Getsystemtime/getsystemtime.h"
#include "plccontrolthread.h"
#include "../_Modules/Protect/protect.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    QRect rectScreen;                   // screen geometry
    QString title;                      // window title
    bool settingsPWDOK;                 // to switch pwd dialog or settings form
    bool setupPWDOK;                    // to switch pwd dialog or setup form

    QSettings *settings;                // settings: to read/write ini file
    QString statusMessage;              // general status message

    plcControlThread *threadPLCControl;

    QUrl urlCam;                        // camera url
    bool playCamonBoot;                 // play camera on app. start
    getImage *imageGetter;              // camera image request & retrieve class
    getImage *cameraChecker;
    networkData *lastData, *prevData;   // current and prev image data vars

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

    // deviation trend vars
    QGraphicsScene *scene;      // to show devitation trend
    QPen penAxis;               // pen for dev. trend axis
    QPen penLimit;              // pen for dev. trend erorr limit lines
    QPen penStopLimit;          // pen for dev. trend erorr stop limit lines
    QPen penTrack;              // pen for dev. trend
    QList<int> deviationData;   // dev. data list
    int yRes;                   // y coor resolution between 2 successive data in pixels
    int yResIndex;              // yRes array index to get yRes value
    int deviationDataSize;      // list size determined by yRes & trend Y axis pixels
    int eCodeDev;               // deviation error code = 200

    // ui control vars
    bool play, pause, showGuide, trackOn, controlOn;
    bool controlDelayValid;

    // image framing vars
    int imageWidth, imageHeight;
    int frameWidth, frameHeight;
    int offsetX, offsetY;               // upper left corner coor. of target image
    int centerX;                        // image center x coor.
    QRect imageFrameRect;
    int offsetXpos;
    QRect guideFrameRect, gfBoxRect, gfLineHorRect, gfLineVerRect, gfTolLeftRect, gfTolRightRect;
    QRect sceneRect;
    int sceneCenterX;

    // image processing parameters
    imgProcess *iprocess, *iprocessLeft, *iprocessRight;    // image processing objects
    bool iprocessInitSwitch;
    QImage targetArea;                  // target area image
    int iprocessInterval;
    int thetaMin, thetaMax;
    float thetaStep;
    int thetaMinSub, thetaMaxSub;
    float thetaStepSub;
    int houghLineNo;
    int voteThreshold, voidThreshold;
    int errorLimit, errorLimitNeg;          // deviation start limit from center in pixels
    int errorStopLimit, errorStopLimitNeg;  // deviation stop limit from center in pixels
    bool subImageProcessingSwitch;          // sub image processing
    int subImageProcessingType;                       // 0: Void Based, 1: Solid Based
    bool thinJointAlgoActive;
    bool errorStopLimitLineVisible;
    float errorStopScale;

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


    // plc vars
    QUrl urlPLC;                            // plc url
    int plcType;                            // selection for S7-200, S7-300, etc
    bool connectRequested;                  // plc connection request
    bool connectRequestedonBoot;            // plc connection request on app. start
    bool plcInteractPrev;
    int cmdState,
        cmdStatePrev,                       // to send cmd plc if cmd is changed
        cmdStatePrev2,                      // to make it histeryzisis between start/stop
        cmdZState,
        cmdZStatePrev,                      // to send cmd plc if cmd is changed
        cmdZStatePrev2;                     // to make it histeryzisis between start/stop
    bool cmdSended;
    bool goX;
    bool goZ;
    int DB_NO;
    int BYTE_NO;
    int controlDelay;
    int controlThreadCount, controlThreadCountSize;
    bool controlPause;
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
    QTimer *timerControl;
    bool timerControlEnabled;
    int timerControlInterval;
    int msecCount;                              // counter to calc. msec in 1sn
    QDateTime currentDateTime;                  // current date & time

    int fpsTarget;                      // wanted fps
    int fpsReal;                        // shown image number
    int fpsRequest;                     // img request fps because of timing problems
    unsigned int frameNo;               // image requestid given by imagegetter
    unsigned int lateFrame;             // total late image no
    unsigned int timeDelayTotal;        // total time delay of images in 1 sn
    int timeDelay;                      // current image time delay
    int timeDelayAvg;                   // average delay in 1 sn

    // file vars
    QString fileBase;
    QString fileExt;
    QString fileName;
    QString savePath;

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
    bool emergencyStop, emergencyBlink;
    bool permWeld;
    bool detectionError;

    Ui::MainWindow *ui;                 // mainwindow
    zctrlDialog *ui_zctrl;

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
    void processImage();                            // detect corners and plc command
    void processStandardHT();                       // standard hough transform
    void processSubImageVoidness();                 // sub image based on VOID areas
    void processSubImageSolidness();                // sub image based on SOLID lines
    void processThinJoint();                        // darkness analsis for thin joint
    void repaintGuide();                            // update guide
    void repaintDevTrend();                         // update deviation trend
    void calcZParameters();

    void readSettings();                            // read settings from ini file
    void writeSettings();                           // write settings to ini file
    ~MainWindow();                                  // destructor

public slots:
    void showSettingsForm();                        // show settings GUI
    void showSetupForm();                           // show system setup form
    void plcControl();
    void target2Left();
    void target2Right();

signals:
    void cameraDown();

protected:
    void closeEvent(QCloseEvent*);                  // exit from application

private slots:
    // gui controls
    void getImageFromCam();                         // saves current image from camera
    void playButton();                              // play video inits
    void stopButton();                              // stop video resets
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
    void zControlButton();
    void getUpTol();
    void getDownTol();

    // process controls
    void update();                                  // 1msec timer actions
    void updateSn();                                // 1sn timer actions
    void startTimer();                              // 2sn bood delay timer
    void initPlcTimer();                            // 2sec first connect(plc) time delay to start plc control timer
    void cameraDownAction();                        // actions handled when camera is not accesible

    void testEdit();

private:
    protect lic;

    void checker();
    bool writeReport();
    void playCam();


};

#endif // MAINWINDOW_H

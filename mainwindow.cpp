
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCodec>
#include "windows.h"

#include "licensedialog.h"
#include "analyzedialog.h"
#include "helpdialog.h"
#include "exitdialog.h"
#include <QCryptographicHash>



//#include "../_Modules/Algo/localMinimum.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    rectScreen = QApplication::desktop()->geometry();


    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    //flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTitleHint;   // only title on title bar
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);


    //ui->plainTextEdit->appendPlainText(QString::number(rectScreen.width())+"x"+QString::number(rectScreen.height()));
    ui->labelTime->hide();
    ui->emergencyButton->hide();
    //ui->testEdit->hide();
    //ui->testButton->hide();

    // icon assignmets
    plcOnlineIcon.addFile(":/resources/s7_200-Enabled-Icon.png");
    plcOfflineIcon.addFile(":/resources/s7_200-Disabled-Icon.png");
    cameraOnlineIcon.addFile(":/resources/Camera-Enabled-Icon.png");
    cameraOfflineIcon.addFile(":/resources/Camera-Disabled-Icon.png");
    trackOnIcon.addFile(":/resources/Phone-Book-icon.png");
    trackOffIcon.addFile(":/resources/Phone-Book-Disabled-icon.png");
    controlOnIcon.addFile(":/resources/Weather-icon.png");
    controlOffIcon.addFile(":/resources/Weather-Disabled-icon.png");
    playOnIcon.addFile(":/resources/Play-Normal-icon.png");
    playOffIcon.addFile(":/resources/Play-Disabled-icon.png");
    stopOnIcon.addFile(":/resources/Stop-Normal-icon.png");
    stopOffIcon.addFile(":/resources/Stop-Disabled-icon.png");
    emergencyOnIcon.addFile(":/resources/Emergency-Stop-Enabled-Icon.png");
    emergencyOffIcon.addFile(":/resources/Emergency-Stop-Disabled-Icon.png");
    calculatorOnIcon.addFile(":/resources/calculator-icon.png");
    calculatorOffIcon.addFile(":/resources/calculator-Disabled-icon.png");
    cmd2RightIcon.addFile(":/resources/forward.png");
    cmd2LeftIcon.addFile(":/resources/backward.png");
    zIconDisabled.addFile(":/resources/height-icon.png");
    zIconEnabled.addFile(":/resources/height-icon-ON.png");
    videoSaveDisabled.addFile(":/resources/Record-Disabled-icon.png");
    videoSaveEnabled.addFile(":/resources/Record-Normal-Red-icon.png");

    ui->trackButton->setEnabled(false);


    statusMessage = "";
    settings = new QSettings(INIFILENAME, QSettings::IniFormat);
    settings->setIniCodec( "UTF-8" );
    readSettings();

    this->setWindowTitle(_MAINTITLE + title);


    // orginal and target image parameters
    imageWidth = 640;   //image->width();
    imageHeight = 480;  //image->height();
    aspectRatioGUI = ((float)imageWidth) / imageHeight;
    mapFactorWidth = 1;
    mapWidth = imageWidth;
    frameWidthMax = mapWidth * frameWidthRatioMax;
    frameWidthCam = frameWidthMax;
    offsetXCam = 0;

    mapFactorHeight = 1;
    mapHeight = imageHeight;
    frameHeightMax = mapHeight * frameHeightRatioMax;
    frameHeightCam = frameHeightMax;
    offsetYCam = 0;

    imageFrameRect = ui->imageFrame->geometry();
    guideFrameRect = ui->guideFrame->geometry();
    gfBoxRect = ui->frame->geometry();
    gfLineHorRect = ui->guideLineHorz->geometry();
    gfLineVerRect = ui->guideLineVert->geometry();
    //gfTolLeftRect = ui->guideTolLeft->geometry();
    //gfTolRightRect = ui->guideTolRight->geometry();
    offsetXpos = 0;

    showGuide = true;       // show guide initially
    repaintGuide();

    sceneRect = ui->trackView->geometry();
    sceneCenterX = (sceneRect.width() - 4)/ 2;  // 4: total border thickness

    // play video controls
    play = playCamonBoot;
    pause = false;
    fpsRequest = 0;
    fpsReal = 0;
    frameInterval = 1000 / fpsTarget;
    frameNo = 0;
    lateFrame = 0;
    timeDelayTotal = 0;
    timeDelayAvg = 0;
    alarmCameraDownLock = false;
    captureVideo = false;


    // file vars init.
    fileBase = "resim";
    fileExt = ".jpg";
    savePath = "resim/";
    fileName = savePath + fileBase + fileExt;
    folderName = "";

    switch ( camStreamType ) {
        case 1: // JPEG
            // image getter class & image data inits.
            imageGetter = new getImage(urlCam.toString(), 10);
            //imageGetter->url.setUserName("admin");imageGetter->url.setPassword("admin");
            connect(imageGetter, SIGNAL(downloadCompleted()), this, SLOT(makeNetworkRequest()));
            connect(imageGetter, SIGNAL(lastDataTaken()), this, SLOT(playCam()));
            connect(imageGetter, SIGNAL(cameraOnlineSignal()), this, SLOT(camConnected()));
            connect(imageGetter, SIGNAL(cameraDownSignal()), this, SLOT(camNotConnected()));
            break;
        case 0: // STREAM
            playStream = new threadPlayStream(&mMutex, urlCamStream.toString(), this);
            connect(playStream, SIGNAL(imageCaptured(int)), this, SLOT(getImageFromStream(int)));
            connect(playStream, SIGNAL(connected()), this, SLOT(camConnected()));
            connect(playStream, SIGNAL(notConnected()), this, SLOT(camNotConnected()));
            break;
    }


    lastData = new networkData();
    prevData = new networkData();

    //**cameraChecker = new getImage(urlCam.toString());

    camApi = new getImage(urlCamStream.host(), false);
    connect(camApi, SIGNAL(focusState(bool)), this, SLOT(focusState(bool)));
    connect(camApi, SIGNAL(focusingActionState(bool)), this, SLOT(focusingActionState(bool)));
    connect(camApi, SIGNAL(requestCompleted()), this, SLOT(apiRequestCompleted()));
    timerAutoFocus = new QTimer(this);
    connect(timerAutoFocus, SIGNAL(timeout()), this, SLOT(checkAutoFocusingState()));

    connect(this, SIGNAL(focusValueCalculated(double)), this, SLOT(focusValueCalculatedSlot(double)));

    iProcessThread = new imgProcessThread();

    connect(iProcessThread, SIGNAL(imageProcessingCompleted()), this, SLOT(imageProcessingCompleted()));
    connect(iProcessThread, SIGNAL(histAnalysisCompleted()), this, SLOT(histAnalysisCompleted()));

    threadVideoSave = new videoSaveThread();
    connect(threadVideoSave, SIGNAL(saveFinished()), this, SLOT(saveFinished()));

    // tracking & plc controls
    trackOn = false;
    controlOn = false;
    emergencyStop = false;
    detectionError = true;
    permOperator = false;
    weldSeamExists = false;
    startTimeControlCount = false;
    timeControlCounter = 0;

    // plc
    connectRequested = connectRequestedonBoot;
    threadPLCControl = new plcControlThread(plcType, urlPLC.toString());

    /**threadPLCControl->plc->portNum = 102;
    if (plcType == 0){              // S7-200
        threadPLCControl->dbNo = DB_NO = 1;     //for all variables - V memory
        threadPLCControl->byteNo = BYTE_NO = 0; //for commands; V0.0-V0.7
        threadPLCControl->dbNoRead = DB_NO;     //for all variables - V memory
        threadPLCControl->readByte = 1;         //for plc2pc data; V1.0-V1.7
        //distance sensor: VW2
    }
    else if (plcType == 1){         // S7-300
        threadPLCControl->dbNo = DB_NO;         //for commands
        threadPLCControl->byteNo = BYTE_NO;     //for commands
        threadPLCControl->byteNo2 = BYTE_NO + 1;//for additional commands
        threadPLCControl->dbNoRead = 2;         //for plc2pc data; DB2
        threadPLCControl->readByte = 0;         //for plc2pc data; 0
        //distance sensor: DBx.DBW2 ; x = dbNo or dbNoRead
    }**/

    // plc-new
    plc=new PlcQtLib("192.168.0.4",1,4,(unsigned char*) &DB);
    plc->protocol = PROTOCOL_S7200;
    plc->loopSleepTime = 100;
    plc->disconnectSleepTime = 500;

    connect(plc, SIGNAL(connected(bool)), this, SLOT(plcConnection(bool)));
    connect(plc, SIGNAL(dataReady()), this, SLOT(plcReadings()));


    if (!threadPLCControl->plc->libraryLoaded && false){    // to bypass check for new library
        permPLC = false;
        ui->plainTextEdit->appendPlainText(threadPLCControl->plc->message);
        ui->plainTextEdit->appendPlainText(MESSAGE5);
    } else {
        if (connectRequestedonBoot){
            //**plcInteractPrev = false;
            timerControlEnabled = false;
            // wait 2sec. to check first init of plc connection
            QTimer::singleShot(2000, this, SLOT(startTimer()));
        }
    }

    checker();

    mak_aktif_now = mak_aktif_old = false;


    // 1sn timer
    timerSn = new QTimer(this);
    connect(timerSn, SIGNAL(timeout()), this, SLOT(updateSn()));
    timerSn->start(1000);

    /* 1msec timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    msecCount = 0;
    firstTimeTick = timeSystem.getSystemTimeMsec();
    timer->start(1);
    */

    settingsPWDOK = false;
    setupPWDOK = false;

    // init deviation trend
    scene = new QGraphicsScene();
    ui->trackView->setScene(scene);

    penAxis.setColor(Qt::black);    penAxis.setWidth(2);
    penLimit.setColor(Qt::red);     penLimit.setWidth(1);
    penStopLimit.setColor(Qt::green);     penStopLimit.setWidth(1);
    penTrack.setColor(Qt::blue);    penTrack.setWidth(1);

    deviationDataSize = sceneRect.height() / yRes + 2;     // sceneRect.height()/yRes+1 + 1 past data to draw first line
    deviationData.clear();
    eCodeDev = 200;

    errorStopLimitLineVisible = false;
    addAxis();

    // weld commands init.
    controlInitiated = false;
    initialJointWidth = jointWidth = 0;
    maxJointWidth = 1000;

    cmdState = _CMD_STOP;
    cmdStatePrev2 = _CMD_CENTER;
    ui->cmdStatus->setIcon(QIcon());

    controlDelay = QString::number(controlDelay).toInt(&controlDelayValid, 10); //controlDelayValid: true if conversion is ok

    timerControlInterval = 5000;
    controlThreadCountSize = 15000 / timerControlInterval;   // timer shot count for 15sec plc check

    weldCommandsSize = controlDelay / timerControlInterval;

    // start message
    ui->plainTextEdit->appendPlainText(timeString() + "Sistem başlatılmıştır.");
    lic.checkLicence();

    if (lic.licenseState != _LIC2){
        QTimer::singleShot(500, this, SLOT(showLicenseDialog()));
    }


    if ( zControlActive || readDistance) {
        ui->labelDistance->show();
        ui->labelDistanceTag->show();
        ui->labelDistanceTag2->show();
    } else {
        ui->labelDistance->hide();
        ui->labelDistanceTag->hide();
        ui->labelDistanceTag2->hide();
    }

    if (timeControl){
        ui->labelTimeTag->show();
        ui->labelTimeTag2->show();
        ui->timeEdit->show();
        ui->timeEdit->setText(QString::number(timeLimit));
        permTime = false;   // enabled when control begun

    } else {
        ui->labelTimeTag->hide();
        ui->labelTimeTag2->hide();
        ui->timeEdit->hide();
        permTime = true;

    }

    // z control
    distanceRaw = 32000;  // to make distance neg read initially
    zStartStopRate = 0.3;


    //if ( thinJointAlgoActive || zControlActive || hardControlStart )
      //  QTimer::singleShot(500, this, SLOT(showInfo()));
    ui->imageFrame->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->imageFrame->setStyleSheet("background: black");
//    ui->imageFrame->setScaledContents(true);

    ui->leftButton->setEnabled( false );
    ui->leftButton->hide();
    ui->rightButton->setEnabled( false );
    ui->rightButton->hide();

    if (!twoPassWelding) autoDetect2ndPass = false;
    ui->passOneButton->setEnabled( twoPassWelding );
    ui->passTwoButton->setEnabled( twoPassWelding );
    if (!twoPassWelding){
        ui->passOneButton->hide();
        ui->passTwoButton->hide();
    }

    firstPass = twoPassWelding;

    //**cameraChecker->cameraDown = false;
    if (play) playButton();

    /*
    player = new QMediaPlayer;
    vw = new QVideoWidget;
    player->setVideoOutput(vw);
//    player->setMedia(QUrl::fromLocalFile("E:/M/Movie/Media1/Film_Yabancı/Anime/Laserion/Laserion02 _Friends for the skin.avi"));
    player->setMedia(QUrl("http://admin:admin@192.168.3.3/cgi-bin/mjpg/video.cgi?channel=1&subtype=1"));
    //player->setMedia(QUrl("http://localhost/laserion.avi"));
    vw->setGeometry(700,10,400,300);
    vw->show();
    player->play();
    qDebug()<<player->state() << player->errorString() << player->isAudioAvailable();
    */
    /*
    d = new VideoTest;
    d->vlcInstance = new VlcInstance(VlcCommon::args(), this);
    d->vlcMedia = new VlcMedia("http://admin:admin@192.168.3.3/cgi-bin/mjpg/video.cgi?channel=1&subtype=1", d->vlcInstance);
    d->vlcMediaPlayer = new VlcMediaPlayer(d->vlcInstance);
    d->vlcVideoWidget = new VlcWidgetVideo(this);

    d->vlcMediaPlayer->setVideoWidget(d->vlcVideoWidget);
    d->vlcVideoWidget->setMediaPlayer(d->vlcMediaPlayer);

    d->vlcVideoWidget->setGeometry(700,10,400,300);
    d->vlcVideoWidget->show();

    d->vlcMediaPlayer->open(d->vlcMedia);
    */
}

void MainWindow::showInfo(){

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        QString str = "";

        if ( lineDetection ) {

            str += "AKTİF !!! : İz Tespiti Modu\n";
        } else {

            if (dynamicAlgo)
                str += "AKTİF !!! : Dinamik Algoritma\n";
            else {
                if (thinJointAlgoActive)
                    str += "AKTİF !!! : Lazersiz Takip\n";
                            //"Lazerin V şekli teşkil edemediği ince ağızlarda kullanılır.\nLazeri kapatın ve\nkaynak ağzının düzgün karanlık siluet oluşturmasına\ndikkat edin!";
            }

            if (zControlActive)
                str += "AKTİF !!! : Yükseklik Kontrolü\n";
            if (hardControlStart)
                str += "AKTİF !!! : Makineden Kaynak Başlatma\n";
            if (timeControl)
                str += "AKTİF !!! : Zaman Kontrolü\n";
        }
        msgBox.setText(str);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
}

void MainWindow::showSetupError(){

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        QString str = "";

        str += " KAMERA - TORCH HİZALAMASINDA ÇOK FAZLA HATA VAR !!!\n";
        str += " OTOMATİK TAKİP BAŞLATILMAYACAKTIR !!!";

        msgBox.setText(str);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
}

void MainWindow::checker(){

//    permWeld = !emergencyStop && permOperator && permPLC && play && !cameraDownStatus && trackOn && controlDelayValid;
    permWeld = !emergencyStop && play && !cameraDownStatus && trackOn && controlDelayValid;

    ui->controlButton->setEnabled(permWeld);
}

QImage* MainWindow::takeTargetAreaImage(){

    QImage *image;
    if (applyCameraEnhancements) {
        image = new QImage(imageFileChanged);
    } else {
        image = new QImage(*lastData->image);
    }
    targetArea = image->copy(offsetX, offsetY, frameWidth, frameHeight);
    return image;
}

void MainWindow::getImageFromCam(){

    fileName = fileBase + "_" + QDateTime::currentDateTime().toString("hhmmss_zzz") + fileExt;

    if (applyCameraEnhancements) {
        if (imageFileChanged.save(savePath + fileName))
            ui->plainTextEdit->appendPlainText(fileName + " başarıyla " + savePath + " klasörüne kayıt edildi.");
        else
            ui->plainTextEdit->appendPlainText(fileName + " kayıt edilemedi !!!");
    } else {
        if (lastData->image->save(savePath + fileName))
            ui->plainTextEdit->appendPlainText(fileName + " başarıyla " + savePath + " klasörüne kayıt edildi.");
        else
            ui->plainTextEdit->appendPlainText(fileName + " kayıt edilemedi !!!");
    }
}

void MainWindow::playButton(){

    // inited vars ?????
    fpsRequest = 0;
    fpsReal = 0;
    lateFrame = 0;
    timeDelayTotal = 0;
    timeDelayAvg = 0;
    msecCount = 0;
    alarmCameraDownLock = false;
    alarmCameraOnlineLock = false;
    getCamImageProperties = true;
    play = true;

    ui->playButton->setIcon(playOnIcon);
    ui->stopButton->setIcon(stopOffIcon);

    ui->trackButton->setEnabled(true);
    ui->controlButton->setEnabled(false);
    ui->videoButton->setIcon(videoSaveEnabled);

    camReconnectLock = true;
    switch ( camStreamType ) {
        case 1: // JPEG
            makeNetworkRequest();
            break;
        case 0: // STREAM
            if (!playStream->isRunning()){
                cameraDownStatus = false;
                playStream->start();
                playStream->measureFpsFn(3000);
            }
            break;
    }
}

void MainWindow::stopButton(){

    // reseted vars ????
    play = false;

    switch ( camStreamType ) {
        case 1: // JPEG
            imageGetter->reset();
            break;
        case 0: // STREAM
            if (playStream->isRunning())
                playStream->stop();
            if (!playStream->wait(1000)) {
                playStream->terminate();
                playStream->wait();
                camNotConnected();
            }
            break;
    }

    ui->playButton->setIcon(playOffIcon);
    ui->stopButton->setIcon(stopOnIcon);

    ui->trackButton->setEnabled(false);
    ui->controlButton->setEnabled(false);

}

void MainWindow::makeNetworkRequest(){

    if (play && !pause){// && !imageGetter->cameraDown){
        imageGetter->run();
        fpsRequest = imageGetter->fpsRequest;
    }
}

void MainWindow::getImageFromStream(int captureTime){

    firstTimeTick = captureTime;

    /* delay buffer code
    if (!playStream->frameBuffer.isEmpty()) {
        cv::cvtColor(playStream->frameBuffer.first(), dest, CV_BGR2RGB);
    }
    playStream->frameBuffer.removeFirst();
    */

    QImage img = QImage( (const uchar*) playStream->dest.data, playStream->dest.cols, playStream->dest.rows, playStream->dest.step, QImage::Format_RGB888 );
    playStream->condition.wakeAll();

    if (img.format() != QImage::Format_Invalid) {
        //qDebug() << playStream->iter;
        lastData->image = new QImage(img);
        lastData->shown = false;

        if (testFlag) {
            fourierTransform(lastData->image, false);
            testFlag = false;
        }

        playCam();
        //ui->imageFrame->setPixmap( QPixmap::fromImage( img.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio) ));
    }
}

void MainWindow::camConnected(){

    cameraDownStatus = false;
    alarmCameraDownLock  = false;

    if( !alarmCameraOnlineLock ) {
        //qDebug()<<Q_FUNC_INFO;
        if (play && camStreamType == 0) {
            playStream->setFps(fpsTarget);
            //playStream->startCapture();
        }

        ui->plainTextEdit->appendPlainText(timeString() + alarm11);
        ui->cameraStatus->setIcon(cameraOnlineIcon);
        alarmCameraOnlineLock = true;
    }
}

void MainWindow::camNotConnected(){

    cameraDownStatus = true;
    camReconnectLock = false;
    alarmCameraOnlineLock = false;

    if( !alarmCameraDownLock ) {
        //qDebug()<<Q_FUNC_INFO;

        ui->plainTextEdit->appendPlainText(timeString() + alarm7);
        ui->statusBar->showMessage("Kameraya bağlanılamıyor !");
        ui->cameraStatus->setIcon(cameraOfflineIcon);
        ui->imageFrame->clear();
        alarmCameraDownLock = true;
        getCamImageProperties = true;
    }

}

void MainWindow::killCamStreamThread(){

    if ( playStream->isRunning() && cameraDownStatus) {
        playStream->stop();
        if (!playStream->wait(1000)) {
            playStream->terminate();
            playStream->wait();
        }
        camReconnectLock = false;
        //qDebug()<<Q_FUNC_INFO;
    }
}

/* OBSOLETE */void MainWindow::update(){
/*
    msecCount++;
    if (play && !pause){
        secondTimeTick = timeSystem.getSystemTimeMsec();    // get current time in msec
        tickDiff = secondTimeTick - firstTimeTick;          // elapsed time from prev.
        if (!cameraChecker->cameraDown){
            // if time has come to display next image request it
            if (tickDiff >= frameInterval){
                //imageGetter->run();
                //playCam();
                firstTimeTick = secondTimeTick;
                //fpsRequest = imageGetter->fpsRequest;
                //qDebug() << tickDiff;
            }
        }
        //if (msecCount % 5 == 0){  if (!cameraChecker->cameraDown && !threadPlay.isRunning()) threadPlay.run();        }
    }*/
}

/* OBSOLETE */void MainWindow:: plcControl(){

    controlThreadCount++;

    //ui->plainTextEdit->appendPlainText(QString::number(controlThreadCount));
    checker();

    int state = _CMD_STOP;
    //int stateZ = _CMD_Z_CENTER;

    if ( readMachineStatus || readDistance || readWeldSeam ) {

        if (controlThreadCount % 4 == 0){

            threadPLCControl->commandRead = true;

            if (hardControlStart && readMachineStatus){

                if (mak_aktif_now && !mak_aktif_old) {

                    ui->plainTextEdit->appendPlainText("Makine çalışıyor");
                    controlInitiated = true;

                    if (timeControl) {
                        timeControlCounter = 0;
                        startTimeControlCount = true;
                    }
                }

                if (!mak_aktif_now && mak_aktif_old) {
                    ui->plainTextEdit->appendPlainText("Makine durdu");

                }
                //else alignGuide2TrackCenter = false;

                mak_aktif_old = mak_aktif_now;
            }

            if ( readDistance ) {

                distance = 300 - ((distanceRaw * 1.0) / 27648.0) * 220.0;
                ui->labelDistance->setText( QString::number(distance, 'f', 1) );
            }
        }
    }
    if (!controlPause){

        // prepare plc command
        if (controlThreadCount == controlThreadCountSize) {

            state = _CMD_CHECK;

            controlThreadCount = 0;
        } else {
            if (emergencyStop){

                state = _CMD_EMERGENCY_ACT;
            } else {

//                if (cameraChecker->cameraDown || !play || detectionError || !permOperator){
                if (imageGetter->cameraDown || !play || !permOperator){

                    state = _CMD_STOP;
                } else {
                    if (play && trackOn && controlOn && permPLC && permTime){

                        if (hardControlStart) {

                            if (mak_aktif_now)
                                state = cmdState;   // Weld Command from image processing
                        } else {
                            state = cmdState;   // Weld Command from image processing
                        }

                        //stateZ = cmdZState; // Z-Control command
                    } else
                        state = _CMD_STOP;
                }
            }
        }


        goX = false;

        if (controlInitiated){
            goX = false;    //dont send command to plc when control is initiated firstly
            //controlInitiated = false;
        } else {
            if (state != cmdStatePrev) {
                goX = true;

                if (state == _CMD_RIGHT)
                    ui->cmdStatus->setIcon(cmd2LeftIcon);
                else if (state == _CMD_LEFT)
                    ui->cmdStatus->setIcon(cmd2RightIcon);
                else if (state != _CMD_CHECK)
                    ui->cmdStatus->setIcon(QIcon());
            }
        }
        /*
        if (state != cmdStatePrev) {

            //if (hardControlStart && alignGuide2TrackCenter){
            if (controlInitiated){
                goX = false;    //dont send command to plc when control is initiated firstly
            } else {

                if (alignGuide2TrackCenter && controlInitiated) {
                    goX = false;
                } else {
                    goX = true;

                    if (state == _CMD_RIGHT)
                        ui->cmdStatus->setIcon(cmd2LeftIcon);
                    else if (state == _CMD_LEFT)
                        ui->cmdStatus->setIcon(cmd2RightIcon);
                    else if (state != _CMD_CHECK)
                        ui->cmdStatus->setIcon(QIcon());
                }
            }
        }
        */
        if (goX || !cmdSended || threadPLCControl->commandRead) {

            if (!PLCSIM) {
                cmdSended = false;

                if (!threadPLCControl->isRunning()){

                    threadPLCControl->commandState = state;

                    threadPLCControl->start();

                    cmdSended = true;
                }

            } else {
                cmdSended = true;
            }
        }

        if (state!= _CMD_CHECK) {
            cmdStatePrev = state;
        }
    }
/*
            if (state != _CMD_CHECK || state != _CMD_EMERGENCY_ACT) {
                if (controlDelay == 0){
                    if (!threadPLCControl->isRunning()){
                        threadPLCControl->commandState = state;
                        threadPLCControl->start();
                        cmdSended = true;
                    }
                } else if (controlDelayValid){
                    weldData wd;
                    wd.state = state;
                    wd.time = timeSystem.getSystemTimeMsec();
                    weldCommands.append(wd);

                    if (weldCommands.size() >= weldCommandsSize){
                        int first = weldCommands[0].state;

                        //ui->plainTextEdit->appendPlainText("st: "+QString::number(first)+" diff: "+QString::number(wd.time-weldCommands[0].time)+"size:"+QString::number(weldCommands.size()));
                        if (!threadPLCControl->isRunning()){
                            threadPLCControl->commandState = first;
                            threadPLCControl->start();
                            cmdSended = true;
                        }
                        weldCommands.removeFirst();
                        //ui->plainTextEdit->appendPlainText(QString::number(cmdState)+"plc "+QString::number(timeSystem.getSystemTimeMsec()));
                    }
                }
            } else {
                if (!threadPLCControl->isRunning()){
                    threadPLCControl->commandState = state;
                    threadPLCControl->start();
                    cmdSended = true;
                }
            }
*/
}

void MainWindow::updateSn(){
    //QVariant boolx(permPLC);
    //ui->plainTextEdit->appendPlainText(boolx.toString());

    if (timeControl) {

        if (startTimeControlCount){
            timeControlCounter++;
            //ui->plainTextEdit->appendPlainText(QString::number(timeControlCounter));
        }

        if (timeControlCounter >= (timeLimit-5)){
            permTime = false;
            startTimeControlCount = false;

        } else {
            permTime = true;
        }

    }

    // blink emergency stop button in case of emergency
    if (emergencyStop){
        emergencyBlink = !emergencyBlink;

        if (emergencyBlink)
            ui->emergencyButton->setIcon(emergencyOffIcon);
        else {
            //QApplication::beep();
            ui->emergencyButton->setIcon(emergencyOnIcon);
        }
    }

    pause = true;

    QString message;

    /* display: time
    currentDateTime = QDateTime::currentDateTime();
    ui->labelTime->setText(currentDateTime.toString("hh:mm:ss"));*/

    /* check camera live state
    cameraChecker->checkHost();
    cameraChecker->cameraDown = false;*/

    if ( play && cameraDownStatus && !camReconnectLock) {

        switch ( camStreamType ) {
            case 1: // JPEG
                makeNetworkRequest();
                break;
            case 0: // STREAM
                if ( playStream->isRunning() ) {
                    playStream->stop();
                    if (!playStream->wait(1000)) {
                        playStream->terminate();
                        playStream->wait();
                    }
                } else {
                    camReconnectLock = true;
                    //QTimer::singleShot(3000, this, SLOT(killCamStreamThread()));
                    playStream->start();
                    //qDebug()<<Q_FUNC_INFO;
                }
                break;
        }
    }

    /*
    if (!cameraDownStatus && !camApi->busy && !timerLock){
        camApi->apiDahuaGetFocusState();
        timerLock = true;
    }
    if (!cameraDownStatus && !camApi->busy && timerLock){
        camApi->apiDahuaGetFocusStatus();
        timerLock = false;
    }
*/
    checker();

    // if video is played
    if (play && !cameraDownStatus){

        switch ( camStreamType ) {
            case 1: // JPEG
                {
                if (fpsReal != 0) timeDelayAvg = timeDelayTotal / fpsReal;      // calc. ave time delay

                fpsRealLast = fpsReal;
                // status bar message
                message = "fps(t/req/real): " + QString::number(fpsTarget) + "/" + QString::number(fpsRequest) + "/" + QString::number(fpsReal);
                message += seperator;

                int rate = 0;
                if (frameNo != 0) rate = (lateFrame * 100 / frameNo);
                message += "lr(%): " + QString::number(rate);
                message += seperator;

                message += "td(ms): " + QString::number(timeDelayAvg);

                message += seperator;
                message += "lf: " + QString::number(lateFrame);

                message += seperator;
                message += "er: " + QString::number(error);

                ui->statusBar->showMessage(message);
                //----------

                fpsRequest = imageGetter->fpsRequest = 0;
                fpsReal = 0;
                timeDelayAvg = 0;
                timeDelayTotal = 0;
                }
                break;
            case 0: // STREAM
                fpsRealLast = fpsReal;
                message = "Streaming: " + QString::number(camImageWidth) + "x" + QString::number(camImageHeight);
                message += "@R=" + QString::number(fpsReal) + "/T=" + QString::number(fpsTarget);
                if (playStream->measureFps)
                    message += "/C=" + QString::number(playStream->realFps);
                message += seperator;
                message += "TD=" + QString::number(timeDelay);
                //message += ":" + QString::number(playStream->propIris);message += ":" + QString::number(playStream->propISO);
                ui->statusBar->showMessage(message);
                fpsReal = 0;
                break;
        }

    }

    msecCount = 0;
    pause = false;

    //ui->plainTextEdit->ensureCursorVisible();
    QTextCursor(ui->plainTextEdit->document()).movePosition(QTextCursor::End);

    /* * plc live state
    if (!plcInteractPrev && threadPLCControl->plc->plcInteract){           // 0 -> 1
        permPLC = true;
        ui->plainTextEdit->appendPlainText(timeString() + MESSAGE6);
    }
    else if (plcInteractPrev && !threadPLCControl->plc->plcInteract){       // 1 -> 0
        permPLC = false;
        ui->plainTextEdit->appendPlainText(timeString() + MESSAGE4);
    }
    plcInteractPrev = threadPLCControl->plc->plcInteract;**/

    /**if (threadPLCControl->plc->plcInteract)
        ui->plcStatus->setIcon(plcOnlineIcon);
    else
        ui->plcStatus->setIcon(plcOfflineIcon);**/

    //if ( thinJointAlgoActive ) ui->thinJointButton->setStyleSheet("color: rgb(255, 0, 0)");
    //else ui->thinJointButton->setStyleSheet("color: rgb(0, 0, 0)");
}

void MainWindow::startTimer(){

    if (!PLCSIM) {
        /**if (!threadPLCControl->isRunning()){
            cmdState = _CMD_CHECK;
            threadPLCControl->commandState = cmdState;
            threadPLCControl->start();
            cmdSended = true;
        }**/
        if (!plc->isRunning()){
            cmdState = _CMD_CHECK;
            //threadPLCControl->commandState = cmdState;
            plc->start();
            cmdSended = true;
        }
    } else {
        threadPLCControl->plc->plcInteract = true;
        cmdSended = true;
    }

    cmdStatePrev = cmdState;
    cmdZStatePrev = _CMD_Z_CENTER;

    // wait 2sec. to check first init of plc connection
    QTimer::singleShot(2000, this, SLOT(initPlcTimer()));
}

void MainWindow::initPlcTimer(){

    //**ui->plainTextEdit->appendPlainText(timeString() + threadPLCControl->plc->message);
    //**if (threadPLCControl->plc->plcInteract) ui->plcStatus->setIcon(plcOnlineIcon);

    timerControlEnabled = true;
    timerControl = new QTimer(this);
    controlThreadCount = 0;
    //**connect(timerControl, SIGNAL(timeout()), this, SLOT(plcControl()));
    connect(timerControl, SIGNAL(timeout()), this, SLOT(plcCheck()));
    controlPause = false;
    timerControl->start(timerControlInterval);

}

void MainWindow::analyzeButton(){

    //if ( !imageGetter->imageList.isEmpty() ) {  // if any image is get
    if (lastData->image->format() != QImage::Format_Invalid) {

        analyzeAction = true;

        analyzeStartTime = timeSystem.getSystemTimeMsec();

        processImage(false);


        //if ( iprocessInitSwitch ) {

            //iprocess->cornerImage(false);   // false for solid canny                                                                    // produce corner image

            //analyzeDialog *_analyzeDialog = new analyzeDialog(iProcessThread->iprocess, processElapsed, this);
            //_analyzeDialog->show();

            //iprocessInitSwitch = false;
            //delete iprocess;
        //}


    } else {
         ui->plainTextEdit->appendPlainText(timeString() + alarm6);
    }
}

void MainWindow::guideButton(){

    //QApplication::beep();
    //ui->plainTextEdit->appendPlainText(lic.makeReport());

    showGuide = !showGuide;
    repaintGuide();

    //ui->leftButton->setEnabled( showGuide && !trackOn );
    //ui->rightButton->setEnabled( showGuide && !trackOn );
}

void MainWindow::on_guideAlignButton_clicked(){

    if (showGuide && trackOn && !controlOn){
        //alignGuide2TrackCenter = true;
        int _offsetX = offsetX + error * mapFactorX;

        if ( _offsetX >= (offsetXmin+10) && ((_offsetX + frameWidth) <= (offsetXmax - 10)) ){
            offsetXpos += error * mapFactorX;
            offsetXCam += error;
        }
        repaintGuide();
    }
}

void MainWindow::trackButton(){

    trackOn = !trackOn;

    if (trackOn){
        ui->trackButton->setIcon(trackOnIcon);
        if (twoPassWelding){
            firstPass = true;
            ui->passOneButton->setStyleSheet("background-color: lime");
            ui->passTwoButton->setStyleSheet("background-color: #F0F0F0");
        }

    } else {
        ui->trackButton->setIcon(trackOffIcon);

        if (twoPassWelding){
            firstPass = true;
            ui->passOneButton->setStyleSheet("background-color: #F0F0F0");
            ui->passTwoButton->setStyleSheet("background-color: #F0F0F0");
        }

        if (controlOn) {
            controlButton();
            ui->controlButton->setEnabled(false);
        }
    }

    //ui->leftButton->setEnabled( showGuide && !trackOn );
    //ui->rightButton->setEnabled( showGuide && !trackOn );

    //ui->thinJointButton->setEnabled(!trackOn);
}

void MainWindow::controlButton(){

    controlOn = !controlOn;

    if (controlOn){

        cmdStatePrev = _CMD_CENTER;

        if (focusCheckBeforeControl){
            /*
            camDoAutoFocus = autoFocusBeforeControl;
            autoFocusPassNo = 0;
            checkFocusState();
            */
            calcFocusValue(2,1,5);  //laplacian,target area,5 frame
        } else {
            startControl();
        }

    } else {

        controlInitiated = false;
        initialJointWidth = jointWidth = 0;
        maxJointWidth = 1000;

        calcImageParametes(*lastData->image, false);
        offsetXpos = 0;
        repaintGuide();

        cmdState = _CMD_CENTER;
        plcCommands();

        ui->controlButton->setIcon(controlOffIcon);
        ui->controlButton->setEnabled(false);
        permOperator = false;
        ui->analyzeButton->setIcon(calculatorOnIcon);

        ui->plainTextEdit->appendPlainText(timeString() + message2);

        // for report
        if (processCount != 0)
            errorAvg = errorTotal / processCount;
        else errorAvg = 0;
        controlElapsed = timeSystem.getSystemTimeMsec() - controlStartTime;
        controlElapsed /= 1000;

        //QString line = QString::number(controlElapsed)+" sn, et:"+QString::number(errorTotal)+", pc:"+QString::number(processCount)+", ea"+QString::number(errorAvg)+", max:"+QString::number(errorMax);
        //ui->plainTextEdit->appendPlainText(line);
        fileData.append("Süre: " + QString::number(controlElapsed) + " sn");
        fileData.append("Ortalama Hata: " + QString::number(errorAvg) + " piksel");
        fileData.append("Max Hata: " + QString::number(errorMax) + " piksel");
        fileData.append(timeString() + "Kaynak bitirildi.");
        fileData.append("---------.---------.---------.---------.---------.---------");

        if (!writeReport()) ui->plainTextEdit->appendPlainText(timeString() + message5);
    }

    //ui->stopButton->setEnabled(!controlOn);
    ui->analyzeButton->setEnabled(!controlOn);
}

void MainWindow::startControl(){

    controlInitiated = true;

    weldCommands.clear();
    weldCommandsSize = controlDelay / timerControlInterval;

    ui->controlButton->setIcon(controlOnIcon);
    ui->plainTextEdit->appendPlainText(timeString() + message1);

    // for report
    errorTotal = 0;
    processCount = 0;
    controlStartTime = timeSystem.getSystemTimeMsec();
    errorMax = 0;
    //QString line = timeString() + "Kaynak başladı."
    fileData.clear();
    fileData.append(timeString() + "Kaynak başlatıldı.");

    // z control
    if (zControlActive) {
        distanceTarget = distance;
        calcZParameters();
    }

    if (timeControl){
        timeControlCounter = 0;
        permTime = false;
        startTimeControlCount = false;
    } else {
        timeControlCounter = 0;
        permTime = true;
        startTimeControlCount = false;

    }
}

void MainWindow::emergencyButton(){

    emergencyStop = !emergencyStop;

    if (!emergencyStop){
        ui->emergencyButton->setIcon(emergencyOffIcon);
    } else {
        weldCommands.clear();
        ui->plainTextEdit->appendPlainText(timeString() + alarm10);
        ui->emergencyButton->setIcon(emergencyOnIcon);
    }
}

/**/void MainWindow::thinJointButton(){ // NOT USED

    thinJointAlgoActive = !thinJointAlgoActive;

    if ( thinJointAlgoActive ) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);

//        QString str = "İnce Kaynak Ağzı Uygulaması:\nLazerin V şekli teşkil edemediği ince ağızlarda kullanılır.\nLazeri kapatın ve\nkaynak ağzının düzgün karanlık siluet oluşturmasına\ndikkat edin!";
        QString str = "İnce Kaynak Ağzı Uygulaması:\nÇok İnce Ağızlarda Aktif Hale Getiriniz!";
        msgBox.setText(str);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    } else {
        //ui->thinJointButton->setStyleSheet("color: rgb(0, 0, 0)");
    }
}

void MainWindow::edgeDetection(imgProcess *iprocess){

    switch ( edgeDetectionState ) {
        case 0: // NONE
            averaging = true;
            matrixFlag = true;
            break;
        case 1: // SOBEL
            iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
            iprocess->detectEdgeSobel(); // detect edges of the mono image
            iprocess->houghTransformFn(iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight);
            averaging = false;
            matrixFlag = false;
            break;
        case 2: // CANNY 4
            iprocess->prepareCannyArrays();
            iprocess->constructGaussianMatrix(gaussianSize, stdDev);
            for (int i = 0; i < 4 ; i++){
                iprocess->constructValueMatrix( iprocess->imgOrginal, i );
                iprocess->gaussianBlur();
                iprocess->detectEdgeSobelwDirections();
                iprocess->nonMaximumSuppression(cannyThinning);
                iprocess->cannyThresholding(true);
                iprocess->edgeTracing();
                iprocess->assignEdgeMap();
            }
            iprocess->mergeEdgeMaps();

            for (int y = 0; y < iprocess->edgeHeight; y++)
                for (int x = 0; x < iprocess->edgeWidth; x++){
                    if (iprocess->edgeMapMatrix[y][x])
                        iprocess->edgeMatrix[y][x]=255;
                    else
                        iprocess->edgeMatrix[y][x]=0;
                }
            iprocess->houghTransformEdgeMap();
            averaging = false;
            matrixFlag = false;
            break;
        case 3: // CANNY 1
            iprocess->prepareCannyArrays();
            iprocess->constructGaussianMatrix(gaussianSize, stdDev);
            iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );
            iprocess->gaussianBlur();
            iprocess->detectEdgeSobelwDirections();
            iprocess->nonMaximumSuppression(cannyThinning);
            iprocess->cannyThresholding(true);
            iprocess->edgeTracing();

            for (int y = 0; y < iprocess->edgeHeight; y++)  // to display edge image
                for (int x = 0; x < iprocess->edgeWidth; x++){
                    if (iprocess->edgeMapMatrix[y][x])
                        iprocess->edgeMatrix[y][x]=255;
                    else
                        iprocess->edgeMatrix[y][x]=0;
                }
            iprocess->houghTransformEdgeMap();
            matrixFlag = false;
            break;
    }

}

void MainWindow::Algo1(imgProcess *iprocess){
// LASER: mono/edge(matrixFlag) > houghTr > detectLongestSolidLines

    if (edgeDetectionState == 0){
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
    }

    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)

    iprocess->detectLongestSolidLines(averaging, matrixFlag);
}

void MainWindow::Algo2(imgProcess *iprocess){
// LASER: mono/edge(matrixFlag) > houghTr > detectPrimaryVoid

    if (edgeDetectionState == 0){
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
    }

    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)

    iprocess->calcAvgDistAndAngle( houghLineNo );           // calc. avg. distance and theta
    iprocess->calcVoteAvg();                                // avg. value of max voted line(s)

    iprocess->voteThreshold = voteThreshold;                // acceptable vote value low-limit
    iprocess->checkPrimaryLine();                           // is max voted line  above the low-limit?
    iprocess->detectVoidLines();                            // detect void lines on hough lines in MONO image

    iprocess->voidThreshold = voidThreshold;                // void threshold to decide max void as primary
    iprocess->detectPrimaryVoid();                          // decide primary void line & corners/center
}

void MainWindow::Algo3(imgProcess *iprocess){
// woLASER: edge > houghTr > detectMainEdges

    if (edgeDetectionState != 0) {
        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        iprocess->thinCornerNum = mainEdgesNumber;
        iprocess->detectMainEdges(0, false);
        /**/iprocess->angleAvg = iprocess->centerLine.angle;
    } else {
        ui->plainTextEdit->appendPlainText("Bir kenar tespiti algoritması seçilmelidir");
    }
}

void MainWindow::Algo4(imgProcess *iprocess){
// woLASER: value > detectThinJointCenter

    iprocess->constructValueMatrix( iprocess->imgOrginal );
    iprocess->detectThinJointCenter(3, 31);
}

void MainWindow::Algo5(imgProcess *iprocess){
// woLASER: value > contrast matrix > houghTr > calcAvgDistAndAngleOfMajors

    iprocess->constructValueMatrix( iprocess->imgOrginal );
    iprocess->constructContrastMatix(3);
    iprocess->houghTransformContrast();;
    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
    iprocess->calcAvgDistAndAngleOfMajors(0.30);    // calc. avg. distance and theta
    iprocess->detectContrastCenter();
}

void MainWindow::Algo6(imgProcess *iprocess){
// woLASER: canny1 > houghTr > detectMainEdges > detectMainEdgesSolidLine

    if (edgeDetectionState == 3) {

        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        iprocess->thinCornerNum = mainEdgesNumber;
        iprocess->detectMainEdgesSolidLine(1, false);
        solidLineLength = iprocess->solidLineLength;

    } else {
        ui->plainTextEdit->appendPlainText("Canny1 kenar tespiti algoritması seçilmelidir");
    }
}

void MainWindow::processImage(bool deleteObject){

    //if ( !imageGetter->imageList.isEmpty() ){
    if (lastData->image->format() != QImage::Format_Invalid) {

        if (applyCameraEnhancements) {
//            targetArea = imageFileChanged.copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image
            targetArea = imageFileChanged.copy( offsetXCam, offsetYCam, frameWidthCam, frameHeightCam );    // take target image
        } else {
//            targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image
            targetArea = lastData->image->copy( offsetXCam, offsetYCam, frameWidthCam, frameHeightCam );    // take target image
        }

        if (!iProcessThread->isRunning() && iProcessThread->ready){
            iProcessThread->targetArea = targetArea.copy(0,0,targetArea.width(), targetArea.height());

            if (twoPassWelding) {
                thinJointAlgoActive = true;     // without laser - VERTICAL SEARCH
                if (firstPass)
                    algorithmType = 4;          // Algo6() LINE DETECTION WITH MAIN EDGES
                else
                    algorithmType = 1;          // Algo3() MAIN EDGES
            }

            iProcessThread->start();
        }
    }
}

void MainWindow::imageProcessingCompleted(){

    // if center of track is not an error, append dev. to trend data list OR append error code to list
    if (iProcessThread->iprocess->detected){

//                error = iprocess->trackCenterX - (frameWidth/2);
        error = iProcessThread->iprocess->trackCenterX - (frameWidthCam/2);
        deviationData.append(error);

        //if (controlInitiated) {                }

        jointWidth = abs(iProcessThread->iprocess->rightCornerX - iProcessThread->iprocess->leftCornerX) + 1;

        if (trackOn){
            errorTotal += abs(error);
            processCount++;
            if (abs(error) > errorMax) errorMax = abs(error);
        }
    } else {
        deviationData.append(eCodeDev);     // eCodeDev: error code
    }


    // assign plc commands using dev. data findings
    if (iProcessThread->iprocess->detected){

        detectionError = false;

        if (jointWidthControlActive && (jointWidth > maxJointWidth || jointWidth < minJointWidth || jointWidth == 1) ) {
            cmdState = _CMD_CENTER;     //ui->plainTextEdit->appendPlainText("error");
        } else
        if (!controlInitiated) {
            int index = deviationData.size() - 1;

            if (deviationData[index] >= errorLimitCam ){
                cmdState = _CMD_RIGHT;
            } else
            if (deviationData[index] <= errorLimitNegCam){
                cmdState = _CMD_LEFT;
            } else
            if ((cmdStatePrev2 == _CMD_LEFT) && (deviationData[index] >= errorStopLimitNegCam)){
                cmdState = _CMD_CENTER;
            } else
            if ((cmdStatePrev2 == _CMD_RIGHT) && (deviationData[index] <= errorStopLimitCam)){
                cmdState = _CMD_CENTER;
            } else
            if ((cmdStatePrev2 != _CMD_RIGHT) && (cmdStatePrev2 != _CMD_LEFT)){
                cmdState = _CMD_CENTER;
            }

            if (controlOn) {
                if (!focusCheckBeforeControl) {
                    plcCommands();
                } else if (focusCheckBeforeControl && camFocusState) {
                    plcCommands();
                }

            }
        }

        cmdStatePrev = cmdState;
        cmdStatePrev2 = cmdState;

    } else {
        //if (deviationData[index] != eCodeDev){
        cmdState = cmdStatePrev2;
        detectionError = true;
    }


    if (iProcessThread->iprocess->detected && controlInitiated){
        goX = false;    //dont send command to plc when control is initiated firstly

        initialJointWidth = abs(iProcessThread->iprocess->rightCornerX - iProcessThread->iprocess->leftCornerX) + 1;
        maxJointWidth = initialJointWidth * 1.2;
        minJointWidth = initialJointWidth * 0.8;
        //ui->plainTextEdit->appendPlainText( QString::number(minJointWidth)+ ", " + QString::number(initialJointWidth) + ", " + QString::number(maxJointWidth) );

        if (alignGuide2TrackCenter) {

            int _offsetX = offsetX + error * mapFactorX;

            if ( _offsetX >= (offsetXmin+10) && ((_offsetX + frameWidth) <= (offsetXmax - 10)) ){
                offsetXpos += error * mapFactorX;
                offsetXCam += error;
                //qDebug() << "aligned offsetX/min: " << _offsetX << "-" << offsetXmin;
            } else {
                controlButton();
                showSetupError();
                //qDebug() << "not aligned offsetX/min: " << _offsetX << "-" << offsetXmin;
            }

            repaintGuide();
            //alignGuide2TrackCenter = false;
        }
        controlInitiated = false;
    }

    //qDebug() << "+";

    if (!analyzeAction) {
        delete iProcessThread->iprocess;
        iProcessThread->ready = true;
    } else {
        analyzeEndTime = timeSystem.getSystemTimeMsec();

        int processElapsed = analyzeEndTime - analyzeStartTime;
        analyzeDialog *_analyzeDialog = new analyzeDialog(iProcessThread->iprocess, processElapsed, this);
        _analyzeDialog->show();
    }
}

void MainWindow::histAnalysisCompleted(){

    qDebug() << iProcessThread->histAreaStat;
    iProcessThread->ready = true;
}

int MainWindow::timeDifference(int first, int last){

    if (last >= first)
        return (last - first);
    else
        return (1000 - first + last);
}

int MainWindow::timeDifference(QTime first, QTime second){

    int firstMS = first.hour()*3600*1000 + first.minute()*60*1000 +first.second()*1000 + first.msec();
    int secondMS = second.hour()*3600*1000 + second.minute()*60*1000 +second.second()*1000 + second.msec();
    if (secondMS >= firstMS)
        return (secondMS - firstMS);
    else
        return (86400000 - firstMS + secondMS);
}

QString MainWindow::timeString(){

    return (QDateTime::currentDateTime().toString("hh:mm:ss") + " ");
}

int MainWindow::calcTotalMsec(int hour, int min, int second, int msec){

    return ( (hour * 3600 + min * 60 + second) * 1000 + msec );
}

void MainWindow::addAxis(){

    scene->addLine(sceneCenterX, 0, sceneCenterX, sceneRect.height(), penAxis);

    scene->addLine(sceneCenterX - errorLimit, 0, sceneCenterX - errorLimit, sceneRect.height(), penLimit);
    scene->addLine((sceneCenterX - 1) + errorLimit, 0, (sceneCenterX - 1) + errorLimit, sceneRect.height(), penLimit);

    if (errorStopLimitLineVisible && errorStopLimit > 1 && errorStopLimit < errorLimit){
        scene->addLine(sceneCenterX - errorStopLimit, 0, sceneCenterX - errorStopLimit, sceneRect.height(), penStopLimit);
        scene->addLine((sceneCenterX - 1) + errorStopLimit, 0, (sceneCenterX - 1) + errorStopLimit, sceneRect.height(), penStopLimit);
    }
}

void MainWindow::clearTrack(){

    scene->clear();
    addAxis();
    ui->trackView->show();
}

void MainWindow::drawTrack(){

    int x,y, xPrev, yPrev;

    if (deviationData.size() >= deviationDataSize){
        clearTrack();
        deviationData.removeFirst();
    }

    for (int i=1; i<deviationData.size(); i++){
        x = sceneCenterX + deviationData[i]*mapFactorX;
        y = i*yRes;
        xPrev = sceneCenterX + deviationData[i-1]*mapFactorX;
        yPrev = (i-1)*yRes;
        scene->addLine(xPrev,yPrev,x,y,penTrack);
    }
    ui->trackView->show();
}

void MainWindow::target2Left(){

    float step = 5;
    if (abs(error * mapFactorX)<=20) step = 1;

    offsetXpos -= step;
    offsetX -= step;

    //offsetXpos -= 5;
    //offsetXCam -= floor(step/mapFactorX+0.5);
    offsetXCam -= step/mapFactorX;

    if ( offsetX < (offsetXmin+10) || ((offsetX + frameWidth) > (offsetXmax - 10)) ){//    if ( offsetX < 5 || ((offsetX + frameWidth) > (imageWidth - 5)) ){
        offsetXpos += step;
        offsetX += step;
        //offsetXCam += floor(step/mapFactorX+0.5);
        offsetXCam += step/mapFactorX;
    }

    //ui->plainTextEdit->appendPlainText("offsetX: " + QString::number(offsetX) + " offsetXpos: " + QString::number(offsetXpos) + " offsetXCam: " + QString::number(offsetXCam));
    repaintGuide();
}

void MainWindow::target2Right(){

    float step = 5;
    if (abs(error * mapFactorX)<=20) step = 1;

    offsetXpos += step;
    offsetX += step;

    //offsetXCam += floor(step/mapFactorX+0.5);
    offsetXCam += step/mapFactorX;

    if ( offsetX < (offsetXmin+10) || ((offsetX + frameWidth) > (offsetXmax - 10)) ){
        offsetXpos -= step;
        offsetX -= step;
        //offsetXCam -= floor(step/mapFactorX+0.5);
        offsetXCam -= step/mapFactorX;
    }

    //ui->plainTextEdit->appendPlainText("offsetX: " + QString::number(offsetX) + " offsetXpos: " + QString::number(offsetXpos) + " offsetXCam: " + QString::number(offsetXCam));
    repaintGuide();
}

void MainWindow::repaintGuide(){

    // update & show/hide guide
    offsetX = (imageWidth - frameWidth)/2 + offsetXpos;
    offsetY = (imageHeight - frameHeight)/2;

    guideFrameRect.setX(imageFrameRect.x() + offsetX);
    guideFrameRect.setWidth(frameWidth + 4);
    ui->guideFrame->setGeometry(guideFrameRect);
    //ui->guideFrame->update();

    gfBoxRect.setY(offsetY - 2);
    gfBoxRect.setWidth(frameWidth + 4);     // 4: total line edges thickness
    gfBoxRect.setHeight(frameHeight + 4);   // 4: total line edges thickness
    ui->frame->setGeometry(gfBoxRect);

    gfLineHorRect.setWidth(frameWidth);
    ui->guideLineHorz->setGeometry(gfLineHorRect);

    ui->guideLineVert->setGeometry(frameWidth/2 + 1, gfLineVerRect.y(), gfLineVerRect.width(), gfLineVerRect.height());
    //centerX = ui->guideLineVert->x();   // x coor. of center vert. guide line

/*
    ui->guideLineVert->setGeometry(frameWidth/2 + 1, gfLineVerRect.y(), gfLineVerRect.width(), gfLineVerRect.height());
    centerX = ui->guideLineVert->x();   // x coor. of center vert. guide line
    ui->guideTolLeft->setGeometry(centerX - errorLimit + 1, gfTolLeftRect.y(), gfTolLeftRect.width(), gfTolLeftRect.height());
    ui->guideTolRight->setGeometry(centerX + errorLimit, gfTolRightRect.y(), gfTolRightRect.width(), gfTolRightRect.height());
    gfBoxRect.setWidth(frameWidth + 4);   // 4: total line edges thickness
    ui->frame->setGeometry(gfBoxRect);
    gfLineHorRect.setWidth(frameWidth);
    ui->guideLineHorz->setGeometry(gfLineHorRect);
*/
    ui->guideFrame->setVisible(showGuide);
}

void MainWindow::repaintDevTrend(){

    // updata dev. trend for error limit value changes
    if (!trackOn) clearTrack();
    if (deviationData.size() != 0) drawTrack();
}

void MainWindow::calcZParameters(){

    distanceUpStart = distanceTarget + distanceUpTol;
    distanceUpStop = distanceTarget + distanceUpTol * zStartStopRate;

    distanceDownStart = distanceTarget - distanceDownTol;
    distanceDownStop = distanceTarget - distanceDownTol * zStartStopRate;
}

void MainWindow::showSettingsForm(){

    if (settingsPWDOK){
        settingsForm *settingsGui = new settingsForm(this);
        settingsGui->getParameters();
        settingsGui->show();
        settingsPWDOK = false;
    } else {
        pwdDialog *pwdGui = new pwdDialog(0,this);
        pwdGui->show();
    }
}

void MainWindow::showSetupForm(){

    if (setupPWDOK){
        setupForm *setupGui = new setupForm(this);
        setupGui->getParameters();
        setupGui->show();
        setupPWDOK = false;
    } else {
        pwdDialog *pwdGui = new pwdDialog(1,this);
        pwdGui->show();
    }
}

void MainWindow::infoButton(){

    infoForm *infoGui = new infoForm(this);
    infoGui->show();
}

void MainWindow::exitButton(){

    exitDialog *exitGui = new exitDialog(this);
    exitGui->show();
}

void MainWindow::clearMsgBoxButton(){

    ui->plainTextEdit->clear();
}

void MainWindow::helpButton(){

    helpDialog *helpGui = new helpDialog(this);
    helpGui->show();
}

void MainWindow::showLicenseDialog(){

    licenseDialog *licenseGui = new licenseDialog(lic, this);
    licenseGui->show();
}

void MainWindow::showReport(){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Rapor Aç"),
                                                     "rapor/",
                                                     tr("Rapor (*.txt)"));
    if (fileName != ""){

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

        QString message = "";
        while (!file.atEnd()) {
             message += file.readLine();
        }
        file.close();

        reportDialog *reportGui = new reportDialog(message ,this);
        reportGui->show();
    }
}

void MainWindow::readSettings(){

    if (QFile::exists(INIFILENAME)){
        settings->beginGroup("kamera");
            camStreamType = settings->value("camstrm", _CAM_STREAM_TYPE).toInt();
            urlCam.setUrl(settings->value("urlCam", _URL_CAM).toString());
            urlCamStream.setUrl(settings->value("urlCamStrm", _URL_CAM_STREAM).toString());
            playCamonBoot = settings->value("play", _PLAY_CAM_ONBOOT).toBool();
            fpsTarget = settings->value("fps", _FPS).toInt();
            focusCheckBeforeControl = settings->value("fcschk", _FOCUS_CHECK).toBool();
            autoFocusBeforeControl = settings->value("autofcs", _AUTO_FOCUS).toBool();
        settings->endGroup();

        settings->beginGroup("plc");
            urlPLC.setUrl(settings->value("urlPLC", _URL_PLC).toString());
            plcType = settings->value("type", _PLC_TYPE).toInt();
            DB_NO = settings->value("dbno", _DB_NO).toInt();
            BYTE_NO = settings->value("byte", _BYTE_NO).toInt();
            connectRequestedonBoot = settings->value("pcon", _PLC_CONN_ONBOOT).toBool();
            //PLCSIM = settings->value("plcsim", _PLCSIM).toBool();
            controlDelay = 0;   //settings->value("ctd", _CONTROL_DELAY).toInt();
            hardControlStart = settings->value("hard", _HARD_START).toBool();
            machineNo = settings->value("makine", _MACHINE_NO).toInt();
                if (machineNo < 1 || machineNo > 8) machineNo = 1;
            readMachineStatus = settings->value("readstat", _READ_MACHINE_STAT).toBool();
            readDistance = settings->value("readdist", _READ_DISTANCE).toBool();
            readWeldSeam = settings->value("readseam", _READ_WELD_SEAM).toBool();
            dynamicAlgo = settings->value("dyna", _DYNAMIC_ALGO).toBool();
            timeControl = settings->value("tctl", _TIME_CONTROL).toBool();
            timeLimit = settings->value("tlmt", _TIME_LIMIT).toInt();
            lineDetection = settings->value("ldtc", _LINE_DETECT).toBool();
            lineScoreLimit = settings->value("scor", _LINE_SCORE).toInt();
        settings->endGroup();

        settings->beginGroup("ipro");
            iprocessInterval = settings->value("ipi", _IPROCESS_INT).toInt();
            frameWidth = settings->value("frw", _FRAME_WIDTH).toInt();
            frameHeight = settings->value("frh", _FRAME_HEIGHT).toInt();
            thetaMinVerLine = settings->value("tmnv", _THETA_MIN_VER).toInt();
            thetaMaxVerLine = settings->value("tmxv", _THETA_MAX_VER).toInt();
            thetaMinHorLine = settings->value("tmnh", _THETA_MIN_HOR).toInt();
            thetaMaxHorLine = settings->value("tmxh", _THETA_MAX_HOR).toInt();
            thetaStep = settings->value("tst", _THETA_STEP).toFloat();
            subImageProcessingSwitch = settings->value("subi", _SUB_IPROCESS).toBool();
            subImageProcessingType = settings->value("subt", _SUB_TYPE).toInt();
            thetaMinSub = settings->value("tmns", _THETA_MIN_SUB).toInt();
            thetaMaxSub = settings->value("tmxs", _THETA_MAX_SUB).toInt();
            thetaStepSub = settings->value("tsts", _THETA_STEP_SUB).toFloat();
            houghLineNo = settings->value("hln", _HOUGH_LINE_NO).toInt();
            voteThreshold = settings->value("vth", _VOTE_THRESHOLD).toInt();
            voidThreshold = settings->value("vdth", _VOID_THRESHOLD).toInt();
            errorLimit = settings->value("elm", _ERROR_LIMIT).toInt();
                errorLimitNeg = -1 * errorLimit;

            errorStopScale = settings->value("esc", _ERROR_SCALE).toFloat();
                if (errorStopScale > 1.0) errorStopScale = 1.0;
                errorStopLimit = errorLimit * errorStopScale;
                errorStopLimitNeg = -1 * errorStopLimit;

            thinJointAlgoActive = settings->value("thin", _THIN_JOINT).toBool();
            if (thinJointAlgoActive) {
                thetaMin = thetaMinVerLine;
                thetaMax = thetaMaxVerLine;
            } else {
                thetaMin = thetaMinHorLine;
                thetaMax = thetaMaxHorLine;
            }

            jointWidthControlActive = settings->value("widthctrl", _WIDTH_CONTROL).toBool();

            applyCameraEnhancements = settings->value("camenhance", _CAM_ENHANCE).toBool();
            brightnessVal = settings->value("brightness", _BRIGHTNESS).toInt();
            contrastVal = settings->value("contrast", _CONTRAST).toInt();
            gammaVal = settings->value("gamma", _GAMMA).toInt();
            gaussianSize = settings->value("gausssize", _GAUSS_SIZE).toInt();
            stdDev = settings->value("gaussdev", _GAUSS_DEV).toFloat();
            cannyThinning = settings->value("cannythin", _CANNY_THIN).toBool();
            edgeDetectionState = settings->value("edgealgo", _EDGE_ALGO).toInt();
            algorithmType = settings->value("algotype", _ALGO_TYPE).toInt();
            mainEdgesNumber = settings->value("medgeno", _MAIN_EDGE_NO).toInt();
            alignGuide2TrackCenter = settings->value("align", _ALIGN).toBool();

            maFilterKernelSize = settings->value("mafs", _MAF_KERNEL).toInt();
            histogramAngleThreshold = settings->value("hang", _HIST_ANGLE).toInt();
            colorMatrix = settings->value("colormat", _COLOR_MATRIX).toBool();
            lenRateThr = settings->value("lrate", _LEN_RATIO).toFloat();
            bandWidthMin = settings->value("bwidm", _BAND_WIDTH_MIN).toFloat();
            bandCenterMax = settings->value("bcenm", _BAND_CENTER_MAX).toFloat();
            histAreaNo = settings->value("harno", _HIST_AREA_NO).toInt();

            twoPassWelding = settings->value("twopass", _2_PASS_WELD).toBool();
            autoDetect2ndPass = settings->value("detpass", _DETECT_2ND_PASS).toBool();

        settings->endGroup();

        settings->beginGroup("oth");
            yResIndex = settings->value("yresi", _YRES_ARRAY_INDEX).toInt();
                yRes = yResArray[yResIndex];
            title = settings->value("title", _TITLE).toString();

            zControlActive = settings->value("zctrl", _Z_CONTROL).toBool();
            distanceUpTol = settings->value("uptol", _UP_TOL).toFloat();
            distanceDownTol = settings->value("dwtol", _DOWN_TOL).toFloat();

        settings->endGroup();

    } else {    // assign default values if file not exist
        camStreamType = _CAM_STREAM_TYPE;
        urlCam.setUrl(_URL_CAM);
        urlCamStream.setUrl(_URL_CAM_STREAM);
        playCamonBoot = _PLAY_CAM_ONBOOT;
        fpsTarget = _FPS;
        focusCheckBeforeControl = _FOCUS_CHECK;
        autoFocusBeforeControl = _AUTO_FOCUS;

        urlPLC.setUrl(_URL_PLC);
        plcType = _PLC_TYPE;
        DB_NO = _DB_NO;
        BYTE_NO = _BYTE_NO;
        connectRequestedonBoot = _PLC_CONN_ONBOOT;
        //PLCSIM = _PLCSIM;
        controlDelay = _CONTROL_DELAY;
        hardControlStart = _HARD_START;
        machineNo = _MACHINE_NO;
        readMachineStatus = _READ_MACHINE_STAT;
        readDistance = _READ_DISTANCE;
        readWeldSeam = _READ_WELD_SEAM;
        dynamicAlgo = _DYNAMIC_ALGO;
        timeControl = _TIME_CONTROL;
        timeLimit = _TIME_LIMIT;
        lineDetection = _LINE_DETECT;
        lineScoreLimit = _LINE_SCORE;

        iprocessInterval = _IPROCESS_INT;
        frameWidth = _FRAME_WIDTH;
        frameHeight = _FRAME_HEIGHT;
        thetaMinVerLine = _THETA_MIN_VER;
        thetaMaxVerLine = _THETA_MAX_VER;
        thetaMinHorLine = _THETA_MIN_HOR;
        thetaMaxHorLine = _THETA_MAX_HOR;

        thetaStep = _THETA_STEP;
        subImageProcessingSwitch = _SUB_IPROCESS;
        subImageProcessingType = _SUB_TYPE;
        thetaMinSub = _THETA_MIN_SUB;
        thetaMaxSub = _THETA_MAX_SUB;
        thetaStepSub = _THETA_STEP_SUB;
        houghLineNo = _HOUGH_LINE_NO;
        voteThreshold = _VOTE_THRESHOLD;
        voidThreshold = _VOID_THRESHOLD;
        errorLimit = _ERROR_LIMIT;
            errorLimitNeg = -1 * errorLimit;

        errorStopScale = _ERROR_SCALE;
            errorStopLimit = errorLimit * errorStopScale;
            errorStopLimitNeg = -1 * errorStopLimit;

        thinJointAlgoActive = _THIN_JOINT;
        if (thinJointAlgoActive) {
            thetaMin = thetaMinVerLine;
            thetaMax = thetaMaxVerLine;
        } else {
            thetaMin = thetaMinHorLine;
            thetaMax = thetaMaxHorLine;
        }

        jointWidthControlActive = _WIDTH_CONTROL;

        applyCameraEnhancements = _CAM_ENHANCE;
        brightnessVal = _BRIGHTNESS;
        contrastVal = _CONTRAST;
        gammaVal = _GAMMA;
        gaussianSize = _GAUSS_SIZE;
        stdDev = _GAUSS_DEV;
        cannyThinning = _CANNY_THIN;
        edgeDetectionState = _EDGE_ALGO;
        algorithmType = _ALGO_TYPE;
        mainEdgesNumber = _MAIN_EDGE_NO;
        alignGuide2TrackCenter = _ALIGN;

        maFilterKernelSize = _MAF_KERNEL;
        histogramAngleThreshold = _HIST_ANGLE;
        colorMatrix = _COLOR_MATRIX;
        lenRateThr = _LEN_RATIO;
        bandWidthMin = _BAND_WIDTH_MIN;
        bandCenterMax = _BAND_CENTER_MAX;
        histAreaNo = _HIST_AREA_NO;

        twoPassWelding = _2_PASS_WELD;
        autoDetect2ndPass = _DETECT_2ND_PASS;

        yResIndex = _YRES_ARRAY_INDEX;
            yRes = yResArray[yResIndex];
        title = _TITLE;

        zControlActive = _Z_CONTROL;
        distanceUpTol = _UP_TOL;
        distanceDownTol = _DOWN_TOL;

        statusMessage = "ini dosyası bulunamadı";
    }
}

void MainWindow::writeSettings(){

    settings->beginGroup("kamera");
        settings->setValue("camstrm", QString::number(camStreamType));
        settings->setValue("urlCam", urlCam.toString());
        settings->setValue("urlCamStrm", urlCamStream.toString());
        QVariant play(playCamonBoot);
            settings->setValue("play", play.toString());
        settings->setValue("fps", QString::number(fpsTarget));
        QVariant fcschk(focusCheckBeforeControl);
            settings->setValue("fcschk", fcschk.toString());
        QVariant autofcs(autoFocusBeforeControl);
            settings->setValue("autofcs", autofcs.toString());
    settings->endGroup();

    settings->beginGroup("plc");
        settings->setValue("urlPLC", urlPLC.toString());
        settings->setValue("type", QString::number(plcType));
        settings->setValue("dbno", QString::number(DB_NO));
        settings->setValue("byte", QString::number(BYTE_NO));
        QVariant pcon(connectRequestedonBoot);
            settings->setValue("pcon", pcon.toString());
        //QVariant plcsim(PLCSIM);
            //settings->setValue("plcsim", plcsim.toString());
        settings->setValue("ctd", QString::number(controlDelay));
        QVariant hardstart(hardControlStart);
            settings->setValue("hard", hardstart.toString());
        settings->setValue("makine", QString::number(machineNo));
        QVariant readstat(readMachineStatus);
            settings->setValue("readstat", readstat.toString());
        QVariant readdist(readDistance);
            settings->setValue("readdist", readdist.toString());
        QVariant readseam(readWeldSeam);
            settings->setValue("readseam", readseam.toString());
        QVariant dyna(dynamicAlgo);
            settings->setValue("dyna", dyna.toString());
        QVariant tctl(timeControl);
            settings->setValue("tctl", tctl.toString());
        settings->setValue("tlmt", QString::number(timeLimit));
        QVariant ldtc(lineDetection);
            settings->setValue("ldtc", ldtc.toString());
        settings->setValue("scor", QString::number(lineScoreLimit));

    settings->endGroup();

    settings->beginGroup("ipro");
        settings->setValue("ipi", QString::number(iprocessInterval));
        settings->setValue("frw", QString::number(frameWidth));
        settings->setValue("frh", QString::number(frameHeight));
        settings->setValue("tmnv", QString::number(thetaMinVerLine));
        settings->setValue("tmxv", QString::number(thetaMaxVerLine));
        settings->setValue("tmnh", QString::number(thetaMinHorLine));
        settings->setValue("tmxh", QString::number(thetaMaxHorLine));
        settings->setValue("tst", QString::number(thetaStep));
        QVariant subimagesw(subImageProcessingSwitch);
            settings->setValue("subi", subimagesw.toString());
        settings->setValue("subt", QString::number(subImageProcessingType));
        settings->setValue("tmns", QString::number(thetaMinSub));
        settings->setValue("tmxs", QString::number(thetaMaxSub));
        settings->setValue("tsts", QString::number(thetaStepSub));
        settings->setValue("hln", QString::number(houghLineNo));
        settings->setValue("vth", QString::number(voteThreshold));
        settings->setValue("vdth", QString::number(voidThreshold));
        settings->setValue("elm", QString::number(errorLimit));
        settings->setValue("esc", QString::number(errorStopScale));

        QVariant thinjointsw(thinJointAlgoActive);
            settings->setValue("thin", thinjointsw.toString());

        QVariant widthsw(jointWidthControlActive);
            settings->setValue("widthctrl", widthsw.toString());

        QVariant camenhancesw(applyCameraEnhancements);
            settings->setValue("camenhance", camenhancesw.toString());

        settings->setValue("brightness", QString::number(brightnessVal));
        settings->setValue("contrast", QString::number(contrastVal));
        settings->setValue("gamma", QString::number(gammaVal));
        settings->setValue("gausssize", QString::number(gaussianSize));
        settings->setValue("gaussdev", QString::number(stdDev));

        QVariant cannythinsw(cannyThinning);
            settings->setValue("cannythin", cannythinsw.toString());

        settings->setValue("edgealgo", QString::number(edgeDetectionState));
        settings->setValue("algotype", QString::number(algorithmType));
        settings->setValue("medgeno", QString::number(mainEdgesNumber));

        QVariant alignsw(alignGuide2TrackCenter);
            settings->setValue("align", alignsw.toString());

       settings->setValue("mafs", QString::number(maFilterKernelSize));
       settings->setValue("hang", QString::number(histogramAngleThreshold));
       QVariant colormatsw(colorMatrix);
           settings->setValue("colormat", colormatsw.toString());
       settings->setValue("lrate", QString::number(lenRateThr));
       settings->setValue("bwidm", QString::number(bandWidthMin));
       settings->setValue("bcenm", QString::number(bandCenterMax));
       settings->setValue("harno", QString::number(histAreaNo));

       QVariant twopasssw(twoPassWelding);
           settings->setValue("twopass", twopasssw.toString());
       QVariant detpasssw(autoDetect2ndPass);
           settings->setValue("detpass", detpasssw.toString());

    settings->endGroup();

    settings->beginGroup("oth");
        settings->setValue("yresi", QString::number(yResIndex));
        settings->setValue("title", title);

        QVariant zcontrolsw(zControlActive);
            settings->setValue("zctrl", zcontrolsw.toString());

        settings->setValue("uptol", QString::number(distanceUpTol));
        settings->setValue("dwtol", QString::number(distanceDownTol));
    settings->endGroup();

    settings->sync();
}

bool MainWindow::writeReport(){

    QString fname = "rapor/rapor_" + QDateTime::currentDateTime().toString("yyyy_MMM_dd") + ".txt";
    QFile file(fname);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    for (int i = 0; i < fileData.size(); i++){
        out << fileData[i] << "\n";
    }
    file.close();
    return true;
}

void MainWindow::closeEvent(QCloseEvent*){

    qApp->quit();
}

QString MainWindow::calcImageParametes(QImage img, bool info){

    camImageWidth = img.width();
    camImageHeight = img.height();
    QString message = "Res: " + QString::number(camImageWidth) + "x" + QString::number(camImageHeight);

    if (camImageHeight != 0) {

        aspectRatio = ((float)camImageWidth )/ camImageHeight;
        message += " asr: " + QString::number(aspectRatio, 'f', 2);

        if (aspectRatio  > 1.333)   // landscape
            imgOrientation = false;
        else                        // portrait
            imgOrientation = true;

        if (info && imgOrientation) message += " portrait\n"; else message += " landscape\n";

        if (!imgOrientation) {  // landscape
            mapFactorWidth = 1;
            mapWidth = imageWidth;
            mapFactorHeight = aspectRatioGUI / aspectRatio;
            mapHeight = imageHeight * mapFactorHeight;
        } else {                // portrait
            mapFactorWidth = aspectRatio / aspectRatioGUI;
            mapWidth = imageWidth * mapFactorWidth;
            mapFactorHeight = 1;
            mapHeight = imageHeight;
        }
        mapFactorX = ((float)mapWidth) / camImageWidth;

        offsetXmin = (imageWidth - mapWidth)/2;
        offsetXmax = offsetXmin + mapWidth;

        frameWidthMax = mapWidth * frameWidthRatioMax;
        frameHeightMax = mapHeight * frameHeightRatioMax;
        if (frameWidth > frameWidthMax) frameWidth = 2 * (frameWidthMax/2); // to obtain even number
        if (frameHeight > frameHeightMax) frameHeight = 2 * (frameHeightMax/2); // to obtain even number

        frameWidthCam = ((float)camImageWidth * frameWidth) / mapWidth;
        frameHeightCam = ((float)camImageHeight * frameHeight) / mapHeight;
        offsetXCam = (camImageWidth - frameWidthCam) / 2;
        offsetYCam = (camImageHeight - frameHeightCam) / 2;


        if(info) {
            message += "mapFactorWidth: " + QString::number(mapFactorWidth, 'f', 2) + "\n" + "mapFactorHeight: " + QString::number(mapFactorHeight, 'f', 2) + "\n";
            message += "mapWidth: " + QString::number(mapWidth) + "\n" + "mapHeight: " + QString::number(mapHeight) + "\n";
            message += "frameWidthMax: " + QString::number(frameWidthMax) + "\n" + "frameHeightMax: " + QString::number(frameHeightMax) + "\n";
            message += "frameWidthCam: " + QString::number(frameWidthCam) + "\n" + "frameHeightCam: " + QString::number(frameHeightCam) + "\n";
            message += "frameWidth: " + QString::number(frameWidth) + "\n" + "frameHeight: " + QString::number(frameHeight) + "\n";
            message += "offsetXCam: " + QString::number(offsetXCam) + "\n" + "offsetYCam: " + QString::number(offsetYCam) + "\n";
            message += "mapFactorX: " + QString::number(mapFactorX, 'f', 2) + "\n";
        }

        message += "Frame Cost: " + QString("%L2").arg(frameWidthCam * frameHeightCam) + " px.";

        if (mapFactorX != 0) {
            errorLimitCam = errorLimit / mapFactorX;
            errorLimitNegCam = errorLimitNeg / mapFactorX;
            errorStopLimitCam = errorStopLimit / mapFactorX;
            errorStopLimitNegCam = errorStopLimitNeg / mapFactorX;
        }


        repaintGuide();
    } else
        aspectRatio = 0;

    //ui->plainTextEdit->appendPlainText(message);
    return message;
}

void MainWindow::playCam(){

    /*switch ( camStreamType ) {
        case 1: // JPEG
            break;
        case 0: // STREAM
            break;
    }*/

    if (play && !pause){


        if (focusValCalc) {

            QImage _targetArea;

            if (focusROI == 0) // full image
                _targetArea = lastData->image->copy();
            else if (focusROI == 1)    // target frame
                _targetArea = lastData->image->copy( offsetXCam, offsetYCam, frameWidthCam, frameHeightCam );
            else {   // target frame portion
                int shiftX =  (frameWidthCam - frameWidthCam*1.0/focusROI) / 2.0;
                int shiftY =  (frameHeightCam - frameHeightCam*1.0/focusROI) / 2.0;
                _targetArea = lastData->image->copy( offsetXCam+shiftX, offsetYCam+shiftY, frameWidthCam/focusROI, frameHeightCam/focusROI );
            }

            double val;
            switch ( focusValueAlgo ) {
                case 0:
                    val = fourierTransform(&_targetArea, false)[2];
                    break;
                case 1:
                    {
                    QImage x = _targetArea.convertToFormat(QImage::Format_Grayscale8);
                    imgProcess z(x, x.width(), x.height());
                    z.constructValueMatrix(x);
                    z.normalizeValueMatrix(255.0);
                    val = z.calcEntropyMatrix(5);
                    }
                    break;
                case 2:
                    val = calcFocusValueLaplacian(_targetArea);
                    break;

            }

            focusVal += val;
            //qDebug() << focusValCalcNo << "   " << y;
            focusValCalcNo++;

            if (focusValCalcNo == focusValCalcLimit){
                focusVal /= focusValCalcLimit;
                focusValCalc = false;
                focusValCalcNo = 0;

                if (doAutoFocus_Algo) {
                    focusValListY.append( focusVal );
                    focusValListX.append( AF->pos*10000 / 10000.0 );

                    //qDebug() << "getImageFromStream doAutoFocus_Algo focus value: " << focusVal;
                    ui->plainTextEdit->appendPlainText(QString::number(AF->j)+" pos: " + QString::number(round(AF->pos*10000) / 10000.0)+
                                                       " mean: "+QString::number(focusValListY.last(),'f',2));

                    AF->condition.wakeAll();
                } else {
                    //qDebug() << "getImageFromStream !doAutoFocus_Algo focus value: " << focusVal;
                }
                emit focusValueCalculated(focusVal);
            }
        }

        //if (!imageGetter->imageList.isEmpty()){  // if any image is get

        bool show = true;

        switch ( camStreamType ) {
            case 1: // JPEG
                {
                if (!imageGetter->imageList.isEmpty()){  // if any image is get
                    int size = imageGetter->imageList.length();  // image buffer size
                    //bool show = true;

                    lastData = imageGetter->imageList[size-1];    // get last data
                    frameNo = lastData->requestId.toUInt();       // request id of net. data

                    // check for current shown data is not a late from prev data
                    if (size >= 2){     // to check min 2 data must exist

                        for (int i = size - 2; i >= 0; i--)

                            if (imageGetter->imageList[i]->shown){       // if past images in the buffer were shown on display
                                prevData = imageGetter->imageList[i];

                                // if prev shown data's request id is higher than the cuurents data's request id
                                // then current data is late & DONT SHOW IT on display
                                if (frameNo <= prevData->requestId.toUInt()){
                                    lateFrame++;
                                    show = false;
                                }
                                break;
                            }
                    }
                }
                }
                break;
            case 0: // STREAM
                break;
        }

        // show current valid image. valid: not late from previous (<show>) for imageGetter
        if (!lastData->shown && show){

            if (getCamImageProperties) {
                calcImageParametes(*lastData->image, false);
                getCamImageProperties = false;
                repaintGuide();
            }

            if (applyCameraEnhancements) {
                QImage step1 = changeBrightness(*lastData->image, brightnessVal);
                QImage step2 = changeContrast(step1, contrastVal);
                imageFileChanged = changeGamma(step2, gammaVal);
                //ui->imageFrame->setPixmap( QPixmap::fromImage( imageFileChanged.scaled(imageFileChanged.width(), imageFileChanged.height(), Qt::KeepAspectRatio) ));
                ui->imageFrame->setPixmap( QPixmap::fromImage( imageFileChanged.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio) ));

            } else {
                ui->imageFrame->setPixmap( QPixmap::fromImage( lastData->image->scaled(imageWidth, imageHeight, Qt::KeepAspectRatio) ));
                //ui->imageFrame->setPixmap( QPixmap::fromImage( *lastData->image ));
            }
            ui->imageFrame->show();
            ui->guideFrame->raise();     // if guide is shown, suppress it
            lastData->shown = true;      // mark last data was SHOWN on display
            fpsReal++;

            if ( camStreamType == 0 ) {
                timeDelay = timeSystem.getSystemTimeMsec() - firstTimeTick;
            }

            if ( camStreamType == 1 ) {
                // calculate (display time - request time) delay in msec
                int displayTime = timeSystem.getSystemTimeMsec();
                int requestTime = calcTotalMsec(lastData->requestHour.toInt(), lastData->requestMinute.toInt(), lastData->requestSecond.toInt(), lastData->requestMSecond.toInt());
                timeDelay = displayTime - requestTime;
                timeDelayTotal += timeDelay;  // overall delay
            }


            if ( captureVideo ) { // VIDEO SAVE

                if (videoFrameCount == 0)
                    folderName = savePath + QDateTime::currentDateTime().toString("yyMMdd_hhmmss") + "/";

                int index = videoFrameCount % threadVideoSave->bufferLength;

                if (applyCameraEnhancements) {
                    //videoList[videoFrameCount] = imageFileChanged.copy();
                    videoList[index] = imageFileChanged.copy();
                } else {
                    //videoList[videoFrameCount] = lastData->image->copy();
                    videoList[index] = lastData->image->copy();
                }
                videoFrameCount++;

                if (videoFrameCount % threadVideoSave->bufferLength == 0 || videoFrameCount >= videoFrameSize) {

                    if (!threadVideoSave->isRunning()){
                        int lastSize = threadVideoSave->bufferLength;
                        threadVideoSave->lastSave = false;

                        if (videoFrameCount >= videoFrameSize){
                            lastSize = index+1;
                            threadVideoSave->lastSave = true;
                            captureVideo = false;
                        }

                        threadVideoSave->saveSize = lastSize;
                        for (int i=0;i<lastSize;i++)
                            threadVideoSave->buffer[i] = videoList[i];
                        threadVideoSave->count++;

                        threadVideoSave->start();
                    }
                }

                //ui->plainTextEdit->appendPlainText(QString::number(videoFrameCount));
            }

            // if joint is tracked for some interval
            if (trackOn && (fpsReal % iprocessInterval) == 0 ){
                processImage();  // detect deviation

                if ( !lineDetection ) {
                    if (deviationData.size() >= 2) drawTrack();   // draw deviation trend
                }
            }
        }
        //} //if (!imageGetter->imageList.isEmpty())
    }
}

void MainWindow::timeEdit(){

    timeLimit = ui->timeEdit->text().toInt();

    if (timeLimit < 20) {
        timeLimit = 20;
        ui->timeEdit->setText("20");
    }

}

void MainWindow::videoButton(){

    videoFrameSize = videoDuration * fpsRealLast;
    //ui->plainTextEdit->appendPlainText(QString::number(videoFrameSize)+"-"+QString::number(videoDuration)+"-"+QString::number(fpsRealLast));
    //videoList = new QImage[videoFrameSize];
    videoList = new QImage[threadVideoSave->bufferLength];
    //videoList = (QImage *)malloc(videoFrameSize * sizeof(QImage));
    videoFrameCount = 0;
    ui->videoButton->setEnabled(false);
    captureVideo = true;

    ui->videoButton->setIcon(videoSaveDisabled);
}

void MainWindow::saveFinished(){

    ui->plainTextEdit->appendPlainText(timeString() + "Video kaydedildi!");
    ui->videoButton->setEnabled(true);
    ui->videoButton->setIcon(videoSaveEnabled);
}

void MainWindow::testEdit(){

    distanceRaw = ui->testEdit->text().toInt();
}

void MainWindow::testButton(){

    //mak_aktif_now = !mak_aktif_now;
    //qDebug() << "Mak Aktif:" << mak_aktif_now;
    /*
    int *array = new int[5];
    for (int i = 0; i < 5; i++) array[i] = i;
    //QList<range> list;
    //findLocalMinimum(array, 5, list);
    */

    //testFlag = true;
    /*
    testFlag2 = !testFlag2;
    if (testFlag2){
        secondTimeTick = timeSystem.getSystemTimeMsec();
        testFloat1 = ui->testEdit->text().toFloat();
        testInt1 = abs(testFloat1 - testFloat2) * 3000;
        QTimer::singleShot(testInt1, this, SLOT(testSlot()));
        camApi->apiDahuaSetFocusPos(testFloat1);
        testFloat2 = testFloat1;
    } else {
        qDebug() << (timeSystem.getSystemTimeMsec() - secondTimeTick);
    }
    */


    //doAutoFocusAlgo_2Step(0.25, 0.75, 3, 8, false, 0.40, 0.55);
    //doAutoFocusAlgo_2Step(0.2, 0.8, 3, 6, true);
    //doAutoFocusAlgo_Deep(0,1,5,4);
    //doAutoFocusAlgo_Local();
    //doAutoFocusAlgo_2StepStart();


    /*
    QList<double> x1;
    x1.append(0.4500);
    x1.append(0.4800);
    x1.append(0.5100);
    x1.append(0.5400);
    x1.append(0.5700);
    x1.append(0.6000);
    x1.append(0.6300);
    x1.append(0.66);
    x1.append(0.69);
    x1.append(0.72);
    x1.append(0.75);
    QList<double> y1;
    y1.append(5860.86);
    y1.append(6712.87);
    y1.append(10196.77);
    y1.append(22098.76);
    y1.append(44638.93);
    y1.append(28472.61);
    y1.append(12381.37);
    y1.append(9225.13);
    y1.append(7032.68);
    y1.append(7048.07);
    y1.append(6451.81);
    qDebug() << findCurveFitting(x1, y1, 10);
*/
}

void MainWindow::testSlot(){
    ui->plainTextEdit->appendPlainText("*");
}

void MainWindow::on_setupButton_clicked(){

    showSetupForm();
}

MainWindow::~MainWindow(){

    //**if (threadPLCControl->plc->plcInteract && !PLCSIM)
      //**  threadPLCControl->disconnect();

    if (plc->isRunning() && !PLCSIM)
        plc->stopThread();

    //    delete settings;
    delete ui;
}

void MainWindow::plcConnection(bool stat){

    plcConnected = stat;

    if (!plcInteractPrev && plcConnected){
        ui->plcStatus->setIcon(plcOnlineIcon);
        permPLC = true;
        ui->plainTextEdit->appendPlainText(timeString() + MESSAGE6);
    } else if (plcInteractPrev && !plcConnected){
        ui->plcStatus->setIcon(plcOfflineIcon);
        permPLC = false;
        ui->plainTextEdit->appendPlainText(timeString() + MESSAGE3);
    }

    plcInteractPrev = plcConnected;
}

void MainWindow::plcCheck(){

    if (!plcConnected && !plc->isRunning()) {
        //qDebug() << "check " << ++count << " new run";
        plc->start();
    } else {
        /*
        if (plcConnected && plc->isRunning())
            qDebug() << "check " << ++count << " connected & thread is running";
        else if (plcConnected)
            qDebug() << "check " << ++count << " connected";
        else if (plc->isRunning())
            qDebug() << "check " << ++count << " thread is running";
        */
    }
}

int MainWindow::getBitofByte(unsigned char byte, int bitNo){

    return (byte >> bitNo) & 0x01;
}

void MainWindow::plcReadings(){

    if ( readDistance ) {

        distanceRaw = PlcWORDtoInt(DB.distance);
        distance = 300 - ((distanceRaw * 1.0) / 27648.0) * 220.0;
        ui->labelDistance->setText( QString::number(distance, 'f', 1) );
    }


    if (readMachineStatus){
        unsigned char *byteptr = &DB.bitDatas;
        unsigned char byte = *byteptr;
        //qDebug()<<getBitofByte(byte,0)<<getBitofByte(byte,1)<<getBitofByte(byte,2)<<getBitofByte(byte,3)<<getBitofByte(byte,4)<<getBitofByte(byte,5)<<getBitofByte(byte,6)<<getBitofByte(byte,7);

        if (getBitofByte(byte, 0) == 1) mak_aktif_now = true; else mak_aktif_now = false;

        if (mak_aktif_now && !mak_aktif_old) {
            ui->plainTextEdit->appendPlainText("Makine çalışıyor");

            if (hardControlStart)
                controlInitiated = true;

            if (timeControl) {
                timeControlCounter = 0;
                startTimeControlCount = true;
            }
        }

        if (!mak_aktif_now && mak_aktif_old) {
            ui->plainTextEdit->appendPlainText("Makine durdu");
        }
        //else alignGuide2TrackCenter = false;
        mak_aktif_old = mak_aktif_now;
    }

}

void MainWindow::plcCommands(){

    //if (controlOn){

        if (cmdState != cmdStatePrev) {

            int commandByte = 0;

            if (cmdState == _CMD_RIGHT){
                commandByte +=1;
                ui->cmdStatus->setIcon(cmd2LeftIcon);
            }
            else if (cmdState == _CMD_LEFT) {
                ui->cmdStatus->setIcon(cmd2RightIcon);
                commandByte +=2;
            }
            else if (cmdState == _CMD_CENTER) {
                ui->cmdStatus->setIcon(QIcon());
            }

            plc->writeByte(0,commandByte);

        }
    //}
}

void MainWindow::doAutoFocus(){
     /* API COMMAND
      * CAM BUILTIN AUTOFOCUS ALGORITHM
      */
    if (!cameraDownStatus && !camApi->busy){
        camApi->apiDahuaAutoFocus();
    }
}

void MainWindow::doAutoFocusAlgo(double start, double end, int sampleNo, int depth){
     /* ALGO BASED AUTOFOCUS
      * recursive autofocus
      * scan number: depth
      * start/end points are autodetermined
      */
    autoFocusPassLimit = depth;
    autoFocusPassNo = 1;

    AF = new autoFocusThread(start, end, sampleNo, depth);
    connect(AF, SIGNAL(setFocusPos(float)), this, SLOT(setFocusPos(float)));
    connect(AF, SIGNAL(iterationFinished()), this, SLOT(iterationFinished()));
    focusValListY.clear();
    focusValListX.clear();
    doAutoFocus_Algo = true;
    AF->start();
}

void MainWindow::doAutoFocusAlgo_2StepStart(){
    /* ALGO BASED AUTOFOCUS
     * get motor position before starting 2 step autofocus algorithm
     */
    autoFocusAlgo2Step = true;
    checkFocusStatus();     // get current motor position // SLOT>> focusingActionState(bool)
}

void MainWindow::doAutoFocusAlgo_2Step(double start1, double end1, int sampleNo1, int sampleNo2, bool start_end_2nd_auto, double start2, double end2){
    /* ALGO BASED AUTOFOCUS
     * 2 step algorithm
     * 1st step for global scan
     * 2nd step for narrower area; manual or autodetermined start/end points
     * if there is a problem at scan results, run 3rd scan for more narrower area
     */
    autoFocusAlgo2Step = true;
    autoFocusAlgo2Step_Auto = start_end_2nd_auto;
    autoFocusPassLimit = 2;
    autoFocusPassNo = 1;
    extraIteration = true;

    if ( !start_end_2nd_auto ) {
        sampleStart = start2;
        sampleEnd = end2;
    }

    sampleNo = sampleNo2;

    AF = new autoFocusThread(start1, end1, sampleNo1, 1);
    connect(AF, SIGNAL(setFocusPos(float)), this, SLOT(setFocusPos(float)));
    connect(AF, SIGNAL(iterationFinished()), this, SLOT(iterationFinished()));
    focusValListY.clear();
    focusValListX.clear();
    doAutoFocus_Algo = true;
    AF->start();
}

void MainWindow::doAutoFocusAlgo_Local_Start(double halfDistance){
    /* ALGO BASED AUTOFOCUS
     * start local autofocus algorithm
     */
    double start = camFocusPos - halfDistance;
    if (start < 0) start = 0;
    double end = camFocusPos + halfDistance;
    if (end > 1) end = 1;
    AF = new autoFocusThread(start, end, 6, 1);
    connect(AF, SIGNAL(setFocusPos(float)), this, SLOT(setFocusPos(float)));
    connect(AF, SIGNAL(iterationFinished()), this, SLOT(iterationFinished()));
    focusValListY.clear();
    focusValListX.clear();
    doAutoFocus_Algo = true;
    AF->start();    // SLOT>> iterationFinished();
}

void MainWindow::doAutoFocusAlgo_Local() {
    /* ALGO BASED AUTOFOCUS
     * check close neighborhood of current position
     * if focus value is not good after scan, run 2step algorithm
     */
    doAutoFocus_Algo = false;
    autoFocusAlgoLocal = true;
    autoFocusAlgoLocal_Start = true;
    checkFocusStatus();     // get current motor position // SLOT>> focusingActionState(bool)
}

void MainWindow::checkFocusState(){
    /* API COMMAND
     * focus state: "in focus" or "out of focus"
     */
    if (!cameraDownStatus && !camApi->busy){
        camApi->apiDahuaGetFocusState();
    }
}

void MainWindow::focusState(bool state){
    /* focus state
     * "in focus" or "out of focus"
     */
    camFocusState = state;

    if (camFocusState) {
        ui->plainTextEdit->appendPlainText(timeString() + alarm12);
        if (controlOn)
            startControl();
    } else {
        ui->plainTextEdit->appendPlainText(timeString() + alarm13);
        if (camDoAutoFocus) {
            if (autoFocusPassNo < autoFocusPassLimit) {
                ui->plainTextEdit->appendPlainText(timeString() + message6);
                timerLock = true;
                autoFocusPassNo++;
                doAutoFocus();
                timerAutoFocus->start(100);
            } else {
                ui->plainTextEdit->appendPlainText(timeString() + alarm14);
            }
        } else {
            if (controlOn) {
                ui->plainTextEdit->appendPlainText(timeString() + alarm14);
                controlOn = false;
            }
        }
    }
}

void MainWindow::checkFocusStatus(){
    /* API COMMAND
     * motor position,
     * motor steps,
     * focussing action; normal: no movement / autofocus: motor in motion
     */
    if (!cameraDownStatus && !camApi->busy){
        camApi->apiDahuaGetFocusStatus();
    }
}

void MainWindow::focusingActionState(bool state){
    /* focussing action; normal: no movement / autofocus: motor in motion
     * motor position
     * from status check
     */
    camFocusingActionState = state;

    if (!timerAutoFocus->isActive()) {      // ONLY STATUS INFO REQUESTED

        camFocusPos = camApi->focusPos;
        qDebug() << "camFocusPos: " << camFocusPos;

        if (autoFocusAlgoLocal) {
            calcFocusValue(2, 1, 5);
        }

        if (autoFocusAlgo2Step) {

            double start = camFocusPos/2;
            double end = camFocusPos + (1-camFocusPos)/2;
            doAutoFocusAlgo_2Step(start, end, 3, 6, true);
        }
    } else {    // AUTO FOCUS PROCESS CHECK

        if (!camFocusingActionState) {

            if (camDoAutoFocus) {

                timerAutoFocus->stop();
                ui->plainTextEdit->appendPlainText("Oto fokus işlemi tamamlandı...");
                QTimer::singleShot(4000, this, SLOT(checkFocusState()));
                //checkFocusState();
            }

            if (doAutoFocus_Algo) {

                timerAutoFocus->stop();
                //ui->plainTextEdit->appendPlainText("motor durdu...");
                QTimer::singleShot(500, this, SLOT(calcFocusValueSlot()));
            }
        }
    }
}

void MainWindow::checkAutoFocusingState(){  // timerAutoFocus slot

    if (!cameraDownStatus && !camApi->busy){
        if (timerLock){
            camApi->apiDahuaGetFocusStatus();
            timerLock = false;
            if (camDoAutoFocus) ui->plainTextEdit->appendPlainText("");
        } else if (camFocusingActionState) {
            camApi->apiDahuaGetFocusStatus();
        }

        if (camDoAutoFocus) {
            QTextCursor text_cursor = QTextCursor(ui->plainTextEdit->document());
            text_cursor.movePosition(QTextCursor::End);
            text_cursor.insertText("*");
        }
    }
}

void MainWindow::setFocusPos(float pos){
    /* API COMMAND
     * send focus motor position
     */
    if (!cameraDownStatus && !camApi->busy){
        camApi->apiDahuaSetFocusPos(pos);

        if (doAutoFocus_Algo) {
            timerLock = true;
            timerAutoFocus->start(100);
        }
    }
}

void MainWindow::apiRequestCompleted(){
    if (doAutoFocus_Algo) {
        //QTimer::singleShot(2000, this, SLOT(calcFocusValue()));
    }
}

float* MainWindow::fourierTransform(QImage *img, bool save){

    cv::Mat imgData0(img->height(), img->width(), CV_8UC3, (uchar*)img->bits(), img->bytesPerLine());
    cv::Mat imgData; // deep copy just in case (my lack of knowledge with open cv)
    cv::Mat imgDataBlur;
    cv::GaussianBlur(imgData0, imgDataBlur, cv::Size(5,5), 0, 0);
    cvtColor(imgDataBlur, imgData, CV_RGB2GRAY);
    //cvtColor(imgData0, imgData, CV_RGB2GRAY);

    // [expand]
    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize( imgData.rows );
    int n = cv::getOptimalDFTSize( imgData.cols ); // on the border add zero values
    copyMakeBorder(imgData, padded, 0, m - imgData.rows, 0, n - imgData.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // [complex_and_real]
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complexI;
    cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    // [dft]
    cv::dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    // [magnitude]
    cv::split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    cv::magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    cv::Mat magI = planes[0];

    // [log]
    //magI += cv::Scalar::all(1);                    // switch to logarithmic scale
    //cv::log(magI, magI);

    // [crop_rearrange]
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

    if (false) {
        ofstream fout0("fftQ0.csv");
        if(!fout0) { cout<<"File Not Opened"<<endl;  return 0;   }
        for(int i=0; i<q0.rows; i++) {
            for(int j=0; j<q0.cols; j++)
                fout0 << QString::number(q0.at<float>(i,j),'f',0).toStdString() <<",";
            fout0<<endl;
        }
        fout0.close();
        ofstream fout1("fftQ1.csv");
        if(!fout1) { cout<<"File Not Opened"<<endl;  return 0;   }
        for(int i=0; i<q1.rows; i++) {
            for(int j=0; j<q1.cols; j++)
                fout1 << QString::number(q1.at<float>(i,j),'f',0).toStdString() <<",";
            fout1<<endl;
        }
        fout1.close();
        ofstream fout2("fftQ2.csv");
        if(!fout2) { cout<<"File Not Opened"<<endl;  return 0;   }
        for(int i=0; i<q2.rows; i++) {
            for(int j=0; j<q2.cols; j++)
                fout2 << QString::number(q2.at<float>(i,j),'f',0).toStdString() <<",";
            fout2<<endl;
        }
        fout2.close();
        ofstream fout3("fftQ3.csv");
        if(!fout3) { cout<<"File Not Opened"<<endl;  return 0;   }
        for(int i=0; i<q3.rows; i++) {
            for(int j=0; j<q3.cols; j++)
                fout3 << QString::number(q3.at<float>(i,j),'f',0).toStdString() <<",";
            fout3<<endl;
        }
        fout3.close();
    }

    //rearrange
    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    // [normalize]
    //cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX); // Transform the matrix with float values into a viewable image form (float between values 0 and 1).
    // [normalize]

    if (save) {
        ofstream fout("fft.csv");
        if(!fout) { cout<<"File Not Opened"<<endl;  return 0; }
        for(int i=0; i<magI.rows; i++) {
            for(int j=0; j<magI.cols; j++) {
    //                        fout<<magI.at<float>(i,j)<<",";
                fout << QString::number(magI.at<float>(i,j),'f',3).toStdString() <<",";
            }
            fout<<endl;
        }
        fout.close();
        /*
        cv::FileStorage file("fft.xml", cv::FileStorage::WRITE);
        file << "matname" << magI;
        file.release();
        */
    }

    double min, max;
    cv::minMaxLoc(magI, &min, &max);
    cv::Scalar scal = cv::mean(magI);
    float mean = scal.val[0];
    //ui->plainTextEdit->appendPlainText("min: " + QString::number(min,'f',3) + " max: " + QString::number(max) + " mean: " + QString::number(mean));
    /*
    int countLow = 0,countHigh = 0;
    for(int i=0; i<magI.rows; i++)
        for(int j=0; j<magI.cols; j++)
            if ( magI.at<float>(i,j) <= mean)
                countLow++;
            else
                countHigh++;

    ui->plainTextEdit->appendPlainText("low#: " + QString::number(countLow) + " high#: " + QString::number(countHigh));
    */
    float *array = new float[3];
    array[0] = min;
    array[1] = max;
    array[2] = mean;

    return array;
}

void MainWindow::calcFocusValueSlot(){
    calcFocusValue(0, 1, 5);
}

void MainWindow::calcFocusValue(int algo, int roi, int number){

    focusValCalcLimit = number;
    focusValCalcNo = 0;
    focusVal = 0;
    focusValCalc = true;
    focusROI = roi;
    focusValueAlgo = algo;

    //getFFT();
    //getFuzzyEntropy();
}

void MainWindow::focusValueCalculatedSlot(double val){

    if (controlOn && focusCheckBeforeControl) {
        if (val > laplacianGoodValue) {
            ui->plainTextEdit->appendPlainText(timeString() + alarm12);
            startControl();
        } else {
            ui->plainTextEdit->appendPlainText(timeString() + alarm13);
            ui->plainTextEdit->appendPlainText(timeString() + alarm14);
            controlButton();
        }
    }

    if (autoFocusAfterFocusCheck){
        autoFocusAfterFocusCheck = false;

        focusVal0 = val;

        if (focusVal0 > laplacianGoodValue)
            ui->plainTextEdit->appendPlainText("Fokus değeri iyi");
        else {
            ui->plainTextEdit->appendPlainText("Fokus değeri iyi değil");
            ui->plainTextEdit->appendPlainText("Oto fokus yapılacak");
            if (focusVal0 > laplacianBypassLocalAlgoValue)
                doAutoFocusAlgo_Local();
            else
                doAutoFocusAlgo_2StepStart();
        }

    } else {

        if (autoFocusAlgoLocal && autoFocusAlgoLocal_Start) {
            focusVal0 = val;
            autoFocusAlgoLocal_Start = false;
            doAutoFocusAlgo_Local_Start(localFocusHalfDist);
        }
    }
}

double MainWindow::calcFocusValueLaplacian(int roi){

    QImage _targetArea;

    if (roi == 0) // full image
        _targetArea = lastData->image->copy();
    else if (roi == 1)    // target frame
        _targetArea = lastData->image->copy( offsetXCam, offsetYCam, frameWidthCam, frameHeightCam );    // take target image
    else {   // target frame portion
        int shiftX =  (frameWidthCam - frameWidthCam*1.0/roi) / 2.0;
        int shiftY =  (frameHeightCam - frameHeightCam*1.0/roi) / 2.0;
        _targetArea = lastData->image->copy( offsetXCam+shiftX, offsetYCam+shiftY, frameWidthCam/roi, frameHeightCam/roi );
    }
    cv::Mat imgData0(_targetArea.height(), _targetArea.width(), CV_8UC3, (uchar*)_targetArea.bits(), _targetArea.bytesPerLine());
    cv::Mat imgData;
    cvtColor(imgData0, imgData, CV_RGB2GRAY);
    cv::Mat imgDataLap;
    cv::Laplacian(imgData, imgDataLap,3);
    cv::Mat imgDataLapAbs;
    cv::convertScaleAbs( imgDataLap, imgDataLapAbs );

    double min, max = 0;
    cv::minMaxLoc(imgDataLapAbs, &min, &max);

    return max;
}

double MainWindow::calcFocusValueLaplacian(QImage img){

    cv::Mat imgData0(img.height(), img.width(), CV_8UC3, (uchar*)img.bits(), img.bytesPerLine());
    cv::Mat imgData;
    cvtColor(imgData0, imgData, CV_RGB2GRAY);
    cv::Mat imgDataLap;
    cv::Laplacian(imgData, imgDataLap,3);
    cv::Mat imgDataLapAbs;
    cv::convertScaleAbs( imgDataLap, imgDataLapAbs );

    double min, max = 0;
    cv::minMaxLoc(imgDataLapAbs, &min, &max);

    return max;
}

void MainWindow::getFFT(){
    if (!cameraDownStatus){
        focusValListY.append( fourierTransform(lastData->image, false)[2] );
        focusValListX.append( AF->pos*10000 / 10000.0 );

        ui->plainTextEdit->appendPlainText(QString::number(AF->j)+" pos: " + QString::number(round(AF->pos*10000) / 10000.0)+
                                           " mean: "+QString::number(focusValListY.last(),'f',2));

        AF->condition.wakeAll();
    }
}

void MainWindow::getFuzzyEntropy(){
    if (!cameraDownStatus){
        QImage x = lastData->image->convertToFormat(QImage::Format_Grayscale8);
        //x.save("gray.jpg");

        imgProcess z(x,x.width(),x.height());
        z.constructValueMatrix(x);
        //z.saveMatrix(z.valueMatrix,x.width(),x.height(),"gray.csv");
        z.normalizeValueMatrix(255.0);
        //z.saveMatrix(z.valueMatrixNorm,x.width(),x.height(),"grayn.csv");
        focusValListY.append( z.calcEntropyMatrix(5) );
        focusValListX.append( AF->pos*10000 / 10000.0 );
        //z.saveMatrix(z.fuzzyEntropyMatrix,z.FEM_width,z.FEM_height,"entropy.csv");
        ui->plainTextEdit->appendPlainText(QString::number(AF->j)+" pos: " + QString::number(round(AF->pos*10000) / 10000.0)+
                                           " mean: "+QString::number(focusValListY.last(),'f',2));

        AF->condition.wakeAll();
    }
}

double* MainWindow::calcFittingPrms(QList<double> x, QList<double> y, QList<double> refY, bool &stat, bool ref){

    QString debugStr = "";
    stat = false;
    double *prm = new double[6];
    for (int i=0; i<6; i++) prm[i] = -12345;

    int samples = x.size();
    double N = 0, A = 0, B = 0, C = 0, D = 0, Y1 = 0, Y2 = 0, Y3 = 0;
    double y2, lnY;
    for (int i = 0; i < samples; i++){
        y2 = pow( y.at(i),2 );
        N += y2;
        A += x.at(i) * y2;
        B += pow( x.at(i),2 ) * y2;
        C += pow( x.at(i),3 ) * y2;
        D += pow( x.at(i),4 ) * y2;
        if (ref)
            lnY = log( refY.at(i) );
        else
            lnY = log( y.at(i) );
        Y1 += y2 * lnY;
        Y2 += x.at(i) * y2 * lnY;
        Y3 += pow( x.at(i),2 ) * y2 * lnY;
    }

    double bDiv = pow(C*N - A*B, 2) - (B*N - pow(A, 2)) * (D*N - pow(B, 2));

    double b = 0;
    if ( bDiv != 0 )
        prm[1] = b = ( (C*N - A*B) * (Y3*N - B*Y1) - (D*N - pow(B, 2)) * (Y2*N - A*Y1) ) / bDiv;
    else
        return prm;

    double c = 0;
    if ( (C*N - A*B) != 0 )
        prm[2] = c = ( Y2*N - A*Y1 - (B*N - pow(A, 2)) * b ) / (C*N - A*B);
    else
        return prm;

    double a = 0;
    if ( N != 0)
        prm[0] = a = ( Y1 - A*b - B*c ) / N;
    else
        return prm;

    double u = 0;
    double sigma = 0;
    double magA = 0;
    if ( c != 0 ) {
        prm[3] = u = -1 * b / (2 * c);
        if ( c < 0 )
            prm[4] = sigma = sqrt( (-1 / (2 * c)) );
        else
            return prm;

        prm[5] = magA = exp( a - pow(b,2) / (4*c) );
    } else
        return prm;

    stat = true;
    return prm;
}

void MainWindow::iterationFinished(){
    bool debugmsg = false;

    double max = focusValListY.at(0);
    int index = 0;
    for (int i=1; i<focusValListY.size(); i++){
        if (focusValListY.at(i) > max){
            index = i;
            max = focusValListY.at(i);
        }
    }

    double pos = round((index*AF->step + AF->sampleStart)*10000) / 10000.0;
    if (debugmsg) ui->plainTextEdit->appendPlainText("posOfMax: " + QString::number(pos, 'f', 12));

    double distLeft = round((pos - AF->sampleStart)*10000) / 10000.0;
    double distRight = round((AF->sampleEnd - pos)*10000) / 10000.0;
    //qDebug() << "pass# " << autoFocusPassNo;// << " distL:" << distLeft << " distR:" << distRight;

    double newDistHalf = round(qMax(abs(distLeft), abs(distRight))*10000/2) / 10000.0;

    double newdistLeft;
    double newdistRight;

    if (index > 0 && index < (focusValListY.size()-1) ) {    // if not at extreme points

        if ((pos - newDistHalf) >= AF->sampleStart)
            newdistLeft = newDistHalf;
        else
            newdistLeft = distLeft;

        if ((pos + newDistHalf) <= AF->sampleEnd)
            newdistRight = newDistHalf;
        else
            newdistRight = distRight;
    } else {

        if ((pos - newDistHalf) >= 0)
            newdistLeft = newDistHalf ;

        if ((pos + newDistHalf) <= 1)
            newdistRight = newDistHalf;

        if (autoFocusAlgo2Step) extraIterationForce = true;

    }


    sampleStartPrev = AF->sampleStart;
    sampleEndPrev = AF->sampleEnd;
    double start = AF->sampleStart = pos - newdistLeft;
    double end = AF->sampleEnd = pos + newdistRight;

    //AF->sampleStart = pos - newdistLeft;
    //AF->sampleEnd = pos + newdistRight;

    /*if (autoFocusPassNo == 1) {
        sampleStart0 = AF->sampleStart;
        sampleEnd0 = AF->sampleEnd;
    }*/

    if (debugmsg) {
        ui->plainTextEdit->appendPlainText("newDistHalf: " + QString::number(newDistHalf, 'f', 12));
        ui->plainTextEdit->appendPlainText("newdistLeft: " + QString::number(newdistLeft, 'f', 12));
        ui->plainTextEdit->appendPlainText("newdistRight: " + QString::number(newdistRight, 'f', 12));
        ui->plainTextEdit->appendPlainText("newDist: " + QString::number(AF->sampleEnd-AF->sampleStart, 'f', 12));
        ui->plainTextEdit->appendPlainText("sampleStart: " + QString::number(AF->sampleStart, 'f', 12));
        ui->plainTextEdit->appendPlainText("sampleEnd: " + QString::number(AF->sampleEnd, 'f', 12));
    }

    bestFocusPos = findCurveFitting(focusValListX, focusValListY, 10);
    ui->plainTextEdit->appendPlainText("bestPos: " + QString::number(bestFocusPos, 'f', 12));

    //double start = pos - sigma;
    //double end = pos + sigma;

    if ( autoFocusAlgo2Step ) {

        AF->stopCmd = true;
        AF->condition.wakeAll();
        //AF->terminate();
        AF->wait();
        delete AF;

        if ( extraIterationForce || (autoFocusPassNo == 2 && extraIteration &&
             ( bestFocusPos < (sampleStartPrev + sigma/2) || bestFocusPos > (sampleEndPrev - sigma/2) ||
               bestFocusPos > 1 || bestFocusPos < 0 || sigma == -12345) )){
            autoFocusPassLimit = 3;
        }

        if ( autoFocusPassNo < autoFocusPassLimit ) {
            autoFocusPassNo++;

            if (bestFocusPos < 1 && bestFocusPos > 0 && autoFocusPassNo == 3) {
                start = pos - localFocusHalfDist;
                end = pos + localFocusHalfDist;
            }
            if ( autoFocusAlgo2Step_Auto ) {
                AF = new autoFocusThread( start, end, sampleNo, 1 );
            } else
                AF = new autoFocusThread( sampleStart, sampleEnd, sampleNo, 1 );

            connect(AF, SIGNAL(setFocusPos(float)), this, SLOT(setFocusPos(float)));
            connect(AF, SIGNAL(iterationFinished()), this, SLOT(iterationFinished()));
            focusValListY.clear();
            focusValListX.clear();
            AF->start();
        } else {
            autoFocusAlgo2Step = false;
            autoFocusAlgo2Step_Auto = false;
            doAutoFocus_Algo = false;
            autoFocusPassNo = 0;
            extraIteration = false;
            extraIterationForce = false;
            if ( bestFocusPos < 1 && bestFocusPos > 0 && sigma != -12345 ){
                setFocusPos(bestFocusPos);
            } else
                setFocusPos(camFocusPos);

        }

    } else
    if ( autoFocusAlgoLocal ) {

        autoFocusAlgoLocal = false;
        doAutoFocus_Algo = false;

        if ( ( bestFocusPos < (sampleStartPrev + sigma) || bestFocusPos > (sampleEndPrev - sigma) ||
               bestFocusPos > 1 || bestFocusPos < 0 || sigma == -12345) ){
            doAutoFocusAlgo_2StepStart();
        } else {
            setFocusPos(bestFocusPos);
        }

        //calcFocusValue(2, 1, 5);

    } else {

        if ( autoFocusPassNo < autoFocusPassLimit ) {
            focusValListY.clear();
            focusValListX.clear();
            autoFocusPassNo++;
            AF->condition.wakeAll();
        } else {
            doAutoFocus_Algo = false;
            autoFocusPassNo = 0;

            AF->stopCmd = true;
            AF->condition.wakeAll();
            //AF->terminate();
            AF->wait();
            delete AF;
            //qDebug() << "terminated";

        }
    }



}

double MainWindow::findCurveFitting(QList<double> x1, QList<double> y1, int iterNo){

    bool debugmsg0 = false;
    bool debugmsg = false;
    QString debugStr = "";

    // x Xnorm
    QList<double> x;
    double minX = *std::min_element(x1.begin(), x1.end());
    debugStr = "x-xmin: ";
    for (int i=0; i<x1.size(); i++){
        x.append( x1.at(i) - minX );
        debugStr += QString::number(x.at(i), 'f', 4) + " ";
    }
    if (debugmsg0) ui->plainTextEdit->appendPlainText(debugStr); debugStr = "";
    focusValX_offset = minX;

    // y Ynorm
    QList<double> y;
    double maxY = *std::max_element(y1.begin(), y1.end());
    debugStr = "y/ymax: ";
    if (maxY != 0)
        for (int i=0; i<y1.size(); i++){
            y.append( y1.at(i) / (maxY * 1.05) );
            debugStr += QString::number(y.at(i), 'f', 4) + " ";
        }
    else
        return -1;
    if (debugmsg0) ui->plainTextEdit->appendPlainText(debugStr); debugStr = "";

    // y Ynorm -YnormMin
    double minY = *std::min_element(y.begin(), y.end());
    debugStr = "y-ymin: ";
    for (int i=0; i<y.size(); i++){
        y[i] -= minY*0.95;
        debugStr += QString::number(y.at(i), 'f', 4) + " ";
    }
    if (debugmsg0) ui->plainTextEdit->appendPlainText(debugStr); debugStr = "";

    bool flag;
    QList<double> yNew;
    double *prm, _y, error, errorprev, errorD, uPrev, u, sigmaPrev;
    QString str;

    for (int j=0; j<iterNo; j++) {

        if (j==0)
            prm = calcFittingPrms(x,y,y,flag,false);
        else
            prm = calcFittingPrms(x,yNew,y,flag,true);

        if (flag){

            if (debugmsg) {
                str = "prms: ";
                for (int i=0; i<6; i++) str += QString::number(prm[i],'f',6) + " ";
            }

            yNew.clear();
            error = 0;
            for (int i=0; i<x1.size(); i++) {
                _y = exp( prm[0]+prm[1]*x[i]+prm[2]*pow(x[i],2) );
                yNew.append(_y);
                error += pow( y[i]-_y, 2 );
            }
            error /= x1.size();
            error = sqrt(error);

            str = "err: " + QString::number(error, 'f', 6);
            ui->plainTextEdit->appendPlainText(str);

            if (error < 0.01){
                u = prm[3];
                sigma = prm[4];
                break;
            }

            if (j > 0) {
                errorD = abs(error - errorprev);

                if (error > errorprev) {
                    u = uPrev;
                    sigma = sigmaPrev;
                    break;
                } else if (errorD < 0.000001) {
                    u = prm[3];
                    sigma = prm[4];
                    break;
                }

            }

            errorprev = error;
            uPrev = prm[3];
            sigmaPrev = prm[4];

        } else {
            ui->plainTextEdit->appendPlainText("Fokus değerlerinde hata");
            return -1;
        }
    }

    return ( focusValX_offset + u );
}


void MainWindow::on_passOneButton_clicked(){
    firstPass = true;
    ui->passOneButton->setStyleSheet("background-color: lime");
    ui->passTwoButton->setStyleSheet("background-color: #F0F0F0");
}

void MainWindow::on_passTwoButton_clicked(){
    firstPass = false;
    ui->passOneButton->setStyleSheet("background-color: #F0F0F0");
    ui->passTwoButton->setStyleSheet("background-color: lime");
}

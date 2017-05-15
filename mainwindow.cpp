#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCodec>
#include "windows.h"

#include "licensedialog.h"
#include "analyzedialog.h"
#include "helpdialog.h"
#include "exitdialog.h"

//#include "../_Modules/Algo/localMinimum.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    rectScreen = QApplication::desktop()->geometry();

    // only title on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    //flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);


    ui->clearMsgBoxButton->hide();  //  not used now
    //ui->plainTextEdit->appendPlainText(QString::number(rectScreen.width())+"x"+QString::number(rectScreen.height()));
    ui->labelLogoCustomer->hide();
    ui->labelTime->hide();
    ui->emergencyButton->hide();
    ui->zControlButton->hide();
    ui->testEdit->hide();
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

    if ( thinJointAlgoActive ) {
        ui->thinJointButton->setStyleSheet("color: rgb(255, 0, 0)");
    } else
        ui->thinJointButton->setStyleSheet("color: rgb(0, 0, 0)");

    // orginal and target image parameters
    imageWidth = 640;   //image->width();
    imageHeight = 480;  //image->height();

    imageFrameRect = ui->imageFrame->geometry();
    guideFrameRect = ui->guideFrame->geometry();
    gfBoxRect = ui->frame->geometry();
    gfLineHorRect = ui->guideLineHorz->geometry();
    gfLineVerRect = ui->guideLineVert->geometry();
    //gfTolLeftRect = ui->guideTolLeft->geometry();
    //gfTolRightRect = ui->guideTolRight->geometry();
    offsetXpos = 0;
    alignGuide2TrackCenter = false;

    showGuide = true;       // show guide initially
    repaintGuide();

    sceneRect = ui->trackView->geometry();
    sceneCenterX = (sceneRect.width() - 4)/ 2;  // 4: total border thickness

    // play video controls
    play = playCamonBoot;
    if (play) playButton();
    pause = false;
    fpsRequest = 0;
    fpsReal = 0;
    frameInterval = 1000 / fpsTarget;
    frameNo = 0;
    lateFrame = 0;
    timeDelayTotal = 0;
    timeDelayAvg = 0;
    alarmCameraDownLock = false;
    connect(this, SIGNAL(cameraDown()), this, SLOT(cameraDownAction()));
    captureVideo = false;


    // file vars init.
    fileBase = "resim";
    fileExt = ".jpg";
    savePath = "resim/";
    fileName = savePath + fileBase + fileExt;
    folderName = "";

    // image getter class & image data inits.
    imageGetter = new getImage(urlCam.toString(), 10);
    lastData = new networkData();
    prevData = new networkData();

    cameraChecker = new getImage(urlCam.toString());

    videoFrameSize = 25;
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
    threadPLCControl->plc->portNum = 102;

    threadPLCControl->dbNo = DB_NO;
    threadPLCControl->dbNoRead = 2;
    threadPLCControl->byteNo = BYTE_NO;
    threadPLCControl->byteNo2 = threadPLCControl->byteNo + 1;

    if (!threadPLCControl->plc->libraryLoaded){
        permPLC = false;
        ui->plainTextEdit->appendPlainText(threadPLCControl->plc->message);
        ui->plainTextEdit->appendPlainText(MESSAGE5);
    } else
        permPLC = true;

    checker();

    mak_aktif_now = mak_aktif_old = false;

    if (permPLC && connectRequestedonBoot){
        plcInteractPrev = false;

        timerControlEnabled = false;
        // wait 2sec. to check first init of plc connection
        QTimer::singleShot(2000, this, SLOT(startTimer()));
    }

    // 1sn timer
    timerSn = new QTimer(this);
    connect(timerSn, SIGNAL(timeout()), this, SLOT(updateSn()));
    timerSn->start(1000);

    // 1msec timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    msecCount = 0;
    firstTimeTick = timeSystem.getSystemTimeMsec();
    timer->start(1);

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

    controlDelay = QString::number(controlDelay).toInt(&controlDelayValid, 10);
    timerControlInterval = 100;
    controlThreadCountSize = 60000 / timerControlInterval;   // timer shot count for 60sec plc check
    weldCommandsSize = controlDelay / timerControlInterval;

    // start message
    ui->plainTextEdit->appendPlainText(timeString() + "Sistem başlatılmıştır. Hoş geldiniz.");
    lic.checkLicence();

    if (lic.licenseState != _LIC2){
        QTimer::singleShot(500, this, SLOT(showLicenseDialog()));
    }


    if ( zControlActive || readDistance) {

        ui->zControlButton->setIcon(zIconEnabled);
        ui->labelDistance->show();
        ui->labelDistanceTag->show();
        ui->labelDistanceTag2->show();
    } else {

        ui->zControlButton->setIcon(zIconDisabled);
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


    if ( thinJointAlgoActive || zControlActive || hardControlStart )
        QTimer::singleShot(500, this, SLOT(showInfo()));

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
                    str += "AKTİF !!! : İnce Kaynak Ağzı Uygulaması\n";
                            //"İnce Kaynak Ağzı Uygulaması:\nLazerin V şekli teşkil edemediği ince ağızlarda kullanılır.\nLazeri kapatın ve\nkaynak ağzının düzgün karanlık siluet oluşturmasına\ndikkat edin!";
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

void MainWindow::checker(){

    permWeld = !emergencyStop && permOperator && permPLC && play && !cameraChecker->cameraDown && trackOn && controlDelayValid;

    ui->controlButton->setEnabled(permWeld);
}

QImage* MainWindow::takeTargetAreaImage(){

    QImage *image = new QImage(*lastData->image);
    targetArea = image->copy(offsetX, offsetY, frameWidth, frameHeight);
    return image;
}

void MainWindow::getImageFromCam(){

    fileName = fileBase + "_" + QDateTime::currentDateTime().toString("hhmmss_zzz") + fileExt;

    if (lastData->image->save(savePath + fileName))
        ui->plainTextEdit->appendPlainText(fileName + " başarıyla " + savePath + " klasörüne kayıt edildi.");
    else
        ui->plainTextEdit->appendPlainText(fileName + " kayıt edilemedi !!!");
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
    play = true;

    ui->playButton->setIcon(playOnIcon);
    ui->stopButton->setIcon(stopOffIcon);

    ui->trackButton->setEnabled(true);
    ui->controlButton->setEnabled(false);
    ui->videoButton->setIcon(videoSaveEnabled);
}

void MainWindow::stopButton(){

    // reseted vars ????
    play = false;
    imageGetter->reset();

    ui->playButton->setIcon(playOffIcon);
    ui->stopButton->setIcon(stopOnIcon);

    ui->trackButton->setEnabled(false);
    ui->controlButton->setEnabled(false);

}

void MainWindow::update(){

    msecCount++;

    if (play && !pause){
        secondTimeTick = timeSystem.getSystemTimeMsec();    // get current time in msec
        tickDiff = secondTimeTick - firstTimeTick;          // elapsed time from prev.

        if (!cameraChecker->cameraDown){
            // if time has come to display next image request it
            if (tickDiff >= frameInterval){
                //if (!threadImgGet.isRunning()) threadImgGet.run();
                imageGetter->run();

                playCam();
                firstTimeTick = secondTimeTick;
                fpsRequest++;
            }

        }

        //if (msecCount % 5 == 0){  if (!cameraChecker->cameraDown && !threadPlay.isRunning()) threadPlay.run();        }
    }

}

void MainWindow:: plcControl(){

    controlThreadCount++;

    checker();

    int state = _CMD_STOP;
    //int stateZ = _CMD_Z_CENTER;


    if ( readMachineStatus || readDistance || readWeldSeam ) {

        if (controlThreadCount % 4 == 0){

            threadPLCControl->commandRead = true;

            if (hardControlStart && readMachineStatus){

                if (mak_aktif_now && !mak_aktif_old) {

                    alignGuide2TrackCenter = true;

                    if (timeControl) {
                        timeControlCounter = 0;
                        startTimeControlCount = true;
                    }
                } else
                    alignGuide2TrackCenter = false;

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
                if (cameraChecker->cameraDown || !play || !permOperator){

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
        if (state != cmdStatePrev) {

            if (hardControlStart && alignGuide2TrackCenter){
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


        if (goX || !cmdSended || threadPLCControl->commandRead) {

            cmdSended = false;


            if (!threadPLCControl->isRunning()){

                threadPLCControl->commandState = state;

                threadPLCControl->start();

                cmdSended = true;

            }
        }

        if (state!= _CMD_CHECK) {
            cmdStatePrev = state;
        }

    }

/* the original
            if (state == _CMD_RIGHT)
                ui->cmdStatus->setIcon(cmd2LeftIcon);
            else if (state == _CMD_LEFT)
                ui->cmdStatus->setIcon(cmd2RightIcon);
            else if (state != _CMD_CHECK)
                ui->cmdStatus->setIcon(QIcon());

            cmdSended = false;
            if (!threadPLCControl->isRunning()){
                threadPLCControl->commandState = state;
                threadPLCControl->start();
                cmdSended = true;
            }
/ *
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
//        }

}

void MainWindow::updateSn(){

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
    QString seperator = "   ||   ";

    /*
    // display: time
    currentDateTime = QDateTime::currentDateTime();
    ui->labelTime->setText(currentDateTime.toString("hh:mm:ss"));
    */

    // check camera live state
    cameraChecker->checkHost();

    if (cameraChecker->cameraDown){
        ui->cameraStatus->setIcon(cameraOfflineIcon);
    } else {
        alarmCameraDownLock = false;
        ui->cameraStatus->setIcon(cameraOnlineIcon);
    }

    if (cameraChecker->cameraDown && !alarmCameraDownLock) emit cameraDown();


    // plc live state
    if (!plcInteractPrev && threadPLCControl->plc->plcInteract){           // 0 -> 1
        permPLC = true;
        ui->plainTextEdit->appendPlainText(timeString() + MESSAGE6);
    }
    else if (plcInteractPrev && !threadPLCControl->plc->plcInteract){       // 1 -> 0
        permPLC = false;
        ui->plainTextEdit->appendPlainText(timeString() + MESSAGE4);
    }

    plcInteractPrev = threadPLCControl->plc->plcInteract;

    if (threadPLCControl->plc->plcInteract)
        ui->plcStatus->setIcon(plcOnlineIcon);
    else
        ui->plcStatus->setIcon(plcOfflineIcon);

    if ( thinJointAlgoActive )
        ui->thinJointButton->setStyleSheet("color: rgb(255, 0, 0)");
    else
        ui->thinJointButton->setStyleSheet("color: rgb(0, 0, 0)");

    // if video is played
    if (play){

        if (fpsReal != 0) timeDelayAvg = timeDelayTotal / fpsReal;      // calc. ave time delay

        // status bar message
        message = "fps(t/req/sh): " + QString::number(fpsTarget) + "/" + QString::number(fpsRequest) + "/" + QString::number(fpsReal);
        message += seperator;

        int rate = 0;
        if (frameNo != 0) rate = (lateFrame * 100 / frameNo);
        message += "lr(%): " + QString::number(rate);
        message += seperator;

        message += "td(ms): " + QString::number(timeDelayAvg);

        message += seperator;
        message += "lf: " + QString::number(lateFrame);

        ui->statusBar->showMessage(message);
        //----------

        fpsRequest = 0;
        fpsReal = 0;
        timeDelayAvg = 0;
        timeDelayTotal = 0;
    }

    msecCount = 0;
    pause = false;
}

void MainWindow::startTimer(){

    if (!threadPLCControl->isRunning()){
        cmdState = _CMD_CHECK;
        threadPLCControl->commandState = cmdState;
        threadPLCControl->start();
        cmdSended = true;
    }
    cmdStatePrev = cmdState;
    cmdZStatePrev = _CMD_Z_CENTER;

    // wait 2sec. to check first init of plc connection
    QTimer::singleShot(2000, this, SLOT(initPlcTimer()));
}

void MainWindow::initPlcTimer(){

    ui->plainTextEdit->appendPlainText(timeString() + threadPLCControl->plc->message);
    if (threadPLCControl->plc->plcInteract) ui->plcStatus->setIcon(plcOnlineIcon);

    timerControlEnabled = true;
    timerControl = new QTimer(this);
    controlThreadCount = 0;
    connect(timerControl, SIGNAL(timeout()), this, SLOT(plcControl()));
    controlPause = false;
    timerControl->start(timerControlInterval);

}

void  MainWindow::cameraDownAction(){

    ui->plainTextEdit->appendPlainText(timeString() + alarm7);
    alarmCameraDownLock = true;
}

void MainWindow::analyzeButton(){

    if ( !imageGetter->imageList.isEmpty() ) {  // if any image is get

        int startTime = timeSystem.getSystemTimeMsec();

        if ( thinJointAlgoActive ) {
            processEdgeDetection();
            iprocess->angleAvg = iprocess->centerLine.angle;
        } else {
            processSolidnessCanny();
        }

        /*
        if ( thinJointAlgoActive ) {

            //processThinJoint();
            processContrastDetection();

        } else {

            if ( !subImageProcessingSwitch ) {

                processStandardHT();
                iprocess->calcAngleAvg();   // ave angle value of max voted line(s)

            } else {

                switch ( subImageProcessingType ) {
                    case 0:
                        processSubImageVoidness();
                        iprocess->calcAngleAvg();   // ave angle value of max voted line(s)
                        break;
                    case 1:
                        processSubImageSolidness();
                        break;
                }
            }
        }
        */

        int endTime = timeSystem.getSystemTimeMsec();

        int processElapsed = endTime - startTime;

        if ( iprocessInitSwitch ) {

            iprocess->cornerImage(false);   // false for solid canny                                                                    // produce corner image

            analyzeDialog *_analyzeDialog = new analyzeDialog(iprocess, processElapsed, this);
            _analyzeDialog->show();

            iprocessInitSwitch = false;
            delete iprocess;
        }


    } else {
         ui->plainTextEdit->appendPlainText(timeString() + alarm6);
    }
}

void MainWindow::guideButton(){

    //QApplication::beep();
    //ui->plainTextEdit->appendPlainText(lic.makeReport());

    showGuide = !showGuide;
    repaintGuide();

    ui->leftButton->setEnabled( showGuide && !trackOn );
    ui->rightButton->setEnabled( showGuide && !trackOn );


    /* SOLID LINE DETECTION WITH CANNY EXPERIMENT, TO BE EMBEDED IN SETUP DIALOG
    if ( !imageGetter->imageList.isEmpty() ){
        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image
        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;
        //iprocess->imgOrginal.save(savePath + "image_org.jpg");

        iprocess->prepareCannyArrays();

        for (int i = 0; i < 4 ; i++){

            iprocess->constructValueMatrix( iprocess->imgOrginal, i );
            //iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "matrix_org_valueX.csv" );

            iprocess->gaussianBlur();
            //iprocess->getImage( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight )->save(savePath + "image_blurred.jpg");

            iprocess->detectEdgeSobelwDirections();
            //iprocess->saveMatrix( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge.csv");
            //iprocess->saveMatrix( iprocess->edgeGradientMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge_gradients.csv");
            //iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_edge.jpg");

            iprocess->nonMaximumSuppression(false);
            //iprocess->saveMatrix( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge_suppressed.csv");
            //iprocess->getImage( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_edge_suppressed.jpg");

            iprocess->cannyThresholding(true);
            ui->plainTextEdit->appendPlainText("lo, med, hi: "+QString::number(iprocess->loValue) +", "+QString::number(iprocess->medianValue) +", "+QString::number(iprocess->hiValue));
            //iprocess->getImage_cannyThresholds(QImage::Format_RGB16)->save(savePath + "image_edge_strongs_weaks.png");

            iprocess->edgeTracing();
            //iprocess->getImage_cannyTracedEdges(QImage::Format_RGB16)->save(savePath + "image_edge_traced.png");
            //iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_canny.png");
            //iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge_map.csv");

            iprocess->assignEdgeMap();
        }

        iprocess->mergeEdgeMaps();

        iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_canny.png");
        //iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge_map.csv");

        iprocess->thetaMin = 84;
        iprocess->thetaMax = 96;
        iprocess->thetaStep = 1.0;

            for (int y = 0; y < iprocess->edgeHeight; y++)
                for (int x = 0; x < iprocess->edgeWidth; x++){
                if (iprocess->edgeMapMatrix[y][x])
                    iprocess->edgeMatrix[y][x]=255;
                else
                    iprocess->edgeMatrix[y][x]=0;
            }

            //iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "matrix_org.csv" );

        iprocess->houghTransformEdgeMap();
        iprocess->calculateHoughMaxs(1);              // get max voted line(s)

        for (int c = 0; c < 1; c++)
            ui->plainTextEdit->appendPlainText("dav: " +QString::number(iprocess->houghLines[c][0], 'f', 1)+", "+QString::number(iprocess->houghLines[c][1], 'f', 1)+", "+QString::number(iprocess->houghLines[c][2], 'f', 1));

        iprocess->detectLongestSolidLines(false, false);

        iprocess->cornerImage(false).save(savePath + "image_corners.png");

        ui->plainTextEdit->appendPlainText("lcr: " +QString::number(iprocess->leftCornerX)+", "+QString::number(iprocess->trackCenterX)+", "+QString::number(iprocess->rightCornerX));
        ui->plainTextEdit->appendPlainText("[0]sedal: " +
                                           QString::number(iprocess->major2Lines[0].start.x())+"-"+
                                           QString::number(iprocess->major2Lines[0].start.y())+", "+
                                           QString::number(iprocess->major2Lines[0].end.x())+"-"+
                                           QString::number(iprocess->major2Lines[0].end.y())+", "+
                                           QString::number(iprocess->major2Lines[0].distance, 'f', 1)+", "+
                                           QString::number(iprocess->major2Lines[0].angle, 'f', 1)+", "+
                                           QString::number(iprocess->major2Lines[0].length)  );
        ui->plainTextEdit->appendPlainText("[1]sedal: " +
                                           QString::number(iprocess->major2Lines[1].start.x())+"-"+
                                           QString::number(iprocess->major2Lines[1].start.y())+", "+
                                           QString::number(iprocess->major2Lines[1].end.x())+"-"+
                                           QString::number(iprocess->major2Lines[1].end.y())+", "+
                                           QString::number(iprocess->major2Lines[1].distance, 'f', 1)+", "+
                                           QString::number(iprocess->major2Lines[1].angle, 'f', 1)+", "+
                                           QString::number(iprocess->major2Lines[1].length)  );

        iprocess->cornerAndPrimaryLineImage(iprocess->major2Lines[0],iprocess->major2Lines[1],0,false).save(savePath + "image_major_2lines.png");

//        iprocess->drawSolidLines(iprocess->majorLines).save(savePath + "image_major_lines.png");

        //for (int c=0; c < iprocess->majorLines.size(); c++)
        //    iprocess->drawSolidLines2EdgeMatrix( iprocess->majorLines[c], QImage::Format_RGB16)->save(savePath + "image_major_lines_" + QString::number(c) + ".png");

        //iprocess->saveList(iprocess->solidSpaceMain, savePath + "matrix_solid_space_main.csv");
        //iprocess->saveList(iprocess->solidSpaceMainTrimmed, savePath + "matrix_solid_space_trimmed.csv");
        iprocess->saveList(iprocess->primaryGroup, savePath + "matrix_primary_group.csv");
        iprocess->saveList(iprocess->secondaryGroup, savePath + "matrix_secondary_group.csv");

        delete iprocess;
        iprocessInitSwitch = false;
    }
    */

    /* LINE DETECTION EXPERIMENT, TO BE EMBEDED IN SETUP DIALOG
    if ( !imageGetter->imageList.isEmpty() ){
        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image
        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->prepareCannyArrays();

        iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );
        iprocess->gaussianBlur();
        iprocess->detectEdgeSobelwDirections();
        iprocess->nonMaximumSuppression();
        iprocess->cannyThresholding(true);
        //ui->plainTextEdit->appendPlainText("lo, med, hi: "+QString::number(iprocess->loValue) +", "+QString::number(iprocess->medianValue) +", "+QString::number(iprocess->hiValue));
        iprocess->edgeTracing();

        iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_canny.png");



        iprocess->thetaMin = -4;
        iprocess->thetaMax = 4;
        iprocess->thetaStep = 1.0;

        iprocess->houghTransformEdgeMap();;

        iprocess->calculateHoughMaxs(200);              // get max voted line(s)
            //iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, savePath + "matrix_max_hough_lines.csv");

        if (thinJointAlgoActive)
            iprocess->thinCornerNum = 1;

        iprocess->detectMainEdges(thinJointAlgoActive, true);

        for (int i=0; i<iprocess->mainEdgesList.size();i++)
            ui->plainTextEdit->appendPlainText("mainEdges dav: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue));

        iprocess->thickenEdgeMap(3);
        iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_canny_thickened.png");

        iprocess->scoreLineCrossing(true);
        ui->plainTextEdit->appendPlainText("score: " + QString::number(iprocess->mainEdgeScore) + ", " + QString::number(iprocess->mainEdgeScorePercent, 'f', 1));

        iprocess->drawLines().save(savePath + "image_mainEdges.png");
        iprocess->cornerImage().save(savePath + "image_corners.png");


        delete iprocess;
        iprocessInitSwitch = false;
    }
    */



    /* EDGE DETECTION EXPERIMENT, TO BE EMBEDED IN SETUP DIALOG
    if ( !imageGetter->imageList.isEmpty() ){
        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image
        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;
            //iprocess->imgOrginal.save(savePath + "image_org.jpg");

        iprocess->prepareCannyArrays();

        for (int i = 0; i < 4 ; i++){

            iprocess->constructValueMatrix( iprocess->imgOrginal, i );
            //iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "matrix_orgvalue.csv" );

            iprocess->gaussianBlur();
            //iprocess->getImage( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight )->save(savePath + "image_blurred.jpg");

            iprocess->detectEdgeSobelwDirections();
            //iprocess->saveMatrix( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge.csv");
            //iprocess->saveMatrix( iprocess->edgeGradientMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge_gradients.csv");
            //iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_edge.jpg");

            iprocess->nonMaximumSuppression(false);
            //iprocess->saveMatrix( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge_suppressed.csv");
            //iprocess->getImage( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_edge_suppressed.jpg");

            iprocess->cannyThresholding(true);
            ui->plainTextEdit->appendPlainText("lo, med, hi: "+QString::number(iprocess->loValue) +", "+QString::number(iprocess->medianValue) +", "+QString::number(iprocess->hiValue));
            //iprocess->getImage_cannyThresholds(QImage::Format_RGB16)->save(savePath + "image_edge_strongs_weaks.png");

            iprocess->edgeTracing();
            //iprocess->getImage_cannyTracedEdges(QImage::Format_RGB16)->save(savePath + "image_edge_traced.png");
            //iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_canny.png");
            //iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge_map.csv");

            iprocess->assignEdgeMap();
        }


        iprocess->mergeEdgeMaps();

        iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(savePath + "image_canny.png");
        iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, savePath + "matrix_edge_map.csv");


        iprocess->thetaMin = -2;
        iprocess->thetaMax = 2;
        //iprocess->thetaMin = 87;
        //iprocess->thetaMax = 93;
        iprocess->thetaStep = 1.0;


        iprocess->centerX = 0;
        iprocess->houghTransformEdgeMap();

        iprocess->calculateHoughMaxs(200);              // get max voted line(s)
            //iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, savePath + "matrix_max_hough_lines.csv");

        if (thinJointAlgoActive)
            iprocess->thinCornerNum = 1;

//        iprocess->detectMainEdges(thinJointAlgoActive, true);
        iprocess->detectMainEdges(false, true);
            //iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, savePath + "matrix_max_hough_lines_distance.csv");
            / *
            ui->plainTextEdit->appendPlainText("-1st-maximas---");
            for (int i=0; i<iprocess->localMaximaSize;i++)
                ui->plainTextEdit->appendPlainText("start: "+QString::number(iprocess->rangeArray[i][0]) +" stop: "+QString::number(iprocess->rangeArray[i][1]));

            ui->plainTextEdit->appendPlainText("-1st hough vals---");
            for (int i=0; i<iprocess->listHoughDataSize;i++)
                ui->plainTextEdit->appendPlainText("dav: ,"+QString::number(iprocess->listHoughDataArray[i][0], 'f', 2) +", "+QString::number(iprocess->listHoughDataArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughDataArray[i][2], 'f', 2));

            ui->plainTextEdit->appendPlainText("-2nd-maximas---");
            for (int i=0; i<iprocess->localMaxima2ndSize;i++)
                ui->plainTextEdit->appendPlainText("start: "+QString::number(iprocess->rangeArray2nd[i][0]) +" stop: "+QString::number(iprocess->rangeArray2nd[i][1]));
            * /
            ui->plainTextEdit->appendPlainText("-2nd hough vals---");
            for (int i=0; i<iprocess->listHoughData2ndSize;i++)
                ui->plainTextEdit->appendPlainText("dav: "+QString::number(iprocess->listHoughData2ndArray[i][0], 'f', 2) +", "+QString::number(iprocess->listHoughData2ndArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughData2ndArray[i][2], 'f', 2));

            iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "matrix_org_with_edges.csv" );
            iprocess->drawLines().save(savePath + "image_mainEdges.png");
            iprocess->cornerImage().save(savePath + "image_corners.png");

        delete iprocess;
        iprocessInitSwitch = false;
    }
    */



    // /* CONTRAST DETECTION EXPERIMENT, TO BE EMBEDED IN SETUP DIALOG
    if ( !imageGetter->imageList.isEmpty() ){
       targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image
       iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
       iprocessInitSwitch = true;
           iprocess->imgOrginal.save(savePath + "image_org.jpg");

       iprocess->constructValueMatrix( iprocess->imgOrginal );
           iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "matrix_orgvalue.csv" );

       iprocess->constructContrastMatix(3);
           //iprocess->saveMatrix( iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "matrix_contrast.csv" );


           QImage img = iprocess->imgOrginal.copy();
           QRgb black, white;
           black = qRgb(0, 0, 0);
           white = qRgb(255, 255, 255);
           img.fill(black);

           for(int y = 0; y < iprocess->imageHeight; y++)
               for(int x = 0; x < iprocess->imageWidth; x++)
                   if (iprocess->contrastMatrix[y][x] == 1) img.setPixel(x, y, white);

           img.save(savePath + "image_contrast.jpg");


       iprocess->thetaMin = -6;
       iprocess->thetaMax = 6;
       iprocess->thetaStep = 0.5;

       iprocess->houghTransformContrast();

       iprocess->calculateHoughMaxs(200);              // get max voted line(s)

       iprocess->calcAvgDistAndAngleOfMajors(0.30);    // calc. avg. distance and theta
           ui->plainTextEdit->appendPlainText("dist: " + QString::number(iprocess->major2Lines[0].distance) + ", angle: " + QString::number(iprocess->major2Lines[0].angle));
           ui->plainTextEdit->appendPlainText("dist: " + QString::number(iprocess->major2Lines[1].distance) + ", angle: " + QString::number(iprocess->major2Lines[1].angle));
           ui->plainTextEdit->appendPlainText("losize: " + QString::number(iprocess->lowLinesList.size()));
           iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, savePath + "matrix_max_hough_lines.csv");
           iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, savePath + "matrix_max_hough_lines_distance.csv");

       iprocess->constructContrastMatrixMajor2Lines();
           //iprocess->saveMatrix( iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "matrix_contrast_lines.csv" );
           iprocess->getImage(iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight)->save(savePath + "image_lines.jpg");


       iprocess->detectContrastCenter();
           iprocess->cornerImage().save(savePath + "image_corner.jpg");
           ui->plainTextEdit->appendPlainText("avg dist, angle: " + QString::number(iprocess->distanceAvg) + ", " + QString::number(iprocess->thetaAvg));
//           ui->plainTextEdit->appendPlainText("avg dist, angle: " + QString::number(iprocess->distanceAvg) + ", " + QString::number(iprocess->angleAvg));

       delete iprocess;
       iprocessInitSwitch = false;
   }
   // */

    /* THIN JOINT EXPERIMENT, TO BE EMBEDED IN SETUP DIALOG
    if ( !imageGetter->imageList.isEmpty() ){
        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image
        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;
        iprocess->constructValueMatrix( iprocess->imgOrginal );
        iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "matrix_orgvalue.csv" );


        iprocess->imgOrginal.save(savePath + "org.jpg");
        iprocess->detectThinJointCenter(3, 31);

        for (int i=0;i<31;i++)
            ui->plainTextEdit->appendPlainText(QString::number(i) + ": "+QString::number(iprocess->slope[i],'f',3));

        minCostedLines *centerline = new minCostedLines();
        centerline->c = iprocess->centerC;
        centerline->cost = 0;
        iprocess->drawLine(centerline, iprocess->slopeBest).save(savePath + "_centerLine.jpg");

        for (int i=0;i<31;i++)
            ui->plainTextEdit->appendPlainText(QString::number(i) + ": (c,cost) "+ QString::number(iprocess->bestLines[i].c) + " , " + QString::number(iprocess->bestLines[i].cost));

        ui->plainTextEdit->appendPlainText("best slope: "+QString::number(iprocess->slopeBest));

        QFile file(savePath + "lineListScanForBestAngle.csv");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);

            for(int i = 0; i < iprocess->lineList.size(); i++) out << i << "," << iprocess->lineList[i].c << "," << iprocess->lineList[i].cost << "\n";
            file.close();
        };

        QFile file2(savePath + "peakPoints.csv");
        if (file2.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file2);

            for(int i = 0; i < iprocess->peakPoints.size(); i++) out << i << "," << iprocess->lineList[ iprocess->peakPoints[i] ].cost << "\n";
            file2.close();
        };

        //        iprocess->drawLines(bestLines, 21).save(savePath + "bestLines.jpg");
        minCostedLines *pointer = new minCostedLines();
        for (int i=0;i<31;i++){
            pointer->c = iprocess->bestLines[i].c;
            pointer->cost = iprocess->bestLines[i].cost;
            iprocess->drawLine(pointer, iprocess->slope[i]).save(savePath + "bestLineOfEachAngle"+QString::number(i)+".jpg");
        }
        delete pointer;
        delete iprocess;
        iprocessInitSwitch = false;
    }
    */

}

void MainWindow::trackButton(){

    trackOn = !trackOn;

    if (trackOn){
        ui->trackButton->setIcon(trackOnIcon);
    } else {
        ui->trackButton->setIcon(trackOffIcon);
    }

    ui->leftButton->setEnabled( showGuide && !trackOn );
    ui->rightButton->setEnabled( showGuide && !trackOn );

    ui->thinJointButton->setEnabled(!trackOn);
}

void MainWindow::controlButton(){

    controlOn = !controlOn;

    if (controlOn){

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

    } else {

        controlInitiated = false;
        initialJointWidth = jointWidth = 0;
        maxJointWidth = 1000;

        cmdState = _CMD_STOP;

        ui->controlButton->setIcon(controlOffIcon);
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

void MainWindow::thinJointButton(){

    thinJointAlgoActive = !thinJointAlgoActive;

    if ( thinJointAlgoActive ) {
        ui->thinJointButton->setStyleSheet("color: rgb(255, 0, 0)");

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);

//        QString str = "İnce Kaynak Ağzı Uygulaması:\nLazerin V şekli teşkil edemediği ince ağızlarda kullanılır.\nLazeri kapatın ve\nkaynak ağzının düzgün karanlık siluet oluşturmasına\ndikkat edin!";
        QString str = "İnce Kaynak Ağzı Uygulaması:\nÇok İnce Ağızlarda Aktif Hale Getiriniz!";
        msgBox.setText(str);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();

    } else {
        ui->thinJointButton->setStyleSheet("color: rgb(0, 0, 0)");
    }

}

void MainWindow::zControlButton(){

    //zControlActive = !zControlActive;



    //ui_zctrl = new zctrlDialog(this);
    //ui_zctrl->show();

    if ( zControlActive ) {

/*
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);

        QString str = "Yükseklik Kontrolü Aktif!";
        msgBox.setText(str);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        */

    } else {
        ui->zControlButton->setStyleSheet("color: rgb(0, 0, 0)");
    }
}

void MainWindow::processImage(){


    if ( lineDetection ) {

        processLineDetection();

    } else {

        //if (dynamicAlgo)    thinJointAlgoActive = !weldSeamExists;

        if ( thinJointAlgoActive ) {
            processEdgeDetection();
        } else {
            processSolidnessCanny();
        }
    }

/*
    if ( thinJointAlgoActive ) {

//        processThinJoint();
        processContrastDetection();

    } else {

        if ( !subImageProcessingSwitch ) {

            processStandardHT();

        } else {

            switch ( subImageProcessingType ) {
                case 0:
                    processSubImageVoidness();
                    break;
                case 1:
                    processSubImageSolidness();
                    break;
            }
        }
    }

*/

    if ( lineDetection ) {

        //....

    } else {


        // if center of track is not an error, append dev. to trend data list OR append error code to list
        if (iprocess->detected){

            if (controlInitiated) {

                initialJointWidth = abs(iprocess->rightCornerX - iprocess->leftCornerX) + 1;
                maxJointWidth = initialJointWidth * 1.2;
                minJointWidth = initialJointWidth * 0.8;
                    //ui->plainTextEdit->appendPlainText( QString::number(minJointWidth)+ ", " + QString::number(initialJointWidth) + ", " + QString::number(maxJointWidth) );
                controlInitiated = false;
            }

            jointWidth = abs(iprocess->rightCornerX - iprocess->leftCornerX) + 1;

            int error = iprocess->trackCenterX - (frameWidth/2);
            deviationData.append(error);

            if (alignGuide2TrackCenter) {

                offsetXpos += error;

                repaintGuide();

                alignGuide2TrackCenter = false;
            }

            if (trackOn){
                errorTotal += abs(error);
                processCount++;
                if (abs(error) > errorMax) errorMax = abs(error);
            }
        } else {
            deviationData.append(eCodeDev);     // eCodeDev: error code
        }


        // assign plc commands using dev. data findings
        if (iprocess->detected){

            detectionError = false;

            if (jointWidthControlActive && (jointWidth > maxJointWidth || jointWidth < minJointWidth || jointWidth == 1) ) {

                cmdState = _CMD_CENTER;
                    //ui->plainTextEdit->appendPlainText("error");

            } else {
                int index = deviationData.size() - 1;

                if (deviationData[index] >= errorLimit ){
                    cmdState = _CMD_RIGHT;
                } else
                if (deviationData[index] <= errorLimitNeg){
                    cmdState = _CMD_LEFT;
                } else
                if ((cmdStatePrev2 == _CMD_LEFT) && (deviationData[index] >= errorStopLimitNeg)){
                    cmdState = _CMD_CENTER;
                } else
                if ((cmdStatePrev2 == _CMD_RIGHT) && (deviationData[index] <= errorStopLimit)){
                    cmdState = _CMD_CENTER;
                } else
                if ((cmdStatePrev2 != _CMD_RIGHT) && (cmdStatePrev2 != _CMD_LEFT)){
                    cmdState = _CMD_CENTER;
                }
            }

            cmdStatePrev2 = cmdState;

        } else {
            //if (deviationData[index] != eCodeDev){
            cmdState = cmdStatePrev2;
            detectionError = true;
        }

    }

    if ( iprocessInitSwitch ) {
        iprocessInitSwitch = false;
        delete iprocess;
    }

}

void MainWindow::processStandardHT(){

    if ( !imageGetter->imageList.isEmpty() ){

        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->toMono();                                     // convert target to mono
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->detectEdgeSobel();                            // detect edges of the mono image

        iprocess->thetaMin = thetaMin;
        iprocess->thetaMax = thetaMax;
        iprocess->thetaStep = thetaStep;
        iprocess->houghTransform();                             // detect lines in edge image

        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        iprocess->calcAvgDistAndAngle( houghLineNo );           // calc. avg. distance and theta

        iprocess->voteThreshold = voteThreshold;                // acceptable vote value low-limit
        iprocess->checkPrimaryLine();                           // is max voted line  above the low-limit?
        iprocess->detectVoidLines();                            // detect void lines on hough lines in MONO image

        iprocess->voidThreshold = voidThreshold;                // void threshold to decide max void as primary
        iprocess->detectPrimaryVoid();                          // decide primary void line & corners/center
    }
}

void MainWindow::processSubImageVoidness(){

    processStandardHT();

    if ( iprocessInitSwitch ) {

        if ( iprocess->detected ) {

            int tCenterX = iprocess->trackCenterX;
            int tEndX = frameWidth;

            QImage targetLeft = targetArea.copy( 0, 0, tCenterX, frameHeight );
            QImage targetRight = targetArea.copy( tCenterX, 0, tEndX - tCenterX, frameHeight );

            // left image process
            iprocessLeft = new imgProcess( targetLeft, targetLeft.width(), targetLeft.height() ); // new imgProcess object
            iprocessLeft->toMono();                                         // convert target to mono
            iprocessLeft->constructValueMatrix( iprocessLeft->imgMono );    // construct mono matrix
            iprocessLeft->detectEdgeSobel();                                // detect edges of the mono image

            iprocessLeft->thetaMin = thetaMinSub;
            iprocessLeft->thetaMax = thetaMaxSub;
            iprocessLeft->thetaStep = thetaStepSub;
            iprocessLeft->houghTransform();                 // detect lines in edge image

            iprocessLeft->calculateHoughMaxs( 50 );         // get max voted line(s)
            iprocessLeft->calcAvgDistAndAngleOfMajors(0.8);    // calc. avg. distance and theta

            iprocessLeft->primaryLineDetected = true;       // bypass line vote check
            iprocessLeft->detectVoidLinesEdge();            // detect void lines on hough lines in edge image

            iprocessLeft->voidThreshold = 0;                // bypass void length check
            iprocessLeft->errorEdgeLimit = 0;               // bypass corner void check
            iprocessLeft->angleAvg = 0;                     // bypass angle value check
            iprocessLeft->detectPrimaryVoid();              // decide primary void line & corners/center

            // right image process
            iprocessRight = new imgProcess( targetRight, targetRight.width(), targetRight.height() );   // new imgProcess object
            iprocessRight->toMono();                                        // convert target to mono
            iprocessRight->constructValueMatrix( iprocessRight->imgMono );  // construct mono matrix
            iprocessRight->detectEdgeSobel();                               // detect edges of the mono image

            iprocessRight->thetaMin = thetaMinSub;
            iprocessRight->thetaMax = thetaMaxSub;
            iprocessRight->thetaStep = thetaStepSub;
            iprocessRight->houghTransform();                // detect lines in edge image

            iprocessRight->calculateHoughMaxs( 50 );        // get max voted line(s)
            iprocessRight->calcAvgDistAndAngleOfMajors(0.8);   // calc. avg. distance and theta

            iprocessRight->primaryLineDetected = true;      // bypass line vote check
            iprocessRight->detectVoidLinesEdge();           // detect void lines on hough lines in edge image

            iprocessRight->voidThreshold = 0;               // bypass void length check
            iprocessRight->errorEdgeLimit = 0;              // bypass corner void check
            iprocessRight->angleAvg = 0;                    // bypass angle value check
            iprocessRight->detectPrimaryVoid();             // decide primary void line & corners/center

            if (iprocessLeft->detected && iprocessRight->detected){

                iprocess->leftCornerX = iprocessLeft->rightMostCornerX;
                iprocess->leftCornerY = iprocessLeft->rightMostCornerY;
                iprocess->rightCornerX = tCenterX + iprocessRight->leftMostCornerX;
                iprocess->rightCornerY = iprocessRight->leftMostCornerY;
                iprocess->trackCenterX = (iprocess->leftCornerX + iprocess->rightCornerX) / 2;
                iprocess->trackCenterY = (iprocess->leftCornerY + iprocess->rightCornerY) / 2;
            }

            delete iprocessLeft;
            delete iprocessRight;
        }
    }
}

void MainWindow::processSubImageSolidness(){

    if ( !imageGetter->imageList.isEmpty() ){

        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->toMono();                                     // convert target to mono
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix

        iprocess->thetaMin = thetaMinSub;
        iprocess->thetaMax = thetaMaxSub;
        iprocess->thetaStep = thetaStepSub;
        iprocess->detectLongestSolidLines();
    }
}

void MainWindow::processThinJoint(){

    if ( !imageGetter->imageList.isEmpty() ){

        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->constructValueMatrix( iprocess->imgOrginal );

        iprocess->detectThinJointCenter(3, 31);
    }
}

void MainWindow::processContrastDetection(){

    if ( !imageGetter->imageList.isEmpty() ){

        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->constructValueMatrix( iprocess->imgOrginal );

        iprocess->constructContrastMatix(3);

        iprocess->thetaMin = -6;
        iprocess->thetaMax = 6;
        iprocess->thetaStep = 0.5;

        iprocess->houghTransformContrast();;

        iprocess->calculateHoughMaxs( 200 );            // get max voted line(s)

        iprocess->calcAvgDistAndAngleOfMajors(0.30);    // calc. avg. distance and theta

        //iprocess->constructContrastMatrixMajor2Lines();

        iprocess->detectContrastCenter();

    }
}

void MainWindow::processEdgeDetection(){

    if ( !imageGetter->imageList.isEmpty() ){

        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

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

        iprocess->centerX = 0;
        iprocess->houghTransformEdgeMap();
        iprocess->calculateHoughMaxs(houghLineNo);              // get max voted line(s)

        if (thinJointAlgoActive)
            iprocess->thinCornerNum = 1;
        iprocess->detectMainEdges(thinJointAlgoActive, false);

        //iprocess->detectLongestSolidLines(false, false);    // no averaging & edge matrix
    }
}

void MainWindow::processLineDetection(){

    if ( !imageGetter->imageList.isEmpty() ){

        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->prepareCannyArrays();

        iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );

        iprocess->gaussianBlur();

        iprocess->detectEdgeSobelwDirections();

        iprocess->nonMaximumSuppression();

        iprocess->cannyThresholding(true);

        iprocess->edgeTracing();

        iprocess->thetaMin = -4;
        iprocess->thetaMax = 4;
        iprocess->thetaStep = 1.0;

        iprocess->houghTransformEdgeMap();

        iprocess->calculateHoughMaxs(200);              // get max voted line(s)

        iprocess->thinCornerNum = 1;

        iprocess->detectMainEdges(true, false);

        iprocess->thickenEdgeMap(3);

        iprocess->scoreLineCrossing(true);

        if ( iprocess->mainEdgeScorePercent > lineScoreLimit){
            ui->plainTextEdit->appendPlainText( timeString() + "İz bulundu, %" + QString::number(iprocess->mainEdgeScorePercent) );
        }
    }
}

void MainWindow::processSolidnessCanny(){

    if ( !imageGetter->imageList.isEmpty() ){

        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->prepareCannyArrays();

        for (int i = 0; i < 4 ; i++){

            iprocess->constructValueMatrix( iprocess->imgOrginal, i );

            iprocess->gaussianBlur();

            iprocess->detectEdgeSobelwDirections();

            iprocess->nonMaximumSuppression(false);

            iprocess->cannyThresholding(true);

            iprocess->edgeTracing();

            iprocess->assignEdgeMap();
        }

        iprocess->mergeEdgeMaps();

        iprocess->thetaMin = 87;
        iprocess->thetaMax = 93;
        iprocess->thetaStep = 1.0;

        for (int y = 0; y < iprocess->edgeHeight; y++)
            for (int x = 0; x < iprocess->edgeWidth; x++){
                if (iprocess->edgeMapMatrix[y][x])
                    iprocess->edgeMatrix[y][x]=255;
                else
                    iprocess->edgeMatrix[y][x]=0;
            }

        iprocess->houghTransformEdgeMap();
        iprocess->calculateHoughMaxs(1);              // get max voted line(s)

        iprocess->detectLongestSolidLines(false, false);    // no averaging & edge matrix
    }
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
        x = sceneCenterX + deviationData[i];
        y = i*yRes;
        xPrev = sceneCenterX + deviationData[i-1];
        yPrev = (i-1)*yRes;
        scene->addLine(xPrev,yPrev,x,y,penTrack);
    }
    ui->trackView->show();
}

void MainWindow::target2Left(){

    offsetXpos--;
    offsetX--;

    if ( offsetX < 5 || ((offsetX + frameWidth) > (imageWidth - 5)) ){
        offsetXpos++;
        offsetX++;
    }

    repaintGuide();
}

void MainWindow::target2Right(){

    offsetXpos++;
    offsetX++;

    if ( offsetX < 5 || ((offsetX + frameWidth) > (imageWidth - 5)) ){
        offsetXpos--;
        offsetX--;
    }

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
            urlCam.setUrl(settings->value("urlCam", _URL_CAM).toString());
            playCamonBoot = settings->value("play", _PLAY_CAM_ONBOOT).toBool();
            fpsTarget = settings->value("fps", _FPS).toInt();
        settings->endGroup();

        settings->beginGroup("plc");
            urlPLC.setUrl(settings->value("urlPLC", _URL_PLC).toString());
            plcType = settings->value("type", _PLC_TYPE).toInt();
            DB_NO = settings->value("dbno", _DB_NO).toInt();
            BYTE_NO = settings->value("byte", _BYTE_NO).toInt();
            connectRequestedonBoot = settings->value("pcon", _PLC_CONN_ONBOOT).toBool();
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
            thetaMin = settings->value("tmn", _THETA_MIN).toInt();
            thetaMax = settings->value("tmx", _THETA_MAX).toInt();
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
            jointWidthControlActive = settings->value("widthctrl", _WIDTH_CONTROL).toBool();

            brightnessVal = settings->value("brightness", _BRIGHTNESS).toInt();
            contrastVal = settings->value("contrast", _CONTRAST).toInt();
            gammaVal = settings->value("gamma", _GAMMA).toInt();
            gaussianSize = settings->value("gausssize", _GAUSS_SIZE).toInt();
            stdDev = settings->value("gaussdev", _GAUSS_DEV).toFloat();
            cannyThinning = settings->value("cannythin", _CANNY_THIN).toBool();

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
        urlCam.setUrl(_URL_CAM);
        playCamonBoot = _PLAY_CAM_ONBOOT;
        fpsTarget = _FPS;

        urlPLC.setUrl(_URL_PLC);
        plcType = _PLC_TYPE;
        DB_NO = _DB_NO;
        BYTE_NO = _BYTE_NO;
        connectRequestedonBoot = _PLC_CONN_ONBOOT;
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
        thetaMin = _THETA_MIN;
        thetaMax = _THETA_MAX;
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
        jointWidthControlActive = _WIDTH_CONTROL;

        brightnessVal = _BRIGHTNESS;
        contrastVal = _CONTRAST;
        gammaVal = _GAMMA;
        gaussianSize = _GAUSS_SIZE;
        stdDev = _GAUSS_DEV;
        cannyThinning = _CANNY_THIN;

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
        settings->setValue("urlCam", urlCam.toString());
        QVariant play(playCamonBoot);
            settings->setValue("play", play.toString());
        settings->setValue("fps", QString::number(fpsTarget));
    settings->endGroup();

    settings->beginGroup("plc");
        settings->setValue("urlPLC", urlPLC.toString());
        settings->setValue("type", QString::number(plcType));
        settings->setValue("dbno", QString::number(DB_NO));
        settings->setValue("byte", QString::number(BYTE_NO));
        QVariant pcon(connectRequestedonBoot);
            settings->setValue("pcon", pcon.toString());
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
        settings->setValue("tmn", QString::number(thetaMin));
        settings->setValue("tmx", QString::number(thetaMax));
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


        settings->setValue("brightness", QString::number(brightnessVal));
        settings->setValue("contrast", QString::number(contrastVal));
        settings->setValue("gamma", QString::number(gammaVal));
        settings->setValue("gausssize", QString::number(gaussianSize));
        settings->setValue("gaussdev", QString::number(stdDev));

        QVariant cannythinsw(cannyThinning);
            settings->setValue("cannythin", cannythinsw.toString());

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
    for (int i = 0; i < fileData.size(); i++){
        out << fileData[i] << "\n";
    }
    file.close();
    return true;
}

void MainWindow::closeEvent(QCloseEvent*){

    qApp->quit();
}

void MainWindow::playCam(){

    if (play && !pause){

        if (!imageGetter->imageList.isEmpty()){  // if any image is get

            int size = imageGetter->imageList.length();  // image buufer size
            bool show = true;

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

            // show current valid image. valid: not late from previous (<show>)
            if (!lastData->shown && show){

                ui->imageFrame->setPixmap(QPixmap::fromImage(*lastData->image));
                ui->imageFrame->show();
                ui->guideFrame->raise();     // if guide is shown, suppress it
                lastData->shown = true;      // mark last data was SHOWN on display
                fpsReal++;

                // calculate (display time - request time) delay in msec
                int displayTime = timeSystem.getSystemTimeMsec();
                int requestTime = calcTotalMsec(lastData->requestHour.toInt(), lastData->requestMinute.toInt(), lastData->requestSecond.toInt(), lastData->requestMSecond.toInt());
                timeDelay = displayTime - requestTime;
                timeDelayTotal += timeDelay;  // overall delay


                if ( captureVideo ) {

                    videoList[videoFrameCount] = lastData->image->copy();
                    videoFrameCount++;

                    if (videoFrameCount >= videoFrameSize) {

                        captureVideo = false;
                        folderName = savePath + QDateTime::currentDateTime().toString("yyMMdd_hhmmss") + "/";
                        if (!threadVideoSave->isRunning())
                            threadVideoSave->start();
                    }
                }

                // if joint is tracked for some interval
                if (trackOn && (fpsReal % iprocessInterval) == 0 ){
                    processImage();  // detect deviation

                    if ( !lineDetection ) {
                        if (deviationData.size() >= 2) drawTrack();   // draw deviation trend
                    }
                }
            }
        }
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

    videoList = new QImage[videoFrameSize];
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

    mak_aktif_now = !mak_aktif_now;
    //alignGuide2TrackCenter = true;
    QString _fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "C:/xampp/htdocs/images/aygaz", tr("Image Files (*.png *.jpg *.bmp)"));

    //imageFile = new QImage();
    imageFile.load(_fileName);
    ui->imageFrame->setPixmap( QPixmap::fromImage( imageFile ));
}

void MainWindow::on_setupButton_clicked(){

    showSetupForm();
}

MainWindow::~MainWindow(){

    if (threadPLCControl->plc->plcInteract)
        threadPLCControl->disconnect();

//    delete settings;
    delete ui;
}


/*
void MainWindow::processSubImageSolidness(){

    processStandardHT();

    if ( iprocessInitSwitch ) {

        if ( iprocess->detected ) {

            int tCenterX = iprocess->trackCenterX;
            int tWidth = frameWidth;

            QImage targetLeft = targetArea.copy( 0, 0, tCenterX, frameHeight );
            QImage targetRight = targetArea.copy( tCenterX, 0, tWidth - tCenterX, frameHeight );

            // left image process
            iprocessLeft = new imgProcess( targetLeft, targetLeft.width(), targetLeft.height() );    // new imgProcess object
            iprocessLeft->toMono();                                         // convert target to mono
            iprocessLeft->constructValueMatrix( iprocessLeft->imgMono );    // construct mono matrix
            iprocessLeft->detectEdgeSobel();                                // detect edges of the mono image
            iprocessLeft->thickenEdges();

            iprocessLeft->thetaMin = thetaMinSub;
            iprocessLeft->thetaMax = thetaMaxSub;
            iprocessLeft->thetaStep = thetaStepSub;
            iprocessLeft->houghTransform();                                 // detect lines in edge image
            iprocessLeft->detectLongestSolidLines();


            // right image process
            iprocessRight = new imgProcess( targetRight, targetRight.width(), targetRight.height() );    // new imgProcess object
            iprocessRight->toMono();                                        // convert target to mono
            iprocessRight->constructValueMatrix( iprocessRight->imgMono );  // construct mono matrix
            iprocessRight->detectEdgeSobel();                               // detect edges of the mono image
            iprocessRight->thickenEdges();

            iprocessRight->thetaMin = thetaMinSub;
            iprocessRight->thetaMax = thetaMaxSub;
            iprocessRight->thetaStep = thetaStepSub;
            iprocessRight->houghTransform();                                // detect lines in edge image
            iprocessRight->detectLongestSolidLines();


            if ( iprocessLeft->primaryLine.length > iprocessRight->primaryLine.length  && iprocessRight->primaryLine.length != -1 ) {

                // right image re-process
                tCenterX = iprocessLeft->primaryLine.end.x() + 5;

                targetRight = targetArea.copy( tCenterX, 0, tWidth - tCenterX, frameHeight );

                delete iprocessRight;

                iprocessRight = new imgProcess( targetRight, targetRight.width(), targetRight.height() );   // new imgProcess object
                iprocessRight->toMono();                                        // convert target to mono
                iprocessRight->constructValueMatrix( iprocessRight->imgMono );  // construct mono matrix
                iprocessRight->detectEdgeSobel();                               // detect edges of the mono image
                iprocessRight->thickenEdges();

                iprocessRight->thetaMin = thetaMinSub;
                iprocessRight->thetaMax = thetaMaxSub;
                iprocessRight->thetaStep = thetaStepSub;
                iprocessRight->houghTransform();                                // detect lines in edge image
                iprocessRight->detectLongestSolidLines();

            } else
            if ( iprocessLeft->primaryLine.length < iprocessRight->primaryLine.length && iprocessLeft->primaryLine.length != -1 ) {

                // left image re-process
                tCenterX = iprocess->trackCenterX + iprocessRight->primaryLine.start.x() - 5;

                targetLeft = targetArea.copy( 0, 0, tCenterX, frameHeight );

                delete iprocessLeft;

                iprocessLeft = new imgProcess( targetLeft, targetLeft.width(), targetLeft.height() );   // new imgProcess object
                iprocessLeft->toMono();                                         // convert target to mono
                iprocessLeft->constructValueMatrix( iprocessLeft->imgMono );    // construct mono matrix
                iprocessLeft->detectEdgeSobel();                                // detect edges of the mono image
                iprocessLeft->thickenEdges();

                iprocessLeft->thetaMin = thetaMinSub;
                iprocessLeft->thetaMax = thetaMaxSub;
                iprocessLeft->thetaStep = thetaStepSub;
                iprocessLeft->houghTransform();                                 // detect lines in edge image
                iprocessLeft->detectLongestSolidLines();

                // to recover for right image coord.
                tCenterX = iprocess->trackCenterX;

            } else {
                // equality in lengths
            }

            // if no left line
            if ( iprocessLeft->primaryLine.length == -1){

                iprocessLeft->primaryLine.start.setX( 0 );
                iprocessLeft->primaryLine.end.setX( 0 );

                if ( iprocessRight->primaryLine.length != -1 ){
                    iprocessLeft->primaryLine.start.setY( iprocessRight->primaryLine.start.y() );
                    iprocessLeft->primaryLine.end.setY( iprocessRight->primaryLine.start.y() );
                } else {
                    iprocessLeft->primaryLine.start.setY( iprocessLeft->imageHeight / 2 );
                    iprocessLeft->primaryLine.end.setY( iprocessLeft->imageHeight / 2 );
                }
            }

            // if no right line
            if ( iprocessRight->primaryLine.length == -1){

                iprocessRight->primaryLine.start.setX( iprocessRight->imageWidth - 1 );
                iprocessRight->primaryLine.end.setX( iprocessRight->imageWidth - 1 );

                if ( iprocessLeft->primaryLine.length != -1 ){
                    iprocessRight->primaryLine.start.setY( iprocessLeft->primaryLine.end.y() );
                    iprocessRight->primaryLine.end.setY( iprocessLeft->primaryLine.end.y() );
                } else {
                    iprocessRight->primaryLine.start.setY( iprocessRight->imageHeight / 2 );
                    iprocessRight->primaryLine.end.setY( iprocessRight->imageHeight / 2 );
                }
            }

            iprocess->leftCornerX = iprocessLeft->primaryLine.end.x();
            iprocess->leftCornerY = iprocessLeft->primaryLine.end.y();

            iprocess->rightCornerX = tCenterX + iprocessRight->primaryLine.start.x();
            iprocess->rightCornerY = iprocessRight->primaryLine.start.y();

            iprocess->trackCenterX = ( iprocess->leftCornerX + iprocess->rightCornerX ) / 2;
            iprocess->trackCenterY = ( iprocess->leftCornerY + iprocess->rightCornerY ) / 2;

            delete iprocessLeft;
            delete iprocessRight;
        }
    }
}
*/



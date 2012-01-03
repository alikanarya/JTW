#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCodec>
#include "windows.h"

#include "licensedialog.h"
#include "analyzedialog.h"
#include "helpdialog.h"
#include "exitdialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){

    rectScreen = QApplication::desktop()->geometry();

    // only title on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);


    ui->clearMsgBoxButton->hide();  //  not used now
    //ui->plainTextEdit->appendPlainText(QString::number(rectScreen.width())+"x"+QString::number(rectScreen.height()));
    ui->labelLogoCustomer->hide();

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

    ui->trackButton->setEnabled(false);

    statusMessage = "";
    settings = new QSettings(INIFILENAME, QSettings::IniFormat);
    readSettings();

    this->setWindowTitle(_MAINTITLE + title);

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

    // file vars init.
    fileBase = "resim";
    fileExt = ".jpg";
    savePath = "resim/";
    fileName = savePath + fileBase + fileExt;

    // image getter class & image data inits.
    imageGetter = new getImage(urlCam.toString(), 10);
    lastData = new networkData();
    prevData = new networkData();

    cameraChecker = new getImage(urlCam.toString());

    // tracking & plc controls
    trackOn = false;
    controlOn = false;
    emergencyStop = false;
    detectionError = true;
    permOperator = false;

    // plc
    connectRequested = connectRequestedonBoot;
    threadPLCControl = new plcControlThread(plcType, urlPLC.toString());
    threadPLCControl->plc->portNum = 102;

    threadPLCControl->dbNo = DB_NO;
    threadPLCControl->byteNo = BYTE_NO;
    /*
    threadPLCControl->right_BYTE = right_VMEM_BYTE;
    threadPLCControl->right_BIT = right_BITofBYTE;
    threadPLCControl->left_BYTE = left_VMEM_BYTE;
    threadPLCControl->left_BIT = left_BITofBYTE;
    threadPLCControl->stop_BYTE = stop_VMEM_BYTE;
    threadPLCControl->stop_BIT = stop_BITofBYTE;
    threadPLCControl->emergency_BYTE = emergency_VMEM_BYTE;
    threadPLCControl->emergency_BIT = emergency_BITofBYTE;
    */

    if (!threadPLCControl->plc->libraryLoaded){
        permPLC = false;
        ui->plainTextEdit->appendPlainText(threadPLCControl->plc->message);
        ui->plainTextEdit->appendPlainText(MESSAGE5);
    } else
        permPLC = true;

    checker();

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
    cmdState = _CMD_STOP;
    cmdStatePrev2 = _CMD_CENTER;
    ui->cmdStatus->setIcon(QIcon());

    controlDelay = QString::number(controlDelay).toInt(&controlDelayValid, 10);
    timerControlInterval = 100;
    controlThreadCountSize = 60000 / timerControlInterval;   // timer shot count for 60sec plc check
    weldCommandsSize = controlDelay / timerControlInterval;

    // start message
    ui->plainTextEdit->appendPlainText(timeString() + "Sistem baþlatýlmýþtýr. Hoþ geldiniz.");
    lic.checkLicence();

    if (lic.licenseState != _LIC2){
        QTimer::singleShot(500, this, SLOT(showLicenseDialog()));
    }

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
        ui->plainTextEdit->appendPlainText(fileName + " baþarýyla " + savePath + " klasörüne kayýt edildi.");
    else
        ui->plainTextEdit->appendPlainText(fileName + " kayýt edilemedi !!!");
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
                    if (play && trackOn && controlOn && permPLC){
                        state = cmdState;   // Weld Command from image processing
                    } else
                        state = _CMD_STOP;
                }
            }
        }


        if (state != cmdStatePrev || !cmdSended) {
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

        if (state!= _CMD_CHECK) {
            cmdStatePrev = state;
        }

    }
}


void MainWindow::updateSn(){

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

    // display: time
    currentDateTime = QDateTime::currentDateTime();
    ui->labelTime->setText(currentDateTime.toString("hh:mm:ss"));

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

        if ( iprocessInitSwitch ) {
            iprocess->calcAngleAvg();   // ave angle value of max voted line(s)
        }

        int endTime = timeSystem.getSystemTimeMsec();

        int processElapsed = endTime - startTime;

        if ( iprocessInitSwitch ) {

            iprocess->cornerImage();                                                                    // produce corner image

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


    if ( !imageGetter->imageList.isEmpty() ){

        targetArea = lastData->image->copy( offsetX, offsetY, frameWidth, frameHeight );    // take target image

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->constructValueMatrix( iprocess->imgOrginal );

        iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "valuematrix.csv" );

        int min,index;
        for (int y = 0; y < iprocess->imgOrginal.height(); y++){
            min = 255, index = 0;
            for (int x = 0; x < iprocess->imgOrginal.width(); x++){
                if (iprocess->valueMatrix[y][x]<min){
                    index = x;
                    min = iprocess->valueMatrix[y][x];
                }
            }
            iprocess->valueMatrix[y][index] = 0;
        }
        iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "valuematrix2.csv" );



/*
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
*/
        delete iprocess;
        iprocessInitSwitch = false;
    }

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

}


void MainWindow::controlButton(){

    controlOn = !controlOn;

    if (controlOn){
        weldCommands.clear();
        weldCommandsSize = controlDelay / timerControlInterval;

        ui->controlButton->setIcon(controlOnIcon);
        ui->plainTextEdit->appendPlainText(timeString() + message1);

        // for report
        errorTotal = 0;
        processCount = 0;
        controlStartTime = timeSystem.getSystemTimeMsec();
        errorMax = 0;
        //QString line = timeString() + "Kaynak baþladý."
        fileData.clear();
        fileData.append(timeString() + "Kaynak baþlatýldý.");
    } else {
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


void MainWindow::processImage(){

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

    // if center of track is not an error, append dev. to trend data list OR append error code to list
    if (iprocess->detected){

        int error = iprocess->trackCenterX - (frameWidth/2);
        deviationData.append(error);

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

        int index = deviationData.size() - 1;

        if (deviationData[index] >= errorLimit){
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

        cmdStatePrev2 = cmdState;

    } else {
        //if (deviationData[index] != eCodeDev){
        cmdState = cmdStatePrev2;
        detectionError = true;
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
            iprocessLeft->calcAvgDistAndAngleOfMajors();    // calc. avg. distance and theta

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
            iprocessRight->calcAvgDistAndAngleOfMajors();   // calc. avg. distance and theta

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
            /*
            right_VMEM_BYTE = settings->value("rbyt", _RIGHT_VMEM_BYTE).toInt();
            right_BITofBYTE = settings->value("rbit", _RIGHT_BITofBYTE).toInt();
            left_VMEM_BYTE = settings->value("lbyt", _LEFT_VMEM_BYTE).toInt();
            left_BITofBYTE = settings->value("lbit", _LEFT_BITofBYTE).toInt();
            stop_VMEM_BYTE = settings->value("stpbyt", _STOP_VMEM_BYTE).toInt();
            stop_BITofBYTE = settings->value("stpbit", _STOP_BITofBYTE).toInt();
            emergency_VMEM_BYTE = settings->value("emrbyt", _EMRGENCY_VMEM_BYTE).toInt();
            emergency_BITofBYTE = settings->value("emrbit", _EMRGENCY_BITofBYTE).toInt();
            */
            connectRequestedonBoot = settings->value("pcon", _PLC_CONN_ONBOOT).toBool();
            controlDelay = 0;   //settings->value("ctd", _CONTROL_DELAY).toInt();
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

        settings->endGroup();

        settings->beginGroup("oth");
            yResIndex = settings->value("yresi", _YRES_ARRAY_INDEX).toInt();
                yRes = yResArray[yResIndex];
            title = settings->value("title", _TITLE).toString();
        settings->endGroup();

    } else {    // assign default values if file not exist
        urlCam.setUrl(_URL_CAM);
        playCamonBoot = _PLAY_CAM_ONBOOT;
        fpsTarget = _FPS;

        urlPLC.setUrl(_URL_PLC);
        plcType = _PLC_TYPE;
        DB_NO = _DB_NO;
        BYTE_NO = _BYTE_NO;
        /*
        right_VMEM_BYTE = _RIGHT_VMEM_BYTE;
        right_BITofBYTE = _RIGHT_BITofBYTE;
        left_VMEM_BYTE = _LEFT_VMEM_BYTE;
        left_BITofBYTE = _LEFT_BITofBYTE;
        stop_VMEM_BYTE = _STOP_VMEM_BYTE;
        stop_BITofBYTE = _STOP_BITofBYTE;
        emergency_VMEM_BYTE = _EMRGENCY_VMEM_BYTE;
        emergency_BITofBYTE = _EMRGENCY_BITofBYTE;
        */
        connectRequestedonBoot = _PLC_CONN_ONBOOT;
        controlDelay = _CONTROL_DELAY;

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

        yResIndex = _YRES_ARRAY_INDEX;
            yRes = yResArray[yResIndex];
        title = _TITLE;

        statusMessage = "ini dosyasý bulunamadý";
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
        /*
        settings->setValue("rbyt", QString::number(right_VMEM_BYTE));
        settings->setValue("rbit", QString::number(right_BITofBYTE));
        settings->setValue("lbyt", QString::number(left_VMEM_BYTE));
        settings->setValue("lbit", QString::number(left_BITofBYTE));
        settings->setValue("stpbyt", QString::number(stop_VMEM_BYTE));
        settings->setValue("stpbit", QString::number(stop_BITofBYTE));
        settings->setValue("emrbyt", QString::number(emergency_VMEM_BYTE));
        settings->setValue("emrbit", QString::number(emergency_BITofBYTE));
        */
        QVariant pcon(connectRequestedonBoot);
            settings->setValue("pcon", pcon.toString());
        settings->setValue("ctd", QString::number(controlDelay));
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
    settings->endGroup();

    settings->beginGroup("oth");
        settings->setValue("yresi", QString::number(yResIndex));
        settings->setValue("title", title);
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

                // if joint is tracked for some interval
                if (trackOn && (fpsReal % iprocessInterval) == 0 ){
                    processImage();  // detect deviation

                    if (deviationData.size() >= 2) drawTrack();   // draw deviation trend
                }
            }
        }
    }
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

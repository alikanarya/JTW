﻿
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
    connect(this, SIGNAL(cameraDown()), this, SLOT(cameraDownAction()));
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
    timerAutoFocus = new QTimer(this);
    connect(timerAutoFocus, SIGNAL(timeout()), this, SLOT(checkAutoFocusingState()));

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
    getCamImageProperties = true;
    play = true;

    ui->playButton->setIcon(playOnIcon);
    ui->stopButton->setIcon(stopOffIcon);

    ui->trackButton->setEnabled(true);
    ui->controlButton->setEnabled(false);
    ui->videoButton->setIcon(videoSaveEnabled);

    switch ( camStreamType ) {
        case 1: // JPEG
            makeNetworkRequest();
            break;
        case 0: // STREAM
            if (!playStream->isRunning()){
                cameraDownStatus = false;
                playStream->start();
                //playStream->measureFpsFn(3000);
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
    QImage img = QImage( (const uchar*) playStream->dest.data, playStream->dest.cols, playStream->dest.rows, playStream->dest.step, QImage::Format_RGB888 );
    if (img.format() != QImage::Format_Invalid) {
        //qDebug() << playStream->iter;
        lastData->image = new QImage(img);
        lastData->shown = false;

        if (testFlag) {
            //cv::Mat imgData = playStream->dest.clone();
            //cv::Mat imgData = cv::imdecode(playStream->dest, cv::IMREAD_GRAYSCALE );

            cv::Mat imgData0(lastData->image->height(),lastData->image->width(),CV_8UC3,(uchar*)lastData->image->bits(),lastData->image->bytesPerLine());
            cv::Mat imgData; // deep copy just in case (my lack of knowledge with open cv)
            cvtColor(imgData0, imgData,CV_RGB2GRAY);

            //! [expand]
                cv::Mat padded;                            //expand input image to optimal size
                int m = cv::getOptimalDFTSize( imgData.rows );
                int n = cv::getOptimalDFTSize( imgData.cols ); // on the border add zero values
                copyMakeBorder(imgData, padded, 0, m - imgData.rows, 0, n - imgData.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
            //! [expand]

            //! [complex_and_real]
                cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
                cv::Mat complexI;
                cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
            //! [complex_and_real]

            //! [dft]
                cv::dft(complexI, complexI);            // this way the result may fit in the source matrix
            //! [dft]

                // compute the magnitude and switch to logarithmic scale
                // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
            //! [magnitude]
                cv::split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
                cv::magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
                cv::Mat magI = planes[0];
            //! [magnitude]

                //! [log]
                    //magI += cv::Scalar::all(1);                    // switch to logarithmic scale
                    //cv::log(magI, magI);
                //! [log]

                //! [crop_rearrange]
                    // crop the spectrum, if it has an odd number of rows or columns
                    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

                    // rearrange the quadrants of Fourier image  so that the origin is at the image center
                    int cx = magI.cols/2;
                    int cy = magI.rows/2;

                    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
                    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
                    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
                    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

                    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
                    q0.copyTo(tmp);
                    q3.copyTo(q0);
                    tmp.copyTo(q3);

                    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
                    q2.copyTo(q1);
                    tmp.copyTo(q2);
                //! [crop_rearrange]

                //! [normalize]
                    //cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX); // Transform the matrix with float values into a
                                                            // viewable image form (float between values 0 and 1).
                //! [normalize]

                /*
                cv::FileStorage file("fft.xml", cv::FileStorage::WRITE);
                file << "matname" << magI;
                file.release();
                */
                    /*
                ofstream fout("fft.csv");
                if(!fout) {
                    cout<<"File Not Opened"<<endl;  return;
                }
                for(int i=0; i<magI.rows; i++) {
                    for(int j=0; j<magI.cols; j++) {
//                        fout<<magI.at<float>(i,j)<<",";
                        fout << QString::number(magI.at<float>(i,j),'f',3).toStdString() <<",";
                    }
                    fout<<endl;
                }
                fout.close();
*/
                    double min, max;
                    cv::minMaxLoc(magI, &min, &max);
                    cv::Scalar scal = cv::mean(magI);
                    float mean = scal.val[0];
                    ui->plainTextEdit->appendPlainText("min: " + QString::number(min,'f',3) + " max: " + QString::number(max) + " mean: " + QString::number(mean));

            testFlag = false;
        }


        playCam();
        //ui->imageFrame->setPixmap( QPixmap::fromImage( img.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio) ));


    }
}

void MainWindow::camConnected(){

    //qDebug()<<Q_FUNC_INFO;
    cameraDownStatus = false;
    alarmCameraDownLock = false;

    if (play && camStreamType == 0) {
        playStream->setFps(fpsTarget);
        playStream->startCapture();
        ui->plainTextEdit->appendPlainText(timeString() + alarm11);
    }

    ui->cameraStatus->setIcon(cameraOnlineIcon);
}

void MainWindow::camNotConnected(){

    //qDebug()<<Q_FUNC_INFO;
    cameraDownStatus = true;
    camReconnectLock = false;
    getCamImageProperties = true;

    ui->cameraStatus->setIcon(cameraOfflineIcon);
    ui->imageFrame->clear();
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
                    QTimer::singleShot(3000, this, SLOT(killCamStreamThread()));
                    playStream->start();
                    //qDebug()<<Q_FUNC_INFO;
                }
                break;
        }
        //**if (cameraChecker->cameraDown && !alarmCameraDownLock) emit cameraDown();
        if (!alarmCameraDownLock) emit cameraDown();
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

void  MainWindow::cameraDownAction(){

    ui->plainTextEdit->appendPlainText(timeString() + alarm7);
    ui->statusBar->showMessage("Kameraya bağlanılamıyor !");

    alarmCameraDownLock = true;
    getCamImageProperties = true;
}

void MainWindow::analyzeButton(){

    //if ( !imageGetter->imageList.isEmpty() ) {  // if any image is get
    if (lastData->image->format() != QImage::Format_Invalid) {

        int startTime = timeSystem.getSystemTimeMsec();

        processImage(false);

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
    } else {
        ui->trackButton->setIcon(trackOffIcon);
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

        if (focusCheckBeforeControl){
            camDoAutoFocus = autoFocusBeforeControl;
            autoFocusPassNo = 0;
            checkFocusState();
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

        cmdState = _CMD_STOP;

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
        iprocess->detectMainEdges(thinJointAlgoActive, false);
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
// woLASER: canny1 > houghTr > detectMainEdges > thickenEdgeMap > scoreLineCrossing

    if (edgeDetectionState == 3) {
        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        iprocess->thinCornerNum = 1;//mainEdgesNumber;
        iprocess->detectMainEdges(thinJointAlgoActive, false);
        iprocess->thickenEdgeMap(3);
        iprocess->scoreLineCrossing(true);

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

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->thetaMin = thetaMin;
        iprocess->thetaMax = thetaMax;
        iprocess->thetaStep = thetaStep;

        if (thinJointAlgoActive){    // without laser
            iprocess->centerX = 0;
            iprocess->centerY = 0;
        } else {                     // with laser
            iprocess->centerX = iprocess->edgeWidth / 2;
            iprocess->centerY = 0;
        }

        iprocess->toMono();                                     // convert target to mono

        edgeDetection(iprocess);

        if (thinJointAlgoActive) {  // without laser - VERTICAL SEARCH

            switch ( algorithmType ) {
                case 0: // NONE
                    break;
                case 1: // MAIN EDGES
                    Algo3(iprocess);
                    break;
                case 2: // THIN JOINT - DARK AREA
                    Algo4(iprocess);
                    break;
                case 3: // CONTRAST
                    Algo5(iprocess);
                    break;
                case 4: // LINE DETECTION WITH MAIN EDGES
                    Algo6(iprocess);
                    break;
                case 5: // EXPERIMENTAL
                    break;
            }
        } else {    // with laser - HORIZONTAL SEARCH
            switch ( algorithmType ) {
                case 0: // NONE
                    break;
                case 1: // LONGEST SOLID LINES
                    Algo1(iprocess);
                    break;
                case 2: // PRIMARY VOID
                    Algo2(iprocess);
                    break;
                case 3: // EXPERIMENTAL
                    break;
            }
        }

        if ( lineDetection ) {

            //....

        } else {


            // if center of track is not an error, append dev. to trend data list OR append error code to list
            if (iprocess->detected){

//                error = iprocess->trackCenterX - (frameWidth/2);
                error = iprocess->trackCenterX - (frameWidthCam/2);
                deviationData.append(error);

                //if (controlInitiated) {                }

                jointWidth = abs(iprocess->rightCornerX - iprocess->leftCornerX) + 1;



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

                    if (controlOn)
                        plcCommands();
                }

                cmdStatePrev = cmdState;
                cmdStatePrev2 = cmdState;

            } else {
                //if (deviationData[index] != eCodeDev){
                cmdState = cmdStatePrev2;
                detectionError = true;
            }


            if (iprocess->detected && controlInitiated){
                goX = false;    //dont send command to plc when control is initiated firstly

                initialJointWidth = abs(iprocess->rightCornerX - iprocess->leftCornerX) + 1;
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
        }

        if ( deleteObject && iprocessInitSwitch ) {
            iprocessInitSwitch = false;
            delete iprocess;
        }

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

    testFlag = true;
//    cv::Mat imgData = cv::imdecode(imgDataCV, cv::IMREAD_GRAYSCALE );
    //cv::Mat imgData = imgDataCV;
/*
    cv::Mat imgData = cv::imread("05.jpg", cv::IMREAD_GRAYSCALE);

    //! [expand]
        cv::Mat padded;                            //expand input image to optimal size
        int m = cv::getOptimalDFTSize( imgData.rows );
        int n = cv::getOptimalDFTSize( imgData.cols ); // on the border add zero values
        copyMakeBorder(imgData, padded, 0, m - imgData.rows, 0, n - imgData.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    //! [expand]

    //! [complex_and_real]
        cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
        cv::Mat complexI;
        cv::merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
    //! [complex_and_real]

    //! [dft]
        cv::dft(complexI, complexI);            // this way the result may fit in the source matrix
    //! [dft]

        // compute the magnitude and switch to logarithmic scale
        // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    //! [magnitude]
        cv::split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
        cv::magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
        cv::Mat magI = planes[0];
    //! [magnitude]

        //! [log]
            //magI += cv::Scalar::all(1);                    // switch to logarithmic scale
            //cv::log(magI, magI);
        //! [log]

        //! [crop_rearrange]
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
    if(!fout0) { cout<<"File Not Opened"<<endl;  return;   }
    for(int i=0; i<q0.rows; i++) {
        for(int j=0; j<q0.cols; j++)
            fout0 << QString::number(q0.at<float>(i,j),'f',0).toStdString() <<",";
        fout0<<endl;
    }
    fout0.close();
    ofstream fout1("fftQ1.csv");
    if(!fout1) { cout<<"File Not Opened"<<endl;  return;   }
    for(int i=0; i<q1.rows; i++) {
        for(int j=0; j<q1.cols; j++)
            fout1 << QString::number(q1.at<float>(i,j),'f',0).toStdString() <<",";
        fout1<<endl;
    }
    fout1.close();
    ofstream fout2("fftQ2.csv");
    if(!fout2) { cout<<"File Not Opened"<<endl;  return;   }
    for(int i=0; i<q2.rows; i++) {
        for(int j=0; j<q2.cols; j++)
            fout2 << QString::number(q2.at<float>(i,j),'f',0).toStdString() <<",";
        fout2<<endl;
    }
    fout2.close();
    ofstream fout3("fftQ3.csv");
    if(!fout3) { cout<<"File Not Opened"<<endl;  return;   }
    for(int i=0; i<q3.rows; i++) {
        for(int j=0; j<q3.cols; j++)
            fout3 << QString::number(q3.at<float>(i,j),'f',0).toStdString() <<",";
        fout3<<endl;
    }
    fout3.close();
}

            cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
            q0.copyTo(tmp);
            q3.copyTo(q0);
            tmp.copyTo(q3);

            q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
            q2.copyTo(q1);
            tmp.copyTo(q2);
        //! [crop_rearrange]

        //! [normalize]
            //cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX); // Transform the matrix with float values into a
                                                    // viewable image form (float between values 0 and 1).
        //! [normalize]

if (false) {
    ofstream fout("fft.csv");
    if(!fout) {
        cout<<"File Not Opened"<<endl;  return;
    }
    for(int i=0; i<magI.rows; i++) {
        for(int j=0; j<magI.cols; j++) {
//                        fout<<magI.at<float>(i,j)<<",";
            fout << QString::number(magI.at<float>(i,j),'f',3).toStdString() <<",";
        }
        fout<<endl;
    }
    fout.close();
}

        double min, max;
        cv::minMaxLoc(magI, &min, &max);
        cv::Scalar scal = cv::mean(magI);
        float mean = scal.val[0];
        ui->plainTextEdit->appendPlainText("min: " + QString::number(min,'f',3) + " max: " + QString::number(max) + " mean: " + QString::number(mean));
*/
        /*
        cv::FileStorage file("fft.xml", cv::FileStorage::WRITE);
        file << "matname" << magI;
        file.release();
        */

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

    if (controlOn){

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

    }

}

void MainWindow::checkFocusState(){
    if (!cameraDownStatus && !camApi->busy){
        camApi->apiDahuaGetFocusState();
    }
}

void MainWindow::doAutoFocus(){
    if (!cameraDownStatus && !camApi->busy){
        camApi->apiDahuaAutoFocus();
    }
}

void MainWindow::focusState(bool state){
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

void MainWindow::focusingActionState(bool state){
    camFocusingActionState = state;

    if (!timerAutoFocus->isActive()) {      // ONLY STATUS INFO REQUESTED
    } else {    // AUTO FOCUS PROCESS CHECK
        if (!camFocusingActionState) {
            timerAutoFocus->stop();
            ui->plainTextEdit->appendPlainText("Oto fokus işlemi tamamlandı...");
            QTimer::singleShot(4000, this, SLOT(checkFocusState()));
            //checkFocusState();
        }
    }
}

void MainWindow::checkAutoFocusingState(){  // timerAutoFocus slot

    if (!cameraDownStatus && !camApi->busy){
        if (timerLock){
            camApi->apiDahuaGetFocusStatus();
            timerLock = false;
            ui->plainTextEdit->appendPlainText("");
        } else if (camFocusingActionState) {
            camApi->apiDahuaGetFocusStatus();
        }
        QTextCursor text_cursor = QTextCursor(ui->plainTextEdit->document());
        text_cursor.movePosition(QTextCursor::End);
        text_cursor.insertText("*");
    }
}

﻿#include "setupform.h"
#include "ui_setupform.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCodec>
#include "imgtools.h"

extern MainWindow *w;

setupForm::setupForm(QWidget *parent) : QDialog(parent), ui(new Ui::setupForm){

    fileBaseTarget = "target";
    fileBaseMono = "mono";
    fileBaseEdge = "edge";
    fileBaseHough = "hough";
    fileBaseCorner = "corner";
    fileExt = ".jpg";
    savePath = "data/";
    fileName = "";

    // only title & close button on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::WindowSystemMenuHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);

    getParameters();
    captured = false;
    ui->labelPrimaryLine->hide();   // hide PRIMARY LINE NOT DETECTED message

    iprocessInitSwitch = iprocessLeftInitSwitch = iprocessRightInitSwitch = false;

    switch ( w->subImageProcessingType ) {
        case 0:
            ui->subTypeBox->setCurrentIndex(0);
            break;
        case 1:
            ui->subTypeBox->setCurrentIndex(1);
            break;
    }

    ui->zctrlBox->setChecked( w->zControlActive );

    ui->widthControlBox->setChecked( w->jointWidthControlActive );

    ui->hardControlStartBox->setChecked( w->hardControlStart );

    ui->readMachineStatusBox->setChecked( w->readMachineStatus );

    ui->readDistanceBox->setChecked( w->readDistance );

    ui->dynamicAlgoBox->setChecked( w->dynamicAlgo );

    ui->readWeldSeamBox->setChecked( w->readWeldSeam );

    ui->timeControlBox->setChecked( w->timeControl );

    ui->lineDetectionBox->setChecked( w->lineDetection );

    ui->editLineScore->setText( QString::number( w->lineScoreLimit ) );

    ui->labelBrightness->setText( QString::number(w->brightnessVal) );
    ui->brightnessSlider->setValue( w->brightnessVal );

    ui->labelContrast->setText( QString::number(w->contrastVal) );
    ui->contrastSlider->setValue( w->contrastVal );

    ui->labelGamma->setText( QString::number(w->gammaVal) );
    ui->gammaSlider->setValue( w->gammaVal );

    ui->labelGaussSize->setText( QString::number(w->gaussianSize) );
    ui->gaussSizeSlider->setValue( (w->gaussianSize-1)/2 );

    ui->labelGaussSDev->setText( QString::number(w->stdDev) );
    ui->gaussSDevSlider->setValue( (int)(w->stdDev*10) );

    ui->cannyThinningBox->setChecked( w->cannyThinning );

    if ( w->thinJointAlgoActive ){
        ui->radioLaser->setChecked(false);
        ui->radioWoLaser->setChecked(true);
    } else {
        ui->radioWoLaser->setChecked(false);
        ui->radioLaser->setChecked(true);
    }
}

void setupForm::processImage(){

    if ( w->play && !w->imageGetter->imageList.isEmpty() ){
        target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->play &&  imageLoadedFromFile){
        target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ){

        if (iprocessInitSwitch) {
            delete iprocess;
            iprocessInitSwitch = false;
        }

        iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
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
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix

        bool matrixFlag = false;
        bool averaging = false;

        switch ( edgeDetectionState ) {
            case 0:
                iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
                averaging = true;
                matrixFlag = true;
                break;
            case 1:
                iprocess->detectEdgeSobel(); // detect edges of the mono image
                iprocess->houghTransformFn(iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight);
                averaging = false;
                matrixFlag = false;
                break;
            case 2:
                // canny
                break;
        }

        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)

        switch ( lineDetectAlgos ) {
            case 0: // none
                break;

            case 1: // detectLongestSolidLines
                iprocess->detectLongestSolidLines(averaging, matrixFlag);    // no averaging & edge matrix
                break;

            case 2: // detectMainEdges
                iprocess->thinCornerNum = 2;
                iprocess->detectMainEdges(thinJointAlgoActive, false);
                break;

            case 3: // detectPrimaryVoid
                iprocess->calcAvgDistAndAngle( houghLineNo );           // calc. avg. distance and theta
                voteAvg = iprocess->calcVoteAvg();                      // avg. value of max voted line(s)

                iprocess->voteThreshold = voteThreshold;                // acceptable vote value low-limit
                if ( !iprocess->checkPrimaryLine() )                    // is max voted line  above the low-limit?
                    ui->labelPrimaryLine->show();
                iprocess->detectVoidLines();                            // detect void lines on hough lines in MONO image

                iprocess->voidThreshold = voidThreshold;                // void threshold to decide max void as primary
                iprocess->detectPrimaryVoid();                          // decide primary void line & corners/center
                break;
        }

    }

}

void setupForm::processSolidnessCanny(){

    if ( w->play && !w->imageGetter->imageList.isEmpty() ){
        target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->play &&  imageLoadedFromFile){
        target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ){

        if (iprocessInitSwitch) {
            delete iprocess;
            iprocessInitSwitch = false;
        }
        iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->thetaMin = thetaMin;
        iprocess->thetaMax = thetaMax;
        iprocess->thetaStep = thetaStep;

        iprocess->prepareCannyArrays();
        iprocess->constructGaussianMatrix(w->gaussianSize, w->stdDev);

        for (int i = 0; i < 4 ; i++){

            iprocess->constructValueMatrix( iprocess->imgOrginal, i );

            iprocess->gaussianBlur();

            iprocess->detectEdgeSobelwDirections();

            iprocess->nonMaximumSuppression(w->cannyThinning);

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

        if (w->thinJointAlgoActive){
            iprocess->centerX = 0;
            iprocess->centerY = 0;
        } else {
            iprocess->centerX = iprocess->edgeWidth / 2;
            iprocess->centerY = 0;
        }
        iprocess->houghTransformEdgeMap();

        iprocess->calculateHoughMaxs(houghLineNo);              // get max voted line(s)

        if (w->thinJointAlgoActive) {
            iprocess->thinCornerNum = 2;
            iprocess->detectMainEdges(w->thinJointAlgoActive, false);
        } else {
            iprocess->detectLongestSolidLines(false, false);    // no averaging & edge matrix
        }

    }
}

void setupForm::processStandardHT(){

    if ( w->play && !w->imageGetter->imageList.isEmpty() ){
        target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->play &&  imageLoadedFromFile){
        target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ){

        if (iprocessInitSwitch) {
            delete iprocess;
            iprocessInitSwitch = false;
        }
        iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->toMono();                                     // convert target to mono
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->detectEdgeSobel();                            // detect edges of the mono image

        iprocess->thetaMin = thetaMin;
        iprocess->thetaMax = thetaMax;
        iprocess->thetaStep = thetaStep;

        if (w->thinJointAlgoActive){
            iprocess->centerX = 0;
            iprocess->centerY = 0;
        } else {
            iprocess->centerX = iprocess->edgeWidth / 2;
            iprocess->centerY = 0;
        }
        iprocess->houghTransform();                             // detect lines in edge image

        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
/*
        iprocess->calcAvgDistAndAngle( houghLineNo );           // calc. avg. distance and theta
        voteAvg = iprocess->calcVoteAvg();                      // avg. value of max voted line(s)

        iprocess->voteThreshold = voteThreshold;                // acceptable vote value low-limit
        if ( !iprocess->checkPrimaryLine() )                    // is max voted line  above the low-limit?
            ui->labelPrimaryLine->show();
        iprocess->detectVoidLines();                            // detect void lines on hough lines in MONO image

        iprocess->voidThreshold = voidThreshold;                // void threshold to decide max void as primary
        iprocess->detectPrimaryVoid();                          // decide primary void line & corners/center
*/
        iprocess->detectLongestSolidLines(false, false);    // no averaging & edge matrix
    }

}

void setupForm::processSubImageVoidness(){

    if ( w->play && !w->imageGetter->imageList.isEmpty() ){
        target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->play &&  imageLoadedFromFile){
        target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ){

        if (iprocessInitSwitch) {
            delete iprocess;
            iprocessInitSwitch = false;
        }
        iprocess = new imgProcess(target, target.width(),target.height());  // new imgProcess object
        iprocessInitSwitch = true;

        iprocess->toMono();                                     // convert target to mono
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->detectEdgeSobel();                            // detect edges of the mono image

        iprocess->thetaMin = thetaMin;
        iprocess->thetaMax = thetaMax;
        iprocess->thetaStep = thetaStep;

        iprocess->centerX = iprocess->edgeWidth / 2;
        iprocess->centerY = 0;

        iprocess->houghTransform();                             // detect lines in edge image

        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        iprocess->calcAvgDistAndAngle( houghLineNo );           // calc. avg. distance and theta
        voteAvg = iprocess->calcVoteAvg();                      // ave value of max voted line(s)

        iprocess->voteThreshold = voteThreshold;                // acceptable vote value low-limit
        if ( !iprocess->checkPrimaryLine() )                    // is max voted line  above the low-limit?
            ui->labelPrimaryLine->show();
        iprocess->detectVoidLines();                            // detect void lines on hough lines in MONO image

        iprocess->voidThreshold = voidThreshold;                // void threshold to decide max void as primary
        iprocess->detectPrimaryVoid();                          // decide primary void line & corners/center

        //iprocess->imgMono.save("data/0.jpg");

ui->plainTextEdit->appendPlainText(".z");
        // divide target image into 2 pieces
        if ( w->subImageProcessingSwitch && iprocess->detected ) {

            if (iprocessLeftInitSwitch) {
                delete iprocessLeft;
                iprocessLeftInitSwitch = false;
            }
            if (iprocessRightInitSwitch) {
                delete iprocessRight;
                iprocessRightInitSwitch = false;
            }

            tCenterX = iprocess->trackCenterX;
            int tEndX = w->frameWidth;

            QImage targetLeft = target.copy( 0, 0, tCenterX, w->frameHeight );
            QImage targetRight = target.copy( tCenterX, 0, tEndX - tCenterX, w->frameHeight );

            // left image process
            iprocessLeft = new imgProcess( targetLeft, targetLeft.width(), targetLeft.height() );   // new imgProcess object
            iprocessLeftInitSwitch = true;
            iprocessLeft->toMono();                                         // convert target to mono
            iprocessLeft->constructValueMatrix( iprocessLeft->imgMono );    // construct mono matrix
            iprocessLeft->detectEdgeSobel();                                // detect edges of the mono image
            //iprocessLeft->thickenEdges();

            iprocessLeft->thetaMin = thetaMinSub;
            iprocessLeft->thetaMax = thetaMaxSub;
            iprocessLeft->thetaStep = thetaStepSub;
            iprocessLeft->centerX = iprocess->edgeWidth / 2;
            iprocessLeft->centerY = 0;

            iprocessLeft->houghTransform();                 // detect lines in edge image

            /*
            iprocessLeft->detectLongestSolidLines();
            iprocessLeft->saveList(iprocessLeft->solidSpaceMain, "data/left1_solidSpaceMain.csv");
            iprocessLeft->saveList(iprocessLeft->solidSpaceMainTrimmed, "data/left1_solidSpaceMainTrimmed.csv");
            iprocessLeft->saveList(iprocessLeft->solidSpaceMainMaximums, "data/left1_solidSpaceMainMaximums.csv");
            iprocessLeft->imgMono.save("data/left1.jpg");
            iprocessLeft->constructHoughExtendedMatrixMajor2Lines();
            iprocessLeft->getImage(iprocessLeft->houghExtendedMatrix, iprocessLeft->imageWidth, iprocessLeft->imageHeight)->save("data/left1_major2.jpg");
            */

            iprocessLeft->calculateHoughMaxs( 50 );         // get max voted line(s)
            iprocessLeft->calcAvgDistAndAngleOfMajors(0.8);    // calc. avg. distance and theta

            iprocessLeft->primaryLineDetected = true;       // bypass line vote check
            iprocessLeft->detectVoidLinesEdge();            // detect void lines on hough lines in EDGE image

            iprocessLeft->voidThreshold = 0;                // bypass void length check
            iprocessLeft->errorEdgeLimit = 0;               // bypass corner void check
            iprocessLeft->angleAvg = 0;                     // bypass angle value check
            iprocessLeft->detectPrimaryVoid();              // decide primary void line & corners/center


            // ------ LEFT IMAGES SAVE
            fileName = savePath + "Left_org" + fileExt;
            iprocessLeft->imgOrginal.save(fileName);

            iprocessLeft->constructHoughMatrixMajor2Lines();
            QImage *houghLeft = iprocessLeft->getImage(iprocessLeft->houghMatrix,iprocessLeft->edgeWidth,iprocessLeft->edgeHeight); // produce hough image
            fileName = savePath + "Left_hough" + fileExt;
            houghLeft->save(fileName);
            delete houghLeft;
            // ------ LEFT IMAGES SAVE

            // right image process
            iprocessRight = new imgProcess( targetRight, targetRight.width(), targetRight.height() );   // new imgProcess object
            iprocessRightInitSwitch = true;
            iprocessRight->toMono();                                        // convert target to mono
            iprocessRight->constructValueMatrix( iprocessRight->imgMono );  // construct mono matrix
            iprocessRight->detectEdgeSobel();                               // detect edges of the mono image
            //iprocessRight->thickenEdges();

            iprocessRight->thetaMin = thetaMinSub;
            iprocessRight->thetaMax = thetaMaxSub;
            iprocessRight->thetaStep = thetaStepSub;
            iprocessRight->centerX = iprocess->edgeWidth / 2;
            iprocessRight->centerY = 0;
            iprocessRight->houghTransform();                // detect lines in edge image

            /*
            iprocessRight->detectLongestSolidLines();
            iprocessRight->saveList(iprocessRight->solidSpaceMain, "data/right1_solidSpaceMain.csv");
            iprocessRight->saveList(iprocessRight->solidSpaceMainTrimmed, "data/right1_solidSpaceMainTrimmed.csv");
            iprocessRight->saveList(iprocessRight->solidSpaceMainMaximums, "data/right1_solidSpaceMainMaximums.csv");
            iprocessRight->imgMono.save("data/right1.jpg");
            iprocessRight->constructHoughExtendedMatrixMajor2Lines();
            iprocessRight->getImage(iprocessRight->houghExtendedMatrix, iprocessRight->imageWidth, iprocessRight->imageHeight)->save("data/right1_major2.jpg");
            */

            iprocessRight->calculateHoughMaxs( 50 );        // get max voted line(s)
            iprocessRight->calcAvgDistAndAngleOfMajors(0.8);   // calc. avg. distance and theta

            iprocessRight->primaryLineDetected = true;      // bypass line vote check
            iprocessRight->detectVoidLinesEdge();           // detect void lines on hough lines in EDGE image

            iprocessRight->voidThreshold = 0;               // bypass void length check
            iprocessRight->errorEdgeLimit = 0;              // bypass corner void check
            iprocessRight->angleAvg = 0;                    // bypass angle value check
            iprocessRight->detectPrimaryVoid();             // decide primary void line & corners/center

            // ------ RIGHT IMAGES SAVE
            fileName = savePath + "Right_org" + fileExt;
            iprocessRight->imgOrginal.save(fileName);

            iprocessRight->constructHoughMatrixMajor2Lines();
            QImage *houghRight = iprocessLeft->getImage(iprocessRight->houghMatrix,iprocessRight->edgeWidth,iprocessRight->edgeHeight); // produce hough image
            fileName = savePath + "Right_hough" + fileExt;
            houghRight->save(fileName);
            delete houghRight;
            // ------ RIGHT IMAGES SAVE

ui->plainTextEdit->appendPlainText(".");


            if (iprocessLeft->detected && iprocessRight->detected){
ui->plainTextEdit->appendPlainText("..");

                iprocess->leftCornerX = iprocessLeft->rightMostCornerX;
                iprocess->leftCornerY = iprocessLeft->rightMostCornerY;
                iprocess->rightCornerX = tCenterX + iprocessRight->leftMostCornerX;
                iprocess->rightCornerY = iprocessRight->leftMostCornerY;
                iprocess->trackCenterX = (iprocess->leftCornerX + iprocess->rightCornerX) / 2;
                iprocess->trackCenterY = (iprocess->leftCornerY + iprocess->rightCornerY) / 2;
            }

        }
    }
}

void setupForm::processSubImageSolidness(){

    if ( w->play && !w->imageGetter->imageList.isEmpty() ){
        target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->play &&  imageLoadedFromFile){
        target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
    }

    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ){

        if (iprocessInitSwitch) {
            delete iprocess;
            iprocessInitSwitch = false;
        }
        iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
        iprocessInitSwitch = true;
        iprocess->toMono();                                 // convert target to mono
        iprocess->constructValueMatrix(iprocess->imgMono);  // construct mono matrix

        iprocess->thetaMin = thetaMinSub;
        iprocess->thetaMax = thetaMaxSub;
        iprocess->thetaStep = thetaStepSub;
        iprocess->centerX = iprocess->edgeWidth / 2;
        iprocess->centerY = 0;
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);                // detect lines in edge image
        iprocess->calculateHoughMaxs( houghLineNo );        // get max voted line(s)
        iprocess->detectLongestSolidLines();

            /*
            iprocess->imgMono.save("data/mono.jpg");
            //iprocess->saveList(iprocess->iprocess, "data/solidSpaceMain.csv");
            iprocess->saveList(iprocess->solidSpaceMainTrimmed, "data/solidSpaceMainTrimmed.csv");
            iprocess->saveList(iprocess->primaryGroup, "data/primaryGroup.csv");
            iprocess->saveList(iprocess->secondaryGroup, "data/secondaryGroup.csv");
            // */

    }

}

void setupForm::captureButton(){

    ui->labelMono->clear();
    ui->labelEdge->clear();
    ui->labelHough->clear();
    ui->labelAnalyze->clear();

    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ) {   // if any image is get

        ui->labelPrimaryLine->hide();               // hide PRIMARY LINE NOT DETECTED message

        // get & check hough parameters from ui
        thetaMin = ui->editHoughThetaMin->text().toInt();
        thetaMax = ui->editHoughThetaMax->text().toInt();
        thetaStep = ui->editHoughThetaStep->text().toFloat();
        if (thetaStep < 0.1){
            thetaStep = 0.1;
            ui->editHoughThetaStep->setText("0.1");
        }

        thetaMinSub = ui->editHoughThetaMinSub->text().toInt();
        thetaMaxSub = ui->editHoughThetaMaxSub->text().toInt();
        thetaStepSub = ui->editHoughThetaStepSub->text().toFloat();
        if (thetaStepSub < 0.1){
            thetaStepSub = 0.1;
            ui->editHoughThetaStepSub->setText("0.1");
        }

        houghLineNo = ui->editHoughLineNo->text().toInt();
        if (houghLineNo == 0){
            houghLineNo = 2;
            ui->editHoughLineNo->setText("2");
        }
        voteThreshold = ui->editHoughThreshold->text().toInt();
        voidThreshold = ui->editVoidThreshold->text().toInt();

        // for file names
        captureTimeStr = QTime::currentTime().toString("hhmmss_");      // for filename

        startTime = w->timeSystem.getSystemTimeMsec();
        // -------------------------------------------------------------------------
        // START IMAGE PROCESSING

        processImage();
        if (!w->subImageProcessingSwitch) {
        } else {
            switch ( w->subImageProcessingType ) {
                case 0:
                    processSubImageVoidness();
                    break;
                case 1:
                    break;
            }
        }

        // END IMAGE PROCESSING
        // -------------------------------------------------------------------------
        endTime = w->timeSystem.getSystemTimeMsec();

        processElapsed = endTime - startTime;


        // UPDATE GUI
        ui->labelTarget->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );
        ui->labelMono->setPixmap( QPixmap::fromImage ( iprocess->imgMono ) );


        QString message = "Analiz " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.";
        ui->plainTextEdit->appendPlainText(message);

        ui->plainTextEdit->appendPlainText(iprocess->statusMessage);    // display message about detection process
        ui->plainTextEdit->appendPlainText("Sol Köşe (x,y) - Merkez (x,y) - Sağ Köşe (x,y): ");
        message = "( " + QString::number(iprocess->leftCornerX) + " , " + QString::number(iprocess->leftCornerY) + " )  -  ( " +
                         QString::number(iprocess->trackCenterX) + " , " + QString::number(iprocess->trackCenterY) + " )  -  ( " +
                         QString::number(iprocess->rightCornerX) + " , " + QString::number(iprocess->rightCornerY) + " )";
        ui->plainTextEdit->appendPlainText(message);


        if ( edgeDetectionState != 0) {

            edge = iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight );   // produce edge image
            ui->labelEdge->setPixmap( QPixmap::fromImage( *edge ) );

            if ( lineDetectAlgos != 0) {

                if (thinJointAlgoActive){
                    iprocess->constructHoughMatrixFindX();   // FOR THINJOINT - edge matrix + coded #houghLineNo lines
                } else {
                    iprocess->constructHoughMatrixMajor2Lines();   // construct hough matrix = edge matrix + coded #houghLineNo lines

                    hough = iprocess->getImage( iprocess->houghMatrix, iprocess->edgeWidth, iprocess->edgeHeight );     // produce hough image
                    ui->labelHough->setPixmap( QPixmap::fromImage( *hough ) );

                    iprocess->cornerImage();
                    ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );
                }


            }

        } else {

            switch ( lineDetectAlgos ) {
                case 0: // none
                    break;
                case 1: // detectLongestSolidLines
                    if (thinJointAlgoActive){
                        //..
                    } else {
                        if ( iprocess->primaryLineFound && iprocess->secondaryLineFound )
                            iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[1], 0 );
                        else
                            if ( iprocess->primaryLineFound )
                                iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[0], 0 );

                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCornerAndPrimaryLines ) );

                        if ( iprocess->primaryLineFound ) {
                            ui->plainTextEdit->appendPlainText("Ortalama Açı: " + QString::number(iprocess->angleAvg));
                        }

                    }
                    break;
                case 2: // detectMainEdges
                    break;
                case 3: // detectPrimaryVoid
                    break;
            }

        }

        if ( lineDetectAlgos != 0) {

        }

        switch ( edgeDetectionState ) {
            case 0: // none
                break;
            case 1: // sobel
                break;
            case 2: // canny
                break;
        }

        switch ( lineDetectAlgos ) {
            case 0: // none
                break;
            case 1: // detectLongestSolidLines
                break;
            case 2: // detectMainEdges
                break;
            case 3: // detectPrimaryVoid
                ui->plainTextEdit->appendPlainText("Bulunan boşluk parametreleri:");
                ui->plainTextEdit->appendPlainText("start.X, start.Y, end.X, end.Y, boy");

                // void line information of primary (avg of max in that case) line
                for (int i=0;i<iprocess->voidSpace.size();i++){

                    message = QString::number(iprocess->voidSpace[i]->start.x()) + ", " +
                              QString::number(iprocess->voidSpace[i]->start.y()) + ", " +
                              QString::number(iprocess->voidSpace[i]->end.x()) + ", " +
                              QString::number(iprocess->voidSpace[i]->end.y()) + ", " +
                              QString::number(iprocess->voidSpace[i]->length);
                    ui->plainTextEdit->appendPlainText(message);
                }

                iprocess->cornerAndPrimaryLineImage( iprocess->primaryLine, iprocess->primaryLine, 0 );
                ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCornerAndPrimaryLines ) );

                //iprocess->cornerImage();
                //ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );
                break;
        }

        if ( iprocess->major2Lines.size() > 0) {
            ui->plainTextEdit->appendPlainText("Çizgi {Uzunluk-Mesafe-Açı-Başlagıç(x,y)-Bitiş(x,y)}");
            for (int i=0;i<iprocess->major2Lines.size();i++){
                message = QString::number(iprocess->major2Lines[i].length) + " - " +
                          QString::number(iprocess->major2Lines[i].distance, 'f', 1) + " - " +
                          QString::number(iprocess->major2Lines[i].angle, 'f', 1) + " -   ( " +
                          QString::number(iprocess->major2Lines[i].start.x()) + ", " +
                          QString::number(iprocess->major2Lines[i].start.y()) + " ) - ( " +
                          QString::number(iprocess->major2Lines[i].end.x()) + " , " +
                          QString::number(iprocess->major2Lines[i].end.y()) + " )";
                ui->plainTextEdit->appendPlainText(message);
            }
        }


        if ( w->subImageProcessingSwitch ) {

            switch ( w->subImageProcessingType ) {

                case 0:     // VOID AREAS
                    edge = iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight );   // produce edge image
                    ui->labelEdge->setPixmap( QPixmap::fromImage( *edge ) );

                    iprocess->constructHoughMatrix();   // construct hough matrix = edge matrix + coded #houghLineNo lines
                    //iprocess->constructHoughMatrixPrimaryLines(iprocessLeft->primaryLine, iprocessRight->primaryLine, tCenterX);
                    hough = iprocess->getImage( iprocess->houghMatrix, iprocess->edgeWidth, iprocess->edgeHeight );     // produce hough image
                    ui->labelHough->setPixmap( QPixmap::fromImage( *hough ) );

                    ui->labelVoteAvg->setText( QString::number( voteAvg ) );

                    iprocess->cornerImage();
                    ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );

                    break;

                case 1:     // SOLID LINES
                    break;
            }

        } else {    // standard HT

/*
            / *
            */

       }

        ui->plainTextEdit->appendPlainText("-----------------");

        captured = true;

    } else {

        ui->plainTextEdit->appendPlainText(alarm6);
   }

}

bool setupForm::saveButton(){

    bool saveStatus = true;

    if (captured){

        if (ui->checkSaveTarget->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseTarget + fileExt;
            saveStatus = saveStatus && iprocess->imgOrginal.save(fileName);
        }

        if (ui->checkSaveMono->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseMono + fileExt;
            saveStatus = saveStatus && iprocess->imgMono.save(fileName);
        }

        if (ui->checkSaveEdge->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseEdge + fileExt;
            saveStatus = saveStatus && edge->save(fileName);
        }

        if (ui->checkSaveHough->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseHough + fileExt;
            saveStatus = saveStatus && hough->save(fileName);
        }

        if (ui->checkSaveAnalyze->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseCorner + fileExt;
            saveStatus = saveStatus && iprocess->imgCorner.save(fileName);
        }

        // matrix save operations
        if (ui->checkSaveMonoMatrix->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseMono + ".csv";
            saveStatus = saveStatus && iprocess->saveMatrix(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, fileName);
        }

        if (ui->checkSaveEdgeMatrix->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseEdge + ".csv";
            saveStatus = saveStatus && iprocess->saveMatrix(iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, fileName);
        }

        if (ui->checkSaveHoughMatrix->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseHough + ".csv";
            saveStatus = saveStatus && iprocess->saveMatrix(iprocess->houghMatrix, iprocess->edgeWidth, iprocess->edgeHeight, fileName);
        }

        if (ui->checkSaveHoughLines->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseHough + "Lines" + ".csv";
            saveStatus = saveStatus && iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, fileName);
        }

        if (ui->checkSaveHoughSpace->isChecked()){
            fileName = savePath + captureTimeStr + fileBaseHough + "Space" + ".csv";
            saveStatus = saveStatus && iprocess->saveMatrix(iprocess->houghSpace, iprocess->houghThetaSize, iprocess->houghDistanceMax, fileName);
        }

        captured = false;
    }

    ui->plainTextEdit->appendPlainText("Dosyalar kaydedildi");

    return saveStatus;
}

void setupForm::getParameters(){

    thetaMin = w->thetaMin;
    thetaMax = w->thetaMax;
    thetaStep = w->thetaStep;

    ui->checkSubImage->setChecked(w->subImageProcessingSwitch);
    thetaMinSub = w->thetaMinSub;
    thetaMaxSub = w->thetaMaxSub;
    thetaStepSub = w->thetaStepSub;

    houghLineNo = w->houghLineNo;
    voteThreshold = w->voteThreshold;
    voidThreshold = w->voidThreshold;

    ui->editHoughThetaMin->setText(QString::number(thetaMin));
    ui->editHoughThetaMax->setText(QString::number(thetaMax));
    ui->editHoughThetaStep->setText(QString::number(thetaStep, 'f', 1));

    ui->editHoughThetaMinSub->setText(QString::number(thetaMinSub));
    ui->editHoughThetaMaxSub->setText(QString::number(thetaMaxSub));
    ui->editHoughThetaStepSub->setText(QString::number(thetaStepSub));
    ui->editHoughThetaMinSub->setEnabled(w->subImageProcessingSwitch);
    ui->editHoughThetaMaxSub->setEnabled(w->subImageProcessingSwitch);
    ui->editHoughThetaStepSub->setEnabled(w->subImageProcessingSwitch);

    ui->editHoughLineNo->setText(QString::number(houghLineNo));
    ui->editHoughThreshold->setText(QString::number(voteThreshold));
    ui->editVoidThreshold->setText(QString::number(voidThreshold));

    ui->editFPS->setText(QString::number(w->fpsTarget));
    ui->editIPI->setText(QString::number(w->iprocessInterval));

    thinJointAlgoActive = w->thinJointAlgoActive;

}

void setupForm::clearButton(){

    ui->plainTextEdit->clear();
}

void setupForm::subImageCheck(){

    w->subImageProcessingSwitch = ui->checkSubImage->isChecked();
    ui->editHoughThetaMinSub->setEnabled( ui->checkSubImage->isChecked() );
    ui->editHoughThetaMaxSub->setEnabled( ui->checkSubImage->isChecked() );
    ui->editHoughThetaStepSub->setEnabled( ui->checkSubImage->isChecked() );
    ui->subTypeBox->setEnabled( ui->checkSubImage->isChecked() );
}

void setupForm::subType(){

    switch ( ui->subTypeBox->currentIndex() ) {
        case 0:
            w->subImageProcessingType = 0;
            break;
        case 1:
            w->subImageProcessingType = 1;
            break;
    }
}

void setupForm::saveExitButton(){

    w->thetaMin = thetaMin = ui->editHoughThetaMin->text().toInt();;
    w->thetaMax = thetaMax = ui->editHoughThetaMax->text().toInt();
    w->thetaStep = thetaStep = ui->editHoughThetaStep->text().toFloat();

    w->thetaMinSub = thetaMinSub = ui->editHoughThetaMinSub->text().toInt();;
    w->thetaMaxSub = thetaMaxSub = ui->editHoughThetaMaxSub->text().toInt();
    w->thetaStepSub = thetaStepSub = ui->editHoughThetaStepSub->text().toFloat();

    w->houghLineNo = houghLineNo = ui->editHoughLineNo->text().toInt();
    w->voteThreshold = voteThreshold = ui->editHoughThreshold->text().toInt();
    w->voidThreshold = voidThreshold = ui->editVoidThreshold->text().toInt();

    w->fpsTarget = ui->editFPS->text().toInt();
    w->frameInterval = 1000 / w->fpsTarget;
    w->iprocessInterval = ui->editIPI->text().toInt();

    w->writeSettings();

    this->close();
}

void setupForm::zctrlBox(){

    w->zControlActive = ui->zctrlBox->isChecked();
}

void setupForm::hardControlStartBox(){

    w->hardControlStart = ui->hardControlStartBox->isChecked();

    w->readMachineStatus = w->hardControlStart;
    ui->readMachineStatusBox->setChecked( w->readMachineStatus );
}

void setupForm::widthControl(){

    w->jointWidthControlActive = ui->widthControlBox->isChecked();
}

void setupForm::readMachineStatusBox(){

    w->readMachineStatus = ui->readMachineStatusBox->isChecked();
}

void setupForm::readDistanceBox(){

    w->readDistance = ui->readDistanceBox->isChecked();
}

void setupForm::dynamicAlgoBox(){

    w->dynamicAlgo = ui->dynamicAlgoBox->isChecked();
}

void setupForm::readWeldSeamBox(){

    w->readWeldSeam = ui->readWeldSeamBox->isChecked();
}

void setupForm::timeControlBox(){

    w->timeControl = ui->timeControlBox->isChecked();
}

void setupForm::lineDetectionBox(){

    w->lineDetection = ui->lineDetectionBox->isChecked();
}

void setupForm::editLineScore(){

    w->lineScoreLimit = ui->editLineScore->text().toInt();
}

void setupForm::exitButton(){

    this->close();
}

setupForm::~setupForm(){

    delete ui;

    if (iprocessInitSwitch) {
        delete iprocess;
        iprocessInitSwitch = false;
    }
    if (iprocessLeftInitSwitch) {
        delete iprocessLeft;
        iprocessLeftInitSwitch = false;
    }
    if (iprocessRightInitSwitch) {
        delete iprocessRight;
        iprocessRightInitSwitch = false;
    }

    if (w->zControlActive || w->readDistance){
        //w->ui->zControlButton->setIcon(w->zIconEnabled);
        w->ui->labelDistance->show();
        w->ui->labelDistanceTag->show();
        w->ui->labelDistanceTag2->show();

    } else {
        //w->ui->zControlButton->setIcon(w->zIconDisabled);
        w->ui->labelDistance->hide();
        w->ui->labelDistanceTag->hide();
        w->ui->labelDistanceTag2->hide();
    }

    if (w->timeControl){
        w->ui->labelTimeTag->show();
        w->ui->labelTimeTag2->show();
        w->ui->timeEdit->show();
        w->ui->timeEdit->setText(QString::number(w->timeLimit));
    } else {
        w->ui->labelTimeTag->hide();
        w->ui->labelTimeTag2->hide();
        w->ui->timeEdit->hide();
    }

    if ( w->lineDetection )
        w->clearTrack();

}

void setupForm::on_captureButton_2_clicked(){

    w->loadedFileNamewPath = QFileDialog::getOpenFileName(this,
        //tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
        tr("Open Image"), "C:/xampp/htdocs/images", tr("Image Files (*.png *.jpg *.bmp)"));


    if (!w->loadedFileNamewPath.isEmpty() && !w->loadedFileNamewPath.isNull()){

        imageLoadedFromFile = true;

        w->fileOpenDir = QFileInfo(w->loadedFileNamewPath).absoluteDir();
        w->filesInDirList = w->fileOpenDir.entryList(w->fileFilters, QDir::Files);

        //foreach(QString temp, w->filesInDirList){ qDebug() << temp; }

        w->loadedFileName = QFileInfo(w->loadedFileNamewPath).fileName();
        w->filesInDirListIndex = w->filesInDirList.indexOf(w->loadedFileName,0);

        ui->fileSlider->setMaximum(w->filesInDirList.size() - 1);
        ui->fileSlider->setValue(w->filesInDirListIndex);

        w->imageFile.load(w->loadedFileNamewPath);
        w->imageFileChanged = w->imageFile;
        w->ui->imageFrame->setPixmap( QPixmap::fromImage( w->imageFile ));
        captureButton();
    }
}

void setupForm::update(){

    QImage step1 = changeBrightness(w->imageFile, w->brightnessVal);
    QImage step2 = changeContrast(step1, w->contrastVal);
    w->imageFileChanged = changeGamma(step2, w->gammaVal);
    w->ui->imageFrame->setPixmap( QPixmap::fromImage( w->imageFileChanged ));

    if (ui->checkProcessing->isChecked())
        captureButton();
}

void setupForm::on_brightnessSlider_sliderReleased(){

    w->brightnessVal = ui->brightnessSlider->value();
    //ui->plainTextEdit->appendPlainText(QString::number(w->brightnessVal));
    update();
}

void setupForm::on_brightnessSlider_sliderMoved(int position){

    ui->labelBrightness->setText(QString::number(position));
}

void setupForm::on_contrastSlider_sliderReleased(){

    w->contrastVal = ui->contrastSlider->value();
    update();
}

void setupForm::on_contrastSlider_sliderMoved(int position){

    ui->labelContrast->setText(QString::number(position));
}

void setupForm::on_gammaSlider_sliderReleased(){

    w->gammaVal = ui->gammaSlider->value();
    update();
}

void setupForm::on_gammaSlider_sliderMoved(int position){

    ui->labelGamma->setText(QString::number(position));
}

void setupForm::on_brightnessReset_clicked(){

    w->brightnessVal = 0;
    ui->brightnessSlider->setValue(0);
    ui->labelBrightness->setText(QString::number(0));
    update();
}

void setupForm::on_contrastReset_clicked(){

    w->contrastVal = 100;
    ui->contrastSlider->setValue(100);
    ui->labelContrast->setText(QString::number(100));
    update();
}

void setupForm::on_gammaReset_clicked(){

    w->gammaVal = 100;
    ui->gammaSlider->setValue(100);
    ui->labelGamma->setText(QString::number(100));
    update();
}

void setupForm::on_fileSlider_sliderMoved(int position){

    w->filesInDirListIndex = position;
    w->loadedFileName = w->filesInDirList.at(position);

    //qDebug() << w->filesInDirListIndex << "." << w->loadedFileName;
    w->imageFile.load(w->fileOpenDir.path() + "/" + w->loadedFileName);
    update();
}

void setupForm::on_gaussSizeSlider_sliderMoved(int position){

    w->gaussianSize = 2*position+1;
    ui->labelGaussSize->setText(QString::number(w->gaussianSize));
    update();
}

void setupForm::on_gaussSDevSlider_sliderMoved(int position){

    w->stdDev = position / 10.0;
    ui->labelGaussSDev->setText(QString::number(w->stdDev));
    update();
}

void setupForm::on_cannyThinningBox_clicked(){

    w->cannyThinning = ui->cannyThinningBox->isChecked();
}

void setupForm::on_algorithmBox_currentIndexChanged(int index){

    algorithmType = index;
}

void setupForm::on_radioLaser_clicked() {

    w->thinJointAlgoActive = false;
    thetaMin = w->thetaMinHorLine;
    thetaMax = w->thetaMaxHorLine;
    ui->editHoughThetaMin->setText(QString::number(thetaMin));
    ui->editHoughThetaMax->setText(QString::number(thetaMax));
}

void setupForm::on_radioWoLaser_clicked() {

    w->thinJointAlgoActive = true;
    thetaMin = w->thetaMinVerLine;
    thetaMax = w->thetaMaxVerLine;
    ui->editHoughThetaMin->setText(QString::number(thetaMin));
    ui->editHoughThetaMax->setText(QString::number(thetaMax));
}

//fileName = savePath + "t" + QString::number(i) + "_houghlines" + ".csv";
        //iprocessSub[i]->saveMatrix(iprocessSub[i]->houghLines, 3, iprocessSub[i]->houghLineNo, fileName);

        //iprocessSub[i]->calcAvgDistAndAngleOfMajors();
        //ui->plainTextEdit->appendPlainText(QString::number(iprocessSub[i]->distanceAvg,'f',3) + " " + QString::number(iprocessSub[i]->thetaAvg,'f',3));


    //target.save(savePath + "target" + fileExt);

        /*
        iprocessSub[i]->constructHoughMatrixAvgLine();
        houghImage[i] = iprocessSub[i]->getImage(iprocessSub[i]->houghMatrix, iprocessSub[i]->edgeWidth, iprocessSub[i]->edgeHeight); // produce hough image
        fileName = savePath + "t" + QString::number(i) + "_hough" + fileExt;
        houghImage[i]->save(fileName);

        lineImage[i] = iprocessSub[i]->getImage(iprocessSub[i]->valueMatrix, iprocessSub[i]->imageWidth, iprocessSub[i]->imageHeight); // produce line image
        fileName = savePath + "t" + QString::number(i) + "_line" + fileExt;
        lineImage[i]->save(fileName);
        edgeImage[i] = iprocessSub[i]->getImage(iprocessSub[i]->edgeMatrix, iprocessSub[i]->edgeWidth, iprocessSub[i]->edgeHeight); // produce line image
        fileName = savePath + "edge" + QString::number(i) + fileExt;
        edgeImage[i]->save(fileName);

        edgeThickenedImage[i] = iprocessSub[i]->getImage(iprocessSub[i]->edgeThickenedMatrix, iprocessSub[i]->edgeWidth, iprocessSub[i]->edgeHeight); // produce line image
        fileName = savePath + "edgethickened" + QString::number(i) + fileExt;
        edgeThickenedImage[i]->save(fileName);

        //targetSub[i].save(savePath + "target" + QString::number(i)+ fileExt);

        iprocessSub[i]->saveList(iprocessSub[i]->solidSpaceMainTrimmed, savePath + "solidSpaceMainTrimmed" + QString::number(i)+ ".csv");
        iprocessSub[i]->saveList(iprocessSub[i]->solidSpaceMainMaximums, savePath + "solidSpaceMainMaximums" + QString::number(i)+ ".csv");

        ui->plainTextEdit->appendPlainText("image: " + QString::number(i));
        ui->plainTextEdit->appendPlainText("maxlength: " + QString::number(iprocessSub[i]->maxSolidLineLength));

        for (int j = 0; j < iprocessSub[i]->majorList.size(); j++)
            ui->plainTextEdit->appendPlainText("start: " + QString::number(iprocessSub[i]->majorList[j]->startIndex) + " end: " + QString::number(iprocessSub[i]->majorList[j]->endIndex));

        iprocessSub[i]->saveList(iprocessSub[i]->majorLines, savePath + "majorLines" + QString::number(i)+ ".csv");
        iprocessSub[i]->saveList(iprocessSub[i]->major2Lines, savePath + "major2Lines" + QString::number(i)+ ".csv");
        */




/*
void setupForm::processSubImageSolidness(){

...

        if ( iprocessLeft->primaryLine.length > iprocessRight->primaryLine.length  && iprocessRight->primaryLine.length != -1 ) {

            // right image re-process
            tCenterX = iprocessLeft->primaryLine.end.x() + 5;

            targetRight = target.copy( tCenterX, 0, tWidth - tCenterX, w->frameHeight );

            if (iprocessRightInitSwitch) {
                delete iprocessRight;
                iprocessRightInitSwitch = false;
            }
            iprocessRight = new imgProcess( targetRight, targetRight.width(), targetRight.height() );   // new imgProcess object
            iprocessRightInitSwitch = true;
            iprocessRight->toMono();                                        // convert target to mono
            iprocessRight->constructValueMatrix( iprocessRight->imgMono );  // construct mono matrix
            iprocessRight->detectEdgeSobel();                               // detect edges of the mono image
            iprocessRight->thickenEdges();

            iprocessRight->thetaMin = thetaMinSub;
            iprocessRight->thetaMax = thetaMaxSub;
            iprocessRight->thetaStep = thetaStepSub;
            //iprocessRight->houghTransform();                                // detect lines in edge image
            iprocessRight->detectLongestSolidLines();

                //iprocessRight->saveList(iprocessRight->solidSpaceMain, "data/right2_solidSpaceMain.csv");
                iprocessRight->saveList(iprocessRight->solidSpaceMainTrimmed, "data/right2_solidSpaceMainTrimmed.csv");
                iprocessRight->saveList(iprocessRight->solidSpaceMainMaximums, "data/right2_solidSpaceMainMaximums.csv");
                iprocessRight->imgMono.save("data/right2.jpg");
                iprocessRight->constructHoughExtendedMatrixMajor2Lines();
                iprocessRight->getImage(iprocessRight->houghExtendedMatrix, iprocessRight->imageWidth, iprocessRight->imageHeight)->save("data/right2_major2.jpg");

        } else
        if ( iprocessLeft->primaryLine.length < iprocessRight->primaryLine.length && iprocessLeft->primaryLine.length != -1 ) {

            // left image re-process
            tCenterX = iprocess->trackCenterX + iprocessRight->primaryLine.start.x() - 5;

            targetLeft = target.copy( 0, 0, tCenterX, w->frameHeight );

            if (iprocessLeftInitSwitch) {
                delete iprocessLeft;
                iprocessLeftInitSwitch = false;
            }
            iprocessLeft = new imgProcess( targetLeft, targetLeft.width(), targetLeft.height() );   // new imgProcess object
            iprocessLeftInitSwitch = true;
            iprocessLeft->toMono();                                         // convert target to mono
            iprocessLeft->constructValueMatrix( iprocessLeft->imgMono );    // construct mono matrix
            iprocessLeft->detectEdgeSobel();                                // detect edges of the mono image
            iprocessLeft->thickenEdges();

            iprocessLeft->thetaMin = thetaMinSub;
            iprocessLeft->thetaMax = thetaMaxSub;
            iprocessLeft->thetaStep = thetaStepSub;
            //iprocessLeft->houghTransform();                                 // detect lines in edge image
            iprocessLeft->detectLongestSolidLines();

            // to recover for right image coord.
            tCenterX = iprocess->trackCenterX;

                iprocessLeft->saveList(iprocessLeft->solidSpaceMainTrimmed, "data/left2_solidSpaceMainTrimmed.csv");
                iprocessLeft->saveList(iprocessLeft->solidSpaceMainMaximums, "data/left2_solidSpaceMainMaximums.csv");
                iprocessLeft->imgMono.save("data/left2.jpg");
                iprocessLeft->constructHoughExtendedMatrixMajor2Lines();
                iprocessLeft->getImage(iprocessLeft->houghExtendedMatrix, iprocessLeft->imageWidth, iprocessLeft->imageHeight)->save("data/left2_major2.jpg");
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


        //iprocessRight->saveList(iprocessRight->major2Lines, savePath + "major2Lines_right.csv");
        //iprocessRight->saveList(iprocessRight->solidSpaceMainMaximums, savePath + "solidSpaceMainMaximums_right.csv");

        // ------ LEFT AND RIGHT IMAGES SAVE
        iprocessLeft->imgMono.save("data/left.jpg");
        iprocessRight->imgMono.save("data/right.jpg");
        iprocessLeft->getImage(iprocessLeft->edgeThickenedMatrix, iprocessLeft->imageWidth, iprocessLeft->imageHeight)->save("data/left_edgethickned.jpg");
        iprocessRight->getImage(iprocessRight->edgeThickenedMatrix, iprocessRight->imageWidth, iprocessRight->imageHeight)->save("data/right_edgethickned.jpg");

        iprocessRight->constructHoughExtendedMatrixMajor2Lines();
        iprocessRight->getImage(iprocessRight->houghExtendedMatrix, iprocessRight->imageWidth, iprocessRight->imageHeight)->save("data/right_major2.jpg");

        // ------ LEFT AND RIGHT IMAGES SAVE
    }
}
*/


void setupForm::on_edgeDetectionBox_currentIndexChanged(int index){

    edgeDetectionState = index;
}

void setupForm::on_detecAlgoBox_currentIndexChanged(int index){

    lineDetectAlgos = index;
}

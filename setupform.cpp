
#include "setupform.h"
#include "ui_setupform.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextCodec>

extern MainWindow *w;

setupForm::setupForm(QWidget *parent) : QDialog(parent), ui(new Ui::setupForm){

    fileBaseTarget = "target";
    fileBaseMono = "mono";
    fileBaseEdge = "edge";
    fileBaseHough = "hough";
    fileBaseCorner = "corner";
    fileExt = ".jpg";
    savePath = "data/";
    path = savePath;
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

    ui->zctrlBox->setChecked( w->zControlActive );

    ui->widthControlBox->setChecked( w->jointWidthControlActive );

    ui->hardControlStartBox->setChecked( w->hardControlStart );

    ui->readMachineStatusBox->setChecked( w->readMachineStatus );

    ui->readDistanceBox->setChecked( w->readDistance );

    ui->dynamicAlgoBox->setChecked( w->dynamicAlgo );

    ui->readWeldSeamBox->setChecked( w->readWeldSeam );

    ui->timeControlBox->setChecked( w->timeControl );

    ui->lineDetectionBox->setChecked( w->lineDetection );

    ui->alignGuideBox->setChecked( w->alignGuide2TrackCenter );

    ui->editLineScore->setText( QString::number( w->lineScoreLimit ) );

    ui->labelBrightness->setText( QString::number(brightnessVal) );
    ui->brightnessSlider->setValue( brightnessVal );

    ui->labelContrast->setText( QString::number(contrastVal) );
    ui->contrastSlider->setValue( contrastVal );

    ui->labelGamma->setText( QString::number(gammaVal) );
    ui->gammaSlider->setValue( gammaVal );

    ui->labelGaussSize->setText( QString::number(gaussianSize) );
    ui->gaussSizeSlider->setValue( (gaussianSize-1)/2 );

    ui->labelGaussSDev->setText( QString::number(stdDev) );
    ui->gaussSDevSlider->setValue( (int)(stdDev*10) );

    ui->cannyThinningBox->setChecked( w->cannyThinning );

    int algo = algorithmType;
    if ( thinJointAlgoActive ){
        ui->radioLaser->setChecked(false);
        ui->radioWoLaser->setChecked(true);
        on_radioWoLaser_clicked();
    } else {
        ui->radioWoLaser->setChecked(false);
        ui->radioLaser->setChecked(true);
        on_radioLaser_clicked();
    }
    algorithmType = algo;

    ui->edgeDetectionBox->setCurrentIndex( edgeDetectionState );
    ui->algorithmBox->setCurrentIndex( algorithmType );
    ui->mainEdgesSlider->setValue( mainEdgesNumber );
    ui->labelMainEdgesNumber->setText(QString::number(mainEdgesNumber));

    if (edgeDetectionState == 2 || edgeDetectionState == 3){
        ui->gaussSizeSlider->setEnabled(true);
        ui->gaussSDevSlider->setEnabled(true);
        ui->label_19->setEnabled(true);
        ui->label_20->setEnabled(true);
        ui->cannyThinningBox->setEnabled(true);
    } else {
        ui->gaussSizeSlider->setEnabled(false);
        ui->gaussSDevSlider->setEnabled(false);
        ui->label_19->setEnabled(false);
        ui->label_20->setEnabled(false);
        ui->cannyThinningBox->setEnabled(false);
    }

    ui->cameraEnhancementsBox->setChecked(w->applyCameraEnhancements);

    // graphs
    scene1 = new QGraphicsScene();
    sceneRect1 = ui->graphicsView->geometry();
    ui->graphicsView->setScene(scene1);

    scene2 = new QGraphicsScene();
    sceneRect2 = ui->graphicsView2->geometry();
    ui->graphicsView2->setScene(scene2);

    penAxis.setColor(Qt::black);    penAxis.setWidth(2);
    penGraph.setColor(Qt::red);     penGraph.setWidth(1);

    //---------------------------------------------------
    ui->checkSubImage->hide();
    ui->editHoughThetaMaxSub->hide();
    ui->editHoughThetaMinSub->hide();
    ui->editHoughThetaStepSub->hide();

}

void setupForm::edgeDetection(imgProcess *iprocess){

    switch ( edgeDetectionState ) {
        case 0: // NONE
            averaging = true;
            matrixFlag = true;
            break;
        case 1: // SOBEL
                /**/if (saveAnalysis) {
                        edgePath = path + "/sobel/";
                        QDir().mkdir(edgePath);
                    }
            iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
            iprocess->detectEdgeSobel(); // detect edges of the mono image
                /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"01-edge matrix.csv");
                /*D*/if (saveAnalysis) iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"02-edge image.jpg");
            iprocess->houghTransformFn(iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight);
                /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghSpace, iprocess->houghThetaSize, iprocess->houghDistanceMax, edgePath+"03-hough space.csv");
            averaging = false;
            matrixFlag = false;
            break;
        case 2: // CANNY 4
                /**/if (saveAnalysis) {
                        edgePath = path + "/canny4/";
                        QDir().mkdir(edgePath);
                    }
            iprocess->prepareCannyArrays();
            iprocess->constructGaussianMatrix(gaussianSize, stdDev);
                /*D*/if (saveAnalysis) iprocess->saveMatrix(iprocess->gaussianMatrix, iprocess->gaussianMatrixSize, iprocess->gaussianMatrixSize, edgePath+"01-gaussian matrix.csv");
            for (int i = 0; i < 4 ; i++){
                iprocess->constructValueMatrix( iprocess->imgOrginal, i );
                    /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, edgePath+"02-value matrix"+QString::number(i)+".csv" );
                iprocess->gaussianBlur();
                    /*D*/if (saveAnalysis) iprocess->getImage( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight )->save(edgePath+"03-blurred image"+QString::number(i)+".jpg");
                iprocess->detectEdgeSobelwDirections();
                    /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"04-edge matrix"+QString::number(i)+".csv");
                    /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeGradientMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"05-edge gradients matrix"+QString::number(i)+".csv");
                    /*D*/if (saveAnalysis) iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"06-edge image"+QString::number(i)+".jpg");
                iprocess->nonMaximumSuppression(w->cannyThinning);
                    /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"07-edge suppressed matrix"+QString::number(i)+".csv");
                    /*D*/if (saveAnalysis) iprocess->getImage( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"08-edge suppressed image"+QString::number(i)+".jpg");
                iprocess->cannyThresholding(true);
                    /*D*/if (DEBUG) ui->plainTextEdit->appendPlainText("cannyThresholding"+QString::number(i)+" lo, med, hi: "+QString::number(iprocess->loValue) +", "+QString::number(iprocess->medianValue) +", "+QString::number(iprocess->hiValue));
                    /*D*/if (saveAnalysis) iprocess->getImage_cannyThresholds(QImage::Format_RGB16)->save(edgePath+"09-edge strongs weaks image"+QString::number(i)+".jpg");
                iprocess->edgeTracing();
                    /*D*/if (saveAnalysis) iprocess->getImage_cannyTracedEdges(QImage::Format_RGB16)->save(edgePath+"10-edge traced image"+QString::number(i)+".jpg");
                    /*D*/if (saveAnalysis) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"11-canny image"+QString::number(i)+".jpg");
                    /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"12-edge map matrix"+QString::number(i)+".csv");
                iprocess->assignEdgeMap();
            }
            iprocess->mergeEdgeMaps();
                /*D*/if (saveAnalysis) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"13-canny combined image.jpg");
                /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"14-edge map matrix.csv");

            for (int y = 0; y < iprocess->edgeHeight; y++)
                for (int x = 0; x < iprocess->edgeWidth; x++){
                    if (iprocess->edgeMapMatrix[y][x])
                        iprocess->edgeMatrix[y][x]=255;
                    else
                        iprocess->edgeMatrix[y][x]=0;
                }
            iprocess->houghTransformEdgeMap();
                /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghSpace, iprocess->houghThetaSize, iprocess->houghDistanceMax, edgePath+"15-hough space.csv");
            averaging = false;
            matrixFlag = false;
            break;
        case 3: // CANNY 1
                /**/if (saveAnalysis) {
                        edgePath = path + "/canny1/";
                        QDir().mkdir(edgePath);
                    }
            iprocess->prepareCannyArrays();
            iprocess->constructGaussianMatrix(gaussianSize, stdDev);
                /*D*/if (saveAnalysis) iprocess->saveMatrix(iprocess->gaussianMatrix, iprocess->gaussianMatrixSize, iprocess->gaussianMatrixSize, edgePath+"01-gaussian matrix.csv");
            iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );
                /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, edgePath+"02-value matrix.csv" );
            iprocess->gaussianBlur();
                /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, edgePath+"03-value matrix.csv" );
                /*D*/if (saveAnalysis) iprocess->getImage( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight )->save(edgePath+"03-blurred image.jpg");
            ui->labelMono->setPixmap( QPixmap::fromImage ( *iprocess->getImage( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight ) ) );
            iprocess->detectEdgeSobelwDirections();
                /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"04-edge matrix.csv");
                /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeGradientMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"05-edge gradients matrix.csv");
                /*D*/if (saveAnalysis) iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"06-edge image.jpg");
            iprocess->nonMaximumSuppression(w->cannyThinning);
                /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"07-edge suppressed matrix.csv");
                /*D*/if (saveAnalysis) iprocess->getImage( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"08-edge suppressed image.jpg");
            iprocess->cannyThresholding(true);
                /*D*/if (DEBUG) ui->plainTextEdit->appendPlainText("cannyThresholding lo, med, hi: "+QString::number(iprocess->loValue) +", "+QString::number(iprocess->medianValue) +", "+QString::number(iprocess->hiValue));
                /*D*/if (saveAnalysis) iprocess->getImage_cannyThresholds(QImage::Format_RGB16)->save(edgePath+"09-edge strongs weaks image.jpg");
            iprocess->edgeTracing();
                /*D*/if (saveAnalysis) iprocess->getImage_cannyTracedEdges(QImage::Format_RGB16)->save(edgePath+"10-edge traced image.jpg");
                /*D*/if (saveAnalysis) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"11-canny image.jpg");
                /*D*/if (saveAnalysis) iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"12-edge map matrix.csv");

            for (int y = 0; y < iprocess->edgeHeight; y++)  // to display edge image
                for (int x = 0; x < iprocess->edgeWidth; x++){
                    if (iprocess->edgeMapMatrix[y][x])
                        iprocess->edgeMatrix[y][x]=255;
                    else
                        iprocess->edgeMatrix[y][x]=0;
                }
            iprocess->houghTransformEdgeMap();
                /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghSpace, iprocess->houghThetaSize, iprocess->houghDistanceMax, edgePath+"13-hough space.csv");
            matrixFlag = false;
            break;
    }

}

void setupForm::Algo1(imgProcess *iprocess){
// LASER: mono/edge(matrixFlag) > houghTr > detectLongestSolidLines

    if (edgeDetectionState == 0){
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghSpace, iprocess->houghThetaSize, iprocess->houghDistanceMax, path+"03-hough space.csv");
    }

    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"04-max hough lines matrix -dist-angle-vote.csv");

    iprocess->detectLongestSolidLines(averaging, matrixFlag);
    algoPrerequestsOk = true;
    captured = true;
}

void setupForm::Algo2(imgProcess *iprocess){
// LASER: mono/edge(matrixFlag) > houghTr > detectPrimaryVoid

    if (edgeDetectionState == 0){
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghSpace, iprocess->houghThetaSize, iprocess->houghDistanceMax, path+"03-hough space.csv");
    }

    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"04-max hough lines matrix -dist-angle-vote.csv");

    iprocess->calcAvgDistAndAngle( houghLineNo );           // calc. avg. distance and theta
    voteAvg = iprocess->calcVoteAvg();                      // avg. value of max voted line(s)

    iprocess->voteThreshold = voteThreshold;                // acceptable vote value low-limit
    if ( !iprocess->checkPrimaryLine() )                    // is max voted line  above the low-limit?
        ui->labelPrimaryLine->show();
    iprocess->detectVoidLines();                            // detect void lines on hough lines in MONO image

    iprocess->voidThreshold = voidThreshold;                // void threshold to decide max void as primary
    iprocess->detectPrimaryVoid();                          // decide primary void line & corners/center
    algoPrerequestsOk = true;
    captured = true;
}

void setupForm::Algo3(imgProcess *iprocess){
// woLASER: edge > houghTr > detectMainEdges

    if (edgeDetectionState != 0) {
        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"03-max hough lines matrix -dist-angle-vote.csv");
        iprocess->thinCornerNum = mainEdgesNumber;
        iprocess->detectMainEdges(thinJointAlgoActive, DEBUG);
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"04-max hough lines distance sorted.csv");
            /**/if (DEBUG) for (int i=0; i<iprocess->mainEdgesList.size();i++)
                    ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue));
        algoPrerequestsOk = true;
        captured = true;
    } else {
        ui->plainTextEdit->appendPlainText("Bir kenar tespiti algoritması seçilmelidir");
        algoPrerequestsOk = false;
    }
}

void setupForm::Algo4(imgProcess *iprocess){
// woLASER: value > detectThinJointCenter

    iprocess->constructValueMatrix( iprocess->imgOrginal );
    iprocess->detectThinJointCenter(3, 31);
    algoPrerequestsOk = true;
    captured = true;
}

void setupForm::Algo5(imgProcess *iprocess){
// woLASER: value > contrast matrix > houghTr > calcAvgDistAndAngleOfMajors

    iprocess->constructValueMatrix( iprocess->imgOrginal );
    iprocess->constructContrastMatix(3);
        /**/if (saveAnalysis) iprocess->saveMatrix( iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight, path+"03-contrast matrix.csv" );
        /**/if (saveAnalysis) iprocess->getImageContrast().save(path+"04-contrast image.jpg");
    iprocess->houghTransformContrast();;
    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"05-max hough lines matrix -dist-angle-vote.csv");
    iprocess->calcAvgDistAndAngleOfMajors(0.30);    // calc. avg. distance and theta
        /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLinesSorted_size, path+"06-max hough lines distance sorted.csv");
        /**/if (DEBUG) {
                ui->plainTextEdit->appendPlainText("major1; dist: " + QString::number(iprocess->major2Lines[0].distance) + ", angle: " + QString::number(iprocess->major2Lines[0].angle));
                ui->plainTextEdit->appendPlainText("major2; dist: " + QString::number(iprocess->major2Lines[1].distance) + ", angle: " + QString::number(iprocess->major2Lines[1].angle));
                ui->plainTextEdit->appendPlainText("losize: " + QString::number(iprocess->lowLinesList.size()));
            }
    iprocess->detectContrastCenter();
    algoPrerequestsOk = true;
    captured = true;
}

void setupForm::Algo6(imgProcess *iprocess){
// woLASER: canny1 > houghTr > detectMainEdges > thickenEdgeMap > scoreLineCrossing

    if (edgeDetectionState == 3) {
        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"03-max hough lines matrix -dist-angle-vote.csv");
        iprocess->thinCornerNum = mainEdgesNumber;
        iprocess->detectMainEdges(thinJointAlgoActive, DEBUG);
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"04-max hough lines distance sorted.csv");
            /**/if (DEBUG) for (int i=0; i<iprocess->mainEdgesList.size();i++)
                    ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue));
        /*
        iprocess->thickenEdgeMap(3);
            //if (saveAnalysis) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"14-canny thickened image.jpg");
        iprocess->scoreLineCrossing(true);
            //ui->plainTextEdit->appendPlainText("skor, yüzde: " + QString::number(iprocess->mainEdgeScore) + ", " + QString::number(iprocess->mainEdgeScorePercent, 'f', 1));
        */
        /*
        for (int i=0; i<iprocess->mainEdgesList.size();i++) {
            int length = iprocess->detectLongestSolidLineVert(iprocess->mainEdgesList[i].distance,iprocess->mainEdgesList[i].angle,0,iprocess->edgeHeight-1).length;
            ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote/length: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue) + ", " + QString::number(length));

        }
        */
        algoPrerequestsOk = true;
        captured = true;
    } else {
        ui->plainTextEdit->appendPlainText("Canny1 kenar tespiti algoritması seçilmelidir");
        algoPrerequestsOk = false;
    }
}

void setupForm::Algo7(imgProcess *iprocess){
// woLASER: edge > houghTr > detectScanHorizontal

    if (edgeDetectionState != 0) {
        iprocess->detectScanHorizontal( iprocess->edgeHeight/2 );            // get max voted line(s)
            if (saveAnalysis) iprocess->saveMatrix(iprocess->horLineVotes, 3, iprocess->edgeWidth, path+"03-max votes on line.csv");
            //if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"04-max hough lines distance sorted.csv");
            //if (DEBUG) for (int i=0; i<iprocess->mainEdgesList.size();i++)
            //        ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue));
        algoPrerequestsOk = true;
        captured = true;
    } else {
        ui->plainTextEdit->appendPlainText("Bir kenar tespiti algoritması seçilmelidir");
        algoPrerequestsOk = false;
    }
}

void setupForm::processImage(){

//    if ( w->play && !w->imageGetter->imageList.isEmpty() ){
    if ( w->play && (w->lastData->image->format() != QImage::Format_Invalid) ){
        if (w->applyCameraEnhancements) {
//            target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
            target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
        } else {
            //target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
            target = w->lastData->image->copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
        }
    }

    if ( !w->play &&  imageLoadedFromFile){
//        target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
        target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
    }

//    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ){
    if ( (w->lastData->image->format() != QImage::Format_Invalid) || imageLoadedFromFile ){

        if (iprocessInitSwitch) {
            delete iprocess;
            iprocessInitSwitch = false;
        }

        /*D*/if (saveAnalysis) {
            path = savePath + captureTimeStr;
            if (thinJointAlgoActive) {  // without laser - VERTICAL SEARCH
                switch ( algorithmType ) {
                    case 0: // NONE
                        path += "/"; break;
                    case 1: // MAIN EDGES
                        path += "-Algo3/"; break;
                    case 2: // THIN JOINT - DARK AREA
                        path += "-Algo4/"; break;
                    case 3: // CONTRAST
                        path += "-Algo5/"; break;
                    case 4: // LINE DETECTION WITH MAIN EDGES
                        path += "-Algo6/"; break;
                    case 5: // SCAN HORIZONTAL
                        path += "-Algo7/"; break;
                    case 6: // EXPERIMENTAL
                        path += "-AlgoY/"; break;
                }
            } else {    // with laser - HORIZONTAL SEARCH
                switch ( algorithmType ) {
                    case 0: // NONE
                        path += "/"; break;
                    case 1: // LONGEST SOLID LINES
                        path += "-Algo1/"; break;
                    case 2: // PRIMARY VOID
                        path += "-Algo2/"; break;
                    case 3: // EXPERIMENTAL
                        path += "-AlgoX/"; break;
                }
            }
            QDir().mkdir(path);
        }

        iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
            /*D*/if (saveAnalysis) iprocess->imgOrginal.save(path+"01-orginal image.jpg");
        iprocessInitSwitch = true;
        //iprocess->_DEBUG = true;

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
        /*D*/if (saveAnalysis) iprocess->imgMono.save(path+"02-mono image.jpg");

        edgeDetection(iprocess);

        algoPrerequestsOk = false;

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
                case 5: // SCAN HORIZONTAL
                    Algo7(iprocess);
                    break;
                case 6: // EXPERIMENTAL
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

    }

}

void setupForm::captureButton(){

    ui->labelMono->clear();
    ui->labelEdge->clear();
    ui->labelHough->clear();
    ui->labelAnalyze->clear();

//    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ) {   // if any image is get
    if ( (w->lastData->image->format() != QImage::Format_Invalid) || imageLoadedFromFile ) {   // if any image is get

        ui->labelPrimaryLine->hide();               // hide PRIMARY LINE NOT DETECTED message

        // get & check hough parameters from ui
        thetaMin = ui->editHoughThetaMin->text().toInt();
        thetaMax = ui->editHoughThetaMax->text().toInt();
        thetaStep = ui->editHoughThetaStep->text().toFloat();
        if (thetaStep < 0.1){
            thetaStep = 0.1;
            ui->editHoughThetaStep->setText("0.1");
        }

        /*
        thetaMinSub = ui->editHoughThetaMinSub->text().toInt();
        thetaMaxSub = ui->editHoughThetaMaxSub->text().toInt();
        thetaStepSub = ui->editHoughThetaStepSub->text().toFloat();
        if (thetaStepSub < 0.1){
            thetaStepSub = 0.1;
            ui->editHoughThetaStepSub->setText("0.1");
        }*/

        houghLineNo = ui->editHoughLineNo->text().toInt();
        if (houghLineNo == 0){
            houghLineNo = 2;
            ui->editHoughLineNo->setText("2");
        }
        voteThreshold = ui->editHoughThreshold->text().toInt();
        voidThreshold = ui->editVoidThreshold->text().toInt();

        // for file names
        captureTimeStr = QDate::currentDate().toString("yyyyMMdd") + "-" + QTime::currentTime().toString("hhmmss");      // for filename

        // -------START IMAGE PROCESSING-------
        startTime = w->timeSystem.getSystemTimeMsec();

        processImage();

        endTime = w->timeSystem.getSystemTimeMsec();
        processElapsed = endTime - startTime;
        // -------END IMAGE PROCESSING-------


        // UPDATE GUI

        QString message = "Analiz " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.";
        //goto labelx;
        ui->plainTextEdit->appendPlainText(message);

        //ui->plainTextEdit->appendPlainText(iprocess->statusMessage);    // display message about detection process
        ui->plainTextEdit->appendPlainText("Sol Köşe(x,y) - Merkez(x,y) - Sağ Köşe(x,y): ");
        message = "( " + QString::number(iprocess->leftCornerX) + " , " + QString::number(iprocess->leftCornerY) + " )  -  ( " +
                         QString::number(iprocess->trackCenterX) + " , " + QString::number(iprocess->trackCenterY) + " )  -  ( " +
                         QString::number(iprocess->rightCornerX) + " , " + QString::number(iprocess->rightCornerY) + " )";
        ui->plainTextEdit->appendPlainText(message);


        ui->labelTarget->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );
        //ui->labelMono->setPixmap( QPixmap::fromImage ( iprocess->imgMono ) );

        if ( edgeDetectionState != 0) {
            edge = iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight );   // produce edge image
            ui->labelEdge->setPixmap( QPixmap::fromImage( *edge ) );
        }

        if (algoPrerequestsOk) {
            if (thinJointAlgoActive) {  // without laser - VERTICAL SEARCH

                switch ( algorithmType ) {

                    case 0: // NONE
                        break;
                    case 1: // MAIN EDGES
//                        iprocess->constructHoughMatrixFindX();   // FOR THINJOINT - edge matrix + coded #houghLineNo lines
//                        hough = iprocess->getImage( iprocess->houghMatrix, iprocess->edgeWidth, iprocess->edgeHeight );     // produce hough image
//                        ui->labelHough->setPixmap( QPixmap::fromImage( *hough ) );

                        /**/if (DEBUG) {
                                ui->plainTextEdit->appendPlainText("-1st-maximas---");
                                for (int i=0; i<iprocess->localMaximaSize;i++)
                                    ui->plainTextEdit->appendPlainText("start: "+QString::number(iprocess->rangeArray[i][0]) +" stop: "+QString::number(iprocess->rangeArray[i][1]));

                                ui->plainTextEdit->appendPlainText("-1st hough vals---");
                                for (int i=0; i<iprocess->listHoughDataSize;i++)
                                    ui->plainTextEdit->appendPlainText("dist/ang/vote: "+QString::number(iprocess->listHoughDataArray[i][0], 'f', 2) +", "+QString::number(iprocess->listHoughDataArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughDataArray[i][2], 'f', 2));

                                ui->plainTextEdit->appendPlainText("-2nd-maximas---");
                                for (int i=0; i<iprocess->localMaxima2ndSize;i++)
                                    ui->plainTextEdit->appendPlainText("start: "+QString::number(iprocess->rangeArray2nd[i][0]) +" stop: "+QString::number(iprocess->rangeArray2nd[i][1]));

                                ui->plainTextEdit->appendPlainText("-2nd hough vals---");
                                for (int i=0; i<iprocess->listHoughData2ndSize;i++)
                                    ui->plainTextEdit->appendPlainText("dist/ang/vote: "+QString::number(iprocess->listHoughData2ndArray[i][0], 'f', 1) +", "+QString::number(iprocess->listHoughData2ndArray[i][1], 'f', 1)+", "+QString::number(iprocess->listHoughData2ndArray[i][2], 'f', 0));
                            }

                        //ui->labelHough->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges(mainEdgesNumber) ) );
                        ui->labelHough->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges_2ndList(true) ) );
                            /**/if (saveAnalysis) iprocess->imgSolidLines.save(path+"05-mainEdges image.jpg");

                        //iprocess->cornerImage();
                            //*D*/if (saveAnalysis) iprocess->imgCorner.save(path+"06-corner image.jpg");
                        ui->plainTextEdit->appendPlainText("leftX-centerX-rightX: " +QString::number(iprocess->leftCornerX)+", "+QString::number(iprocess->trackCenterX)+", "+QString::number(iprocess->rightCornerX));
                        //ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );

                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges(0) ) );
                            /**/if (saveAnalysis) iprocess->imgSolidLines.save(path+"06-mainEdges centerline image.jpg");

                        break;
                    case 2: // THIN JOINT
                        minCostedLines *centerline;
                        centerline = new minCostedLines();
                        centerline->c = iprocess->centerC;
                        centerline->cost = 0;
                            /**/if (saveAnalysis) {
                                    iprocess->drawLine(centerline, iprocess->slopeBest).save(path+"03-centerLine.jpg");

                                    iprocess->saveMinCostedLinesArray(iprocess->bestLines, iprocess->anglePrecision, path+"04-best lines.csv");
                                    iprocess->saveMinCostedLinesList(iprocess->lineList, path+"05-lineList.csv");
                                    iprocess->saveList(iprocess->peakPoints, path+"06-peakPoints.csv");
                                }
                        ui->plainTextEdit->appendPlainText("best slope: "+ QString::number(iprocess->slopeBestIndex));
                        ui->labelHough->setPixmap( QPixmap::fromImage( iprocess->drawLine(centerline, iprocess->slopeBest) ));
                        iprocess->cornerImage();
                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );
                        break;
                    case 3: // CONTRAST
                        ui->labelMono->setPixmap( QPixmap::fromImage ( iprocess->getImageContrast() ) );
                        iprocess->constructContrastMatrixMajor2Lines();
                            /**/if (saveAnalysis) iprocess->saveMatrix( iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight, path+"07-contrast with lines matrix.csv" );
                        hough = iprocess->getImage(iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight);
                            /**/if (saveAnalysis) hough->save(path+"08-major 2 lines image.jpg");
                        ui->labelHough->setPixmap( QPixmap::fromImage( *hough ) );

                        iprocess->cornerImage();
                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );
                            /**/if (saveAnalysis) iprocess->imgCorner.save(path+"09-corner image.jpg");
                        ui->plainTextEdit->appendPlainText("avg dist, angle: " + QString::number(iprocess->distanceAvg) + ", " + QString::number(iprocess->thetaAvg));
                        break;
                    case 4: // LINE DETECTION WITH MAIN EDGES
                        {
                            /**/if (DEBUG) {
                                ui->plainTextEdit->appendPlainText("-1st-maximas---");
                                for (int i=0; i<iprocess->localMaximaSize;i++)
                                    ui->plainTextEdit->appendPlainText("start: "+QString::number(iprocess->rangeArray[i][0]) +" stop: "+QString::number(iprocess->rangeArray[i][1]));

                                ui->plainTextEdit->appendPlainText("-1st hough vals---");
                                    for (int i=0; i<iprocess->listHoughDataSize;i++)
                                        ui->plainTextEdit->appendPlainText("dist/ang/vote: "+QString::number(iprocess->listHoughDataArray[i][0], 'f', 2) +", "+QString::number(iprocess->listHoughDataArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughDataArray[i][2], 'f', 2));

                                ui->plainTextEdit->appendPlainText("-2nd-maximas---");
                                for (int i=0; i<iprocess->localMaxima2ndSize;i++)
                                    ui->plainTextEdit->appendPlainText("start: "+QString::number(iprocess->rangeArray2nd[i][0]) +" stop: "+QString::number(iprocess->rangeArray2nd[i][1]));

                                ui->plainTextEdit->appendPlainText("-2nd hough vals---");
                                for (int i=0; i<iprocess->listHoughData2ndSize;i++)
                                    ui->plainTextEdit->appendPlainText("dist/ang/vote: "+QString::number(iprocess->listHoughData2ndArray[i][0], 'f', 2) +", "+QString::number(iprocess->listHoughData2ndArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughData2ndArray[i][2], 'f', 2));
                            }

                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges(1) ) );
                            /**/if (saveAnalysis) iprocess->imgSolidLines.save(path+"05-mainEdges image.jpg");
                            /**/if (saveAnalysis) iprocess->cornerImage().save(path+"06-corners image.jpg");

                        ui->plainTextEdit->appendPlainText("-2nd hough vals max solid lines---");

                        int *graphArray = new int[iprocess->listHoughData2ndSize];
                        int *xArray = new int[iprocess->listHoughData2ndSize];
                        for (int i=0; i<iprocess->listHoughData2ndSize; i++){
                            xArray[i] = iprocess->listHoughData2ndArray[i][0];
                            graphArray[i] = iprocess->listHoughData2ndArray[i][2];
                        }

                        int *graphArray2 = new int[iprocess->listHoughData2ndSize];
                        for (int i=0; i<iprocess->listHoughData2ndSize;i++) {
                            int length = iprocess->detectLongestSolidLineVert(iprocess->listHoughData2ndArray[i][0], iprocess->listHoughData2ndArray[i][1], 0, iprocess->edgeHeight-1).length;
                            graphArray2[i] = length;
                            ui->plainTextEdit->appendPlainText("dist/ang/vote/length: "+QString::number(iprocess->listHoughData2ndArray[i][0], 'f', 0) +", "+QString::number(iprocess->listHoughData2ndArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughData2ndArray[i][2], 'f', 0) + ", " + QString::number(length));
                        }

                        // vote graph
                        drawGraphXY(ui->graphicsView, xArray, graphArray, iprocess->listHoughData2ndSize);
                        // max solid line graph
                        drawGraphXY(ui->graphicsView2, xArray, graphArray2, iprocess->listHoughData2ndSize);

                        ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );

                        if ( iprocess->mainEdgeScorePercent > w->lineScoreLimit){
                            ui->plainTextEdit->appendPlainText( "Ana çizgi bulundu, %" + QString::number(iprocess->mainEdgeScorePercent, 'f', 1) );
                        }

                        }
                        break;

                    case 5: // SCAN HORIZONTAL
                        {
                        int *graphArray = new int[iprocess->edgeWidth];
                        for (int i=0; i<iprocess->edgeWidth; i++){
                            graphArray[i] = iprocess->horLineVotes[i][2];
                        }
                        drawGraph(ui->graphicsView, graphArray, iprocess->edgeWidth);

                        int *valArray = new int[iprocess->imageWidth];
                        float sum = 0;
                        for (int x=0; x<iprocess->imageWidth; x++){
                            sum = 0;
                            for (int y=0; y<iprocess->imageHeight; y++)
                                sum += iprocess->valueMatrix[y][x];
                            valArray[x] = sum / iprocess->imageHeight;
                        }
                        drawGraph(ui->graphicsView2, valArray, iprocess->imageWidth);

                        ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );

                        }break;
                    case 6: // EXPERIMENTAL
                        break;
                }

            } else {    // with laser - HORIZONTAL SEARCH

                switch ( algorithmType ) {

                    case 0: // NONE
                        break;
                    case 1: // LONGEST SOLID LINES
                            /**/if (saveAnalysis) iprocess->saveList(iprocess->solidSpaceMain, path+"05-solid space main matrix.csv");
                            /**/if (saveAnalysis) iprocess->saveList(iprocess->solidSpaceMainTrimmed, path+"06-solid space trimmed matrix.csv");
                            /**/if (saveAnalysis) iprocess->saveList(iprocess->primaryGroup, path+"07-primary group matrix.csv");
                            /**/if (saveAnalysis) iprocess->saveList(iprocess->secondaryGroup, path+"08-secondary group matrix.csv");

                        if ( iprocess->primaryLineFound && iprocess->secondaryLineFound )
                            iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[1], 0 );
                        else
                            if ( iprocess->primaryLineFound )
                                iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[0], 0 );

                            /**/if (saveAnalysis) iprocess->imgCornerAndPrimaryLines.save(path+"09-major2lines image.jpg");
                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCornerAndPrimaryLines ) );

                            /**/if (saveAnalysis) iprocess->drawSolidLines(iprocess->majorLines).save(path+"10-major lines image.jpg");
                            /**/if (saveAnalysis && edgeDetectionState != 0)
                                    for (int c=0; c < iprocess->majorLines.size(); c++)
                                        iprocess->drawSolidLines2EdgeMatrix( iprocess->majorLines[c], QImage::Format_RGB16)->save(path+"xx-major lines image-" + QString::number(c) + ".jpg");
                            /**/if (saveAnalysis && edgeDetectionState == 0)
                                    for (int c=0; c < iprocess->majorLines.size(); c++)
                                        iprocess->drawLine2OrginalImage( iprocess->majorLines[c], QImage::Format_RGB16).save(path+"xx-major lines image-" + QString::number(c) + ".jpg");

                        if ( iprocess->primaryLineFound ) {
                            ui->plainTextEdit->appendPlainText("Ortalama Açı: " + QString::number(iprocess->angleAvg));
                        }
                        break;
                    case 2: // PRIMARY VOID
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
                            /**/if (saveAnalysis) iprocess->imgCornerAndPrimaryLines.save(path+"05-major2lines image.jpg");
                        break;
                    case 3: // EXPERIMENTAL
                        break;
                }
            }

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


        ui->plainTextEdit->appendPlainText("-----------------");
//labelx:

        captured = true;
        saveAnalysis = false;

    } else {

        ui->plainTextEdit->appendPlainText(alarm6);
   }

}

bool setupForm::saveButton(){

    saveAnalysis = true;

    DEBUG = true;
    ui->debugModeBox->setChecked(DEBUG);

    captureButton();

    saveAnalysis = false;

    ui->plainTextEdit->appendPlainText("Dosyalar kaydedildi");

    return saveAnalysis;
}

void setupForm::getParameters(){

    thetaMinHorLine = w->thetaMinHorLine;
    thetaMaxHorLine = w->thetaMaxHorLine;
    thetaMinVerLine = w->thetaMinVerLine;
    thetaMaxVerLine = w->thetaMaxVerLine;
    thetaMin = w->thetaMin;
    thetaMax = w->thetaMax;
    thetaStep = w->thetaStep;

    subImageProcessingSwitch = w->subImageProcessingSwitch;
    ui->checkSubImage->setChecked(subImageProcessingSwitch);
    thetaMinSub = w->thetaMinSub;
    thetaMaxSub = w->thetaMaxSub;
    thetaStepSub = w->thetaStepSub;

    houghLineNo = w->houghLineNo;
    voteThreshold = w->voteThreshold;
    voidThreshold = w->voidThreshold;

    brightnessVal = w->brightnessVal;
    contrastVal = w->contrastVal;
    gammaVal = w->gammaVal;
    gaussianSize = w->gaussianSize;
    stdDev = w->stdDev;
    thinJointAlgoActive = w->thinJointAlgoActive;
    edgeDetectionState = w->edgeDetectionState;
    algorithmType = w->algorithmType;
    mainEdgesNumber = w->mainEdgesNumber;

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
    ui->editVideoDuration->setText(QString::number(w->videoDuration));

    applyCameraEnhancements = w->applyCameraEnhancements;

}

void setupForm::clearButton(){

    ui->plainTextEdit->clear();
}

void setupForm::subImageCheck(){

    subImageProcessingSwitch = ui->checkSubImage->isChecked();
    ui->editHoughThetaMinSub->setEnabled( ui->checkSubImage->isChecked() );
    ui->editHoughThetaMaxSub->setEnabled( ui->checkSubImage->isChecked() );
    ui->editHoughThetaStepSub->setEnabled( ui->checkSubImage->isChecked() );
}

void setupForm::saveExitButton(){

    if (thinJointAlgoActive) {
        w->thetaMinVerLine = w->thetaMin = thetaMin;
        w->thetaMaxVerLine = w->thetaMax = thetaMax;
    } else {
        w->thetaMinHorLine = w->thetaMin = thetaMin;
        w->thetaMaxHorLine = w->thetaMax = thetaMax;
    }

    w->thetaStep = thetaStep = ui->editHoughThetaStep->text().toFloat();

    //w->thetaMinSub = thetaMinSub = ui->editHoughThetaMinSub->text().toInt();;
    //w->thetaMaxSub = thetaMaxSub = ui->editHoughThetaMaxSub->text().toInt();
    //w->thetaStepSub = thetaStepSub = ui->editHoughThetaStepSub->text().toFloat();

    w->houghLineNo = houghLineNo = ui->editHoughLineNo->text().toInt();
    w->voteThreshold = voteThreshold = ui->editHoughThreshold->text().toInt();
    w->voidThreshold = voidThreshold = ui->editVoidThreshold->text().toInt();
    w->brightnessVal = brightnessVal;
    w->contrastVal = contrastVal;
    w->gammaVal = gammaVal;
    w->gaussianSize = gaussianSize;
    w->stdDev = stdDev;
    w->thinJointAlgoActive = thinJointAlgoActive;
    w->edgeDetectionState = edgeDetectionState;
    w->algorithmType = algorithmType;
    w->mainEdgesNumber = mainEdgesNumber;

    w->fpsTarget = ui->editFPS->text().toInt();
    w->frameInterval = 1000 / w->fpsTarget;
    w->iprocessInterval = ui->editIPI->text().toInt();
    w->videoDuration = ui->editVideoDuration->text().toInt();

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

void setupForm::on_alignGuideBox_clicked(){

    w->alignGuide2TrackCenter = ui->alignGuideBox->isChecked();
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

    w->applyCameraEnhancements = applyCameraEnhancements;
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
        w->ui->labelDistance->show();
        w->ui->labelDistanceTag->show();
        w->ui->labelDistanceTag2->show();

    } else {
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

        w->stopButton();
        ui->fileSlider->setEnabled(true);
        ui->capturePrev->setEnabled(true);
        ui->captureNext->setEnabled(true);
        ui->checkProcessing->setEnabled(true);

        imageLoadedFromFile = true;

        w->fileOpenDir = QFileInfo(w->loadedFileNamewPath).absoluteDir();
        w->filesInDirList = w->fileOpenDir.entryList(w->fileFilters, QDir::Files);

        //foreach(QString temp, w->filesInDirList){ qDebug() << temp; }

        w->loadedFileName = QFileInfo(w->loadedFileNamewPath).fileName();
        w->filesInDirListIndex = w->filesInDirList.indexOf(w->loadedFileName,0);
        ui->labelPicNo->setText(QString::number(w->filesInDirListIndex)+": "+ w->loadedFileName);

        ui->fileSlider->setMaximum(w->filesInDirList.size() - 1);
        ui->fileSlider->setValue(w->filesInDirListIndex);

        w->imageFile.load(w->loadedFileNamewPath);
        w->imageFileChanged = w->imageFile;
        w->calcImageParametes(w->imageFile, true);
        w->ui->imageFrame->setPixmap( QPixmap::fromImage( w->imageFile ).scaled(w->imageWidth, w->imageHeight, Qt::KeepAspectRatio));
        captureButton();
    }
}

void setupForm::on_fileSlider_sliderMoved(int position){

    w->filesInDirListIndex = position;
    w->loadedFileName = w->filesInDirList.at(position);
    ui->labelPicNo->setText(QString::number(w->filesInDirListIndex)+": "+ w->loadedFileName);

    //qDebug() << w->filesInDirListIndex << "." << w->loadedFileName;
    w->imageFile.load(w->fileOpenDir.path() + "/" + w->loadedFileName);
    w->calcImageParametes(w->imageFile, true);
    update();
}

void setupForm::on_captureNext_clicked(){

    if(w->filesInDirListIndex != (w->filesInDirList.size()- 1)){
        w->filesInDirListIndex++;
        ui->fileSlider->setSliderPosition(w->filesInDirListIndex);

        w->loadedFileName = w->filesInDirList.at(w->filesInDirListIndex);
        ui->labelPicNo->setText(QString::number(w->filesInDirListIndex)+": "+ w->loadedFileName);
        w->imageFile.load(w->fileOpenDir.path() + "/" + w->loadedFileName);
        w->calcImageParametes(w->imageFile, true);
        update();
    }
}

void setupForm::on_capturePrev_clicked(){

    if(w->filesInDirListIndex != 0){
        w->filesInDirListIndex--;
        ui->fileSlider->setSliderPosition(w->filesInDirListIndex);

        w->loadedFileName = w->filesInDirList.at(w->filesInDirListIndex);
        ui->labelPicNo->setText(QString::number(w->filesInDirListIndex)+": "+ w->loadedFileName);
        w->imageFile.load(w->fileOpenDir.path() + "/" + w->loadedFileName);
        w->calcImageParametes(w->imageFile, true);
        update();
    }
}

void setupForm::update(){

    QImage step1 = changeBrightness(w->imageFile, brightnessVal);
    QImage step2 = changeContrast(step1, contrastVal);
    w->imageFileChanged = changeGamma(step2, gammaVal);
    if (!w->play)
        w->ui->imageFrame->setPixmap( QPixmap::fromImage( w->imageFileChanged.scaled(w->imageWidth, w->imageHeight, Qt::KeepAspectRatio) ));

    if (ui->checkProcessing->isChecked())
        captureButton();
}

void setupForm::on_brightnessSlider_sliderReleased(){

    brightnessVal = ui->brightnessSlider->value();
    if (w->applyCameraEnhancements)
        w->brightnessVal = brightnessVal;
    update();
}

void setupForm::on_brightnessSlider_sliderMoved(int position){

    ui->labelBrightness->setText(QString::number(position));
}

void setupForm::on_contrastSlider_sliderReleased(){

    contrastVal = ui->contrastSlider->value();
    if (w->applyCameraEnhancements)
        w->contrastVal = contrastVal;
    update();
}

void setupForm::on_contrastSlider_sliderMoved(int position){

    ui->labelContrast->setText(QString::number(position));
}

void setupForm::on_gammaSlider_sliderReleased(){

    gammaVal = ui->gammaSlider->value();
    if (w->applyCameraEnhancements)
        w->gammaVal = gammaVal;
    update();
}

void setupForm::on_gammaSlider_sliderMoved(int position){

    ui->labelGamma->setText(QString::number(position));
}

void setupForm::on_brightnessReset_clicked(){

    brightnessVal = 0;
    ui->brightnessSlider->setValue(0);
    ui->labelBrightness->setText(QString::number(0));
    if (w->applyCameraEnhancements)
        w->brightnessVal = brightnessVal;
    update();
}

void setupForm::on_contrastReset_clicked(){

    contrastVal = 100;
    ui->contrastSlider->setValue(100);
    ui->labelContrast->setText(QString::number(100));
    if (w->applyCameraEnhancements)
        w->contrastVal = contrastVal;
    update();
}

void setupForm::on_gammaReset_clicked(){

    gammaVal = 100;
    ui->gammaSlider->setValue(100);
    ui->labelGamma->setText(QString::number(100));
    if (w->applyCameraEnhancements)
        w->gammaVal = gammaVal;
    update();
}

void setupForm::on_gaussSizeSlider_sliderMoved(int position){

    gaussianSize = 2*position+1;
    ui->labelGaussSize->setText(QString::number(gaussianSize));
    update();
}

void setupForm::on_gaussSDevSlider_sliderMoved(int position){

    stdDev = position / 10.0;
    ui->labelGaussSDev->setText(QString::number(stdDev));
    update();
}

void setupForm::on_cannyThinningBox_clicked(){

    w->cannyThinning = ui->cannyThinningBox->isChecked();
}

void setupForm::on_algorithmBox_currentIndexChanged(int index){

    algorithmType = index;

    QString algoName = "";
    if (thinJointAlgoActive) {  // without laser - VERTICAL SEARCH
        switch ( algorithmType ) {
            case 0: ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(false);
                    ui->editHoughLineNo->setEnabled(false);
                    ui->label_9->setEnabled(false);
                    ui->editHoughThetaMax->setEnabled(false);
                    ui->editHoughThetaMin->setEnabled(false);
                    ui->label_8->setEnabled(false);
                    ui->editHoughThetaStep->setEnabled(false);
                    break;
            case 1: algoName = "Algo3: woLASER: edge > houghTr > detectMainEdges";
                    ui->label_21->setEnabled(true);
                    ui->mainEdgesSlider->setEnabled(true);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(true);
                    ui->editHoughLineNo->setEnabled(true);
                    ui->label_9->setEnabled(true);
                    ui->editHoughThetaMax->setEnabled(true);
                    ui->editHoughThetaMin->setEnabled(true);
                    ui->label_8->setEnabled(true);
                    ui->editHoughThetaStep->setEnabled(true);
                    break;  // MAIN EDGES
            case 2: algoName = "Algo4: woLASER: value > detectThinJointCenter";
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(false);
                    ui->editHoughLineNo->setEnabled(false);
                    ui->label_9->setEnabled(false);
                    ui->editHoughThetaMax->setEnabled(false);
                    ui->editHoughThetaMin->setEnabled(false);
                    ui->label_8->setEnabled(false);
                    ui->editHoughThetaStep->setEnabled(false);
                    break;  // THIN JOINT - DARK AREA
            case 3: algoName = "Algo5: woLASER: value > contrast matrix > houghTr > calcAvgDistAndAngleOfMajors";
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(true);
                    ui->editHoughLineNo->setEnabled(true);
                    ui->label_9->setEnabled(true);
                    ui->editHoughThetaMax->setEnabled(true);
                    ui->editHoughThetaMin->setEnabled(true);
                    ui->label_8->setEnabled(true);
                    ui->editHoughThetaStep->setEnabled(true);
                    break;  // CONTRAST
            case 4: algoName = "Algo6: woLASER: canny1 > houghTr > detectMainEdges > thickenEdgeMap > scoreLineCrossing";
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(true);
                    ui->editLineScore->setEnabled(true);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(true);
                    ui->editHoughLineNo->setEnabled(true);
                    ui->label_9->setEnabled(true);
                    ui->editHoughThetaMax->setEnabled(true);
                    ui->editHoughThetaMin->setEnabled(true);
                    ui->label_8->setEnabled(true);
                    ui->editHoughThetaStep->setEnabled(true);
                    break;  // LINE DETECTION WITH MAIN EDGES
            case 5: algoName = "Algo7: woLASER: edge > houghTr > detectScanHorizontal";
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(false);
                    ui->editHoughLineNo->setEnabled(false);
                    ui->label_9->setEnabled(false);
                    ui->editHoughThetaMax->setEnabled(false);
                    ui->editHoughThetaMin->setEnabled(false);
                    ui->label_8->setEnabled(false);
                    ui->editHoughThetaStep->setEnabled(false);
                    break;  // PROJECTION OF FOUND LINES ON PARTICULAR HORIZONTAL LINE
            case 6: algoName = "AlgoY";
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(false);
                    ui->editHoughLineNo->setEnabled(false);
                    ui->label_9->setEnabled(false);
                    ui->editHoughThetaMax->setEnabled(false);
                    ui->editHoughThetaMin->setEnabled(false);
                    ui->label_8->setEnabled(false);
                    ui->editHoughThetaStep->setEnabled(false);
                    break;  // EXPERIMENTAL
            default:
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(false);
                    ui->editHoughLineNo->setEnabled(false);
                    ui->label_9->setEnabled(false);
                    ui->editHoughThetaMax->setEnabled(false);
                    ui->editHoughThetaMin->setEnabled(false);
                    ui->label_8->setEnabled(false);
                    ui->editHoughThetaStep->setEnabled(false);
                    break;
        }
    } else {    // with laser - HORIZONTAL SEARCH
        switch ( algorithmType ) {
            case 0: ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(false);
                    ui->editHoughLineNo->setEnabled(false);
                    ui->label_9->setEnabled(false);
                    ui->editHoughThetaMax->setEnabled(false);
                    ui->editHoughThetaMin->setEnabled(false);
                    ui->label_8->setEnabled(false);
                    ui->editHoughThetaStep->setEnabled(false);
                    break;
            case 1: algoName = "Algo1: LASER: mono/edge(matrixFlag) > houghTr > detectLongestSolidLines";
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(true);
                    ui->editHoughLineNo->setEnabled(true);
                    ui->label_9->setEnabled(true);
                    ui->editHoughThetaMax->setEnabled(true);
                    ui->editHoughThetaMin->setEnabled(true);
                    ui->label_8->setEnabled(true);
                    ui->editHoughThetaStep->setEnabled(true);
                    break;  // LONGEST SOLID LINES
            case 2: algoName = "Algo2: LASER: mono/edge(matrixFlag) > houghTr > detectPrimaryVoid";
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(true);
                    ui->editVoidThreshold->setEnabled(true);
                    ui->label_7->setEnabled(true);
                    ui->editHoughThreshold->setEnabled(true);
                    ui->label_6->setEnabled(true);
                    ui->editHoughLineNo->setEnabled(true);
                    ui->label_9->setEnabled(true);
                    ui->editHoughThetaMax->setEnabled(true);
                    ui->editHoughThetaMin->setEnabled(true);
                    ui->label_8->setEnabled(true);
                    ui->editHoughThetaStep->setEnabled(true);
                    break;  // PRIMARY VOID
            case 3: algoName = "AlgoX";
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(false);
                    ui->editHoughLineNo->setEnabled(false);
                    ui->label_9->setEnabled(false);
                    ui->editHoughThetaMax->setEnabled(false);
                    ui->editHoughThetaMin->setEnabled(false);
                    ui->label_8->setEnabled(false);
                    ui->editHoughThetaStep->setEnabled(false);
                    break;  // EXPERIMENTAL
            default:
                    ui->label_21->setEnabled(false);
                    ui->mainEdgesSlider->setEnabled(false);
                    ui->lineDetectionBox->setEnabled(false);
                    ui->editLineScore->setEnabled(false);
                    ui->label_10->setEnabled(false);
                    ui->editVoidThreshold->setEnabled(false);
                    ui->label_7->setEnabled(false);
                    ui->editHoughThreshold->setEnabled(false);
                    ui->label_6->setEnabled(false);
                    ui->editHoughLineNo->setEnabled(false);
                    ui->label_9->setEnabled(false);
                    ui->editHoughThetaMax->setEnabled(false);
                    ui->editHoughThetaMin->setEnabled(false);
                    ui->label_8->setEnabled(false);
                    ui->editHoughThetaStep->setEnabled(false);
                    break;
        }
    }

    ui->plainTextEdit->appendPlainText(algoName);
}

void setupForm::on_radioLaser_clicked() {

    thinJointAlgoActive = false;
    thetaMin = thetaMinHorLine;
    thetaMax = thetaMaxHorLine;
    ui->editHoughThetaMin->setText(QString::number(thetaMin));
    ui->editHoughThetaMax->setText(QString::number(thetaMax));

    int algo = algorithmType;
    ui->algorithmBox->clear();
    ui->algorithmBox->addItem("Yok");
    ui->algorithmBox->addItem("Çizgi");
    ui->algorithmBox->addItem("Boşluk");
    ui->algorithmBox->addItem("Deneme");
    if (algo > 3) algo = 3;
    algorithmType = algo;
    ui->algorithmBox->setCurrentIndex( algorithmType );
}

void setupForm::on_radioWoLaser_clicked() {

    thinJointAlgoActive = true;
    thetaMin = thetaMinVerLine;
    thetaMax = thetaMaxVerLine;
    ui->editHoughThetaMin->setText(QString::number(thetaMin));
    ui->editHoughThetaMax->setText(QString::number(thetaMax));

    int algo = algorithmType;
    ui->algorithmBox->clear();
    ui->algorithmBox->addItem("Yok");
    ui->algorithmBox->addItem("Ana Kenarlar");
    ui->algorithmBox->addItem("İnce Ağız");
    ui->algorithmBox->addItem("Kontrast");
    ui->algorithmBox->addItem("Ana Kenarlar ile Çizgi");
    ui->algorithmBox->addItem("Yatay Tarama");
    ui->algorithmBox->addItem("Deneme");
    if (algo > 6) algo = 6;
    algorithmType = algo;
    ui->algorithmBox->setCurrentIndex( algorithmType );
}

void setupForm::on_edgeDetectionBox_currentIndexChanged(int index){

    edgeDetectionState = index;

    if (edgeDetectionState == 2 || edgeDetectionState == 3){
        ui->gaussSizeSlider->setEnabled(true);
        ui->gaussSDevSlider->setEnabled(true);
        ui->label_19->setEnabled(true);
        ui->label_20->setEnabled(true);
        ui->cannyThinningBox->setEnabled(true);
    } else {
        ui->gaussSizeSlider->setEnabled(false);
        ui->gaussSDevSlider->setEnabled(false);
        ui->label_19->setEnabled(false);
        ui->label_20->setEnabled(false);
        ui->cannyThinningBox->setEnabled(false);
    }
}

void setupForm::on_mainEdgesSlider_valueChanged(int value){
    mainEdgesNumber = value;
    ui->labelMainEdgesNumber->setText(QString::number(mainEdgesNumber));
}

void setupForm::on_saveButton_clicked(){

}

void setupForm::on_cameraEnhancementsBox_stateChanged(int arg1){
    w->applyCameraEnhancements = ui->cameraEnhancementsBox->isChecked();
    if (w->applyCameraEnhancements){
        w->brightnessVal = brightnessVal;
        w->contrastVal = contrastVal;
        w->gammaVal = gammaVal;
    }
}

void setupForm::on_editVideoDuration_returnPressed(){}

void setupForm::on_debugModeBox_clicked(){

    DEBUG = ui->debugModeBox->isChecked();
}

void setupForm::clearGraph(QGraphicsView *graph){
    graph->scene()->clear();
    graph->show();
}

void setupForm::drawGraph(QGraphicsView *graph, int *array, int size){

    clearGraph(graph);

    int min=2000, max=-1;
    for (int i=0; i<size; i++){
        if (array[i] > max) max = array[i];
        if (array[i] < min) min = array[i];
    }
    int sceneHeight = graph->geometry().height();
    int sceneWidth = graph->geometry().width();

    float yScale = sceneHeight*1.0 / (max - min);
    float xScale = sceneWidth*1.0 / size;

    //qDebug() << min << ":" << max << ":" << QString::number(xScale,'f',2) << ":" << QString::number(yScale,'f',2) << ":" << sceneRect.height() << ":" << sceneRect.width();
    for (int i=1; i<size; i++){
        graph->scene()->addLine((i-1)*xScale, sceneHeight-(array[i-1]-min)*yScale,
                i*xScale, sceneHeight-(array[i]-min)*yScale, penGraph);
    }

    graph->show();
}

void setupForm::drawGraphXY(QGraphicsView *graph,int *xAarray,int *yAarray, int size){

    clearGraph(graph);

    int min=2000, max=-1;
    for (int i=0; i<size; i++){
        if (yAarray[i] > max) max = yAarray[i];
        if (yAarray[i] < min) min = yAarray[i];
    }
    int sceneHeight = graph->geometry().height();
    float yScale = sceneHeight*1.0 / (max - min);
    qDebug() << max << "-" << min << "-" << yScale;

    min=2000;   max=-1;
    for (int i=0; i<size; i++){
        if (xAarray[i] > max) max = xAarray[i];
        if (xAarray[i] < min) min = xAarray[i];
    }
    int sceneWidth = graph->geometry().width();
    float xScale = sceneWidth*1.0 / iprocess->edgeWidth;
    qDebug() << max << "-" << min << "-" << xScale;

    //qDebug() << min << ":" << max << ":" << QString::number(xScale,'f',2) << ":" << QString::number(yScale,'f',2) << ":" << sceneRect.height() << ":" << sceneRect.width();
    for (int i=1; i<size; i++){
        graph->scene()->addLine(xAarray[i-1]*xScale, sceneHeight-(yAarray[i-1]-min)*yScale,
                xAarray[i]*xScale, sceneHeight-(yAarray[i]-min)*yScale, penGraph);
    }

    graph->show();
}

void setupForm::on_imgParametersButton_clicked(){

    QString message = "";

//    if ( w->play && !w->imageGetter->imageList.isEmpty() ){
    if ( w->play && (w->lastData->image->format() != QImage::Format_Invalid) ){
        message = w->calcImageParametes(*w->lastData->image, true);
    } else if ( !w->play &&  imageLoadedFromFile){
        message = w->calcImageParametes(w->imageFile, true);
    }
    ui->plainTextEdit->appendPlainText(message);
}

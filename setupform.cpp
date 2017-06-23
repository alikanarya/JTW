#include "setupform.h"
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

void setupForm::edgeDetection(imgProcess *iprocess){

    switch ( edgeDetectionState ) {
        case 0: // NONE
            averaging = true;
            matrixFlag = true;
            break;
        case 1: // SOBEL
                /**/if (DEBUG) {
                        edgePath = path + "/sobel/";
                        QDir().mkdir(edgePath);
                    }
            iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
            iprocess->detectEdgeSobel(); // detect edges of the mono image
                /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"01-edge matrix.csv");
                /*D*/if (DEBUG) iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"02-edge image.jpg");
            iprocess->houghTransformFn(iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight);
            averaging = false;
            matrixFlag = false;
            break;
        case 2: // CANNY 4
                /**/if (DEBUG) {
                        edgePath = path + "/canny4/";
                        QDir().mkdir(edgePath);
                    }
            iprocess->prepareCannyArrays();
            iprocess->constructGaussianMatrix(w->gaussianSize, w->stdDev);
                /*D*/if (DEBUG) iprocess->saveMatrix(iprocess->gaussianMatrix, iprocess->gaussianMatrixSize, iprocess->gaussianMatrixSize, edgePath+"02-gaussian matrix.csv");
            for (int i = 0; i < 4 ; i++){
                iprocess->constructValueMatrix( iprocess->imgOrginal, i );
                    /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, edgePath+"03-value matrix"+QString::number(i)+".csv" );
                iprocess->gaussianBlur();
                    /*D*/if (DEBUG) iprocess->getImage( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight )->save(edgePath+"04-blurred image"+QString::number(i)+".jpg");
                iprocess->detectEdgeSobelwDirections();
                    /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"05-edge matrix"+QString::number(i)+".csv");
                    /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeGradientMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"06-edge gradients matrix"+QString::number(i)+".csv");
                    /*D*/if (DEBUG) iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"07-edge image"+QString::number(i)+".jpg");
                iprocess->nonMaximumSuppression(w->cannyThinning);
                    /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"08-edge suppressed matrix"+QString::number(i)+".csv");
                    /*D*/if (DEBUG) iprocess->getImage( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"09-edge suppressed image"+QString::number(i)+".jpg");
                iprocess->cannyThresholding(true);
                    /*D*/if (DEBUG) ui->plainTextEdit->appendPlainText("cannyThresholding"+QString::number(i)+" lo, med, hi: "+QString::number(iprocess->loValue) +", "+QString::number(iprocess->medianValue) +", "+QString::number(iprocess->hiValue));
                    /*D*/if (DEBUG) iprocess->getImage_cannyThresholds(QImage::Format_RGB16)->save(edgePath+"10-edge strongs weaks image"+QString::number(i)+".jpg");
                iprocess->edgeTracing();
                    /*D*/if (DEBUG) iprocess->getImage_cannyTracedEdges(QImage::Format_RGB16)->save(edgePath+"11-edge traced image"+QString::number(i)+".jpg");
                    /*D*/if (DEBUG) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"12-canny image"+QString::number(i)+".jpg");
                    /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"13-edge map matrix"+QString::number(i)+".csv");
                iprocess->assignEdgeMap();
            }
            iprocess->mergeEdgeMaps();
                /*D*/if (DEBUG) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"14-canny combined image.jpg");
                /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"15-edge map matrix.csv");

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
                /**/if (DEBUG) {
                        edgePath = path + "/canny1/";
                        QDir().mkdir(edgePath);
                    }
            iprocess->prepareCannyArrays();
            iprocess->constructGaussianMatrix(w->gaussianSize, w->stdDev);
                /*D*/if (DEBUG) iprocess->saveMatrix(iprocess->gaussianMatrix, iprocess->gaussianMatrixSize, iprocess->gaussianMatrixSize, edgePath+"02-gaussian matrix.csv");
            iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );
                /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, edgePath+"03-value matrix.csv" );
            iprocess->gaussianBlur();
                /*D*/if (DEBUG) iprocess->getImage( iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight )->save(edgePath+"04-blurred image.jpg");
            iprocess->detectEdgeSobelwDirections();
                /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"05-edge matrix.csv");
                /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeGradientMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"06-edge gradients matrix.csv");
                /*D*/if (DEBUG) iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"07-edge image.jpg");
            iprocess->nonMaximumSuppression(w->cannyThinning);
                /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"08-edge suppressed matrix.csv");
                /*D*/if (DEBUG) iprocess->getImage( iprocess->edgeSuppressedMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"09-edge suppressed image.jpg");
            iprocess->cannyThresholding(true);
                /*D*/if (DEBUG) ui->plainTextEdit->appendPlainText("cannyThresholding lo, med, hi: "+QString::number(iprocess->loValue) +", "+QString::number(iprocess->medianValue) +", "+QString::number(iprocess->hiValue));
                /*D*/if (DEBUG) iprocess->getImage_cannyThresholds(QImage::Format_RGB16)->save(edgePath+"10-edge strongs weaks image.jpg");
            iprocess->edgeTracing();
                /*D*/if (DEBUG) iprocess->getImage_cannyTracedEdges(QImage::Format_RGB16)->save(edgePath+"11-edge traced image.jpg");
                /*D*/if (DEBUG) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"12-canny image.jpg");
                /*D*/if (DEBUG) iprocess->saveMatrix( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight, edgePath+"13-edge map matrix.csv");

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

void setupForm::Algo1(imgProcess *iprocess){
// LASER: mono/edge(matrixFlag) > houghTr > detectLongestSolidLines

    if (edgeDetectionState == 0){
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
    }

    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        /**/if (DEBUG) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"max hough lines matrix -dist-angle-vote.csv");

    iprocess->detectLongestSolidLines(averaging, matrixFlag);
    algoPrerequestsOk = true;
    captured = true;
}

void setupForm::Algo2(imgProcess *iprocess){
// LASER: mono/edge(matrixFlag) > houghTr > detectPrimaryVoid

    if (edgeDetectionState == 0){
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
    }

    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        /**/if (DEBUG) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"max hough lines matrix -dist-angle-vote.csv");

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
            /**/if (DEBUG) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"max hough lines matrix -dist-angle-vote.csv");
        iprocess->thinCornerNum = mainEdgesNumber;
        iprocess->detectMainEdges(thinJointAlgoActive, DEBUG);
            /**/if (DEBUG) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"max hough lines distance sorted.csv");
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
        /**/if (DEBUG) iprocess->saveMatrix( iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight, path+"contrast matrix.csv" );
        /**/if (DEBUG) iprocess->getImageContrast().save(path+"contrast image.jpg");
    iprocess->houghTransformContrast();;
    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
        /**/if (DEBUG) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"max hough lines matrix -dist-angle-vote.csv");
    iprocess->calcAvgDistAndAngleOfMajors(0.30);    // calc. avg. distance and theta
        /**/if (DEBUG) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"max hough lines distance sorted.csv");
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
            /**/if (DEBUG) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"max hough lines matrix -dist-angle-vote.csv");
        iprocess->thinCornerNum = 1;//mainEdgesNumber;
        iprocess->detectMainEdges(thinJointAlgoActive, DEBUG);
            /**/if (DEBUG) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"max hough lines distance sorted.csv");
            /**/if (DEBUG) for (int i=0; i<iprocess->mainEdgesList.size();i++)
                    ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue));
        iprocess->thickenEdgeMap(3);
            /**/if (DEBUG) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"canny thickened image.jpg");
        iprocess->scoreLineCrossing(true);
            /**/ui->plainTextEdit->appendPlainText("skor, yüzde: " + QString::number(iprocess->mainEdgeScore) + ", " + QString::number(iprocess->mainEdgeScorePercent, 'f', 1));

        algoPrerequestsOk = true;
        captured = true;
    } else {
        ui->plainTextEdit->appendPlainText("Canny1 kenar tespiti algoritması seçilmelidir");
        algoPrerequestsOk = false;
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

        /*D*/if (DEBUG) {
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
                    case 5: // EXPERIMENTAL
                        path += "-AlgoY/"; break;
                        break;
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
            /*D*/if (DEBUG) iprocess->imgOrginal.save(path+"01-orginal image.jpg");
        iprocessInitSwitch = true;
        iprocess->_DEBUG = true;

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
        /*D*/if (DEBUG) iprocess->imgMono.save(path+"02-mono image.jpg");

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
        captureTimeStr = QDate::currentDate().toString("yyyyMMdd") + "-" + QTime::currentTime().toString("hhmmss");      // for filename

        // -------START IMAGE PROCESSING-------
        startTime = w->timeSystem.getSystemTimeMsec();

        processImage();

        endTime = w->timeSystem.getSystemTimeMsec();
        processElapsed = endTime - startTime;
        // -------END IMAGE PROCESSING-------



        // UPDATE GUI

        QString message = "Analiz " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.";
        ui->plainTextEdit->appendPlainText(message);

        //ui->plainTextEdit->appendPlainText(iprocess->statusMessage);    // display message about detection process
        ui->plainTextEdit->appendPlainText("Sol Köşe (x,y) - Merkez (x,y) - Sağ Köşe (x,y): ");
        message = "( " + QString::number(iprocess->leftCornerX) + " , " + QString::number(iprocess->leftCornerY) + " )  -  ( " +
                         QString::number(iprocess->trackCenterX) + " , " + QString::number(iprocess->trackCenterY) + " )  -  ( " +
                         QString::number(iprocess->rightCornerX) + " , " + QString::number(iprocess->rightCornerY) + " )";
        ui->plainTextEdit->appendPlainText(message);


        ui->labelTarget->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );
        ui->labelMono->setPixmap( QPixmap::fromImage ( iprocess->imgMono ) );

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
                                    ui->plainTextEdit->appendPlainText("dist/ang/vote: "+QString::number(iprocess->listHoughData2ndArray[i][0], 'f', 2) +", "+QString::number(iprocess->listHoughData2ndArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughData2ndArray[i][2], 'f', 2));
                            }

                        ui->labelHough->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges(1) ) );
                            /**/if (DEBUG) iprocess->imgSolidLines.save(path+"mainEdges image.jpg");

                        iprocess->cornerImage();
                            /*D*/if (DEBUG) iprocess->imgCorner.save(path+"--corner image.jpg");
                        ui->plainTextEdit->appendPlainText("leftX-centerX-rightX: " +QString::number(iprocess->leftCornerX)+", "+QString::number(iprocess->trackCenterX)+", "+QString::number(iprocess->rightCornerX));

                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );
                        break;
                    case 2: // THIN JOINT
                        iprocess->cornerImage();
                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );
                        break;
                    case 3: // CONTRAST
                        iprocess->constructContrastMatrixMajor2Lines();
                            /**/if (DEBUG) iprocess->saveMatrix( iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight, savePath + "contrast with lines matrix.csv" );
                        hough = iprocess->getImage(iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight);
                        ui->labelHough->setPixmap( QPixmap::fromImage( *hough ) );

                        iprocess->cornerImage();
                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );
                        ui->plainTextEdit->appendPlainText("avg dist, angle: " + QString::number(iprocess->distanceAvg) + ", " + QString::number(iprocess->thetaAvg));
                        break;
                    case 4: // LINE DETECTION WITH MAIN EDGES
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
                            /**/if (DEBUG) iprocess->imgSolidLines.save(path+"mainEdges image.jpg");
                            /**/if (DEBUG) iprocess->cornerImage().save(path+"corners image.jpg");

                        if ( iprocess->mainEdgeScorePercent > w->lineScoreLimit){
                            ui->plainTextEdit->appendPlainText( "Ana çizgi bulundu, %" + QString::number(iprocess->mainEdgeScorePercent, 'f', 1) );
                        }
                        break;
                    case 5: // EXPERIMENTAL
                        break;
                }

            } else {    // with laser - HORIZONTAL SEARCH

                switch ( algorithmType ) {

                    case 0: // NONE
                        break;
                    case 1: // LONGEST SOLID LINES
                            /**/if (DEBUG) iprocess->saveList(iprocess->solidSpaceMain, path+"solid space main matrix.csv");
                            /**/if (DEBUG) iprocess->saveList(iprocess->solidSpaceMainTrimmed, path+"solid space trimmed matrix.csv");
                            /**/if (DEBUG) iprocess->saveList(iprocess->primaryGroup, path+"primary group matrix.csv");
                            /**/if (DEBUG) iprocess->saveList(iprocess->secondaryGroup, path+"secondary group matrix.csv");

                        if ( iprocess->primaryLineFound && iprocess->secondaryLineFound )
                            iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[1], 0 );
                        else
                            if ( iprocess->primaryLineFound )
                                iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[0], 0 );

                            /**/if (DEBUG) iprocess->imgCornerAndPrimaryLines.save(path+"--major2lines image.jpg");
                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCornerAndPrimaryLines ) );

                            /**/if (DEBUG) iprocess->drawSolidLines(iprocess->majorLines).save(path+"--major lines image.jpg");
                            /**/if (DEBUG && edgeDetectionState != 0)
                                    for (int c=0; c < iprocess->majorLines.size(); c++)
                                        iprocess->drawSolidLines2EdgeMatrix( iprocess->majorLines[c], QImage::Format_RGB16)->save(path+"--major lines image-" + QString::number(c) + ".jpg");
                            /**/if (DEBUG && edgeDetectionState == 0)
                                    for (int c=0; c < iprocess->majorLines.size(); c++)
                                        iprocess->drawLine2OrginalImage( iprocess->majorLines[c], QImage::Format_RGB16).save(path+"--major lines image-" + QString::number(c) + ".jpg");

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

        captured = true;

    } else {

        ui->plainTextEdit->appendPlainText(alarm6);
   }

}

bool setupForm::saveButton(){

    bool saveStatus = true;

    /*
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
    */
    DEBUG = true;

    captureButton();
//    saveAlgo6();

    ui->plainTextEdit->appendPlainText("Dosyalar kaydedildi");
    DEBUG = false;

    return saveStatus;
}

bool setupForm::saveAlgo1(){
    bool saveStatus = true;
    return saveStatus;
}

bool setupForm::saveAlgo2(){
    bool saveStatus = true;
    return saveStatus;
}

bool setupForm::saveAlgo3(){
    bool saveStatus = true;
    return saveStatus;
}

bool setupForm::saveAlgo4(){
    bool saveStatus = true;
    return saveStatus;
}

bool setupForm::saveAlgo5(){
    bool saveStatus = true;
    return saveStatus;
}

bool setupForm::saveAlgo6(){

    bool saveStatus = true;

/*    if (captured){

        path = savePath + "Algo6-" + captureTimeStr + "/";
        QDir().mkdir(path);
        fileName = path + "01-" + fileBaseTarget + fileExt;
        saveStatus = saveStatus && iprocess->imgOrginal.save(fileName);

        fileName = savePath + captureTimeStr + fileBaseMono + fileExt;
        saveStatus = saveStatus && iprocess->imgMono.save(fileName);

        fileName = savePath + captureTimeStr + fileBaseEdge + fileExt;
        saveStatus = saveStatus && edge->save(fileName);

        fileName = savePath + captureTimeStr + fileBaseHough + fileExt;
        saveStatus = saveStatus && hough->save(fileName);

        fileName = savePath + captureTimeStr + fileBaseCorner + fileExt;
        saveStatus = saveStatus && iprocess->imgCorner.save(fileName);

        fileName = savePath + captureTimeStr + fileBaseMono + ".csv";
        saveStatus = saveStatus && iprocess->saveMatrix(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight, fileName);

        fileName = savePath + captureTimeStr + fileBaseEdge + ".csv";
        saveStatus = saveStatus && iprocess->saveMatrix(iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight, fileName);

        fileName = savePath + captureTimeStr + fileBaseHough + ".csv";
        saveStatus = saveStatus && iprocess->saveMatrix(iprocess->houghMatrix, iprocess->edgeWidth, iprocess->edgeHeight, fileName);

        fileName = savePath + captureTimeStr + fileBaseHough + "Lines" + ".csv";
        saveStatus = saveStatus && iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, fileName);

        fileName = savePath + captureTimeStr + fileBaseHough + "Space" + ".csv";
        saveStatus = saveStatus && iprocess->saveMatrix(iprocess->houghSpace, iprocess->houghThetaSize, iprocess->houghDistanceMax, fileName);

        captured = false;
    }
*/
    ui->plainTextEdit->appendPlainText("Dosyalar kaydedildi");

    return saveStatus;
}

void setupForm::getParameters(){

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

    subImageProcessingSwitch = ui->checkSubImage->isChecked();
    ui->editHoughThetaMinSub->setEnabled( ui->checkSubImage->isChecked() );
    ui->editHoughThetaMaxSub->setEnabled( ui->checkSubImage->isChecked() );
    ui->editHoughThetaStepSub->setEnabled( ui->checkSubImage->isChecked() );
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

    thinJointAlgoActive = false;
    thetaMin = w->thetaMinHorLine;
    thetaMax = w->thetaMaxHorLine;
    ui->editHoughThetaMin->setText(QString::number(thetaMin));
    ui->editHoughThetaMax->setText(QString::number(thetaMax));

    ui->algorithmBox->clear();
    ui->algorithmBox->addItem("Yok");
    ui->algorithmBox->addItem("Çizgi");
    ui->algorithmBox->addItem("Boşluk");
    ui->algorithmBox->addItem("Deneme");
}

void setupForm::on_radioWoLaser_clicked() {

    thinJointAlgoActive = true;
    thetaMin = w->thetaMinVerLine;
    thetaMax = w->thetaMaxVerLine;
    ui->editHoughThetaMin->setText(QString::number(thetaMin));
    ui->editHoughThetaMax->setText(QString::number(thetaMax));

    ui->algorithmBox->clear();
    ui->algorithmBox->addItem("Yok");
    ui->algorithmBox->addItem("Ana Kenarlar");
    ui->algorithmBox->addItem("İnce Ağız");
    ui->algorithmBox->addItem("Kontrast");
    ui->algorithmBox->addItem("Ana Kenarlar ile Çizgi");
    ui->algorithmBox->addItem("Deneme");

}

void setupForm::on_edgeDetectionBox_currentIndexChanged(int index){

    edgeDetectionState = index;
}

void setupForm::on_detecAlgoBox_currentIndexChanged(int index){

    lineDetectAlgos = index;
}

void setupForm::on_mainEdgesSlider_valueChanged(int value){
    mainEdgesNumber = value;
}

void setupForm::on_saveButton_clicked()
{

}

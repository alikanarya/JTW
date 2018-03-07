
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

    ui->labelGaussSDev->setText( QString::number(stdDev,'f',2) );
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

    ui->labelMAFilterSize->setText(QString::number(maFilterKernelSize));
    ui->maFilterSizeSlider->setValue((maFilterKernelSize-1)/2);

    ui->labelHistAngle->setText(QString::number(histogramAngleThreshold));
    ui->histAngleSlider->setValue(histogramAngleThreshold);

    // graphs
    scene1 = new QGraphicsScene();
    sceneRect1 = ui->graphicsView->geometry();
    ui->graphicsView->setScene(scene1);

    scene2 = new QGraphicsScene();
    sceneRect2 = ui->graphicsView2->geometry();
    ui->graphicsView2->setScene(scene2);

    scene3 = new QGraphicsScene();
    ui->graphicsView3->setScene(scene3);

    penAxis.setColor(Qt::black);    penAxis.setWidth(2);
    penRed = new QPen(Qt::red);
    penRed->setWidth(1);
    penBlue = new QPen(Qt::blue);
    penBlue->setWidth(1);
    penGreen = new QPen(Qt::green);
    penGreen->setWidth(1);

    ui->tabWidget->setCurrentIndex(0);

    connect(ui->histAreaNoSlider,SIGNAL(valueChanged(int)),this,SLOT(restrictMove_histAreaNoSlider(int)));
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
        iprocess->detectMainEdges(0, DEBUG);
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"04-max hough lines distance sorted.csv");

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
// woLASER: canny1 > houghTr > detectMainEdges > detectMainEdgesSolidLine

    if (edgeDetectionState == 3) {

        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"03-max hough lines matrix -dist-angle-vote.csv");

        iprocess->thinCornerNum = mainEdgesNumber;
        houghData hd = iprocess->detectMainEdgesSolidLine(1, DEBUG);
        solidLineLength = iprocess->solidLineLength;

            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"04-max hough lines distance sorted.csv");
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->listHoughData2ndArray, 3, iprocess->listHoughData2ndSize, path+"05-listHoughData2ndArray.csv");

        hdDistance = hd.distance;
        hdAngle = hd.angle;
        hdVoteValue = hd.voteValue;

        /*
        iprocess->thickenEdgeMap(3);
            //if (saveAnalysis) iprocess->getImage( iprocess->edgeMapMatrix, iprocess->edgeWidth, iprocess->edgeHeight )->save(edgePath+"14-canny thickened image.jpg");
        iprocess->scoreLineCrossing(true);
            //ui->plainTextEdit->appendPlainText("skor, yüzde: " + QString::number(iprocess->mainEdgeScore) + ", " + QString::number(iprocess->mainEdgeScorePercent, 'f', 1));
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

void setupForm::Algo8(int center){
// woLASER: edge > houghTr > detectMainEdges with multiple regions

    areaNumber = w->areaNumber;

    if (edgeDetectionState != 0) {

        int step = (target.height()*1.0) / areaNumber;

        if (!iproList.empty()){
            qDeleteAll(iproList);
            iproList.clear();
        }

        int totalTime = 0;

        for (int area=0; area<areaNumber; area++) {

            QImage pic = target.copy( 0, step*area, target.width(), step );    // take target image
            //pic.save("_area"+QString::number(area)+".jpg");

            startTime = w->timeSystem.getSystemTimeMsec();

            imgProcess *ipro = new imgProcess( pic, pic.width(), pic.height() );   // new imgProcess object
            iproList.append(ipro);

            ipro->thetaMin = thetaMin;
            ipro->thetaMax = thetaMax;
            ipro->thetaStep = thetaStep;
            ipro->maFilterKernelSize = maFilterKernelSize;
            ipro->centerX = 0;
            ipro->centerY = 0;

            ipro->toMono();                                     // convert target to mono
            /*D*/if (saveAnalysis) iprocess->imgMono.save(path+"02-mono image.jpg");

            edgeDetection(ipro);

            ipro->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
                /**/if (saveAnalysis) ipro->saveMatrix(ipro->houghLines, 3, ipro->houghLineNo, path+"03-max hough lines matrix -dist-angle-vote.csv");
            ipro->thinCornerNum = mainEdgesNumber;
            ipro->detectMainEdges(0, DEBUG);
                /**/if (saveAnalysis) ipro->saveMatrix(ipro->houghLinesSorted, 3, ipro->houghLineNo, path+"04-max hough lines distance sorted.csv");

            algoPrerequestsOk = true;
            captured = true;

            endTime = w->timeSystem.getSystemTimeMsec();
            processElapsed = endTime - startTime;
            totalTime += processElapsed;
            // -------END PROCESSING-------

            QString message = "\nKenar Analizi " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.";
            ui->plainTextEdit->appendPlainText(message);

            for (int i=0; i<ipro->mainEdgesList.size();i++)
                ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote: " + QString::number(ipro->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(ipro->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(ipro->mainEdgesList[i].voteValue));
            ui->plainTextEdit->appendPlainText("leftX-centerX-rightX: " +QString::number(ipro->leftCornerX)+", "+QString::number(ipro->trackCenterX)+", "+QString::number(ipro->rightCornerX));
        }

        int refCenter;
        if (center == 0){
            // -------HISTOGRAM PROCESSING-------
            startTime = w->timeSystem.getSystemTimeMsec();

            iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
            iprocess->maFilterKernelSize = maFilterKernelSize;
            iprocess->histogramAngleThreshold = histogramAngleThreshold;
            iprocess->lenRateThr = lenRateThr;
            iprocess->bandWidthMin = bandWidthMin;
            iprocess->bandCenterMax = bandCenterMax;
            iprocess->histDDLimit = histDDLimit;

            iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );

            iprocess->histogramAnalysis(colorMatrix, invertHist);
            histogramCenterX = (iprocess->natBreaksMax1.x()+iprocess->natBreaksMax2.x())/2;

            endTime = w->timeSystem.getSystemTimeMsec();
            processElapsed = endTime - startTime;
            totalTime += processElapsed;
            delete iprocess;

            QString message = "\nHistogram Analizi " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.";
            ui->plainTextEdit->appendPlainText(message);
            // -------END PROCESSING-------
            refCenter = histogramCenterX;
        } else {
            refCenter = center;
        }

        variance = target.width() * 0.02;
        /*
        variance = 0;
        for (int c=0; c<iproList.size(); c++)
            variance += pow( refCenter - iproList[c]->trackCenterX, 2 );
        if ( iproList.size() != 0 ) variance /= iproList.size();
        variance = sqrt(variance);
        */
        QList<int> areaStats;

        int cnt = 0;
        mainEdgeCenterX = 0;
        for (int c=0; c<iproList.size(); c++)
            if ( abs( refCenter - iproList[c]->trackCenterX ) <= variance ) {
                cnt++;
                mainEdgeCenterX += iproList[c]->trackCenterX;
                areaStats.append(0);    // "BAND DOĞRU"
            } else {
                areaStats.append(1);    // "Band merkezden çok uzak"
            }
        if (cnt != 0) mainEdgeCenterX /= (cnt*1.0);
        else mainEdgeCenterX = refCenter;

        ui->plainTextEdit->appendPlainText("\nToplam Süre: " + QString::number(totalTime) + " ms");

        for (int c=0; c<iproList.size(); c++){
//            qDebug() << QString::number(abs(iproList[c]->rightCornerX - iproList[c]->leftCornerX)) << " " << QString::number(bandWidthMin);
            if ( abs(iproList[c]->rightCornerX - iproList[c]->leftCornerX) < (iproList[c]->imageWidth * bandWidthMin) ){
                if ( areaStats[c] == 1 )
                    areaStats[c] = 3;   // "Band merkezden çok uzak" ve "Band genişliği düşük"
                else
                    areaStats[c] = 2;   // "Band genişliği düşük"
            }
        }

        int found2ndPass = 0;
        QString statList = "";
        for (int c=0; c<areaStats.size(); c++) {
            switch ( areaStats[c] ) {
                case 0: ui->plainTextEdit->appendPlainText("*** " + QString(alarm20)); found2ndPass++; break;
                case 1: ui->plainTextEdit->appendPlainText("*** " + QString(alarm25)); break;
                case 2: ui->plainTextEdit->appendPlainText("*** " + QString(alarm24)); break;
                case 3: ui->plainTextEdit->appendPlainText("*** " + QString(alarm25) + " ve " + QString(alarm24)); break;
            }
            if (areaStats[c] == 0)
                statList += "1";
            else
                statList += "0";
        }
        QString stat = "Doğru bölge #: " + QString::number(found2ndPass) + " >> " + statList;
        ui->plainTextEdit->appendPlainText(stat);

    } else {
        ui->plainTextEdit->appendPlainText("Bir kenar tespiti algoritması seçilmelidir");
        algoPrerequestsOk = false;
    }
}

void setupForm::Algo9(imgProcess *iprocess){
// woLASER: edge > houghTr > detectMainEdges > histAnalysis for dark tracks

    if (edgeDetectionState != 0) {
        iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLines, 3, iprocess->houghLineNo, path+"03-max hough lines matrix -dist-angle-vote.csv");
        iprocess->thinCornerNum = mainEdgesNumber;
        iprocess->detectMainEdges(0, DEBUG);
            /**/if (saveAnalysis) iprocess->saveMatrix(iprocess->houghLinesSorted, 3, iprocess->houghLineNo, path+"04-max hough lines distance sorted.csv");
edgeHist = new int[iprocess->edgeWidth];
double totalSum = 0;
for (int x=0; x<iprocess->edgeWidth; x++){
    int sum = 0;
    for (int y=0; y<iprocess->edgeHeight; y++)
         if( iprocess->edgeMapMatrix[y][x] ) sum++;
    edgeHist[x] = sum;
    totalSum += sum;
}
edgeHistAvg = totalSum / iprocess->edgeWidth;

totalSum = 0;
double moments = 0;
for (int x=0; x<iprocess->edgeWidth; x++){
    if (edgeHist[x] > edgeHistAvg) {
        moments += x*edgeHist[x];
        totalSum += edgeHist[x];
    }
}
edgeHistMean = moments/totalSum + 1;
/*
int rightDist = iprocess->edgeWidth - edgeHistMean1;
if (edgeHistMean1 >= rightDist ){
    totalSum = 0;
    double moments = 0;
    for (int x=0; x<iprocess->edgeWidth; x++){
        if (edgeHist[x] > edgeHistAvg) {
            if (x < edgeHistMean1) {
                moments += x*edgeHist[x];
                totalSum += edgeHist[x];
            } else {
                moments += (2*edgeHistMean1-x)*edgeHist[x];
                totalSum += edgeHist[x];
            }

        }
    }
    edgeHistMean = moments/totalSum;
} else {
    totalSum = 0;
    double moments = 0;
    for (int x=0; x<iprocess->edgeWidth; x++){
        if (edgeHist[x] > edgeHistAvg) {
            if (x < edgeHistMean1) {
                moments += (rightDist-edgeHistMean1+x) *edgeHist[x];
                totalSum += edgeHist[x];
            } else {
                moments += (rightDist+edgeHistMean1-x)*edgeHist[x];
                totalSum += edgeHist[x];
            }

        }
    }
    edgeHistMean = moments/totalSum;
}
*/
/*
if (iprocess->edgeWidth != 0) {
    double sampleAve = totalSum / iprocess->edgeWidth;
    double powSum = 0;
    for (int c=0; c<iprocess->edgeWidth; c++)
        powSum += pow(sampleAve-edgeHist[c], 2);
    powSum /= iprocess->edgeWidth;
    edgeHistMean = sqrt(powSum);
}
*/
//qDebug() << edgeHistMean;
iprocess->trackCenterX = edgeHistMean;
        linesForHist.clear();
        linesForHist.append(iprocess->leftCornerX);
        linesForHist.append(iprocess->trackCenterX);
        linesForHist.append(iprocess->rightCornerX);

        // -------START PROCESSING-------

        iprocessHist = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
        iprocessHist->maFilterKernelSize = maFilterKernelSize;
        iprocessHist->bandWidthMin = bandWidthMin;
        iprocessHist->bandCenterMax = bandCenterMax;

        iprocessHist->constructValueMatrix( iprocessHist->imgOrginal, 0 );

        invertHist = ui->invertHist->isChecked();

        iprocessHist->histogramAnalysisDarkTracks(colorMatrix, invertHist);

        // -------END PROCESSING-------

        int _leftLimit = iprocess->trackCenterX / 2.0;
        int _rightLimit = (iprocess->imageWidth - iprocess->trackCenterX) / 2.0  + iprocess->trackCenterX;
        int _leftMostLimit = iprocess->imageWidth * 0.2;
        int _rightMostLimit = iprocess->imageWidth * 0.8;
        int _minLeftX = iprocess->leftCornerX;
        int _minRightX = iprocess->rightCornerX;
        int val;
/*
        QList<QPoint> leftList;
        QList<QPoint> rightList;
        for (int i=0; i<iprocessHist->histogramMins.size(); i++){
            val = iprocessHist->histogramFiltered[ iprocessHist->histogramMins[i].start ];
            if (iprocessHist->histogramMins[i].end < iprocess->trackCenterX && iprocessHist->histogramMins[i].start > _leftLimit ){
                int j=1; bool flag=true; int valMax=val;
                do {
                    if (iprocessHist->histogramPeaks[j].end > iprocessHist->histogramMins[i].start) {
                        valMax = iprocessHist->histogramFiltered[ iprocessHist->histogramPeaks[j-1].end ];
                        flag = false;
                    }
                    j++;
                } while (j<iprocessHist->histogramPeaks.size() && flag);

                QPoint p(iprocessHist->histogramMins[i].end,valMax-val);
                leftList.append(p);
            }

            if (iprocessHist->histogramMins[i].start > iprocess->trackCenterX && iprocessHist->histogramMins[i].end < _rightLimit ){
                int j=0; bool flag=true; int valMax=val;
                do {
                    if (iprocessHist->histogramPeaks[j].start > iprocessHist->histogramMins[i].end) {
                        valMax = iprocessHist->histogramFiltered[ iprocessHist->histogramPeaks[j].end ];
                        flag = false;
                    }
                    j++;
                } while (j<iprocessHist->histogramPeaks.size() && flag);

                QPoint p(iprocessHist->histogramMins[i].start,valMax-val);
                rightList.append(p);
            }
        }
        //qDebug() << leftList;
        //qDebug() << rightList;

        int maxDeriv = 0;
        for (int i=0; i<leftList.size(); i++){
            if (leftList[i].y() > maxDeriv) {
                maxDeriv = leftList[i].y();
                _minLeftX = leftList[i].x();
            }
        }

        maxDeriv = 0;
        for (int i=0; i<rightList.size(); i++){
            if (rightList[i].y() > maxDeriv) {
                maxDeriv = rightList[i].y();
                _minRightX = rightList[i].x();
            }
        }
*/

        int _minLeft = 2000;
        int _minRight = 2000;

        for (int i=0; i<iprocessHist->histogramMins.size(); i++){

            val = iprocessHist->histogramFiltered[ iprocessHist->histogramMins[i].start ];

            if (iprocessHist->histogramMins[i].start > iprocess->trackCenterX && iprocessHist->histogramMins[i].start < _rightLimit ){
                if (val <= _minRight) {
                    _minRight = val;
                    _minRightX = iprocessHist->histogramMins[i].start;
                }
            }

            if (iprocessHist->histogramMins[i].start >= _rightLimit && iprocessHist->histogramMins[i].start < _rightMostLimit && _minRight > iprocessHist->histogramAvg){
                if (val <= _minRight) {
                    _minRight = val;
                    _minRightX = iprocessHist->histogramMins[i].start;
                }
            }
        }

        for (int i=iprocessHist->histogramMins.size()-1; 0<=i; i--){

            val = iprocessHist->histogramFiltered[ iprocessHist->histogramMins[i].end ];

            if (iprocessHist->histogramMins[i].end < iprocess->trackCenterX && iprocessHist->histogramMins[i].end > _leftLimit ){
                if (val <= _minLeft) {
                    _minLeft = val;
                    _minLeftX = iprocessHist->histogramMins[i].end;
                }
            }

            if (iprocessHist->histogramMins[i].end <= _leftLimit && iprocessHist->histogramMins[i].end > _leftMostLimit && _minLeft > iprocessHist->histogramAvg){
                if (val <= _minLeft) {
                    _minLeft = val;
                    _minLeftX = iprocessHist->histogramMins[i].end;
                }
            }
        }
/*
        for (int i=0; i<iprocessHist->histogramMins.size(); i++){

            val = iprocessHist->histogramFiltered[ iprocessHist->histogramMins[i].start ];

            if (iprocessHist->histogramMins[i].end < iprocess->trackCenterX && iprocessHist->histogramMins[i].end > _leftLimit ){
                if (val < _minLeft) {
                    _minLeft = val;
                    _minLeftX = iprocessHist->histogramMins[i].end;
                }
            }

            if (iprocessHist->histogramMins[i].end <= _leftLimit && iprocessHist->histogramMins[i].end > _leftMostLimit && _minLeft > iprocessHist->histogramAvg){
                if (val < _minLeft) {
                    _minLeft = val;
                    _minLeftX = iprocessHist->histogramMins[i].end;
                }
            }

            if (iprocessHist->histogramMins[i].start > iprocess->trackCenterX && iprocessHist->histogramMins[i].start < _rightLimit ){
                if (val < _minRight) {
                    _minRight = val;
                    _minRightX = iprocessHist->histogramMins[i].start;
                }
            }

            if (iprocessHist->histogramMins[i].start >= _rightLimit && iprocessHist->histogramMins[i].start < _rightMostLimit && _minRight > iprocessHist->histogramAvg){
                if (val < _minRight) {
                    _minRight = val;
                    _minRightX = iprocessHist->histogramMins[i].start;
                }
            }
        }
*/
        linesForHist.append(_minLeftX);
        linesForHist.append(_minRightX);
        //qDebug() << linesForHist;

        for (int i=0; i<iprocess->mainEdgesList.size();i++)
            ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue));
        ui->plainTextEdit->appendPlainText("leftX-centerX-rightX: " +QString::number(iprocess->leftCornerX)+", "+QString::number(iprocess->trackCenterX)+", "+QString::number(iprocess->rightCornerX));
        ui->labelHough->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges( iprocess->naturalBreaksNumber ) ) );

        iprocess->natBreaksMax1.setX(_minLeftX);
        iprocess->natBreaksMax2.setX(_minRightX);
        iprocess->mainEdgesList[0].distance = _minLeftX;
        iprocess->mainEdgesList[1].distance = _minRightX;
        iprocess->mainEdgesList[0].angle = iprocess->mainEdgesList[1].angle = 0;
        iprocess->leftCornerX = _minLeftX;
        iprocess->rightCornerX = _minRightX;
        iprocess->trackCenterX = (_minLeftX+_minRightX) / 2.0;
        iprocess->centerLine.distance = iprocess->trackCenterX;

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
            targetCopy = w->imageFileChanged.copy( 0, 0, w->imageFileChanged.width(), w->imageFileChanged.height() );
        } else {
            //target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
            target = w->lastData->image->copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
            targetCopy = w->lastData->image->copy( 0, 0, w->lastData->image->width(), w->lastData->image->height() );
        }
    }

    if ( !w->play &&  imageLoadedFromFile){
//        target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
        target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
        targetCopy = w->imageFileChanged.copy( 0, 0, w->imageFileChanged.width(), w->imageFileChanged.height() );
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
                    case 6: // MAIN EDGES MULTIPLE AREAS
                        path += "-Algo3MA/"; break;
                    case 7: // MAIN EDGES WITH HISTOGRAM DARK
                        path += "-Algo9/"; break;
                    case 8: // EXPERIMENTAL
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

        if (!(thinJointAlgoActive && algorithmType==6)){
            iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
                /*D*/if (saveAnalysis) iprocess->imgOrginal.save(path+"01-orginal image.jpg");
            iprocessInitSwitch = true;
            //iprocess->_DEBUG = true;

            iprocess->thetaMin = thetaMin;
            iprocess->thetaMax = thetaMax;
            iprocess->thetaStep = thetaStep;
            iprocess->maFilterKernelSize = maFilterKernelSize;

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
        }
//on_testButton_clicked();
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
                case 6: // MAIN EDGES MULTIPLE AREAS
                    Algo8(target.width()/2.0);
                    break;
                case 7: // MAIN EDGES WITH HISTOGRAM DARK
                    Algo9(iprocess);
                    break;
                case 8: // EXPERIMENTAL
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

        ui->labelTarget->setPixmap( QPixmap::fromImage( target ) );
        QString message = "";

        if (!(thinJointAlgoActive && algorithmType==6)){
            message = "\nAnaliz " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.";
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
                                /*ui->plainTextEdit->appendPlainText("-1st-maximas---");
                                for (int i=0; i<iprocess->localMaximaSize;i++)
                                    ui->plainTextEdit->appendPlainText("start: "+QString::number(iprocess->rangeArray[i][0]) +" stop: "+QString::number(iprocess->rangeArray[i][1]));

                                ui->plainTextEdit->appendPlainText("-1st hough vals---");
                                for (int i=0; i<iprocess->listHoughDataSize;i++)
                                    ui->plainTextEdit->appendPlainText("dist/ang/vote: "+QString::number(iprocess->listHoughDataArray[i][0], 'f', 2) +", "+QString::number(iprocess->listHoughDataArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughDataArray[i][2], 'f', 2));

                                ui->plainTextEdit->appendPlainText("-2nd-maximas---");
                                for (int i=0; i<iprocess->localMaxima2ndSize;i++)
                                    ui->plainTextEdit->appendPlainText("start: "+QString::number(iprocess->rangeArray2nd[i][0]) +" stop: "+QString::number(iprocess->rangeArray2nd[i][1]));
                                */
                                ui->plainTextEdit->appendPlainText("-2nd hough vals---");
                                for (int i=0; i<iprocess->listHoughData2ndSize;i++)
                                    ui->plainTextEdit->appendPlainText("dist/ang/vote: "+QString::number(iprocess->listHoughData2ndArray[i][0], 'f', 1) +", "+QString::number(iprocess->listHoughData2ndArray[i][1], 'f', 1)+", "+QString::number(iprocess->listHoughData2ndArray[i][2], 'f', 0));

                                if ( iprocess->naturalBreaksNumber != 0 ){
                                    ui->plainTextEdit->appendPlainText("-pointListSorted---");
                                    for (int i=0; i<iprocess->pointListSorted.size();i++)
                                        ui->plainTextEdit->appendPlainText("x/vote: "+QString::number(iprocess->pointListSorted[i].x())+", "+QString::number(iprocess->pointListSorted[i].y()));
                                    ui->plainTextEdit->appendPlainText("-mainPointsList---");
                                    for (int i=0; i<iprocess->mainPointsList.size();i++)
                                        ui->plainTextEdit->appendPlainText("x/vote: "+QString::number(iprocess->mainPointsList[i].x())+", "+QString::number(iprocess->mainPointsList[i].y()));
                                }
                            }


                        for (int i=0; i<iprocess->mainEdgesList.size();i++)
                            ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue));

                        //ui->labelHough->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges(mainEdgesNumber) ) );
                        ui->labelHough->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges_2ndList(true) ) );
                            /**/if (saveAnalysis) iprocess->imgSolidLines.save(path+"05-mainEdges image.jpg");

                        ui->plainTextEdit->appendPlainText("leftX-centerX-rightX: " +QString::number(iprocess->leftCornerX)+", "+QString::number(iprocess->trackCenterX)+", "+QString::number(iprocess->rightCornerX));

                        //iprocess->cornerImage();
                            //*D*/if (saveAnalysis) iprocess->imgCorner.save(path+"06-corner image.jpg");
                        //ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );

                        ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges( iprocess->naturalBreaksNumber ) ) );
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

                        if (solidLineLength != -1) {
                            ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->getImageMainEdges(0) ) );
                            /**/if (saveAnalysis) iprocess->imgSolidLines.save(path+"06-mainEdge image.jpg");
                        }

                        if (DEBUG) ui->plainTextEdit->appendPlainText("-2nd hough vals max solid lines---");

                        int *graphArray = new int[iprocess->listHoughData2ndSize];
                        int *xArray = new int[iprocess->listHoughData2ndSize];
                        for (int i=0; i<iprocess->listHoughData2ndSize; i++){
                            xArray[i] = iprocess->listHoughData2ndArray[i][0];
                            graphArray[i] = iprocess->listHoughData2ndArray[i][2];  // vote values
                        }

                        int *graphArray2 = new int[iprocess->listHoughData2ndSize];
                        for (int i=0; i<iprocess->listHoughData2ndSize;i++) {
                            int length = iprocess->detectLongestSolidLineVert(iprocess->listHoughData2ndArray[i][0], iprocess->listHoughData2ndArray[i][1], 0, iprocess->edgeHeight-1).length;
                            graphArray2[i] = length;    // max length of solid lines for each hough data
                            if (DEBUG) ui->plainTextEdit->appendPlainText("dist/ang/vote/length: "+QString::number(iprocess->listHoughData2ndArray[i][0], 'f', 0) +", "+QString::number(iprocess->listHoughData2ndArray[i][1], 'f', 2)+", "+QString::number(iprocess->listHoughData2ndArray[i][2], 'f', 0) + ", " + QString::number(length));
                        }

                        for (int i=0; i<iprocess->mainEdgesList.size();i++)
                            ui->plainTextEdit->appendPlainText("mainEdges dist/ang/vote: " + QString::number(iprocess->mainEdgesList[i].distance, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].angle, 'f', 1) + ", " + QString::number(iprocess->mainEdgesList[i].voteValue));

                        ui->plainTextEdit->appendPlainText( "dist/ang/vote: " + QString::number(hdDistance) + " / " + QString::number(hdAngle, 'f', 2) + " / "+ QString::number(hdVoteValue) );
                        ui->plainTextEdit->appendPlainText( "trackCenterX: " + QString::number(iprocess->trackCenterX) );
                        ui->plainTextEdit->appendPlainText( "length/score(%): " + QString::number(solidLineLength) + " / " + QString::number(iprocess->mainEdgeScorePercent, 'f', 2) );

                        clearGraph(ui->graphicsView);
                        clearGraph(ui->graphicsView2);
                        clearGraph(ui->graphicsView3);

                        // vote graph
                        drawGraphXY(ui->graphicsView, xArray, graphArray, iprocess->listHoughData2ndSize);
                        // max solid line graph
                        //drawGraphXY(ui->graphicsView2, xArray, graphArray2, iprocess->listHoughData2ndSize);


//                        ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );
//                        ui->labelTarget2->setPixmap( QPixmap::fromImage( *iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight ) ) );
                        ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal.convertToFormat(QImage::Format_Grayscale8) ) );


                        /*if ( iprocess->mainEdgeScorePercent > w->lineScoreLimit){
                            ui->plainTextEdit->appendPlainText( "Ana çizgi bulundu, %" + QString::number(iprocess->mainEdgeScorePercent, 'f', 1) );
                        }*/

                        }
                        break;

                    case 5: // SCAN HORIZONTAL
                        {
                        int *graphArray = new int[iprocess->edgeWidth];
                        for (int i=0; i<iprocess->edgeWidth; i++){
                            graphArray[i] = iprocess->horLineVotes[i][2];
                        }
                        drawGraph(ui->graphicsView, penRed, graphArray, iprocess->edgeWidth, QPoint(-1,-1));

                        int *valArray = new int[iprocess->imageWidth];
                        float sum = 0;
                        for (int x=0; x<iprocess->imageWidth; x++){
                            sum = 0;
                            for (int y=0; y<iprocess->imageHeight; y++)
                                sum += iprocess->valueMatrix[y][x];
                            valArray[x] = sum / iprocess->imageHeight;
                        }
                        drawGraph(ui->graphicsView2, penRed, valArray, iprocess->imageWidth, QPoint(-1,-1));

                        ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );

                        }break;
                    case 6: // MAIN EDGES MULTIPLE AREAS
                        {
                        ui->plainTextEdit->appendPlainText("---------------------");
                        QString msg = "Area Centers: ";
                        for (int i=0; i<areaNumber; i++)
                            msg +=  QString::number(iproList[i]->trackCenterX) + ",";
                        ui->plainTextEdit->appendPlainText(msg);
                        ui->plainTextEdit->appendPlainText("Histogram Center: " + QString::number(histogramCenterX));
                        ui->plainTextEdit->appendPlainText("Variance: " + QString::number(variance,'f',1) + " Ortalama Merkez: " + QString::number(mainEdgeCenterX));

                        // draw global mono & edge images
                        iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
                        iprocess->centerX = 0; iprocess->centerY = 0;
                        iprocess->toMono();                                     // convert target to mono
                        edgeDetection(iprocess);
                        if ( edgeDetectionState != 0) {
                            edge = iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight );   // produce edge image
                            ui->labelEdge->setPixmap( QPixmap::fromImage( *edge ) );
                        }

                        delete iprocess;

                        int imgWidth = iproList[0]->imageWidth;
                        int imgHeight = 0;
                        for (int i=0; i<areaNumber; i++)
                            imgHeight += iproList[i]->imageHeight;

                        // draw listHoughData2nd image
                        QImage mainEdgesImage2ndList(imgWidth, imgHeight, QImage::Format_RGB16); // image to hold the join of image 1 & 2
                        QPainter mainEdgesImagePainter2ndList(&mainEdgesImage2ndList);
                        int y = 0;
                        mainEdgesImagePainter2ndList.setPen(QPen(Qt::yellow));
                        for (int i=0; i<areaNumber; i++){
                            mainEdgesImagePainter2ndList.drawImage(0, y, iproList[i]->getImageMainEdges_2ndList(true));
                            y += iproList[i]->imageHeight;
                            mainEdgesImagePainter2ndList.drawLine(0, y-1, imgWidth, y-1);
                        }
                        ui->labelHough->setPixmap( QPixmap::fromImage(mainEdgesImage2ndList) );

                        // draw main edges image
                        QImage mainEdgesImage(imgWidth, imgHeight, QImage::Format_RGB16); // image to hold the join of image 1 & 2
                        QPainter mainEdgesImagePainter(&mainEdgesImage);
                        y = 0;
                        mainEdgesImagePainter.setPen(QPen(Qt::yellow));
                        for (int i=0; i<areaNumber; i++){
                            mainEdgesImagePainter.drawImage(0, y, iproList[i]->getImageMainEdges( iproList[i]->naturalBreaksNumber ));
                            y += iproList[i]->imageHeight;
                            mainEdgesImagePainter.drawLine(0, y-1, imgWidth, y-1);
                        }
                        ui->labelAnalyze->setPixmap( QPixmap::fromImage(mainEdgesImage) );

                        //ui->labelHough->setPixmap( QPixmap::fromImage( iproList[2]->getImageMainEdges_2ndList(true) ) );
                        //ui->labelAnalyze->setPixmap( QPixmap::fromImage( iproList[2]->getImageMainEdges( iproList[2]->naturalBreaksNumber ) ) );
                        }
                        break;
                    case 7: // MAIN EDGES WITH HISTOGRAM DARK
                        {
                        /**/if (DEBUG) {
                                ui->plainTextEdit->appendPlainText("-2nd hough vals---");
                                for (int i=0; i<iprocess->listHoughData2ndSize;i++)
                                    ui->plainTextEdit->appendPlainText("dist/ang/vote: "+QString::number(iprocess->listHoughData2ndArray[i][0], 'f', 1) +", "+QString::number(iprocess->listHoughData2ndArray[i][1], 'f', 1)+", "+QString::number(iprocess->listHoughData2ndArray[i][2], 'f', 0));

                                if ( iprocess->naturalBreaksNumber != 0 ){
                                    ui->plainTextEdit->appendPlainText("-pointListSorted---");
                                    for (int i=0; i<iprocess->pointListSorted.size();i++)
                                        ui->plainTextEdit->appendPlainText("x/vote: "+QString::number(iprocess->pointListSorted[i].x())+", "+QString::number(iprocess->pointListSorted[i].y()));
                                    ui->plainTextEdit->appendPlainText("-mainPointsList---");
                                    for (int i=0; i<iprocess->mainPointsList.size();i++)
                                        ui->plainTextEdit->appendPlainText("x/vote: "+QString::number(iprocess->mainPointsList[i].x())+", "+QString::number(iprocess->mainPointsList[i].y()));
                                }
                            }

                        ui->labelAnalyze->clear();

                        clearGraph(ui->graphicsView);
                        clearGraph(ui->graphicsView2);
                        clearGraph(ui->graphicsView3);

                        if (DEBUG) {
                            for (int i=0; i<iprocessHist->histogramPeaks.size(); i++)
                                ui->plainTextEdit->appendPlainText("hist peaks i/start/end/val: " +QString::number(i) +", "+QString::number(iprocessHist->histogramPeaks[i].start) +", "+QString::number(iprocessHist->histogramPeaks[i].end) +", " + QString::number(iprocessHist->histogramFiltered[ iprocessHist->histogramPeaks[i].start ]) );
                            for (int i=0; i<iprocessHist->histogramMins.size(); i++)
                                ui->plainTextEdit->appendPlainText("hist mins i/start/end/val: " +QString::number(i) +", "+QString::number(iprocessHist->histogramMins[i].start) +", "+QString::number(iprocessHist->histogramMins[i].end) +", " + QString::number(iprocessHist->histogramFiltered[ iprocessHist->histogramMins[i].start ]) );
                        }
                        if (colorMatrix)
                            ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );
                        else
                            ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal.convertToFormat(QImage::Format_Grayscale8) ) );

                        if (iprocessHist->bandCheck_errorState!=10 && !iprocessHist->histogramExtremesFiltered.isEmpty()) {
                            drawExtremes = false;
                            drawhistogramMaxPoint = false;
                            clearGraph(ui->graphicsView);
                            drawGraphHist(ui->graphicsView, penRed, edgeHist, iprocess->edgeWidth, QPoint(-1,-1), true);
                            drawEdges = false;
                            drawExtremes = true;
                            clearGraph(ui->graphicsView2);
                            drawGraphHist2(iprocessHist, ui->graphicsView2, penRed, iprocessHist->histogramFiltered, iprocessHist->histogramSize, QPoint(-1,-1), true); // recursive MA filter
                            drawEdges = true;
                            drawExtremes = false;
                            clearGraph(ui->graphicsView3);
                            drawGraphHist2(iprocessHist, ui->graphicsView3, penRed, iprocessHist->histogramFiltered, iprocessHist->histogramSize, QPoint(-1,-1), true); // recursive MA filter
                        }
                        iprocess->getImageMainEdges( iprocess->naturalBreaksNumber );

                        QPixmap px = QPixmap::fromImage(iprocess->imgSolidLines);
                        QPainter p(&px);
                        p.setPen(QPen(Qt::red, 3, Qt::DotLine));
                        p.drawLine (linesForHist[1], 0, linesForHist[1], iprocess->imgSolidLines.height());
                        p.end ();

                        ui->labelAnalyze->setPixmap( px );

                        ui->plainTextEdit->appendPlainText("Corrected leftX-centerX-rightX: " +QString::number(iprocess->leftCornerX)+", "+QString::number(iprocess->trackCenterX)+", "+QString::number(iprocess->rightCornerX));

                        delete iprocessHist;
                        }
                        break;
                    case 8: // EXPERIMENTAL
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

        if (!(thinJointAlgoActive && algorithmType==6)){
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
        }


        ui->plainTextEdit->appendPlainText("-----------------");

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

    maFilterKernelSize = w->maFilterKernelSize;
    histogramAngleThreshold = w->histogramAngleThreshold;
    colorMatrix = w->colorMatrix;
    lenRateThr = w->lenRateThr;
    bandWidthMin = w->bandWidthMin;
    bandCenterMax = w->bandCenterMax;
    histAreaNo = w->histAreaNo;
    twoPassWelding = w->twoPassWelding;
    autoDetect2ndPass = w->autoDetect2ndPass;
    timeControlTwoPass = w->timeControlTwoPass;

    ui->labelMAFilterSize->setText( QString::number(maFilterKernelSize) );
    ui->maFilterSizeSlider->setValue( (int)(maFilterKernelSize/2-1) );
    ui->labelHistAngle->setText( QString::number(histogramAngleThreshold) );
    ui->histAngleSlider->setValue( histogramAngleThreshold );
    ui->radioColored->setChecked( colorMatrix );
    ui->radioGray->setChecked( !colorMatrix );
    ui->labellenRateThr->setText( QString::number(lenRateThr) );
    ui->lenRateThrSlider->setValue( lenRateThr*100 );
    ui->labelBandWidthMin->setText( QString::number(bandWidthMin) );
    ui->bandWidthMinSlider->setValue( bandWidthMin*100 );
    ui->labelBandCenterMax->setText( QString::number(bandCenterMax) );
    ui->bandCenterMaxSlider->setValue( bandCenterMax*100 );
    ui->labelHistAreaNo->setText( QString::number(histAreaNo) );
    ui->histAreaNoSlider->setValue( histAreaNo );
        on_histAreaNoSlider_sliderReleased();
    ui->twoPassWeldingBox->setChecked( twoPassWelding );
    ui->autoDetect2ndPassBox->setChecked( autoDetect2ndPass );
    ui->autoDetect2ndPassBox->setEnabled(twoPassWelding);
    ui->twoPassTimeControlBox->setChecked( timeControlTwoPass );
    ui->twoPassTimeControlBox->setEnabled(twoPassWelding);
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

    w->maFilterKernelSize = maFilterKernelSize;
    w->histogramAngleThreshold = histogramAngleThreshold;
    w->colorMatrix = colorMatrix;
    w->lenRateThr = lenRateThr;
    w->bandWidthMin = bandWidthMin;
    w->bandCenterMax = bandCenterMax;
    w->histAreaNo = histAreaNo;
    w->histLo = 1; //round(histAreaNo * w->histLoRate);
    w->histHi = round(histAreaNo * w->histHiRate);

    w->twoPassWelding = ui->twoPassWeldingBox->isChecked();
    w->autoDetect2ndPass = ui->autoDetect2ndPassBox->isChecked();
    w->timeControlTwoPass = ui->twoPassTimeControlBox->isChecked();

    w->ui->passOneButton->setEnabled( twoPassWelding && !autoDetect2ndPass );
    w->ui->passTwoButton->setEnabled( twoPassWelding && !autoDetect2ndPass );
    w->ui->targetDriftLeft->setEnabled( false );
    w->ui->targetDriftCenter->setEnabled( false );
    w->ui->targetDriftRight->setEnabled( false );

    if (w->twoPassWelding){
        if(w->autoDetect2ndPass) {
            if (!w->trackOn) {
                w->ui->passOneButton->setStyleSheet(w->SS_OFF);
                w->ui->passTwoButton->setStyleSheet(w->SS_OFF);
            }
        } else {
            w->on_passOneButton_clicked();
        }
        w->ui->passOneButton->show();
        w->ui->passTwoButton->show();
        w->ui->targetDriftLeft->show();
        w->ui->targetDriftCenter->show();
        w->ui->targetDriftRight->show();
    } else {
        w->ui->passOneButton->hide();
        w->ui->passTwoButton->hide();
        w->ui->passOneButton->setStyleSheet(w->SS_OFF);
        w->ui->passTwoButton->setStyleSheet(w->SS_OFF);
        w->ui->targetDriftLeft->hide();
        w->ui->targetDriftCenter->hide();
        w->ui->targetDriftRight->hide();
    }

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
        w->ui->timeEdit->show();
        w->ui->timeEdit->setText(QString::number(w->timeLimit));
    } else {
        w->ui->labelTimeTag->hide();
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
        ui->checkProcessingHist->setEnabled(true);

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
    if (ui->checkProcessingHist->isChecked())
        //on_histogramAnalysisButton_clicked();
    on_testButton_clicked();
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
    ui->labelGaussSDev->setText(QString::number(stdDev,'f',2));
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
            case 4: algoName = "Algo6: woLASER: canny1 > houghTr > detectMainEdges > detectMainEdgesSolidLine";
                    ui->label_21->setEnabled(true);
                    ui->mainEdgesSlider->setEnabled(true);
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
            case 6: algoName = "Algo8: woLASER: edge > houghTr > detectMainEdges with multiple regions";
                    ui->label_21->setEnabled(true);
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
                    break;  // EXPERIMENTAL
            case 7: algoName = "Algo3: edge > houghTr > detectMainEdges > histAnalysis for dark tracks";
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
    ui->algorithmBox->addItem("Ana Kenarlar: Bölgeli");
    ui->algorithmBox->addItem("Ana Kenar+Histogram");
    ui->algorithmBox->addItem("Deneme");
    if (algo > 8) algo = 8;
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

void setupForm::on_saveButton_clicked(){}

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

void setupForm::drawGraph(QGraphicsView *graph, QPen *pen, int *array, int size, QPoint yRange, bool scaleMin) {

    //clearGraph(graph);
    //graph->setScene( new  QGraphicsScene() );

    int min=0, max=-1;

    if (yRange.x() == -1 && yRange.y() == -1) {
        if ( scaleMin ) {
            min = 2000;
        }

        for (int i=0; i<size; i++){
            if (array[i] > max) max = array[i];
            if (scaleMin) if (array[i] < min) min = array[i];
        }
    } else {
        min = yRange.x();
        max = yRange.y();
    }

    int sceneHeight = graph->geometry().height();
    int sceneWidth = graph->geometry().width();

    float yScale = sceneHeight*1.0 / (max - min);
    float xScale = sceneWidth*1.0 / size;

    //qDebug() << min << ":" << max << ":" << QString::number(xScale,'f',2) << ":" << QString::number(yScale,'f',2) << ":" << graph->geometry().height() << ":" << graph->geometry().width();
    graph->update(graph->geometry());

    // to fix graphic scaling issue
    penX.setColor(Qt::white);penAxis.setWidth(1);
    graph->scene()->addLine(0, 0, sceneWidth, sceneHeight, penX);

    for (int i=1; i<size; i++){
        //if (array[i] == max) qDebug() << max << " - " << (int)(sceneHeight-(array[i]-min)*yScale);
        graph->scene()->addLine((i-1)*xScale, (int)(sceneHeight-(array[i-1]-min)*yScale), i*xScale, (int)(sceneHeight-(array[i]-min)*yScale), *pen);
    }

    int y0 = (int)(sceneHeight-(0-min)*yScale);
    graph->scene()->addLine(0, y0, sceneWidth, y0, QPen(Qt::black));

    graph->show();
}

void setupForm::drawGraphHist(QGraphicsView *graph, QPen *pen, int *array, int size, QPoint yRange, bool scaleMin) {

    //clearGraph(graph);
    //graph->setScene( new  QGraphicsScene() );

    int min=0, max=-1;

    if (yRange.x() == -1 && yRange.y() == -1) {
        if ( scaleMin ) {
            min = 2000;
        }

        for (int i=0; i<size; i++){
            if (array[i] > max) max = array[i];
            if (scaleMin) if (array[i] < min) min = array[i];
        }
    } else {
        min = yRange.x();
        max = yRange.y();
    }

    int sceneHeight = graph->geometry().height();
    int sceneWidth = graph->geometry().width();

    float yScale = sceneHeight*0.9 / (max - min);
    int yOffset = sceneHeight*0.05;
    float xScale = sceneWidth*1.0 / size;

    //qDebug() << min << ":" << max << ":" << QString::number(xScale,'f',2) << ":" << QString::number(yScale,'f',2) << ":" << graph->geometry().height() << ":" << graph->geometry().width();
    graph->update(graph->geometry());

    // to fix graphic scaling issue
    penX.setColor(Qt::white);penAxis.setWidth(1);
    graph->scene()->addLine(0, 0, sceneWidth, sceneHeight, penX);

    for (int i=1; i<size; i++){
        //if (array[i] == max) qDebug() << max << " - " << (int)(sceneHeight-(array[i]-min)*yScale);
        //graph->scene()->addLine((i-1)*xScale, (int)(sceneHeight-(array[i-1]-min)*yScale), i*xScale, (int)(sceneHeight-(array[i]-min)*yScale), *pen);
        graph->scene()->addLine((i-1)*xScale, (int)(sceneHeight-yOffset-(array[i-1]-min)*yScale), i*xScale, (int)(sceneHeight-yOffset-(array[i]-min)*yScale), *pen);
    }

    if (edgeHistMean != 0){
        graph->scene()->addLine(edgeHistMean*xScale, 0, edgeHistMean*xScale, sceneHeight, QPen(Qt::green));
        graph->scene()->addLine(0, (int)(sceneHeight-yOffset-(edgeHistAvg-min)*yScale), sceneWidth, (int)(sceneHeight-yOffset-(edgeHistAvg-min)*yScale), QPen(Qt::blue));
    }

    // **** int avgVal = (int)(sceneHeight-(iprocess->histogramAvg-min)*yScale);
    //graph->scene()->addLine(0, avgVal, sceneWidth, avgVal, *penGreen);

//    for (int i=1; i<iprocess->histogramExtremes.size(); i++)
//        graph->scene()->addLine(iprocess->histogramExtremes[i-1].end*xScale, (int)(sceneHeight-(iprocess->histogramFiltered[ iprocess->histogramExtremes[i-1].end ]-min)*yScale), iprocess->histogramExtremes[i].start*xScale, (int)(sceneHeight-(iprocess->histogramFiltered[ iprocess->histogramExtremes[i].start ]-min)*yScale), *penBlue);

    if (drawExtremes) {
        for (int i=0; i<iprocess->histogramExtremes.size(); i++) {
            //graph->scene()->addEllipse(iprocess->histogramExtremes[i].end*xScale - 1, (int)(sceneHeight-(iprocess->histogramFiltered[ iprocess->histogramExtremes[i].end ]-min)*yScale) - 1, 2, 2, *penBlue, QBrush(Qt::blue));
            QPen pS, pE;
            if (iprocess->histogramExtremes[i].start == iprocess->histogramExtremes[i].end){
                pS.setColor(Qt::green); pE.setColor(Qt::green);
            } else {
                pS.setColor(Qt::blue); pE.setColor(Qt::black);
            }
            graph->scene()->addLine(iprocess->histogramExtremes[i].start*xScale, (int)(sceneHeight-(iprocess->histogramFiltered[ iprocess->histogramExtremes[i].start ]-min)*yScale -10),
                                    iprocess->histogramExtremes[i].start*xScale, (int)(sceneHeight-(iprocess->histogramFiltered[ iprocess->histogramExtremes[i].start ]-min)*yScale +10), pS);
            graph->scene()->addLine(iprocess->histogramExtremes[i].end*xScale, (int)(sceneHeight-(iprocess->histogramFiltered[ iprocess->histogramExtremes[i].end ]-min)*yScale -10),
                                    iprocess->histogramExtremes[i].end*xScale, (int)(sceneHeight-(iprocess->histogramFiltered[ iprocess->histogramExtremes[i].end ]-min)*yScale +10), pE);
        }
    }

    if (drawhistogramMaxPoint) {
        penBlue->setWidth(2);
        for (int i=0; i<iprocess->histogramMaxPoint.size(); i++){
            graph->scene()->addLine(iprocess->histogramMaxPoint[i].x()*xScale, (int)(sceneHeight-(iprocess->histogramMaxPoint[i].y()-min)*yScale), iprocess->histogramMaxPointPair[i].x()*xScale, (int)(sceneHeight-(iprocess->histogramMaxPointPair[i].y()-min)*yScale), *penBlue);
        }
    }

    graph->show();
}

void setupForm::drawGraphHist2(imgProcess *ipro, QGraphicsView *graph, QPen *pen, int *array, int size, QPoint yRange, bool scaleMin) {

    int min=0, max=-1;

    if (yRange.x() == -1 && yRange.y() == -1) {
        if ( scaleMin ) {
            min = 2000;
        }

        for (int i=0; i<size; i++){
            if (array[i] > max) max = array[i];
            if (scaleMin) if (array[i] < min) min = array[i];
        }
    } else {
        min = yRange.x();
        max = yRange.y();
    }

    int sceneHeight = graph->geometry().height();
    int sceneWidth = graph->geometry().width();

    float yScale = sceneHeight*0.9 / (max - min);
    int yOffset = sceneHeight*0.05;
    float xScale = sceneWidth*1.0 / size;

    graph->update(graph->geometry());

    // to fix graphic scaling issue
    penX.setColor(Qt::white);penAxis.setWidth(1);
    graph->scene()->addLine(0, 0, sceneWidth, sceneHeight, penX);

    for (int i=1; i<size; i++){
        graph->scene()->addLine((i-1)*xScale, (int)(sceneHeight-yOffset-(array[i-1]-min)*yScale), i*xScale, (int)(sceneHeight-yOffset-(array[i]-min)*yScale), *pen);
    }

    int avgVal = (int)(sceneHeight-yOffset-(ipro->histogramAvg-min)*yScale);
    graph->scene()->addLine(0, avgVal, sceneWidth, avgVal, *penGreen);

    if (drawExtremes) {
        for (int i=0; i<ipro->histogramExtremesFiltered.size(); i++){
            //graph->scene()->addEllipse(iprocess->histogramExtremesFiltered[i].end*xScale - 1, (int)(sceneHeight-(iprocess->histogramFiltered[ iprocess->histogramExtremesFiltered[i].end ]-min)*yScale) - 1, 2, 2, *penBlue, QBrush(Qt::blue));
            QPen pS, pE;
            pS.setWidth(2); pE.setWidth(2);
            if (ipro->histogramExtremesFiltered[i].start == ipro->histogramExtremesFiltered[i].end){
                pS.setColor(Qt::gray); pE.setColor(Qt::gray);
            } else {
                pS.setColor(Qt::blue); pE.setColor(Qt::black);
            }

            graph->scene()->addLine(ipro->histogramExtremesFiltered[i].start*xScale, (int)(sceneHeight-yOffset-(ipro->histogramFiltered[ ipro->histogramExtremesFiltered[i].start ]-min)*yScale -10),
                                    ipro->histogramExtremesFiltered[i].start*xScale, (int)(sceneHeight-yOffset-(ipro->histogramFiltered[ ipro->histogramExtremesFiltered[i].start ]-min)*yScale +10), pS);
            graph->scene()->addLine(ipro->histogramExtremesFiltered[i].end*xScale, (int)(sceneHeight-yOffset-(ipro->histogramFiltered[ ipro->histogramExtremesFiltered[i].end ]-min)*yScale -10),
                                    ipro->histogramExtremesFiltered[i].end*xScale, (int)(sceneHeight-yOffset-(ipro->histogramFiltered[ ipro->histogramExtremesFiltered[i].end ]-min)*yScale +10), pE);
        }
    }

    if (drawEdges) {
        QPen pen;
        pen.setWidth(2);
        for (int i=0; i<ipro->mainPointsList.size(); i++){
            int y=0, idx = 0;
            for (int j=0; j<ipro->histogramMaxPoint.size(); j++){
                if (ipro->mainPointsList[i].x() == ipro->histogramMaxPoint[j].x()){
                    y = ipro->histogramMaxPoint[j].y();
                    idx = j;
                }
            }
            if (ipro->mainPointsList[i].x() == ipro->natBreaksMax1.x() || ipro->mainPointsList[i].x() == ipro->natBreaksMax2.x())
                pen.setColor(Qt::green);    // primary lines
            else
                pen.setColor(Qt::blue);

            graph->scene()->addLine(ipro->mainPointsList[i].x()*xScale, (int)(sceneHeight-yOffset-(y-min)*yScale), ipro->histogramMaxPointPair[idx].x()*xScale, (int)(sceneHeight-yOffset-(ipro->histogramMaxPointPair[idx].y()-min)*yScale), pen);
        }

        //for (int i=0; i<ipro->histogramMaxPoint.size(); i++)
          //  graph->scene()->addLine(ipro->histogramMaxPoint[i].x()*xScale, (int)(sceneHeight-yOffset-(ipro->histogramMaxPoint[i].y()-min)*yScale), ipro->histogramMaxPointPair[i].x()*xScale, (int)(sceneHeight-yOffset-(ipro->histogramMaxPointPair[i].y()-min)*yScale), *penBlue);
    }

    if (!linesForHist.isEmpty()) {
        for (int i=0; i<linesForHist.size(); i++){
            if (i==3)
                graph->scene()->addLine(linesForHist[i]*xScale, 0, linesForHist[i]*xScale, sceneHeight, QPen(Qt::yellow));
            else if (i==4)
                graph->scene()->addLine(linesForHist[i]*xScale, 0, linesForHist[i]*xScale, sceneHeight, QPen(Qt::red));
            else
                graph->scene()->addLine(linesForHist[i]*xScale, 0, linesForHist[i]*xScale, sceneHeight, QPen(Qt::green));

        }
    }

    graph->show();
}

void setupForm::drawGraphList(QGraphicsView *graph, QPen *pen, QList<range> list, int *yVals, QPoint xRange, QPoint yRange) {

    //clearGraph(graph);
    //graph->setScene( new  QGraphicsScene() );

    int min=0, max=-1;

    if (yRange.x() == -1 && yRange.y() == -1) {
        min = 2000;
        for (int i=0; i<list.size(); i++){
            if (yVals[ list[i].start ] > max) max = yVals[ list[i].start ];
            if (yVals[ list[i].start ] < min) min = yVals[ list[i].start ];
        }
    } else {
        min = yRange.x();
        max = yRange.y();
    }

    int sceneHeight = graph->geometry().height();
    int sceneWidth = graph->geometry().width();

    float yScale = sceneHeight*1.0 / (max - min);
    float xScale = sceneWidth*1.0 / (xRange.y()-xRange.x());

    //qDebug() << min << ":" << max << ":" << QString::number(xScale,'f',2) << ":" << QString::number(yScale,'f',2) << ":" << graph->geometry().height() << ":" << graph->geometry().width();
    graph->update(graph->geometry());

    // to fix graphic scaling issue
    penX.setColor(Qt::white);penAxis.setWidth(1);
    graph->scene()->addLine(0, 0, sceneWidth, sceneHeight, penX);

    pen->setWidth(2);
    graph->scene()->addLine(xRange.x()*xScale, (int)(sceneHeight-(yVals[ list[0].start ]-min)*yScale), list[0].start*xScale, (int)(sceneHeight-(yVals[ list[0].start ]-min)*yScale), *pen);

    for (int i=1; i<list.size(); i++){
        //if (array[i] == max) qDebug() << max << " - " << (int)(sceneHeight-(array[i]-min)*yScale);
        graph->scene()->addLine(list[i-1].end*xScale, (int)(sceneHeight-(yVals[ list[i-1].end ]-min)*yScale), list[i].start*xScale, (int)(sceneHeight-(yVals[ list[i].start ]-min)*yScale), *pen);
    }

    graph->scene()->addLine(list[list.size()-1].end*xScale, (int)(sceneHeight-(yVals[ list[list.size()-1].end ]-min)*yScale), xRange.y()*xScale, (int)(sceneHeight-(yVals[ list[list.size()-1].end ]-min)*yScale), *pen);
    pen->setWidth(1);

    penBlue->setWidth(2);
    for (int i=0; i<iprocess->histogramMaxPoint.size(); i++){
        graph->scene()->addLine(iprocess->histogramMaxPoint[i].x()*xScale, (int)(sceneHeight-(iprocess->histogramMaxPoint[i].y()-min)*yScale), iprocess->histogramMaxPointPair[i].x()*xScale, (int)(sceneHeight-(iprocess->histogramMaxPointPair[i].y()-min)*yScale), *penBlue);
    }
    penBlue->setWidth(2);

    int avgVal = (int)(sceneHeight-(iprocess->histogramAvg-min)*yScale);
    graph->scene()->addLine(0, avgVal, sceneWidth, avgVal, *penGreen);

    graph->show();
}

void setupForm::drawGraphXY(QGraphicsView *graph,int *xAarray,int *yAarray, int size){

    //clearGraph(graph);
    graph->setScene( new  QGraphicsScene() );

    int min=2000, max=-1;
    for (int i=0; i<size; i++){
        if (yAarray[i] > max) max = yAarray[i];
        if (yAarray[i] < min) min = yAarray[i];
    }
    int sceneHeight = graph->geometry().height();
    float yScale = sceneHeight*1.0 / (max - min);
    //qDebug() << max << "-" << min << "-" << yScale;

    min=2000;   max=-1;
    for (int i=0; i<size; i++){
        if (xAarray[i] > max) max = xAarray[i];
        if (xAarray[i] < min) min = xAarray[i];
    }
    int sceneWidth = graph->geometry().width();
    float xScale = sceneWidth*1.0 / iprocess->edgeWidth;
    //qDebug() << max << "-" << min << "-" << xScale;

    //qDebug() << min << ":" << max << ":" << QString::number(xScale,'f',2) << ":" << QString::number(yScale,'f',2) << ":" << sceneRect.height() << ":" << sceneRect.width();
    for (int i=1; i<size; i++){
        graph->scene()->addLine(xAarray[i-1]*xScale, sceneHeight-(yAarray[i-1]-min)*yScale,
                xAarray[i]*xScale, sceneHeight-(yAarray[i]-min)*yScale, *penRed);
    }

    graph->update(graph->geometry());
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

void setupForm::on_testButton_clicked(){
    //targetCopy.save(savePath+QDateTime::currentDateTime().toString("hhmmss_zzz")+".jpg");
}

void setupForm::on_maFilterSizeSlider_sliderMoved(int position){
    maFilterKernelSize = 2*position+1;
    ui->labelMAFilterSize->setText(QString::number(maFilterKernelSize));
}

void setupForm::on_maFilterSizeSlider_sliderReleased(){
    update();
}

void setupForm::on_histogramAnalysisButton_clicked() {

    extend = ui->extendHistHeight->isChecked();
    invertHist = ui->invertHist->isChecked();

    on_histAreaNoSlider_sliderMoved(histAreaNo);

    if ( w->play && (w->lastData->image->format() != QImage::Format_Invalid) ){
        if (w->applyCameraEnhancements) {
            //target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
            if (extend)
                target = w->imageFileChanged.copy( w->offsetXCam, 0, w->frameWidthCam, w->lastData->image->height() );    // take target image
            else
                target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
        } else {
            //target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
            if (extend)
                target = w->lastData->image->copy( w->offsetXCam, 0, w->frameWidthCam, w->lastData->image->height() );    // take target image
            else
                target = w->lastData->image->copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
            //qDebug()<<w->offsetXCam<<" 0 "<<" "<<w->frameWidthCam<<" "<<w->lastData->image->height();
        }
    }

    if ( !w->play &&  imageLoadedFromFile){
        //target = w->imageFileChanged.copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight );    // take target image
        if (extend)
            target = w->imageFileChanged.copy( w->offsetXCam, 0, w->frameWidthCam, w->imageFileChanged.height() );    // take target image
        else
            target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
    }

//    if ( !w->imageGetter->imageList.isEmpty() || imageLoadedFromFile ){
    if ( (w->lastData->image->format() != QImage::Format_Invalid) || imageLoadedFromFile ){

        if (iprocessInitSwitch) {
            delete iprocess;
            iprocessInitSwitch = false;
        }

        ui->labelTarget->setPixmap( QPixmap::fromImage( target ) );
        //ui->labelMono->clear();
        ui->labelEdge->clear();
        ui->labelHough->clear();
        ui->labelAnalyze->clear();

        clearGraph(ui->graphicsView);
        clearGraph(ui->graphicsView2);
        clearGraph(ui->graphicsView3);


        if (histAreaNo == 1) {
            // -------START PROCESSING-------
            startTime = w->timeSystem.getSystemTimeMsec();

            iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
            iprocessInitSwitch = true;
            iprocess->maFilterKernelSize = maFilterKernelSize;
            iprocess->histogramAngleThreshold = histogramAngleThreshold;
            iprocess->lenRateThr = lenRateThr;
            iprocess->bandWidthMin = bandWidthMin;
            iprocess->bandCenterMax = bandCenterMax;
            iprocess->histDDLimit = histDDLimit;

            iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );

            iprocess->histogramAnalysis(colorMatrix, invertHist);

            endTime = w->timeSystem.getSystemTimeMsec();
            processElapsed = endTime - startTime;
            // -------END PROCESSING-------

            QString message = "\nAnaliz " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.";
            ui->plainTextEdit->appendPlainText(message);

            if (DEBUG) {
                for (int i=0; i<iprocess->histogramPeaks.size(); i++)
                    ui->plainTextEdit->appendPlainText("hist peaks i/start/end/vote: " +QString::number(i) +", "+QString::number(iprocess->histogramPeaks[i].start) +", "+QString::number(iprocess->histogramPeaks[i].end) +", " + QString::number(iprocess->histogramFiltered[ iprocess->histogramPeaks[i].start ]) );
                for (int i=0; i<iprocess->histogramMins.size(); i++)
                    ui->plainTextEdit->appendPlainText("hist mins i/start/end/vote: " +QString::number(i) +", "+QString::number(iprocess->histogramMins[i].start) +", "+QString::number(iprocess->histogramMins[i].end) +", " + QString::number(iprocess->histogramFiltered[ iprocess->histogramMins[i].start ]) );
            }

            if (colorMatrix)
                ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal ) );
            else
                ui->labelTarget2->setPixmap( QPixmap::fromImage( iprocess->imgOrginal.convertToFormat(QImage::Format_Grayscale8) ) );

            if (iprocess->bandCheck_errorState!=10) {
                drawEdges = false;
                drawExtremes = true;
                clearGraph(ui->graphicsView2);
                drawGraphHist2(iprocess, ui->graphicsView2, penRed, iprocess->histogramFiltered, iprocess->histogramSize, QPoint(-1,-1), true); // recursive MA filter
                drawEdges = true;
                drawExtremes = false;
                clearGraph(ui->graphicsView3);
                drawGraphHist2(iprocess, ui->graphicsView3, penRed, iprocess->histogramFiltered, iprocess->histogramSize, QPoint(-1,-1), true); // recursive MA filter
            }

            ui->plainTextEdit->appendPlainText("hist/D/DD max/min: " +
                                               QString::number(iprocess->histogramMax) +" / "+ QString::number(iprocess->histogramMin)  + " , " +
                                               QString::number(iprocess->histogramDMax) +" / "+ QString::number(iprocess->histogramDMin)  + " , " +
                                               QString::number(iprocess->histogramDDMax) +" / "+ QString::number(iprocess->histogramDDMin) );

            double histRange = iprocess->histogramMax - iprocess->histogramMin;

            QString s;
            for (int i=0; i<iprocess->histogramMaxPoint.size(); i++){
                s = "";
                if (iprocess->breakPointList.indexOf(iprocess->histogramMaxPoint[i].x()) != -1)
                    s += "*"; // break point
                if (iprocess->mainPointsList.indexOf( QPoint(iprocess->histogramMaxPoint[i].x(), iprocess->histogramMaxPointLen[i])) != -1)
                    s += "+"; // chosen point

                ui->plainTextEdit->appendPlainText(s+"peak/pair 0(x,y) P(x,y) (len,deg,len%): (" +
                                                   QString::number(iprocess->histogramMaxPoint[i].x()) + "," + QString::number(iprocess->histogramMaxPoint[i].y()) + ") (" +
                                                   QString::number(iprocess->histogramMaxPointPair[i].x()) + "," + QString::number(iprocess->histogramMaxPointPair[i].y()) + ") (" +
                                                   QString::number(iprocess->histogramMaxPointLen[i],'f',0) + " : " + QString::number(qRadiansToDegrees(qAtan(iprocess->histogramMaxPointAng[i])),'f',2) + " : " + QString::number(iprocess->histogramMaxPointLen[i]/histRange,'f',2) +
                                                   ")**" + QString::number(iprocess->histogramMaxPoint[i].y()-iprocess->histogramMaxPointPair[i].y())
                                                   );
            }
            int centerX = (iprocess->natBreaksMax1.x()+iprocess->natBreaksMax2.x())/2;
            ui->plainTextEdit->appendPlainText("bandWidth: " + QString::number(iprocess->bandWidth) + " / " + QString::number((1.0*iprocess->bandWidth)/iprocess->imageWidth, 'f', 2) +
                                               "  bandCenter: (" + QString::number(centerX) + ") " + QString::number(iprocess->bandCenter) + " / " + QString::number((1.0*iprocess->bandCenter)/iprocess->imageWidth, 'f', 3) +
                                               "  bandShape: " + QString::number(iprocess->bandShape, 'f', 2) );

            switch ( iprocess->bandCheck_errorState ) {
                case 0: ui->plainTextEdit->appendPlainText("*** " + QString(alarm20)); break;
                case 1: ui->plainTextEdit->appendPlainText("*** " + QString(alarm21)); break;
                case 2: ui->plainTextEdit->appendPlainText("*** " + QString(alarm22)); break;
                case 3: ui->plainTextEdit->appendPlainText("*** " + QString(alarm23)); break;
                case 4: ui->plainTextEdit->appendPlainText("*** " + QString(alarm24)); break;
                case 5: ui->plainTextEdit->appendPlainText("*** " + QString(alarm25)); break;
                case 6: ui->plainTextEdit->appendPlainText("*** " + QString(alarm26)); break;
                case 7: ui->plainTextEdit->appendPlainText("*** " + QString(alarm27)); break;
            }
        } else {
            histMultAreas();
        }
    }
}

void setupForm::histMultAreas() {

    int step = (target.height()*1.0) / histAreaNo;

    if (!iproList.empty()){
        qDeleteAll(iproList);
        iproList.clear();
    }

    int found2ndPass = 0;
    QList<int> status;
    int totalTime = 0;

    for (int area=0; area<histAreaNo; area++) {

        QImage pic = target.copy( 0, step*area, target.width(), step );    // take target image
        //pic.save("_area"+QString::number(area)+".jpg");

        startTime = w->timeSystem.getSystemTimeMsec();

        imgProcess *ipro = new imgProcess( pic, pic.width(), pic.height() );   // new imgProcess object
        iproList.append(ipro);
        ipro->maFilterKernelSize = maFilterKernelSize;
        ipro->histogramAngleThreshold = histogramAngleThreshold;
        ipro->lenRateThr = lenRateThr;
        ipro->bandWidthMin = bandWidthMin;
        ipro->bandCenterMax = bandCenterMax;
        ipro->histDDLimit = histDDLimit;

        ipro->constructValueMatrix( ipro->imgOrginal, 0 );

        ipro->histogramAnalysis(colorMatrix, invertHist);

        endTime = w->timeSystem.getSystemTimeMsec();
        processElapsed = endTime - startTime;
        totalTime += processElapsed;
        // -------END PROCESSING-------

        QString message = "\nAnaliz " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.";
        ui->plainTextEdit->appendPlainText(message);

        ui->plainTextEdit->appendPlainText("hist/D/DD max/min: " +
                                           QString::number(ipro->histogramMax) +" / "+ QString::number(ipro->histogramMin)  + " , " +
                                           QString::number(ipro->histogramDMax) +" / "+ QString::number(ipro->histogramDMin)  + " , " +
                                           QString::number(ipro->histogramDDMax) +" / "+ QString::number(ipro->histogramDDMin) );

        double histRange = ipro->histogramMax - ipro->histogramMin;

        QString s;
        for (int i=0; i<ipro->histogramMaxPoint.size(); i++){
            s = "";
            if (ipro->breakPointList.indexOf(ipro->histogramMaxPoint[i].x()) != -1)
                s += "*"; // break point
            if (ipro->mainPointsList.indexOf( QPoint(ipro->histogramMaxPoint[i].x(), ipro->histogramMaxPointLen[i])) != -1)
                s += "+"; // chosen point

            ui->plainTextEdit->appendPlainText(s+"peak/pair 0(x,y) P(x,y) (len,deg,len%): (" +
                                               QString::number(ipro->histogramMaxPoint[i].x()) + "," + QString::number(ipro->histogramMaxPoint[i].y()) + ") (" +
                                               QString::number(ipro->histogramMaxPointPair[i].x()) + "," + QString::number(ipro->histogramMaxPointPair[i].y()) + ") (" +
                                               QString::number(ipro->histogramMaxPointLen[i],'f',0) + " : " + QString::number(qRadiansToDegrees(qAtan(ipro->histogramMaxPointAng[i])),'f',2) + " : " + QString::number(ipro->histogramMaxPointLen[i]/histRange,'f',2) +
                                               ")**" + QString::number(ipro->histogramMaxPoint[i].y()-ipro->histogramMaxPointPair[i].y())
                                               );
        }
        ui->plainTextEdit->appendPlainText("bandWidth: " + QString::number(ipro->bandWidth) + " / " + QString::number((1.0*ipro->bandWidth)/ipro->imageWidth, 'f', 2) +
                                           "  bandCenter: " + QString::number(ipro->bandCenter) + " / " + QString::number((1.0*ipro->bandCenter)/ipro->imageWidth, 'f', 3) +
                                           "  bandShape: " + QString::number(ipro->bandShape, 'f', 2) );

        switch ( ipro->bandCheck_errorState ) {
            case 0: ui->plainTextEdit->appendPlainText("*** " + QString(alarm20)); found2ndPass++; break;
            case 1: ui->plainTextEdit->appendPlainText("*** " + QString(alarm21)); break;
            case 2: ui->plainTextEdit->appendPlainText("*** " + QString(alarm22)); break;
            case 3: ui->plainTextEdit->appendPlainText("*** " + QString(alarm23)); break;
            case 4: ui->plainTextEdit->appendPlainText("*** " + QString(alarm24)); break;
            case 5: ui->plainTextEdit->appendPlainText("*** " + QString(alarm25)); break;
            case 6: ui->plainTextEdit->appendPlainText("*** " + QString(alarm26)); break;
            case 7: ui->plainTextEdit->appendPlainText("*** " + QString(alarm27)); break;
        }

        if ( ipro->bandCheck_errorState == 0 )
            status.append(1);
        else
            status.append(0);

        if (DEBUG) {
            QString path = savePath + "histogram/";
            QDir().mkdir(path);
            clearGraph(ui->graphicsView3);
            drawGraphHist2(ipro, ui->graphicsView3, penRed, ipro->histogramFiltered, ipro->histogramSize, QPoint(-1,-1), true); // recursive MA filter
            QPixmap pixMap = ui->graphicsView3->grab();
            pixMap.save(path+"_graph"+QString::number(area)+".jpg");
            //ipro->saveArray(ipro->histogramFiltered, ipro->histogramSize, path+"_histogramFiltered"+QString::number(area)+".csv");
            ipro->saveArray(ipro->histogramDD, ipro->histogramSize, path+"_histogramDD"+QString::number(area)+".csv");
            //ipro->saveList(ipro->histogramDIdx,path+"_histogramDIdx"+QString::number(area)+".csv");
        }
        //delete ipro;
    }


    ui->plainTextEdit->appendPlainText("\nToplam Süre " + QString::number(totalTime) + " ms");

    if (found2ndPass == 0)
        ui->plainTextEdit->appendPlainText("\n...BAND YOK");
    if (found2ndPass > 0 && found2ndPass < histAreaNo)
        ui->plainTextEdit->appendPlainText("\n...BAND GEÇİŞİ");
    if (found2ndPass == histAreaNo)
        ui->plainTextEdit->appendPlainText("\n...BAND BULUNDU");
    QString stat = "Doğru bölge #: " + QString::number(found2ndPass) + " >> ";
    for (int i=0; i<status.size(); i++)
        stat += QString::number(status[i]);
    ui->plainTextEdit->appendPlainText(stat);

    QString msg;
    for (int i=0; i<iproList.size(); i++) {
        msg = "#" + QString::number(i+1) + " ";
        if (iproList[i]->bandCheck_errorState != 0){
            switch ( iproList[i]->bandCheck_errorState ) {
                case 1: msg += QString(alarm21); break;
                case 2: msg += QString(alarm22); break;
                case 3: msg += QString(alarm23); break;
                case 4: msg += QString(alarm24); break;
                case 5: msg += QString(alarm25); break;
                case 6: msg += QString(alarm26); break;
                case 7: msg += QString(alarm27); break;
            }
            ui->plainTextEdit->appendPlainText(msg);
        }
    }

    if (!iproList.isEmpty() && iproList[0]->bandCheck_errorState!=10) {
        drawEdges = false;
        drawExtremes = true;
        clearGraph(ui->graphicsView2);
        drawGraphHist2(iproList[0], ui->graphicsView2, penRed, iproList[0]->histogramFiltered, iproList[0]->histogramSize, QPoint(-1,-1), true); // recursive MA filter
        drawEdges = true;
        drawExtremes = false;
        clearGraph(ui->graphicsView3);
        drawGraphHist2(iproList[0], ui->graphicsView3, penRed, iproList[0]->histogramFiltered, iproList[0]->histogramSize, QPoint(-1,-1), true); // recursive MA filter
        /*
        QString path = savePath + "histogram/";
        iproList[0]->saveArray(iproList[0]->histogramFiltered, iproList[0]->histogramSize, path+"_histogramFiltered"+QString::number(histDDLimit*10)+".csv");
        iproList[0]->saveArray(iproList[0]->histogramD, iproList[0]->histogramSize, path+"_histogramD"+QString::number(histDDLimit*10)+".csv");
        iproList[0]->saveArray(iproList[0]->histogramDD, iproList[0]->histogramSize, path+"_histogramDD"+QString::number(histDDLimit*10)+".csv");
        */
        /*
        if ( iproList[0]->naturalBreaksNumber != 0 ){
            ui->plainTextEdit->appendPlainText("-histogramMaxPoint---");
            for (int i=0; i<iproList[0]->histogramMaxPoint.size();i++)
                ui->plainTextEdit->appendPlainText("x/vote: "+QString::number(iproList[0]->histogramMaxPoint[i].x())+", "+QString::number(iproList[0]->histogramMaxPoint[i].y()));
            ui->plainTextEdit->appendPlainText("-mainPointsList---");
            for (int i=0; i<iproList[0]->mainPointsList.size();i++)
                ui->plainTextEdit->appendPlainText("x/lentgh: "+QString::number(iproList[0]->mainPointsList[i].x())+", "+QString::number(iproList[0]->mainPointsList[i].y()));
        }
        */ /*
        qDebug() << "--histogramExtremes-xS-xE-yS-yE---------";
        for (int i=0; i<iproList[0]->histogramExtremes.size();i++)
            qDebug() << iproList[0]->histogramExtremes[i].start << "," << iproList[0]->histogramExtremes[i].end << ","<< iproList[0]->histogramFiltered[ iproList[0]->histogramExtremes[i].start ] << ","<< iproList[0]->histogramFiltered[ iproList[0]->histogramExtremes[i].end ];
        qDebug() << "--histogramExtremesFiltered-xS-xE-yS-yE---------";
        for (int i=0; i<iproList[0]->histogramExtremesFiltered.size();i++)
            qDebug() << iproList[0]->histogramExtremesFiltered[i].start << "," << iproList[0]->histogramExtremesFiltered[i].end << ","<< iproList[0]->histogramFiltered[ iproList[0]->histogramExtremesFiltered[i].start ] << ","<< iproList[0]->histogramFiltered[ iproList[0]->histogramExtremesFiltered[i].end ];
        */ /*
        //for (int i=0; i<iproList[8]->histogramMaxPeaksList.size();i++)
            qDebug() << "histogramMaxPeaksList " << iproList[8]->histogramMaxPeaksList;
        for (int i=0; i<iproList[8]->histogramMaxPoint.size();i++)
            qDebug() << iproList[8]->histogramMaxPoint[i].x() << "," << iproList[8]->histogramMaxPoint[i].y();
         qDebug() << "avg: " << iproList[8]->histogramAvg;
         */
    }
    graphLock = false;
    ui->regionBox->setEnabled(true);
    /*
    ui->plainTextEdit->appendPlainText("hist list start/end/vote: ");
    for (int i=0; i<iproList[0]->histogramExtremes.size(); i++) {
        ui->plainTextEdit->appendPlainText(QString::number(iproList[0]->histogramExtremes[i].start) +", "+QString::number(iproList[0]->histogramExtremes[i].end) +", " + QString::number(iproList[0]->histogramFiltered[ iproList[0]->histogramExtremes[i].start ]) );
    }
    ui->plainTextEdit->appendPlainText("histF list start/end/vote: ");
    for (int i=0; i<iproList[0]->histogramExtremesFiltered.size(); i++) {
        ui->plainTextEdit->appendPlainText(QString::number(iproList[0]->histogramExtremesFiltered[i].start) +", "+QString::number(iproList[0]->histogramExtremesFiltered[i].end) +", " + QString::number(iproList[0]->histogramFiltered[ iproList[0]->histogramExtremesFiltered[i].start ]) );
    }
    */
}

void setupForm::on_histAngleSlider_sliderMoved(int position){
    ui->labelHistAngle->setText(QString::number(position));
}

void setupForm::on_histAngleSlider_sliderReleased(){
    histogramAngleThreshold = ui->histAngleSlider->value();
}

void setupForm::on_radioColored_clicked(){
    colorMatrix = true;
}

void setupForm::on_radioGray_clicked(){
    colorMatrix = false;
}

void setupForm::on_lenRateThrSlider_sliderMoved(int position){
    ui->labellenRateThr->setText(QString::number(position/100.0));
}

void setupForm::on_lenRateThrSlider_sliderReleased(){
    lenRateThr = ui->lenRateThrSlider->value()/100.0;
}

void setupForm::on_bandWidthMinSlider_sliderMoved(int position){

    ui->labelBandWidthMin->setText(QString::number(position/100.0));

    if ( w->play && (w->lastData->image->format() != QImage::Format_Invalid) ){
        if (w->applyCameraEnhancements) {
            //target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
            target = w->imageFileChanged.copy( w->offsetXCam, 0, w->frameWidthCam, w->lastData->image->height() );    // take target image
        } else {
            //target = w->lastData->image->copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
            target = w->lastData->image->copy( w->offsetXCam, 0, w->frameWidthCam, w->lastData->image->height() );    // take target image
        }
    }

    if ( !w->play &&  imageLoadedFromFile){
        //target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
        target = w->imageFileChanged.copy( w->offsetXCam, 0, w->frameWidthCam, w->imageFileChanged.height() );    // take target image
    }

    if ( (w->lastData->image->format() != QImage::Format_Invalid) || imageLoadedFromFile ){

        int width = target.width() * (position/100.0);
        int x0 = ( target.width() - width ) / 2;
        int x1 = x0 + width;

        QPixmap px = QPixmap::fromImage(target);
        QPainter p(&px);
        p.setPen (QPen(Qt::red, 3));
        p.drawLine (x0, 0, x0, target.height());
        p.drawLine (x1, 0, x1, target.height());
        p.setPen(QPen(Qt::green, 3));
        p.drawLine (target.width()/2, 0, target.width()/2, target.height());
        p.end ();

        ui->labelTarget->setPixmap( px );
        //ui->labelMono->setPixmap( QPixmap::fromImage( target ) );
    }
}

void setupForm::on_bandWidthMinSlider_sliderReleased(){

    bandWidthMin = ui->bandWidthMinSlider->value()/100.0;
}

void setupForm::on_bandCenterMaxSlider_sliderMoved(int position){

    ui->labelBandCenterMax->setText(QString::number(position/100.0));

    if ( w->play && (w->lastData->image->format() != QImage::Format_Invalid) ){
        if (w->applyCameraEnhancements) {
            //target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
            target = w->imageFileChanged.copy( w->offsetXCam, 0, w->frameWidthCam, w->lastData->image->height() );    // take target image
        } else {
            //target = w->lastData->image->copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
            target = w->lastData->image->copy( w->offsetXCam, 0, w->frameWidthCam, w->lastData->image->height() );    // take target image
        }
    }

    if ( !w->play &&  imageLoadedFromFile){
        //target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
        target = w->imageFileChanged.copy( w->offsetXCam, 0, w->frameWidthCam, w->imageFileChanged.height() );    // take target image
    }

    if ( (w->lastData->image->format() != QImage::Format_Invalid) || imageLoadedFromFile ){

        int width = target.width() * (position/100.0);
        int x0 = ( target.width() - width ) / 2;
        int x1 = x0 + width;

        QPixmap px = QPixmap::fromImage(target);
        QPainter p(&px);
        p.setPen(QPen(Qt::blue, 3));
        p.drawLine (x0, 0, x0, target.height());
        p.drawLine (x1, 0, x1, target.height());
        p.setPen(QPen(Qt::green, 3));
        p.drawLine (target.width()/2, 0, target.width()/2, target.height());
        p.end ();

        ui->labelTarget->setPixmap( px );
        //ui->labelMono->setPixmap( QPixmap::fromImage( target ) );
    }
}

void setupForm::on_bandCenterMaxSlider_sliderPressed(){}

void setupForm::on_bandCenterMaxSlider_sliderReleased(){
    bandCenterMax = ui->bandCenterMaxSlider->value()/100.0;
}

void setupForm::on_twoPassWeldingBox_clicked(bool checked){
    twoPassWelding = checked;
    if( !twoPassWelding ){
        autoDetect2ndPass = false;
        ui->autoDetect2ndPassBox->setChecked(false);
        ui->twoPassTimeControlBox->setChecked(false);
    }

    ui->autoDetect2ndPassBox->setEnabled(twoPassWelding);
    ui->twoPassTimeControlBox->setEnabled(twoPassWelding);
}

void setupForm::on_autoDetect2ndPassBox_clicked(){
    autoDetect2ndPass = ui->autoDetect2ndPassBox->isChecked();
    timeControlTwoPass = !autoDetect2ndPass;
    ui->twoPassTimeControlBox->setChecked(timeControlTwoPass);
}

void setupForm::on_twoPassTimeControlBox_clicked(){
    timeControlTwoPass = ui->twoPassTimeControlBox->isChecked();
    autoDetect2ndPass = !timeControlTwoPass;
    ui->autoDetect2ndPassBox->setChecked(autoDetect2ndPass);
}

void setupForm::on_histAreaNoSlider_sliderMoved(int position){
    ui->labelHistAreaNo->setText(QString::number(position));

    if ( w->play && (w->lastData->image->format() != QImage::Format_Invalid) ){
        if (w->applyCameraEnhancements) {
            if (extend)
                target = w->imageFileChanged.copy( w->offsetXCam, 0, w->frameWidthCam, w->lastData->image->height() );    // take target image
            else
                target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
        } else {
            if (extend)
                target = w->lastData->image->copy( w->offsetXCam, 0, w->frameWidthCam, w->lastData->image->height() );    // take target image
            else
                target = w->lastData->image->copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
        }
    }

    if ( !w->play &&  imageLoadedFromFile){
        if (extend)
            target = w->imageFileChanged.copy( w->offsetXCam, 0, w->frameWidthCam, w->imageFileChanged.height() );    // take target image
        else
            target = w->imageFileChanged.copy( w->offsetXCam, w->offsetYCam, w->frameWidthCam, w->frameHeightCam );    // take target image
    }

    if ( (w->lastData->image->format() != QImage::Format_Invalid) || imageLoadedFromFile ){

        QPixmap px = QPixmap::fromImage(target);
        QPainter p(&px);
        if (position>1) {
            p.setPen(QPen(Qt::green, 3));
            int step = (target.height()*1.0) / position;
            for (int i=1; i<position; i++) {
                p.drawLine (0, step*i, target.width(), step*i);
            }
            p.end ();
        }
        ui->labelMono->setPixmap( px );
    }
}

void setupForm::on_histAreaNoSlider_sliderReleased(){
    histAreaNo = ui->histAreaNoSlider->value();

    graphLock = true;
    ui->regionBox->setEnabled(false);

    ui->regionBox->clear();
    for (int i=0; i<histAreaNo; i++)
        ui->regionBox->addItem(QString::number(i+1));
    ui->regionBox->setCurrentIndex(0);
}

void setupForm::restrictMove_histAreaNoSlider(int position){
    if (position % 2 == 0)
        ui->histAreaNoSlider->setValue(position+1);
}

void setupForm::on_regionBox_currentIndexChanged(int index){

    if (!iproList.isEmpty() && !graphLock) {
        drawEdges = false;
        drawExtremes = true;
        clearGraph(ui->graphicsView2);
        drawGraphHist2(iproList[index], ui->graphicsView2, penRed, iproList[index]->histogramFiltered, iproList[index]->histogramSize, QPoint(-1,-1), true); // recursive MA filter
        drawEdges = true;
        drawExtremes = false;
        clearGraph(ui->graphicsView3);
        drawGraphHist2(iproList[index], ui->graphicsView3, penRed, iproList[index]->histogramFiltered, iproList[index]->histogramSize, QPoint(-1,-1), true); // recursive MA filter
    }
}

void setupForm::on_histCornerPrecSlider_sliderMoved(int position){
    ui->labelhistCornerPrec->setText(QString::number(position));
}

void setupForm::on_histCornerPrecSlider_sliderReleased(){
    histDDLimit = ui->histCornerPrecSlider->value();
    on_histogramAnalysisButton_clicked();
}


//if ( (w->lastData->image->format() != QImage::Format_Invalid) || imageLoadedFromFile ) {   // if any image is get
    /*
    edge = iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight );   // produce edge image
    edge->save("edge0.jpg");
    cv::Mat image(edge->height(), edge->width(), CV_8UC3, (uchar*)edge->bits(), edge->bytesPerLine());
    imwrite("edge.jpg", image);
    */
    //cv::Mat image(iprocess->edgeHeight, iprocess->edgeWidth, CV_32S, *iprocess->edgeMatrix);
    //std::memcpy(image.data, iprocess->edgeMatrix, iprocess->edgeHeight*iprocess->edgeWidth*sizeof(int));

//cv::Mat image(iprocess->edgeHeight, iprocess->edgeWidth, CV_32S);
//for (int y=0; y<iprocess->edgeHeight; y++)
//for (int x=0; x<iprocess->edgeWidth; x++)
//image.at<int>(y,x) = iprocess->edgeMatrix[y][x];

    //cv::Mat image(w->lastData->image->height(), w->lastData->image->width(), CV_8UC3, (uchar*)w->lastData->image->bits(), w->lastData->image->bytesPerLine());
    /*
    cv::Mat ret;
    cv::Mat thresh;
    cv::Mat imgray;
    //QImage _targetArea = lastData->image->copy( offsetXCam, offsetYCam, frameWidthCam, frameHeightCam );    // take target image

    cvtColor(imgData, imgray, CV_RGB2GRAY);
    cv::threshold(imgray, thresh, 127, 255, 0);
    cv::Mat im2, hierarchy;
    std::vector<std::vector<cv::Point> > contours;
    //cv::findContours(thresh, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(thresh, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    */

    //Prepare the image for findContours
    //cv::cvtColor(image, image, CV_BGR2GRAY);
    //cv::Mat imaged;
//cv::threshold(image, image, 128, 255, CV_THRESH_BINARY);
    /*
    //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
    std::vector<std::vector<cv::Point> > contours;
    cv::Mat contourOutput = image.clone();
    cv::findContours( contourOutput, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );

    //Draw the contours
    cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0,0,0));
    cv::Scalar colors[3];
    colors[0] = cv::Scalar(255, 0, 0);
    colors[1] = cv::Scalar(0, 255, 0);
    colors[2] = cv::Scalar(0, 0, 255);
    for (size_t idx = 0; idx < contours.size(); idx++) {
        cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
    }

    QImage img = QImage( (const uchar*) contourImage.data, contourImage.cols, contourImage.rows, contourImage.step, QImage::Format_RGB888 );
    ui->labelMono->setPixmap( QPixmap::fromImage( img ) );
*/
//        cv::imshow("Input Image", image);
//        cvMoveWindow("Input Image", 0, 0);
//        cv::imshow("Contours", contourImage);
//        cv::waitKey(0);
//}

void setupForm::on_twoPassTimeControlBox_stateChanged(int arg1){ }


#include "imgprocessthread.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;

imgProcessThread::imgProcessThread(){

}

imgProcessThread::~imgProcessThread(){

}

void imgProcessThread::run(){

    iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
    iprocessInitSwitch = true;

    ready = false;

    iprocess->thetaMin = w->thetaMin;
    iprocess->thetaMax = w->thetaMax;
    iprocess->thetaStep = w->thetaStep;

    if (w->thinJointAlgoActive){    // without laser
        iprocess->centerX = 0;
        iprocess->centerY = 0;
    } else {                     // with laser
        iprocess->centerX = iprocess->edgeWidth / 2;
        iprocess->centerY = 0;
    }

    iprocess->toMono();
    edgeDetection();

    if (w->thinJointAlgoActive) {  // without laser - VERTICAL SEARCH

        switch ( w->algorithmType ) {
            case 0: // NONE
                break;
            case 1: // MAIN EDGES
                Algo3();
                break;
            case 2: // THIN JOINT - DARK AREA
                Algo4();
                break;
            case 3: // CONTRAST
                Algo5();
                break;
            case 4: // LINE DETECTION WITH MAIN EDGES
                Algo6();
                break;
            case 5: // EXPERIMENTAL
                break;
        }
    } else {    // with laser - HORIZONTAL SEARCH
        switch ( w->algorithmType ) {
            case 0: // NONE
                break;
            case 1: // LONGEST SOLID LINES
                Algo1();
                break;
            case 2: // PRIMARY VOID
                Algo2();
                break;
            case 3: // EXPERIMENTAL
                break;
        }
    }

    emit imageProcessingCompleted();
}

void imgProcessThread::edgeDetection(){
    //qDebug()<<Q_FUNC_INFO;

    switch ( w->edgeDetectionState ) {
        case 0: // NONE
            w->averaging = true;
            w->matrixFlag = true;
            break;
        case 1: // SOBEL
            iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
            iprocess->detectEdgeSobel(); // detect edges of the mono image
            iprocess->houghTransformFn(iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight);
            w->averaging = false;
            w->matrixFlag = false;
            break;
        case 2: // CANNY 4
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
            iprocess->houghTransformEdgeMap();
            w->averaging = false;
            w->matrixFlag = false;
            break;
        case 3: // CANNY 1
            iprocess->prepareCannyArrays();
            iprocess->constructGaussianMatrix(w->gaussianSize, w->stdDev);
            iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );
            iprocess->gaussianBlur();
            iprocess->detectEdgeSobelwDirections();
            iprocess->nonMaximumSuppression(w->cannyThinning);
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
            w->matrixFlag = false;
            break;
    }

}

void imgProcessThread::Algo1(){
// LASER: mono/edge(matrixFlag) > houghTr > detectLongestSolidLines
    //qDebug()<<Q_FUNC_INFO;

    if (w->edgeDetectionState == 0){
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
    }

    iprocess->calculateHoughMaxs( w->houghLineNo );            // get max voted line(s)

    iprocess->detectLongestSolidLines(w->averaging, w->matrixFlag);
}

void imgProcessThread::Algo2(){
// LASER: mono/edge(matrixFlag) > houghTr > detectPrimaryVoid
    //qDebug()<<Q_FUNC_INFO;

    if (w->edgeDetectionState == 0){
        iprocess->constructValueMatrix( iprocess->imgMono );    // construct mono matrix
        iprocess->houghTransformFn(iprocess->valueMatrix, iprocess->imageWidth, iprocess->imageHeight);
    }

    iprocess->calculateHoughMaxs( w->houghLineNo );            // get max voted line(s)

    iprocess->calcAvgDistAndAngle( w->houghLineNo );           // calc. avg. distance and theta
    iprocess->calcVoteAvg();                                // avg. value of max voted line(s)

    iprocess->voteThreshold = w->voteThreshold;                // acceptable vote value low-limit
    iprocess->checkPrimaryLine();                           // is max voted line  above the low-limit?
    iprocess->detectVoidLines();                            // detect void lines on hough lines in MONO image

    iprocess->voidThreshold = w->voidThreshold;                // void threshold to decide max void as primary
    iprocess->detectPrimaryVoid();                          // decide primary void line & corners/center
}

void imgProcessThread::Algo3(){
// woLASER: edge > houghTr > detectMainEdges
    //qDebug()<<Q_FUNC_INFO;

    if (w->edgeDetectionState != 0) {
        iprocess->calculateHoughMaxs( w->houghLineNo );            // get max voted line(s)
        iprocess->thinCornerNum = w->mainEdgesNumber;
        iprocess->detectMainEdges(w->thinJointAlgoActive, false);
        /**/iprocess->angleAvg = iprocess->centerLine.angle;
    } else {
        //ui->plainTextEdit->appendPlainText("Bir kenar tespiti algoritmasý seçilmelidir");
    }
}

void imgProcessThread::Algo4(){
// woLASER: value > detectThinJointCenter
    //qDebug()<<Q_FUNC_INFO;

    iprocess->constructValueMatrix( iprocess->imgOrginal );
    iprocess->detectThinJointCenter(3, 31);
}

void imgProcessThread::Algo5(){
// woLASER: value > contrast matrix > houghTr > calcAvgDistAndAngleOfMajors
    //qDebug()<<Q_FUNC_INFO;

    iprocess->constructValueMatrix( iprocess->imgOrginal );
    iprocess->constructContrastMatix(3);
    iprocess->houghTransformContrast();;
    iprocess->calculateHoughMaxs( w->houghLineNo );            // get max voted line(s)
    iprocess->calcAvgDistAndAngleOfMajors(0.30);    // calc. avg. distance and theta
    iprocess->detectContrastCenter();
}

void imgProcessThread::Algo6(){
// woLASER: canny1 > houghTr > detectMainEdges > thickenEdgeMap > scoreLineCrossing
    //qDebug()<<Q_FUNC_INFO;

    if (w->edgeDetectionState == 3) {
        iprocess->calculateHoughMaxs( w->houghLineNo );            // get max voted line(s)
        iprocess->thinCornerNum = 1;//mainEdgesNumber;
        iprocess->detectMainEdges(w->thinJointAlgoActive, false);
        iprocess->thickenEdgeMap(3);
        iprocess->scoreLineCrossing(true);

    } else {
        //ui->plainTextEdit->appendPlainText("Canny1 kenar tespiti algoritmasý seçilmelidir");
    }
}

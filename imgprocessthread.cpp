#include "imgprocessthread.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;

imgProcessThread::imgProcessThread(){
}

imgProcessThread::~imgProcessThread(){
}

void imgProcessThread::run(){

    ready = false;

    if (!algoSwitch) {

        histAnalysis();

    } else {

        if (!(w->thinJointAlgoActive && w->algorithmType == 6)) {
            iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
            iprocessInitSwitch = true;

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
            edgeDetection(iprocess);
        }

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
                case 5: // SCAN HORIZONTAL
                    Algo7();
                    break;
                case 6: // MAIN EDGES WITH AREAS
                    Algo8();
                    break;
                case 7: // MAIN EDGES WITH DARY TRACKS
                    Algo9();
                    break;
                case 8: // EXPERIMENTAL
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

        emit imageProcessingCompleted(imageCaptureTime);
    }
}

void imgProcessThread::edgeDetection(imgProcess *ipro){
    //qDebug()<<Q_FUNC_INFO;

    switch ( w->edgeDetectionState ) {
        case 0: // NONE
            w->averaging = true;
            w->matrixFlag = true;
            break;
        case 1: // SOBEL
            ipro->constructValueMatrix( ipro->imgMono );    // construct mono matrix
            ipro->detectEdgeSobel(); // detect edges of the mono image
            ipro->houghTransformFn(ipro->edgeMatrix, ipro->edgeWidth, ipro->edgeHeight);
            w->averaging = false;
            w->matrixFlag = false;
            break;
        case 2: // CANNY 4
            ipro->prepareCannyArrays();
            ipro->constructGaussianMatrix(w->gaussianSize, w->stdDev);
            for (int i = 0; i < 4 ; i++){
                ipro->constructValueMatrix( ipro->imgOrginal, i );
                ipro->gaussianBlur();
                ipro->detectEdgeSobelwDirections();
                ipro->nonMaximumSuppression(w->cannyThinning);
                ipro->cannyThresholding(true);
                ipro->edgeTracing();
                ipro->assignEdgeMap();
            }
            ipro->mergeEdgeMaps();

            for (int y = 0; y < ipro->edgeHeight; y++)
                for (int x = 0; x < ipro->edgeWidth; x++){
                    if (ipro->edgeMapMatrix[y][x])
                        ipro->edgeMatrix[y][x]=255;
                    else
                        ipro->edgeMatrix[y][x]=0;
                }
            ipro->houghTransformEdgeMap();
            w->averaging = false;
            w->matrixFlag = false;
            break;
        case 3: // CANNY 1
            ipro->prepareCannyArrays();
            ipro->constructGaussianMatrix(w->gaussianSize, w->stdDev);
            ipro->constructValueMatrix( ipro->imgOrginal, 0 );
            ipro->gaussianBlur();
            ipro->detectEdgeSobelwDirections();
            ipro->nonMaximumSuppression(w->cannyThinning);
            ipro->cannyThresholding(true);
            ipro->edgeTracing();

            for (int y = 0; y < ipro->edgeHeight; y++)  // to display edge image
                for (int x = 0; x < ipro->edgeWidth; x++){
                    if (ipro->edgeMapMatrix[y][x])
                        ipro->edgeMatrix[y][x]=255;
                    else
                        ipro->edgeMatrix[y][x]=0;
                }
            ipro->houghTransformEdgeMap();
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
        iprocess->detectMainEdges(0, false);
        /**/iprocess->angleAvg = iprocess->centerLine.angle;

    } else {
        //ui->plainTextEdit->appendPlainText("Bir kenar tespiti algoritması seçilmelidir");
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
// woLASER: canny1 > houghTr > detectMainEdges > detectMainEdgesSolidLine
    //qDebug()<<Q_FUNC_INFO;

    if (w->edgeDetectionState == 3) {
        iprocess->calculateHoughMaxs( w->houghLineNo );            // get max voted line(s)
        iprocess->thinCornerNum = w->mainEdgesNumber;
        houghData hd = iprocess->detectMainEdgesSolidLine(1, false);
        w->mainLineDistance = hd.distance;
        w->mainLineAngle = hd.angle;
        w->mainLineVoteValue = hd.voteValue;
        w->solidLineLength = iprocess->solidLineLength;
    } else {
        //ui->plainTextEdit->appendPlainText("Canny1 kenar tespiti algoritması seçilmelidir");
    }
}

void imgProcessThread::Algo7(){
// woLASER: edge > houghTr > detectScanHorizontal

    if (w->edgeDetectionState != 0) {
        iprocess->detectScanHorizontal( iprocess->edgeHeight/2 );            // get max voted line(s)
    } else {
        //ui->plainTextEdit->appendPlainText("Bir kenar tespiti algoritması seçilmelidir");
    }
}

void imgProcessThread::Algo8(){
// woLASER: edge > houghTr > detectMainEdges with multiple regions
    //qDebug()<<Q_FUNC_INFO;

    if (w->edgeDetectionState != 0) {

        int step = (targetArea.height()*1.0) / w->areaNumber;
        processElapsed = 0;
        QList<int> xList;

        for (int area=0; area < w->areaNumber; area++) {

            QImage pic = targetArea.copy( 0, step*area, targetArea.width(), step );    // take target image

            startTime = w->timeSystem.getSystemTimeMsec();

            imgProcess *ipro = new imgProcess( pic, pic.width(), pic.height() );   // new imgProcess object

            ipro->thetaMin = w->thetaMin;
            ipro->thetaMax = w->thetaMax;
            ipro->thetaStep = w->thetaStep;
            ipro->maFilterKernelSize = w->maFilterKernelSize;
            ipro->centerX = 0;
            ipro->centerY = 0;

            ipro->toMono();                                     // convert target to mono

            edgeDetection(ipro);

            ipro->calculateHoughMaxs( w->houghLineNo );            // get max voted line(s)
            ipro->thinCornerNum = w->mainEdgesNumber;
            ipro->detectMainEdges(0, false);
            xList.append(ipro->trackCenterX);

            delete ipro;
            endTime = w->timeSystem.getSystemTimeMsec();
            processElapsed += (endTime - startTime);
            // -------END PROCESSING-------
        }


        // -------HISTOGRAM PROCESSING-------
        startTime = w->timeSystem.getSystemTimeMsec();

        iprocess = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
        iprocess->maFilterKernelSize = w->maFilterKernelSize;
        iprocess->histogramAngleThreshold = w->histogramAngleThreshold;
        iprocess->lenRateThr = w->lenRateThr;
        iprocess->bandWidthMin = w->bandWidthMin;
        iprocess->bandCenterMax = w->bandCenterMax;

        iprocess->constructValueMatrix( iprocess->imgOrginal, 0 );

        iprocess->histogramAnalysis(w->colorMatrix);
        int histogramCenterX = (iprocess->natBreaksMax1.x()+iprocess->natBreaksMax2.x())/2;

        endTime = w->timeSystem.getSystemTimeMsec();
        processElapsed += (endTime - startTime);
        // -------END PROCESSING-------

        double variance = targetArea.width() * 0.02;
        /*
        for (int c=0; c < xList.size(); c++)
            variance += pow( histogramCenterX - xList[c], 2 );
        if ( xList.size() != 0 ) variance /= xList.size();
        variance = sqrt(variance);
        */
        int cnt = 0;
        double sum = 0;
        for (int c=0; c<xList.size(); c++)
            if ( abs( histogramCenterX - xList[c] ) < variance ) {
                cnt++;
                sum += xList[c];
            }
        if (cnt != 0)
            iprocess->trackCenterX = sum / cnt;
        else
            iprocess->trackCenterX = histogramCenterX;

    } else {
    }
}

void imgProcessThread::histAnalysis() {
// histogram analysis
    //qDebug()<<Q_FUNC_INFO;

    int step = (targetArea.height()*1.0) / w->histAreaNo;
    int ptime;
    histAreaStatList.clear();

    for (int area = 0; area < w->histAreaNo; area++) {

        QImage pic = targetArea.copy( 0, step*area, targetArea.width(), step );    // take target image

        startTime = w->timeSystem.getSystemTimeMsec();

        imgProcess *ipro = new imgProcess( pic, pic.width(), pic.height() );   // new imgProcess object
        ipro->maFilterKernelSize = w->maFilterKernelSize;
        ipro->histogramAngleThreshold = w->histogramAngleThreshold;
        ipro->lenRateThr = w->lenRateThr;
        ipro->bandWidthMin = w->bandWidthMin;
        ipro->bandCenterMax = w->bandCenterMax;

        ipro->constructValueMatrix( ipro->imgOrginal, 0 );

        ipro->histogramAnalysis(w->colorMatrix);

        endTime = w->timeSystem.getSystemTimeMsec();
        ptime = endTime - startTime;
        processElapsed += ptime;
        // -------END PROCESSING-------

        if ( ipro->bandCheck_errorState == 0 )
            histAreaStatList.append(1);
        else
            histAreaStatList.append(0);

        delete ipro;
    }

    //QString stat = "";
    histAreaStat = 0;

    for (int i=0; i<histAreaStatList.size(); i++)
        histAreaStat += histAreaStatList[i];

    emit histAnalysisCompleted();

    //qDebug() << stat;

}

void imgProcessThread::Algo9() {
// woLASER: edge > houghTr > detectMainEdges > histAnalysis for dark tracks
    //qDebug()<<Q_FUNC_INFO;

        if (w->edgeDetectionState != 0) {
            iprocess->calculateHoughMaxs( w->houghLineNo );            // get max voted line(s)
            iprocess->thinCornerNum = w->mainEdgesNumber;
            iprocess->detectMainEdges(0, false);

            if (iprocess->detected) {
                int mainEdgeMean = iprocess->trackCenterX;
                int *edgeHist = new int[iprocess->edgeWidth];
                double totalSum = 0;
                for (int x=0; x<iprocess->edgeWidth; x++){
                    int sum = 0;
                    for (int y=0; y<iprocess->edgeHeight; y++)
                        if( iprocess->edgeMapMatrix[y][x] ) sum++;
                    edgeHist[x] = sum;
                    totalSum += sum;
                }
                int edgeHistAvg = totalSum / iprocess->edgeWidth;

                totalSum = 0;
                double moments = 0;
                for (int x=0; x<iprocess->edgeWidth; x++){
                    if (edgeHist[x] > edgeHistAvg) {
                        moments += x*edgeHist[x];
                        totalSum += edgeHist[x];
                    }
                }
                int edgeHistMean = moments/totalSum + 1;

                imgProcess *iprocessHist = new imgProcess( targetArea, targetArea.width(), targetArea.height() );   // new imgProcess object
                iprocessHist->maFilterKernelSize = w->maFilterKernelSize;
                iprocessHist->bandWidthMin = w->bandWidthMin;
                iprocessHist->bandCenterMax = w->bandCenterMax;

                iprocessHist->constructValueMatrix( iprocessHist->imgOrginal, 0 );

                iprocessHist->histogramAnalysisDarkTracks(w->colorMatrix, false);

                iprocess->trackCenterX = edgeHistMean;
                int _leftLimit = iprocess->trackCenterX / 2.0;
                int _rightLimit = (iprocess->imageWidth - iprocess->trackCenterX) / 2.0  + iprocess->trackCenterX;
                int _leftMostLimit = iprocess->imageWidth * 0.2;
                int _rightMostLimit = iprocess->imageWidth * 0.8;
                int _minLeftX = iprocess->leftCornerX;
                int _minRightX = iprocess->rightCornerX;
                int val;
                int _minLeft = 2000;
                int _minRight = 2000;

                QList<QPoint> leftList, rightList;
                for (int i=0; i<iprocessHist->histogramMins.size(); i++){

                    val = iprocessHist->histogramFiltered[ iprocessHist->histogramMins[i].start ];

                    if (iprocessHist->histogramMins[i].start > iprocess->trackCenterX && iprocessHist->histogramMins[i].start < _rightLimit ){
                        if (val <= _minRight) {
                            _minRight = val;
                            _minRightX = iprocessHist->histogramMins[i].start;
                            rightList.append(QPoint(_minRight,_minRightX));
                        }
                    }

                    if (iprocessHist->histogramMins[i].start >= _rightLimit && iprocessHist->histogramMins[i].start < _rightMostLimit && _minRight > iprocessHist->histogramAvg){
                        if (val <= _minRight) {
                            _minRight = val;
                            _minRightX = iprocessHist->histogramMins[i].start;
                            rightList.append(QPoint(_minRight,_minRightX));
                        }
                    }
                }

                for (int i=iprocessHist->histogramMins.size()-1; 0<=i; i--){

                    val = iprocessHist->histogramFiltered[ iprocessHist->histogramMins[i].end ];

                    if (iprocessHist->histogramMins[i].end < iprocess->trackCenterX && iprocessHist->histogramMins[i].end > _leftLimit ){
                        if (val <= _minLeft) {
                            _minLeft = val;
                            _minLeftX = iprocessHist->histogramMins[i].end;
                            leftList.append(QPoint(_minLeft,_minLeftX));
                        }
                    }

                    if (iprocessHist->histogramMins[i].end <= _leftLimit && iprocessHist->histogramMins[i].end > _leftMostLimit && _minLeft > iprocessHist->histogramAvg){
                        if (val <= _minLeft) {
                            _minLeft = val;
                            _minLeftX = iprocessHist->histogramMins[i].end;
                            leftList.append(QPoint(_minLeft,_minLeftX));
                        }
                    }
                }

                if (!leftList.isEmpty() && !rightList.isEmpty()) {
                    _minLeftX = leftList.last().y();
                    _minRightX = rightList.last().y();

                    iprocess->leftCornerX = _minLeftX;
                    iprocess->rightCornerX = _minRightX;
                    iprocess->trackCenterX = (_minLeftX+_minRightX) / 2.0;
                    //qDebug() << iprocess->leftCornerX << " " << iprocess->trackCenterX << " " << iprocess->rightCornerX;
                } else {
                    iprocess->detected = false;
                }
                delete iprocessHist;
            }

        } else {
            //ui->plainTextEdit->appendPlainText("Bir kenar tespiti algoritması seçilmelidir");
        }

}

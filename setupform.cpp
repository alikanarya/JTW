#include "setupform.h"
#include "ui_setupform.h"

#include "mainwindow.h"
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
}


void setupForm::processStandardHT(){

    target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth,w->frameHeight );  // take target image

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
    iprocess->houghTransform();                             // detect lines in edge image

    iprocess->calculateHoughMaxs( houghLineNo );            // get max voted line(s)
    iprocess->calcAvgDistAndAngle( houghLineNo );           // calc. avg. distance and theta
    voteAvg = iprocess->calcVoteAvg();                      // avg. value of max voted line(s)

    iprocess->voteThreshold = voteThreshold;                // acceptable vote value low-limit
    if ( !iprocess->checkPrimaryLine() )                    // is max voted line  above the low-limit?
        ui->labelPrimaryLine->show();
    iprocess->detectVoidLines();                            // detect void lines on hough lines in MONO image

    iprocess->voidThreshold = voidThreshold;                // void threshold to decide max void as primary
    iprocess->detectPrimaryVoid();                          // decide primary void line & corners/center
}


void setupForm::processSubImageVoidness(){

    target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight);  // take target image

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

        iprocessLeft->thetaMin = thetaMinSub;
        iprocessLeft->thetaMax = thetaMaxSub;
        iprocessLeft->thetaStep = thetaStepSub;
        iprocessLeft->houghTransform();                 // detect lines in edge image

        iprocessLeft->calculateHoughMaxs( 50 );         // get max voted line(s)
        iprocessLeft->calcAvgDistAndAngleOfMajors();    // calc. avg. distance and theta

        iprocessLeft->primaryLineDetected = true;       // bypass line vote check
        iprocessLeft->detectVoidLinesEdge();            // detect void lines on hough lines in EDGE image

        iprocessLeft->voidThreshold = 0;                // bypass void length check
        iprocessLeft->errorEdgeLimit = 0;               // bypass corner void check
        iprocessLeft->angleAvg = 0;                     // bypass angle value check
        iprocessLeft->detectPrimaryVoid();              // decide primary void line & corners/center

        // right image process
        iprocessRight = new imgProcess( targetRight, targetRight.width(), targetRight.height() );   // new imgProcess object
        iprocessRightInitSwitch = true;
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
        iprocessRight->detectVoidLinesEdge();           // detect void lines on hough lines in EDGE image

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

        /*
        // ------ LEFT AND RIGHT IMAGES SAVE
        fileName = savePath + "Left_org" + fileExt;
        iprocessLeft->imgOrginal.save(fileName);

        iprocessLeft->constructHoughMatrix();
        QImage *houghLeft = iprocessLeft->getImage(iprocessLeft->houghMatrix,iprocessLeft->edgeWidth,iprocessLeft->edgeHeight); // produce hough image
        fileName = savePath + "Left_hough" + fileExt;
        houghLeft->save(fileName);
        delete houghLeft;

        fileName = savePath + "Right_org" + fileExt;
        iprocessRight->imgOrginal.save(fileName);

        iprocessRight->constructHoughMatrix();
        QImage *houghRight = iprocessLeft->getImage(iprocessRight->houghMatrix,iprocessRight->edgeWidth,iprocessRight->edgeHeight); // produce hough image
        fileName = savePath + "Right_hough" + fileExt;
        houghRight->save(fileName);
        delete houghRight;
        // ------ LEFT AND RIGHT IMAGES SAVE
        */
    }
}


void setupForm::processSubImageSolidness(){

    target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight ); // take target image

    if (iprocessInitSwitch) {
        delete iprocess;
        iprocessInitSwitch = false;
    }
    iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
    iprocessInitSwitch = true;
    iprocess->toMono();                                 // convert target to mono
    iprocess->constructValueMatrix(iprocess->imgMono);  // construct mono matrix
    iprocess->detectEdgeSobel();                        // detect edges of the mono image
    iprocess->thickenEdges();

    iprocess->thetaMin = thetaMinSub;
    iprocess->thetaMax = thetaMaxSub;
    iprocess->thetaStep = thetaStepSub;
    iprocess->detectLongestSolidLines();

    iprocess->imgMono.save("data/mono.jpg");
    //iprocess->saveList(iprocess->iprocess, "data/solidSpaceMain.csv");
    iprocess->saveList(iprocess->solidSpaceMainTrimmed, "data/solidSpaceMainTrimmed.csv");
    //iprocess->saveList(iprocess->solidSpaceMainMaximums, "data/solidSpaceMainMaximums.csv");
    iprocess->saveList(iprocess->primaryGroup, "data/primaryGroup.csv");
    iprocess->saveList(iprocess->secondaryGroup, "data/secondaryGroup.csv");

    //iprocess->constructHoughExtendedMatrixMajor2Lines();
    //iprocess->getImage(iprocess->houghExtendedMatrix, iprocess->imageWidth, iprocess->imageHeight)->save("data/major2.jpg");

/*
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
*/
/*
        iprocess->leftCornerX = iprocessLeft->primaryLine.end.x();
        iprocess->leftCornerY = iprocessLeft->primaryLine.end.y();

        iprocess->rightCornerX = tCenterX + iprocessRight->primaryLine.start.x();
        iprocess->rightCornerY = iprocessRight->primaryLine.start.y();

        iprocess->trackCenterX = ( iprocess->leftCornerX + iprocess->rightCornerX ) / 2;
        iprocess->trackCenterY = ( iprocess->leftCornerY + iprocess->rightCornerY ) / 2;
*/

        //iprocessRight->saveList(iprocessRight->major2Lines, savePath + "major2Lines_right.csv");
        //iprocessRight->saveList(iprocessRight->solidSpaceMainMaximums, savePath + "solidSpaceMainMaximums_right.csv");

/*
        // ------ LEFT AND RIGHT IMAGES SAVE
        iprocessLeft->imgMono.save("data/left.jpg");
        iprocessRight->imgMono.save("data/right.jpg");
        iprocessLeft->getImage(iprocessLeft->edgeThickenedMatrix, iprocessLeft->imageWidth, iprocessLeft->imageHeight)->save("data/left_edgethickned.jpg");
        iprocessRight->getImage(iprocessRight->edgeThickenedMatrix, iprocessRight->imageWidth, iprocessRight->imageHeight)->save("data/right_edgethickned.jpg");

        iprocessRight->constructHoughExtendedMatrixMajor2Lines();
        iprocessRight->getImage(iprocessRight->houghExtendedMatrix, iprocessRight->imageWidth, iprocessRight->imageHeight)->save("data/right_major2.jpg");

        // ------ LEFT AND RIGHT IMAGES SAVE
*/
    //}
}


void setupForm::captureButton(){

    if ( !w->imageGetter->imageList.isEmpty() ) {   // if any image is get

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

        if (!w->subImageProcessingSwitch) {

            processStandardHT();

        } else {

            switch ( w->subImageProcessingType ) {
                case 0:
                    processSubImageVoidness();
                    break;
                case 1:
                    processSubImageSolidness();
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


        QString message = "Analiz " + QString::number(processElapsed) + " milisaniye içinde gerçekleþtirildi.";
        ui->plainTextEdit->appendPlainText(message);

        ui->plainTextEdit->appendPlainText(iprocess->statusMessage);    // display message about detection process
        ui->plainTextEdit->appendPlainText("Sol Köþe (x,y) - Merkez (x,y) - Sað Köþe (x,y): ");
        message = "( " + QString::number(iprocess->leftCornerX) + " , " + QString::number(iprocess->leftCornerY) + " )  -  ( " +
                         QString::number(iprocess->trackCenterX) + " , " + QString::number(iprocess->trackCenterY) + " )  -  ( " +
                         QString::number(iprocess->rightCornerX) + " , " + QString::number(iprocess->rightCornerY) + " )";
        ui->plainTextEdit->appendPlainText(message);

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
                    edge = iprocess->getImage( iprocess->edgeThickenedMatrix, iprocess->imageWidth, iprocess->imageHeight );   // produce edge image
                    ui->labelEdge->setPixmap( QPixmap::fromImage( *edge ) );

                    ui->plainTextEdit->appendPlainText(QString::number(iprocess->distanceAvgPrimary, 'f', 1) + " - " + QString::number(iprocess->thetaAvgPrimary, 'f', 1));
                    ui->plainTextEdit->appendPlainText(QString::number(iprocess->distanceAvgSecondary, 'f', 1) + " - " + QString::number(iprocess->thetaAvgSecondary, 'f', 1));

                    iprocess->detected = true;
                    if ( iprocess->primaryLineFound && iprocess->secondaryLineFound )
                        iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[1], 0 );
                    else
                        if ( iprocess->primaryLineFound )
                            iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[0], 0 );
                    //iprocess->cornerAndPrimaryLineImage( iprocess->major2Lines[0], iprocess->major2Lines[0], 0 );

                    //iprocess->cornerAndPrimaryLineImage( iprocessLeft->primaryLine, iprocessRight->primaryLine, tCenterX );
                    ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCornerAndPrimaryLines ) );
/*
                    ui->plainTextEdit->appendPlainText("2.aþama:");
                    ui->plainTextEdit->appendPlainText("Sol Ýmaj {Uzunluk-Mesafe-Açý-Baþlagýç(x,y)-Bitiþ(x,y)}");
                    message = QString::number(iprocessLeft->primaryLine.length) + " - " +
                              QString::number(iprocessLeft->primaryLine.distance) + " - " +
                              QString::number(iprocessLeft->primaryLine.angle) + " -   ( " +
                              QString::number(iprocessLeft->primaryLine.start.x()) + ", " +
                              QString::number(iprocessLeft->primaryLine.start.y()) + " ) - ( " +
                              QString::number(iprocessLeft->primaryLine.end.x()) + " , " +
                              QString::number(iprocessLeft->primaryLine.end.y()) + " )";
                    ui->plainTextEdit->appendPlainText(message);

                    ui->plainTextEdit->appendPlainText("Sað Ýmaj {Uzunluk-Mesafe-Açý-Baþlagýç(x,y)-Bitiþ(x,y)}");
                    message = QString::number(iprocessRight->primaryLine.length) + " - " +
                              QString::number(iprocessRight->primaryLine.distance) + " - " +
                              QString::number(iprocessRight->primaryLine.angle) + " -   ( " +
                              QString::number(iprocessRight->primaryLine.start.x()) + ", " +
                              QString::number(iprocessRight->primaryLine.start.y()) + " ) - ( " +
                              QString::number(iprocessRight->primaryLine.end.x()) + " , " +
                              QString::number(iprocessRight->primaryLine.end.y()) + " )";
                    ui->plainTextEdit->appendPlainText(message);
*/
                    break;
            }

        } else {    // standard HT


            edge = iprocess->getImage( iprocess->edgeMatrix, iprocess->edgeWidth, iprocess->edgeHeight );   // produce edge image
            ui->labelEdge->setPixmap( QPixmap::fromImage( *edge ) );

            iprocess->constructHoughMatrix();   // construct hough matrix = edge matrix + coded #houghLineNo lines
            //iprocess->constructHoughMatrixPrimaryLines(iprocessLeft->primaryLine, iprocessRight->primaryLine, tCenterX);
            hough = iprocess->getImage( iprocess->houghMatrix, iprocess->edgeWidth, iprocess->edgeHeight );     // produce hough image
            ui->labelHough->setPixmap( QPixmap::fromImage( *hough ) );

            iprocess->cornerImage();
            ui->labelAnalyze->setPixmap( QPixmap::fromImage( iprocess->imgCorner ) );

            ui->plainTextEdit->appendPlainText("Bulunan tüm boþluk parametreleri:");
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

       }

        ui->plainTextEdit->appendPlainText("------------------------------------------------------------");

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
    ui->editHoughThetaStep->setText(QString::number(thetaStep));

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

    target = w->lastData->image->copy( w->offsetX, w->offsetY, w->frameWidth, w->frameHeight ); // take target image

    if (iprocessInitSwitch) {
        delete iprocess;
        iprocessInitSwitch = false;
    }
    iprocess = new imgProcess( target, target.width(), target.height() );   // new imgProcess object
    iprocessInitSwitch = true;
    iprocess->toMono();                                 // convert target to mono
    iprocess->constructValueMatrix(iprocess->imgMono);  // construct mono matrix
    iprocess->detectEdgeSobel();                        // detect edges of the mono image

    iprocess->thetaMin = thetaMin;
    iprocess->thetaMax = thetaMax;
    iprocess->thetaStep = thetaStep;
    iprocess->houghTransform();                         // detect lines in edge image

    iprocess->calculateHoughMaxs( houghLineNo );        // get max voted line(s)
    iprocess->calcAvgDistAndAngle( houghLineNo );       // calc. avg. distance and theta
    voteAvg = iprocess->calcVoteAvg();                  // ave value of max voted line(s)

    iprocess->voteThreshold = voteThreshold;            // acceptable vote value low-limit
    if ( !iprocess->checkPrimaryLine() )                // is max voted line  above the low-limit?
        ui->labelPrimaryLine->show();
    iprocess->detectVoidLines();                        // detect void lines on hough lines in MONO image

    iprocess->voidThreshold = voidThreshold;            // void threshold to decide max void as primary
    iprocess->detectPrimaryVoid();                      // decide primary void line & corners/center

    iprocess->imgMono.save("data/0.jpg");

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
        int tWidth = w->frameWidth;

        QImage targetLeft = target.copy( 0, 0, tCenterX, w->frameHeight );
        QImage targetRight = target.copy( tCenterX, 0, tWidth - tCenterX, w->frameHeight );

        // left image process
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

            //iprocessLeft->saveList(iprocessLeft->solidSpaceMain, "data/left1_solidSpaceMain.csv");
            iprocessLeft->saveList(iprocessLeft->solidSpaceMainTrimmed, "data/left1_solidSpaceMainTrimmed.csv");
            iprocessLeft->saveList(iprocessLeft->solidSpaceMainMaximums, "data/left1_solidSpaceMainMaximums.csv");
            iprocessLeft->imgMono.save("data/left1.jpg");
            iprocessLeft->constructHoughExtendedMatrixMajor2Lines();
            iprocessLeft->getImage(iprocessLeft->houghExtendedMatrix, iprocessLeft->imageWidth, iprocessLeft->imageHeight)->save("data/left1_major2.jpg");

        // right image process
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

            //iprocessRight->saveList(iprocessRight->solidSpaceMain, "data/right1_solidSpaceMain.csv");
            iprocessRight->saveList(iprocessRight->solidSpaceMainTrimmed, "data/right1_solidSpaceMainTrimmed.csv");
            iprocessRight->saveList(iprocessRight->solidSpaceMainMaximums, "data/right1_solidSpaceMainMaximums.csv");
            iprocessRight->imgMono.save("data/right1.jpg");
            iprocessRight->constructHoughExtendedMatrixMajor2Lines();
            iprocessRight->getImage(iprocessRight->houghExtendedMatrix, iprocessRight->imageWidth, iprocessRight->imageHeight)->save("data/right1_major2.jpg");

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

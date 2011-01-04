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
}

void setupForm::captureButton(){
    if (!w->imageGetter->imageList.isEmpty()){  // if any image is get
        ui->labelPrimaryLine->hide();   // hide PRIMARY LINE NOT DETECTED message

        // get & check hough parameters from ui
        thetaMin = ui->editHoughThetaMin->text().toInt();
        thetaMax = ui->editHoughThetaMax->text().toInt();
        thetaStep = ui->editHoughThetaStep->text().toFloat();
        if (thetaStep < 0.1){
            thetaStep = 0.1;
            ui->editHoughThetaStep->setText("0.1");
        }
        houghLineNo = ui->editHoughLineNo->text().toInt();
        if (houghLineNo == 0){
            houghLineNo = 2;
            ui->editHoughLineNo->setText("2");
        }
        voteThreshold = ui->editHoughThreshold->text().toInt();
        voidThreshold = ui->editVoidThreshold->text().toInt();

        captureTimeStr = QTime::currentTime().toString("hhmmss_");                                  // for filename

        // START IMAGE PROCESSING
        startTime = w->timeSystem.getSystemTimeMsec();

        target = w->lastData->image->copy(w->offsetX,w->offsetY,w->frameWidth,w->frameHeight);      // take target image

        iprocess = new imgProcess(target, target.width(),target.height());                          // new imgProcess object
        iprocess->toMono();                                                                         // convert target to mono
        iprocess->constructValueMatrix(iprocess->imgMono);                                          // construct mono matrix
        iprocess->detectEdgeSobel();                                                                // detect edges of the mono image

        iprocess->thetaMin = thetaMin;
        iprocess->thetaMax = thetaMax;
        iprocess->thetaStep = thetaStep;
        iprocess->houghTransform();                                                                 // detect lines in edge image

        iprocess->calculateHoughMaxs(houghLineNo);                                                  // get max voted line(s)
        voteAvg = iprocess->calcVoteAvg();                                                          // ave value of max voted line(s)

        iprocess->voteThreshold = voteThreshold;                                                    // acceptable vote value low-limit
        if (!iprocess->checkPrimaryLine())                                                          // is max voted line  above the low-limit?
            ui->labelPrimaryLine->show();

        iprocess->detectVoidLines();                                                                // detect void lines on hough lines in mono image
        iprocess->voidThreshold = voidThreshold;                                                    // void threshold to decide max void as primary
        iprocess->detectPrimaryVoid();                                                              // decide primary void line & corners/center

        endTime = w->timeSystem.getSystemTimeMsec();
        // END IMAGE PROCESSING

        processElapsed = endTime - startTime;

        // produce images
        edge = iprocess->getImage(iprocess->edgeMatrix,iprocess->edgeWidth,iprocess->edgeHeight);   // produce edge image
        iprocess->constructHoughMatrix();                                                           // construct hough matrix = edge matrix + coded lines
        hough = iprocess->getImage(iprocess->houghMatrix,iprocess->edgeWidth,iprocess->edgeHeight); // produce hough image
        iprocess->cornerImage();                                                                    // produce corner image

        // update GUI
        ui->labelTarget->setPixmap(QPixmap::fromImage(iprocess->imgOrginal));
        ui->labelMono->setPixmap(QPixmap::fromImage(iprocess->imgMono));
        ui->labelEdge->setPixmap(QPixmap::fromImage(*edge));
        ui->labelHough->setPixmap(QPixmap::fromImage(*hough));
        ui->labelAnalyze->setPixmap(QPixmap::fromImage(iprocess->imgCorner));
        ui->labelVoteAvg->setText(QString::number(voteAvg));


        QString message = "Analiz " + QString::number(processElapsed) + " milisaniye içinde gerçekleþtirildi.";
        ui->plainTextEdit->appendPlainText(message);
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
        ui->plainTextEdit->appendPlainText(iprocess->statusMessage);    // display message about detection process
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
    houghLineNo = w->houghLineNo;
    voteThreshold = w->voteThreshold;
    voidThreshold = w->voidThreshold;

    ui->editHoughThetaMin->setText(QString::number(thetaMin));
    ui->editHoughThetaMax->setText(QString::number(thetaMax));
    ui->editHoughThetaStep->setText(QString::number(thetaStep));
    ui->editHoughLineNo->setText(QString::number(houghLineNo));
    ui->editHoughThreshold->setText(QString::number(voteThreshold));
    ui->editVoidThreshold->setText(QString::number(voidThreshold));
}

void setupForm::clearButton(){
    ui->plainTextEdit->clear();
}

void setupForm::saveExitButton(){
    w->thetaMin = thetaMin = ui->editHoughThetaMin->text().toInt();;
    w->thetaMax = thetaMax = ui->editHoughThetaMax->text().toInt();
    w->thetaStep = thetaStep = ui->editHoughThetaStep->text().toFloat();
    w->houghLineNo = houghLineNo = ui->editHoughLineNo->text().toInt();
    w->voteThreshold = voteThreshold = ui->editHoughThreshold->text().toInt();
    w->voidThreshold = voidThreshold = ui->editVoidThreshold->text().toInt();

    this->close();
}

void setupForm::exitButton(){
    this->close();
}

setupForm::~setupForm(){
    delete ui;
}

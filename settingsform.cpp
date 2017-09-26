#include "settingsform.h"
#include "ui_settingsform.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;

settingsForm::settingsForm(QWidget *parent) : QDialog(parent), ui(new Ui::settingsForm){

    // only title & close button on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::WindowSystemMenuHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);

    itemR0C0 = new QTableWidgetItem("camera jpeg");
    itemR1C0 = new QTableWidgetItem("camera stream");
    itemR2C0 = new QTableWidgetItem("plc ip");
    itemR3C0 = new QTableWidgetItem("byte");
    /*
    itemR3C0 = new QTableWidgetItem("right bit");
    itemR4C0 = new QTableWidgetItem("left byte");
    itemR5C0 = new QTableWidgetItem("left bit");
    itemR6C0 = new QTableWidgetItem("stop byte");
    itemR7C0 = new QTableWidgetItem("stop bit");
    itemR8C0 = new QTableWidgetItem("emer byte");
    itemR9C0 = new QTableWidgetItem("emer bit");
    */
    ui->table->setItem(0,0,itemR0C0);
    ui->table->setItem(1,0,itemR1C0);
    ui->table->setItem(2,0,itemR2C0);
    ui->table->setItem(3,0,itemR3C0);
    /*
    ui->table->setItem(4,0,itemR4C0);
    ui->table->setItem(5,0,itemR5C0);
    ui->table->setItem(6,0,itemR6C0);
    ui->table->setItem(7,0,itemR7C0);
    ui->table->setItem(8,0,itemR8C0);
    ui->table->setItem(9,0,itemR9C0);
    */

    ui->table->verticalHeader()->setVisible(true);

    ui->label_4->hide();
    ui->editControlDelay->hide();
    ui->checkPLCSIM->setChecked(w->PLCSIM);

    camApi = new getImage(w->urlCamStream.host(), false);
    connect(camApi, SIGNAL(focusState(bool)), this, SLOT(focusState(bool)));
    connect(camApi, SIGNAL(focusingActionState(bool)), this, SLOT(focusingActionState(bool)));
    ui->manFocusSlider->setEnabled(false);

  /*
    QIntValidator *validEditHoughLineNo = new QIntValidator(01, 10, this);
    validEditHoughLineNo->setBottom(01);
    validEditHoughLineNo->setTop(10);
    ui->editHoughLineNo->setValidator(validEditHoughLineNo);
*/
}

void settingsForm::showSetupForm(){
    w->errorStopLimitLineVisible = false;
    w->repaintDevTrend();

    w->showSetupForm();
    //this->close();
}

void settingsForm::errorLimitSlider(){
    w->errorLimit = ui->errorLimitSlider->sliderPosition();
    w->errorLimitNeg = -1 * w->errorLimit;
    ui->labelErrorLimit->setText(QString::number(w->errorLimit));

    w->errorStopLimit = w->errorLimit * w->errorStopScale;
    w->errorStopLimitNeg = -1 * w->errorStopLimit;

    if (w->mapFactorX != 0) {
        w->errorLimitCam = w->errorLimit / w->mapFactorX;
        w->errorLimitNegCam = w->errorLimitNeg / w->mapFactorX;
        w->errorStopLimitCam = w->errorStopLimit / w->mapFactorX;
        w->errorStopLimitNegCam = w->errorStopLimitNeg / w->mapFactorX;
    }

    w->repaintDevTrend();
}

void settingsForm::errorScaleSlider(){
    w->errorStopScale = ui->errorScaleSlider->sliderPosition() / 100.0;
    w->errorStopLimit = w->errorLimit * w->errorStopScale;
    w->errorStopLimitNeg = -1 * w->errorStopLimit;
    ui->labelErrorScale->setText(QString::number(w->errorStopScale * 100));

    if (w->mapFactorX != 0) {
        w->errorStopLimitCam = w->errorStopLimit / w->mapFactorX;
        w->errorStopLimitNegCam = w->errorStopLimitNeg / w->mapFactorX;
    }

    w->repaintDevTrend();
}

void settingsForm::targetSlider(){
    int pos = ui->targetSlider->sliderPosition();

    if (pos % 2 == 0){
        if (pos <= w->frameWidthMax)
            w->frameWidth = pos;
    }

    ui->labelTarget->setText(QString::number(w->frameWidth));

    if (w->play && !w->pause)
        w->calcImageParametes(*w->lastData->image, true);
    w->repaintGuide();
}

void settingsForm::targetVertSlider(){
    int pos = ui->targetVertSlider->sliderPosition();

    if (pos % 2 == 0){
       // if (pos <= w->frameHeightMax)
            w->frameHeight = pos;
    }
    ui->labelTargetVert->setText(QString::number(w->frameHeight));

    //ui->labelTargetVert->setText(QString::number(pos));
    if (w->play && !w->pause)
        w->calcImageParametes(*w->lastData->image, true);
    w->repaintGuide();
}

void settingsForm::getParameters(){

    ui->labelErrorLimit->setText(QString::number(w->errorLimit));
    ui->errorLimitSlider->setSliderPosition(w->errorLimit);

    ui->errorScaleSlider->setSliderPosition(w->errorStopScale * 100);
    ui->labelErrorScale->setText(QString::number(w->errorStopScale * 100));

    ui->targetSlider->setMaximum(w->frameWidthMax);
    ui->labelTarget->setText(QString::number(w->frameWidth));
    ui->targetSlider->setSliderPosition(w->frameWidth);

    ui->targetVertSlider->setMaximum(w->frameHeightMax);
    ui->labelTargetVert->setText(QString::number(w->frameHeight));
    ui->targetVertSlider->setSliderPosition(w->frameHeight);

    //ui->labelTargetVert->setText(QString::number(w->frameHeightRatio*100));
    //ui->targetVertSlider->setSliderPosition(w->frameHeightRatio*100);

    ui->editTitle->setText(w->title);

    ui->labelyRes->setText(QString::number(w->yRes));
    ui->yResIndexSlider->setSliderPosition(w->yResIndex);

    ui->editControlDelay->setText(QString::number(w->controlDelay));
    ui->editControlDelay->setEnabled(!w->controlOn);

    ui->checkCamonBoot->setChecked(w->playCamonBoot);

    ui->checkPLConBoot->setChecked(w->connectRequestedonBoot);

    ui->checkErrorStopVisible->setChecked(w->errorStopLimitLineVisible);

    switch (w->plcType){
        case 0:
            ui->radio200->setChecked(true);
            ui->radio300->setChecked(false);
            ui->editDBNo->setText("1");
            ui->editDBNo->setEnabled(false);
            break;
        case 1:
            ui->radio200->setChecked(false);
            ui->radio300->setChecked(true);
            ui->editDBNo->setText(QString::number(w->DB_NO));
            ui->editDBNo->setEnabled(true);
            break;
    }

    switch (w->camStreamType){
        case 0:
            ui->radioCamStream->setChecked(true);
            ui->radioCamJpeg->setChecked(false);
            break;
        case 1:
            ui->radioCamStream->setChecked(false);
            ui->radioCamJpeg->setChecked(true);
            break;
    }

    itemR0C0->setText(w->urlCam.toString());
    itemR1C0->setText(w->urlCamStream.toString());
    itemR2C0->setText(w->urlPLC.toString());
    itemR3C0->setText(QString::number(w->BYTE_NO));
    /*
    itemR3C0->setText(QString::number(w->right_BITofBYTE));
    itemR4C0->setText(QString::number(w->left_VMEM_BYTE));
    itemR5C0->setText(QString::number(w->left_BITofBYTE));
    itemR6C0->setText(QString::number(w->stop_VMEM_BYTE));
    itemR7C0->setText(QString::number(w->stop_BITofBYTE));
    itemR8C0->setText(QString::number(w->emergency_VMEM_BYTE));
    itemR9C0->setText(QString::number(w->emergency_BITofBYTE));
    */
}

void settingsForm::commitChanges(){

    w->urlCam.setUrl(itemR0C0->text());
    w->urlCamStream.setUrl(itemR1C0->text());
    w->urlPLC.setUrl(itemR2C0->text());

    switch (ui->radioGroup->checkedId()){
        case -2:    // S7-200
            w->plcType = 0;
            w->DB_NO = 1;
            break;
        case -3:    // S7-300
            w->plcType = 1;
            w->DB_NO = ui->editDBNo->text().toInt();
            break;
    }

    w->BYTE_NO = itemR3C0->text().toInt();
    /*
    w->right_BITofBYTE = itemR3C0->text().toInt();
    w->left_VMEM_BYTE = itemR4C0->text().toInt();
    w->left_BITofBYTE = itemR5C0->text().toInt();
    w->stop_VMEM_BYTE = itemR6C0->text().toInt();
    w->stop_BITofBYTE = itemR7C0->text().toInt();
    w->emergency_VMEM_BYTE = itemR8C0->text().toInt();
    w->emergency_BITofBYTE = itemR9C0->text().toInt();
    */

    switch (ui->camStreamGroup->checkedId()){
        case -2:    // JPEG
            w->camStreamType = 1;
            break;
        case -3:    // STREAM
            w->camStreamType = 0;
            break;
    }

    w->yResIndex = ui->yResIndexSlider->sliderPosition();
    w->yRes = yResArray[w->yResIndex];

}

void settingsForm::saveExitButton(){
    restartDialog *dialog = new restartDialog(this);
    dialog->show();
    connect( dialog, SIGNAL(acceptRestart()), this, SLOT(saveExit()) );
}

void settingsForm::exitButton(){
    this->close();
    w->writeSettings();
}

void settingsForm::saveExit(){
    commitChanges();
    w->writeSettings();

    qApp->quit();

    WinExec("JTW.exe", SW_SHOW);    // restart application

}

void settingsForm::yResIndexSlider(){
    int yResIndex = ui->yResIndexSlider->sliderPosition();
    ui->labelyRes->setText(QString::number(yResArray[yResIndex]));
}

void settingsForm::checkPLConBoot(){
    w->connectRequestedonBoot = ui->checkPLConBoot->isChecked();
}

void settingsForm::checkCamonBoot(){
    w->playCamonBoot = ui->checkCamonBoot->isChecked();
}

void settingsForm::checkErrorStopLimitVisible(){
    w->errorStopLimitLineVisible = ui->checkErrorStopVisible->isChecked();
    w->repaintDevTrend();
}

void settingsForm::changePLCtype(){
    switch (ui->radioGroup->checkedId()){
        case -2:    // S7-200
            ui->editDBNo->setEnabled(false);
            break;
        case -3:    // S7-300
            ui->editDBNo->setEnabled(true);
            break;
    }
}

void settingsForm::changeCamStreamType(){
    switch (ui->camStreamGroup->checkedId()){
        case -2:    // Jpeg
            //qDebug() << "Jpeg";
            break;
        case -3:    // Stream
            //qDebug() << "Stream";
            break;
    }
}

void settingsForm::getControlDelay(){
    w->controlDelay = ui->editControlDelay->text().toInt(&w->controlDelayValid, 10);

    if (!w->controlDelayValid)
        w->ui->plainTextEdit->appendPlainText(alarm8);
    else {
        if (w->controlDelay > 0 && w->controlDelay < 500){
            w->controlDelay = 500;
            ui->editControlDelay->setText("500");
            w->ui->plainTextEdit->appendPlainText(alarm9);
        } else
            w->ui->plainTextEdit->appendPlainText(message4 + QString::number(w->controlDelay) + " mili saniye");
    }
}

void settingsForm::setTitle(){
    w->title = ui->editTitle->text();
}

void settingsForm::on_checkPLCSIM_clicked(){
    if (!ui->checkPLCSIM->isChecked()){
        w->threadPLCControl->plc->plcInteract = false;
        if (w->timerControlEnabled)
            w->timerControl->stop();
    } else {
        w->threadPLCControl->plc->plcInteract = true;

        if (w->connectRequestedonBoot)
            w->timerControl->start();
        else
            w->initPlcTimer();
    }
    w->PLCSIM = ui->checkPLCSIM->isChecked();
}

void settingsForm::on_checkTargetAlignButtons_clicked(){
    w->ui->leftButton->setEnabled( true );
    w->ui->leftButton->show();
    w->ui->rightButton->setEnabled( true );
    w->ui->rightButton->show();
}

settingsForm::~settingsForm(){
    delete ui;
}

void settingsForm::on_apiCheckFocus_clicked(){
    if (!w->cameraDownStatus && !camApi->busy){
        camApi->apiDahuaGetFocusState();
    }
}

void settingsForm::focusState(bool state){
    if (state)
        ui->plainTextEdit->appendPlainText("Odak Dogru");
    else
        ui->plainTextEdit->appendPlainText("Odak Bozuk");
}

void settingsForm::on_apiFocusStatus_clicked(){
    if (!w->cameraDownStatus && !camApi->busy){
        camApi->apiDahuaGetFocusStatus();
    }
}

void settingsForm::focusingActionState(bool state){
    ui->plainTextEdit->appendPlainText("Odak Pozisyonu   : " + QString::number(camApi->focusPos, 'f', 3));
    ui->plainTextEdit->appendPlainText("Odak Motoru Adim : " + camApi->focusMotorSteps);
    ui->plainTextEdit->appendPlainText("Odaklama Durumu  : " + camApi->focusStatus);
    ui->manFocusSlider->setEnabled(true);
    ui->manFocusSlider->setValue(100*camApi->focusPos);
}

void settingsForm::on_apiAutoFocus_clicked(){
    if (!w->cameraDownStatus && !camApi->busy){
        camApi->apiDahuaAutoFocus();
        ui->plainTextEdit->appendPlainText("Otomatik fokus komutu gonderildi");
    }
}

void settingsForm::on_manFocusSlider_sliderReleased(){
    if (!w->cameraDownStatus && !camApi->busy){
        float pos = ui->manFocusSlider->value() / 100.0;
        camApi->apiDahuaSetFocusPos(pos);
        ui->plainTextEdit->appendPlainText("Gonderilen fokus pozisyonu: " + QString::number(pos,'f',3));
    }
}

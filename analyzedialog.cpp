#include "analyzedialog.h"
#include "ui_analyzedialog.h"

#include "sampledialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;

analyzeDialog::analyzeDialog(imgProcess *iprocess, int processElapsed, QWidget *parent) :
    QDialog(parent), ui(new Ui::analyzeDialog){

    // only title on title bar
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);

    ui->labelMono->setPixmap(QPixmap::fromImage(iprocess->imgMono));
    ui->labelAnalyze->setPixmap(QPixmap::fromImage(iprocess->imgCorner));

    ui->plainTextEdit->appendPlainText("ANALÝZ SONUCU");
    ui->plainTextEdit->appendPlainText("--------------------------------------------------");
    ui->plainTextEdit->appendPlainText("Analiz " + QString::number(processElapsed) + " milisaniye içinde gerçekleþtirildi.");
    ui->plainTextEdit->appendPlainText("Analiz Mesajý: " + iprocess->statusMessage);
    ui->plainTextEdit->appendPlainText("Çizgi açýsý: " + QString::number(iprocess->angleAvg) + " Derece");
    ui->plainTextEdit->appendPlainText("--------------------------------------------------");

    ui->plainTextEdit->appendPlainText("BU YAZILIM;");
    ui->plainTextEdit->appendPlainText("YAPTIÐI TESPÝTÝN DOÐRULUÐUNA KENDÝLÝÐÝNDEN KARAR VEREMEZ !");
    ui->plainTextEdit->appendPlainText("BU SEBEPLE YUKARIDAKÝ GÖRÜNTÜLERÝN OPERATÖR TARAFINDAN ONAYLANMASI GEREKMEKTEDÝR.\n");

//    ui->plainTextEdit->appendPlainText("* Yukarýda soldaki resimde, görüntüde PARLAMA olup olmadýðý ve lazer çizgisinin");
//    ui->plainTextEdit->appendPlainText("NET bir þekilde seçilebilir olup olmadýðýna dikkat edilir.(ÝNCE KAYNAK AÐZI UYGULMASI HARÝÇ)\n");

    ui->plainTextEdit->appendPlainText("* Yukarýda saðdaki resimde ise KÖÞElerin ve");
    ui->plainTextEdit->appendPlainText("MERKEZ noktasýnýn doðru tespit edilip edilemediðine dikkat edilir.\n");

    ui->plainTextEdit->appendPlainText("Eðer bu kriter(ler)e göre doðru algýlama yapýlamadýysa, kamera kurulumunda bir hata var demektir.");
    ui->plainTextEdit->appendPlainText("Yazýlýmýn tespit edebildiði hatalar, yukarýdaki analiz mesajýnda belirtilir ve sizin onay vermeniz engellenir.");
    ui->plainTextEdit->appendPlainText("Hata durumunda veya tespit doðruluðundaki anormalliklerde TEKNÝK SERVÝSÝNÝZ ile irtibata geçiniz.");

    if (!iprocess->detected) ui->acceptButton->setEnabled(false);

    if (w->thinJointAlgoActive) ui->cancelButton_2->setEnabled(false);  // disable sample button
}

void analyzeDialog::acceptButton(){
    w->permOperator = true;
    w->ui->analyzeButton->setIcon(w->calculatorOffIcon);
    //w->ui->trackButton->setEnabled(true);

    this->close();
}

void analyzeDialog::cancelButton(){
    w->permOperator = false;
    w->ui->analyzeButton->setIcon(w->calculatorOnIcon);
    //w->ui->trackButton->setEnabled(false);

    this->close();
}

void analyzeDialog::sampleButton(){
    sampleDialog *_sampleDialog = new sampleDialog(this);
    _sampleDialog->show();
}

analyzeDialog::~analyzeDialog(){
    delete ui;
}

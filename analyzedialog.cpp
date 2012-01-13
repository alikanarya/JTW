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

    ui->plainTextEdit->appendPlainText("ANAL�Z SONUCU");
    ui->plainTextEdit->appendPlainText("--------------------------------------------------");
    ui->plainTextEdit->appendPlainText("Analiz " + QString::number(processElapsed) + " milisaniye i�inde ger�ekle�tirildi.");
    ui->plainTextEdit->appendPlainText("Analiz Mesaj�: " + iprocess->statusMessage);
    ui->plainTextEdit->appendPlainText("�izgi a��s�: " + QString::number(iprocess->angleAvg) + " Derece");
    ui->plainTextEdit->appendPlainText("--------------------------------------------------");

    ui->plainTextEdit->appendPlainText("BU YAZILIM;");
    ui->plainTextEdit->appendPlainText("YAPTI�I TESP�T�N DO�RULU�UNA KEND�L���NDEN KARAR VEREMEZ !");
    ui->plainTextEdit->appendPlainText("BU SEBEPLE YUKARIDAK� G�R�NT�LER�N OPERAT�R TARAFINDAN ONAYLANMASI GEREKMEKTED�R.\n");
    ui->plainTextEdit->appendPlainText("1. Yukar�da soldaki resimde, g�r�nt�de PARLAMA olup olmad��� ve");
    ui->plainTextEdit->appendPlainText("lazer �izgisinin NET bir �ekilde se�ilebilir olup olmad���na dikkat edilir.(K���K BORU UYGULMASI HAR��)\n");
    ui->plainTextEdit->appendPlainText("2. Yukar�da sa�daki resimde ise K��Elerin ve");
    ui->plainTextEdit->appendPlainText("MERKEZ noktas�n�n do�ru tespit edilip edilemedi�ine dikkat edilir.\n");
    ui->plainTextEdit->appendPlainText("E�er bu iki kritere g�re do�ru alg�lama yap�lamad�ysa, kamera kurulumunda bir hata var demektir.");
    ui->plainTextEdit->appendPlainText("Yaz�l�m�n tespit edebildi�i hatalar, yukar�daki analiz mesaj�nda belirtilir ve sizin onay vermeniz engellenir.");
    ui->plainTextEdit->appendPlainText("Hata durumunda veya tespit do�rulu�undaki anormalliklerde TEKN�K SERV�S�N�Z ile irtibata ge�iniz.");

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

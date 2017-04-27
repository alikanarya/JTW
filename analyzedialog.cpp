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
    //flags |= Qt::FramelessWindowHint;
    flags |= Qt::WindowTitleHint;
    this->setWindowFlags(flags);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    ui->setupUi(this);

    ui->labelMono->setPixmap(QPixmap::fromImage(iprocess->imgMono));
    ui->labelAnalyze->setPixmap(QPixmap::fromImage(iprocess->imgCorner));

    ui->plainTextEdit->appendPlainText("ANALİZ SONUCU");
    ui->plainTextEdit->appendPlainText("--------------------------------------------------");
    ui->plainTextEdit->appendPlainText("Analiz " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.");
    ui->plainTextEdit->appendPlainText("Analiz Mesajı: " + iprocess->statusMessage);
    ui->plainTextEdit->appendPlainText("Çizgi açısı: " + QString::number(iprocess->angleAvg) + " Derece");
    ui->plainTextEdit->appendPlainText("--------------------------------------------------");

    ui->plainTextEdit->appendPlainText("BU YAZILIM;");
    ui->plainTextEdit->appendPlainText("YAPTIĞI TESPİTİN DOĞRULUĞUNA KENDİLİĞİNDEN KARAR VEREMEZ !");
    ui->plainTextEdit->appendPlainText("BU SEBEPLE YUKARIDAKİ GÖRÜNTÜLERİN OPERATÖR TARAFINDAN ONAYLANMASI GEREKMEKTEDİR.\n");

//    ui->plainTextEdit->appendPlainText("* Yukarıda soldaki resimde, görüntüde PARLAMA olup olmadığı ve lazer çizgisinin");
//    ui->plainTextEdit->appendPlainText("NET bir şekilde seçilebilir olup olmadığına dikkat edilir.(İNCE KAYNAK AĞZI UYGULMASI HARİÇ)\n");

    ui->plainTextEdit->appendPlainText("* Yukarıda sağdaki resimde ise KÖŞElerin ve");
    ui->plainTextEdit->appendPlainText("MERKEZ noktasının doğru tespit edilip edilemediğine dikkat edilir.\n");

    ui->plainTextEdit->appendPlainText("Eğer bu kriter(ler)e göre doğru algılama yapılamadıysa, kamera kurulumunda bir hata var demektir.");
    ui->plainTextEdit->appendPlainText("Yazılımın tespit edebildiği hatalar, yukarıdaki analiz mesajında belirtilir ve sizin onay vermeniz engellenir.");
    ui->plainTextEdit->appendPlainText("Hata durumunda veya tespit doğruluğundaki anormalliklerde TEKNİK SERVİSİNİZ ile irtibata geçiniz.");

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

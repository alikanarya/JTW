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

    QImage correctImage;
    correctImage.load(":/pixmaps/resources/Algo1.jpg");
    //ui->labelMono->setPixmap(QPixmap::fromImage(iprocess->imgMono));
    ui->labelMono->setPixmap(QPixmap::fromImage(correctImage));

    if (w->thinJointAlgoActive && w->algorithmType == 2) {
        minCostedLines *centerline = new minCostedLines();
        centerline->c = iprocess->centerC;
        centerline->cost = 0;
        ui->labelAnalyze->setPixmap(QPixmap::fromImage( iprocess->drawLine(centerline, iprocess->slopeBest) ));
    } else
    if (w->thinJointAlgoActive && w->algorithmType == 3) {
        iprocess->constructContrastMatrixMajor2Lines();
        ui->labelAnalyze->setPixmap(QPixmap::fromImage( *iprocess->getImage(iprocess->contrastMatrix, iprocess->imageWidth, iprocess->imageHeight) ));
    } else
    if (w->thinJointAlgoActive && w->algorithmType == 4) {
        ui->labelAnalyze->setPixmap(QPixmap::fromImage( iprocess->getImageMainEdges(1) ));
    } else
        ui->labelAnalyze->setPixmap(QPixmap::fromImage(iprocess->imgCorner));


    /*
    ui->plainTextEdit->appendPlainText("ANALİZ SONUCU");
    ui->plainTextEdit->appendPlainText("--------------------------------------------------");
    ui->plainTextEdit->appendPlainText("Analiz " + QString::number(processElapsed) + " milisaniye içinde gerçekleştirildi.");
    ui->plainTextEdit->appendPlainText("Analiz Mesajı: " + iprocess->statusMessage);
    ui->plainTextEdit->appendPlainText("Çizgi açısı: " + QString::number(iprocess->angleAvg) + " Derece");
    ui->plainTextEdit->appendPlainText("--------------------------------------------------");
    */
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

    ui->cancelButton_2->setEnabled(false);
    ui->cancelButton_2->hide();
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

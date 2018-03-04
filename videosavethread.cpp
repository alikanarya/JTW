#include "videosavethread.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;


videoSaveThread::videoSaveThread(){

    stopped = false;
}


void videoSaveThread::run(){

    if (!stopped){

        if (w->videoMode == 0) {
            if (!QDir(w->folderName).exists())
                QDir().mkdir(w->folderName);

            for (int i = 0; i < saveSize; i++){
               buffer[i].save(w->folderName + "pic" + QString("%1").arg(count * bufferLength + i, 5, 10, QChar('0')) + ".jpg");
               //buffer[i].save(w->folderName + "pic" + QString::number(count * bufferLength + i) + ".jpg");
            }

            if (lastSave){
                count = -1;
                saveSize = bufferLength;
                lastSave = false;
                delete[] w->videoList;
                emit this->saveFinished();
            }
        }
        else if (w->videoMode == 1) {
            bufferImage.save(w->folderName + QDateTime::currentDateTime().toString("hhmmss_zzz") + ".jpg");
            //emit this->saveFinished();
        }
    }

    stopped = false;
}


void videoSaveThread::stop(){

    stopped = true;

}


videoSaveThread::~videoSaveThread(){
}


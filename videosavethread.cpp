#include "videosavethread.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;


videoSaveThread::videoSaveThread(){

    stopped = false;
}


void videoSaveThread::run(){

    if (!stopped){

        if (!QDir(w->folderName).exists())
            QDir().mkdir(w->folderName);

        for (int i = 0; i < w->videoFrameSize; i++){
            w->videoList[i].save(w->folderName + "pic" + QString::number(i) + ".jpg");
        }

        delete[] w->videoList;

        emit this->saveFinished();
    }

    stopped = false;
}


void videoSaveThread::stop(){

    stopped = true;

}


videoSaveThread::~videoSaveThread(){
}


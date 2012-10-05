#include "videosavethread.h"

#include "mainwindow.h"

extern MainWindow *w;


videoSaveThread::videoSaveThread(){

    stopped = false;
}


void videoSaveThread::run(){

    if (!stopped){

        for (int i = 0; i < w->videoList.size(); i++){
            w->videoList[i]->save(w->savePath + w->folderName + "pic" + QString::number(i) + ".jpg");
        }

    }

    stopped = false;
}


void videoSaveThread::stop(){

    stopped = true;

}


videoSaveThread::~videoSaveThread(){
}


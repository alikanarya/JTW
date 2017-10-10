#include "autofocusthread.h"

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
extern MainWindow *w;

autoFocusThread::autoFocusThread(int _initialSampleSize, int _iterNumber){
    initialSampleSize = _initialSampleSize;
    iterNumber = _iterNumber;
}

void autoFocusThread::run(){

    qDebug() << "thread started";

    sampleSize = initialSampleSize;
    dMax = 1.0;

    for (int i=0; i<iterNumber; i++) {

        step = dMax / (sampleSize+1);
        sampleStart = step;
        sampleEnd = step*sampleSize;
        dMax = sampleEnd - sampleStart;

        for (j=1; j<=sampleSize; j++) {

            //w->camApi->apiDahuaSetFocusPos(j*step);
            emit setFocusPos(j*step);

            mMutex.lock();
            //if (restart)
                condition.wait(&mMutex);
            mMutex.unlock();
        }

    }

}



/*
while (!mStop) {
    for (int i=0; i<5; i++){
        QThread::msleep(1000);
        qDebug() << i << " ,  " << restart;
        mMutex.lock();
        //if (restart)
            condition.wait(&mMutex);
        mMutex.unlock();
    }
}
*/

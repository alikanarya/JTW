#include "autofocusthread.h"

#include <QDebug>

autoFocusThread::autoFocusThread()
{

}

void autoFocusThread::run(){

    while (!mStop) {

        for (int i=0; i<5; i++){

            sleep(1000);
            qDebug() << i;
            mMutex.lock();
            if (restart) condition.wait(&mMutex);
            mMutex.unlock();
        }

    }

}

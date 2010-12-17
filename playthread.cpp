#include "playthread.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

extern MainWindow *w;

playThread::playThread(){}

void playThread::run(){

    if (w->play && !w->pause){

        if (!w->imageGetter->imageList.isEmpty()){  // if any image is get
            int size = w->imageGetter->imageList.length();  // image buufer size
            bool show = true;

            w->lastData = w->imageGetter->imageList[size-1];    // get last data
            w->frameNo = w->lastData->requestId.toUInt();       // request id of net. data

            // check for current shown data is not a late from prev data
            if (size >= 2){     // to check min 2 data must exist

                for (int i = size - 2; i >= 0; i--)

                    if (w->imageGetter->imageList[i]->shown){       // if past images in the buffer were shown on display
                        w->prevData = w->imageGetter->imageList[i];

                        // if prev shown data's request id is higher than the cuurents data's request id
                        // then current data is late & DONT SHOW IT on display
                        if (w->frameNo <= w->prevData->requestId.toUInt()){
                            w->lateFrame++;
                            show = false;
                        }
                        break;
                    }
            }

            // show current valid image. valid: not late from previous (<show>)
            if (!w->lastData->shown && show){
                w->ui->imageFrame->setPixmap(QPixmap::fromImage(*w->lastData->image));
                w->ui->imageFrame->show();
                w->ui->guideFrame->raise();     // if guide is shown, suppress it
                w->lastData->shown = true;      // mark last data was SHOWN on display
                w->fpsReal++;

                // calculate (display time - request time) delay in msec
                int displayTime = time.getSystemTimeMsec();
                int requestTime = calcTotalMsec(w->lastData->requestHour.toInt(), w->lastData->requestMinute.toInt(), w->lastData->requestSecond.toInt(), w->lastData->requestMSecond.toInt());
                w->timeDelay = displayTime - requestTime;
                w->timeDelayTotal += w->timeDelay;  // overall delay

                // if joint is tracked for some???? interval
                if (w->trackOn && (w->fpsReal % 1) == 0 ){
                    w->processImage();  // detect deviation

                    if (w->deviationData.size() >= 2) w->drawTrack();   // draw deviation trend
                }
            }
        }
    }
}

int playThread::calcTotalMsec(int hour, int min, int second, int msec){
    return ( (hour * 3600 + min * 60 + second) * 1000 + msec );
}

#ifndef IMGPROCESSTHREAD_H
#define IMGPROCESSTHREAD_H

#include <QThread>

#include "../_Modules/Imgprocess/imgprocess.h"

class imgProcessThread : public QThread {

    Q_OBJECT

public:

    QImage targetArea;
    imgProcess *iprocess;
    bool iprocessInitSwitch;
    bool ready = true;
    int startTime, endTime;     // start/end time of image processing
    int processElapsed;         // elapsed time for image processing

    QList<int> histAreaStat;

    explicit imgProcessThread();
     ~imgProcessThread();

    void run();
    void stop();

    void edgeDetection();
    void Algo1();   // LASER: mono/edge(matrixFlag) > houghTr > detectLongestSolidLines
    void Algo2();   // LASER: mono/edge(matrixFlag) > houghTr > detectPrimaryVoid
    void Algo3();   // woLASER: edge > houghTr > detectMainEdges
    void Algo4();   // woLASER: value > detectThinJointCenter
    void Algo5();   // woLASER: value > contrast matrix > houghTr > calcAvgDistAndAngleOfMajors
    void Algo6();   // woLASER: canny1 > houghTr > detectMainEdges > thickenEdgeMap > scoreLineCrossing
    void Algo7();   // woLASER: edge > houghTr > detectScanHorizontal

    void histAnalysis();    // histogram analysis

signals:

    void imageProcessingCompleted();
    void histAnalysisCompleted();

private:

};

#endif // IMGPROCESSTHREAD_H

#ifndef VIDEOSAVETHREAD_H
#define VIDEOSAVETHREAD_H

#include <QThread>
#include <QImage>

class videoSaveThread : public QThread {
    Q_OBJECT

public:
    videoSaveThread();
    void stop();
    ~videoSaveThread();

    int bufferLength = 50;
    int count = -1;
    QImage buffer[50];
    bool lastSave = false;
    int saveSize = 0;

public slots:

protected:
    void run();

private:
    volatile bool stopped;

signals:
    void saveFinished();


};

#endif // VIDEOSAVETHREAD_H

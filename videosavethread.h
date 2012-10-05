#ifndef VIDEOSAVETHREAD_H
#define VIDEOSAVETHREAD_H

#include <QThread>

class videoSaveThread : public QThread {
    Q_OBJECT

public:
    videoSaveThread();
    void stop();

    ~videoSaveThread();

public slots:

protected:
    void run();

private:
    volatile bool stopped;

};

#endif // VIDEOSAVETHREAD_H

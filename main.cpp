#include <QtGui/QApplication>
#include "mainwindow.h"

MainWindow *w;

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    w = new MainWindow();

    //w->showMaximized();
    //settingsGui->move(rectScreen.center() - settingsGui->rect().center());
    w->showFullScreen();

    return a.exec();
}

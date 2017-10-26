//#include <QtWidgets/QApplication>
#include "mainwindow.h"

MainWindow *w;

int main(int argc, char *argv[]){
    int ix = 1;
    QApplication a(ix, argv);
    Q_INIT_RESOURCE (icons);


    // turkish char set support
    //QTextCodec::setCodecForTr( QTextCodec::codecForName("ISO-8859-9") );
    //QTextCodec::setCodecForCStrings( QTextCodec::codecForName("ISO-8859-9") );

    w = new MainWindow();
    w->show();

    //w->showMaximized();
    //settingsGui->move(rectScreen.center() - settingsGui->rect().center());
    //w->showFullScreen();

    return a.exec();
}

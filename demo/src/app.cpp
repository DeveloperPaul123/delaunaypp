#include <QDebug>
#include <QFileOpenEvent>

#include "demo/app.h"
#include "demo/window.h"

App::App(int argc, char *argv[]) :
    QApplication(argc, argv), window(new Window())
{
    window->show();
}

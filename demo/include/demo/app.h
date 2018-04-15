#pragma once

#include <QApplication>

class Window;

class App : public QApplication
{
    Q_OBJECT
public:
    explicit App(int argc, char *argv[]);
private:
    Window* const window;

};

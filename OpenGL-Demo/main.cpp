#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // create the OpenGL Versions and core profile
    // Select version 3.3
    QSurfaceFormat glFormat;
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(glFormat);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

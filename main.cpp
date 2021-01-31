#include "docklea.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DockLea w;
    w.showMaximized();;
    return a.exec();
}

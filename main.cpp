#include "banshee.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    banshee w;
    w.show();

    return a.exec();
}

#include "universalconverter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UniversalConverter w;
    w.show();
    return a.exec();
}

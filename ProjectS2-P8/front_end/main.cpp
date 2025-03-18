#include "front_end.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    front_end w;
    w.show();
    return a.exec();
}

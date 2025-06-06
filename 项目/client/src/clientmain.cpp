#include "clientwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    clientwidget w;
    w.show();
    return a.exec();
} 
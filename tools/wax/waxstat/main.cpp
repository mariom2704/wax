#include <QtGui/QApplication>
#include "waxstat.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WaxStat w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}

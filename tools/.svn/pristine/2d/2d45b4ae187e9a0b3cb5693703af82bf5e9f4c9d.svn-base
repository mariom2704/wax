#include <QtGui/QApplication>
#include "updateassistqt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    updateassistqt w;
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();
}

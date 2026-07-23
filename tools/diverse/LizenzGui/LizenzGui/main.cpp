#include <QApplication>
#include "LizenzGui.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LizenzGui w;

	if(w.get_ExitState())
	{
		return 1;
	}

	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}

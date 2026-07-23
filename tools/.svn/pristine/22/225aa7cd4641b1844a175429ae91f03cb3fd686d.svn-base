#include "spanxdiff.h"
#include <qt\qapplication.h>
#include <qt\qtranslator.h>

QApplication *mainapp;
int main( int argc, char ** argv )
{
	QApplication app(argc, argv);
	
	
	mainapp = &app;

	spanxdiff a;

	app.setActiveWindow(&a);
	a.init();

   return app.exec();
}

#include "Kaufhof.h"
#include <qt\qapplication.h>
#include <qt\qtranslator.h>

QApplication *mainapp;
int main( int argc, char ** argv )
{
	QApplication app(argc, argv);
	//QTranslator translator( 0 );
	//translator.load( "tt2_fr.qm", "." );
	//translator.load( "tt2_enu.qm", "." );
	
	//app.installTranslator( &translator );
	
	mainapp = &app;

	Kaufhof a;
	app.setActiveWindow(&a);
	a.init();

   return app.exec();
}

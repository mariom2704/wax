#include "stdafx.h"
#include <mxutil/tracer.h>
#include <iostream>

#include "SqlFile.h"
#include "Output.h"
#include "SqlExecutor.h"

using namespace MxUtil;
using namespace std;
using namespace SqlExec;

#define TRFILE "Trace.txt"

int main(int argc, char* argv[])
{
	if (argc != 5 && argc != 3)
	{
		cout << "Aufruf: sqlexec <ODBC-DSN oder Dateiname (bei z.B. \"odwax->spanx\" die Tuetelchen nicht vergessen!> <SQLDATEI> [USER] [PASSWORT]" << endl;
		cout << "USER und PASSWORT nur fuer MySql etc." << endl;
		return 1;
	}

	string fn = argv[2];
	string dsn = argv[1];
	string user, passwd;
	
	if (argc == 5)
	{
		user = argv[3];
		passwd = argv[4];
	}
	cout << "Datenbank " << dsn << " wird geoeffnet" << endl;

	MxOdbc::Database db(dsn, user, passwd);
	if (!db.open())
	{
		cout << "Datenbank nicht erreichbar!" << endl;
		cout << db.get_errorhandler().get_errors().begin()->get_description() << endl;
		return 1;
	}

	SqlFile sqlf(fn);
	Output cmt;
	SqlExecutor sqlex;
	SqlExecutor::set_db(&db);

	if (!sqlf.is_open())
	{
		cmt.comment("Datei " + fn + " nicht gefunden");
		//Tracer::trace2file(TRFILE, "SQL File nicht gefunden" + fn);
		return 1;
	}

	sqlf.signal_comment.connect( boost::bind(&Output::comment, cmt, _1) );
	sqlf.signal_sql.connect( boost::bind(&SqlExecutor::exec, sqlex, _1) );

	try
	{
		sqlf.process();
	}
	catch (MxBase::Error e)
	{
		cout << e.get_description() << endl;
		cout << "Fehler bei SQL Ausführung" << endl;
		db.rollback_trans();
	}
}

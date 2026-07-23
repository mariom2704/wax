#include "stdafx.h"

#include <iostream>
#include <mxodbc2/Connection.h>
#include <boost/boost.h>
#include <mxodbc2/Database.h>

#define OTL_ODBC
#define OTL_EXPLICIT_NAMESPACES
#include <mxodbc/otlv4.h>

#include "FetchNl.h"
#include "FetchRegion.h"
#include "FetchPreise.h"
#include "PreisInsert.h"


#include <fstream>

using namespace std;

// Wenn NL Kennung nicht gefunden wurde
const int default_region = 2006;

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 3)
	{
		cout << "Aufruf: initpreise spanx.mdb spanxstamm.mdb" << endl;
		return 1;
	}

	string dsn = argv[1];
	string stammdb = argv[2];

	odbc::otl_connect db, sdb; // connect object
	

	dsn = MxOdbc::Connection().create_connectstring(dsn);
	stammdb = MxOdbc::Connection().create_connectstring(stammdb);

	
	 odbc::otl_connect::otl_initialize(); 


	 // initialize ODBC environment 
	 try
	 {  
		db.rlogon(dsn.c_str(), true);   
		sdb.rlogon(stammdb.c_str());
		
		int nl = FetchNl(db).query();
		int region = FetchRegion(sdb).query(nl, default_region);

		std::map<std::string, double> strlst;
		std::map<int, double> artnrlst;

		int cnt = FetchPreise(db).query(strlst, artnrlst);

		cout << "Nl Kennung " << nl << endl;
		cout << "Preisumsetzung auf Region " << region << " von " << cnt << " Artikeln" << endl;

		for (std::map<std::string, double>::iterator it = strlst.begin(); it != strlst.end(); it++)
		{
			PreisInsert(db).insert(it->first, 0, region, it->second);
			cout << "Code: " << it->first << " Preis: " << it->second << endl;
		}

		for (std::map<int, double>::iterator it = artnrlst.begin(); it != artnrlst.end(); it++)
		{
			PreisInsert(db).insert("-", it->first, region, it->second);
			cout << "Artikelnr: " << it->first << " Preis: " << it->second << endl;
		}
	 } 
	 catch(odbc::otl_exception& p)
	 { // intercept OTL exceptions  
		 cerr<<p.msg<<endl; // print out error message 
		 cerr<<p.stm_text<<endl; // print out SQL that caused the error  
		 cerr<<p.sqlstate<<endl; // print out SQLSTATE message  
		 cerr<<p.var_info<<endl; // print out the variable that caused the error 
	 } 
	 db.logoff(); // disconnect from the database return 0;

	return 0;
}


#pragma once

// for BOOST testing
#include <boost/boost.h>
#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;



#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header ausschließen
#define WINVER 0x0501		// Ändern Sie den entsprechenden Wert, um auf andere Versionen von Windows abzuzielen.
#include "../waxstam.h"
#include "../rohrstutzenbrennzugabe.h"


class test_rohrstutzenbrennzugabe
{
public:
	
	test_rohrstutzenbrennzugabe() 
	{		
	}

	~test_rohrstutzenbrennzugabe()
	{
		
	}

	virtual void test_brennzugabe()
	{
		cout << "Test der Tabelle rohrstutzen_brennzugabe" << endl;
		static struct _testd { int verteilerdn; int stutzendn; int result; } testdata[] =
		{
			// Alte Werte übernommen aus Sourcecode bis stutzendn = 50
			// Neue Daten aus Tabelle ab Stutzendn 65
			65, 32, 15,
			65, 40, 15,
			65, 50, 20,
			65, 65, 30,

			80, 32, 15,
			80, 40, 15,
			80, 50, 20,
			80, 65, 30,
			80, 80, 30,

			100, 32, 15,
			100, 40, 15,
			100, 50, 20,
			100, 65, 20,
			100, 80, 25,
			100, 100, 45,

			125, 32, 15,
			125, 40, 15,
			125, 50, 20,
			125, 65, 20,
			125, 80, 20,
			125, 100, 35,
			125, 125, 55,
			
			150, 32, 15,
			150, 40, 15,
			150, 50, 20,
			150, 65, 20,
			150, 80, 20,
			150, 100, 30,
			150, 125, 45,
			150, 150, 65,

			// Falscher Aufruf liefert 0
			151, 65, 0,
			150, 82, 0,
			-1, 5, 0,
			-2222150, 150, 0,
			-9999, -9999, 0,
			0, 0, 0,
			-1,-1,-1,
		};
/*
StutzenDN   =   32, 40, 50, 65, 80, 100, 125
Brennzugabe = 15, 15, 20, 45, 50,  65,  75
*/
		for (int i = 0; testdata[i].verteilerdn != -1 && testdata[i].stutzendn != -1 && testdata[i].result != -1; i++)
		{
			string msg = boost::str( boost::format("VerteilerDN=%1%, StutzenDN = %2% -> Brennzugabe = %3%") % testdata[i].verteilerdn % testdata[i].stutzendn % testdata[i].result);
			BOOST_CHECK_MESSAGE(getStammdaten().queryBrennzugabe.get_brennzugabe(testdata[i].verteilerdn, testdata[i].stutzendn) == testdata[i].result, msg);
		}

	}
};
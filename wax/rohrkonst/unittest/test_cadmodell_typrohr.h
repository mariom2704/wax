#pragma once

// for BOOST testing
#include <boost/boost.h>
#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;
using boost::unit_test_framework::test_case;



#define VC_EXTRALEAN		// Selten verwendete Teile der Windows-Header ausschließen
#define WINVER 0x0501		// Ändern Sie den entsprechenden Wert, um auf andere Versionen von Windows abzuzielen.

#include "../cadmodell_typrohr.h"
using namespace CadModell;

class test_cadmodell_typrohr
{
public:
	
	void test_len()
	{
		/**
		Erzeugt 3D-Koordiaten für Rohre (Typrohr), deren Länge richtig ermittelt werden muss
		*/
		cout << "Teste Ermittlung der Rohrlaenge" << endl;

		const struct __testst 
		{ 
			Point a; // Rohranfang im 3D Raum
			Point b; // Rohrende im 3D Raum
			int result; // Länge des Rohres
		} testdat[] =
		{
			// Rohr nach rechts
			Point(0,0), Point(10,0), 10,
			Point(0,0), Point(20,0), 20,
			// Rohr nach unten
			Point(0,0), Point(0,10), 10,
			// Rohr im ersten Quadrant 
			Point(0,0), Point(10,20), 22,
			// Rohr im zweiten Quadrant 
			Point(0,0), Point(-10,20), 22,
			// Rohr im dritten Quadrant 
			Point(0,0), Point(-10,-20), 22,
			// Rohr im vierten Quadrant 
			Point(0,0), Point(10,-20), 22,
			Point(0,0), Point(10,0), 10,
			Point(0,0), Point(0,0), 0,
		};
		
		for (int i = 0; testdat[i].a != Point() || testdat[i].b != Point() || testdat[i].result; i++)
		{
			std::string msg = boost::str( boost::format("Len(): [%1%,%2%] -> [%3%,%4%] : %5%") % testdat[i].a.x % testdat[i].a.y % testdat[i].b.x % testdat[i].b.y % testdat[i].result);
			TypRohr x(0, 0, testdat[i].a, testdat[i].b);

			BOOST_CHECK_MESSAGE(x.len() == testdat[i].result, msg);
		}
	}
};
#pragma once

#include "StammDat.h"

/**
Tabelle Montagezeiten. Wird für die Spanx/Wax Schnittstelle gebraucht
und enthält die Montagezeiten für einen Sprinkler bei Strangrohr bzw. 
einen Meter Verteilerleitung.
*/
class WAXSTAMMDLL Montagezeiten 
: public StammDat
{
public:
	class WAXSTAMMDLL data 
	: public CObject
	{
	public:
		/// Entspricht den Einträgen in der Spalte rohrtyp
		enum ROHRTYPEN 
		{
		
			STRANGROHR = 1,

			VERTEILERLEITUNG = 2,

			HAUPTLEITUNG = 3,

			GEWINDEROHR = 4
		};

		enum ARBEITSCHRITTE_STRANGROHR
		{
			S_ROHR_TRANSPORT = 1,
			S_HALTER_SETZEN = 2,
			S_SPRINKLER_MONTIEREN = 3,
			S_SPRINKLERSCHELLE_MONTIEREN = 4,
			S_ROHR_TRANSPORT2 = 5,
			S_ROHR_IN_HALTER_MONTIEREN = 6,
			S_ROHRKUPPLUNG_MONTIEREN = 7
		};

		enum ARBEITSCHRITTE_VERTEILER
		{
			V_ROHR_TRANSPORT = 1,
			V_HALTER_SETZEN = 2,
			V_ANBOHRSCHELLE_MONTIEREN = 3,
			V_ROHR_AUF_HEBEBUEHNE = 4,
			V_ROHR_IN_HALTER_MONTIEREN = 5,
			V_ROHRKUPPLUNG_MONTIEREN = 6,
			V_FORMSTUECKE = 7,
		};
	
		data();

		virtual ~data();

		const int get_dn() const;

		void set_dn(int value);

		const int get_rohrtyp() const;

		void set_rohrtyp(int value);

		const int get_hoehe() const;

		void set_hoehe(int value);

		const double get_zeit() const;

		void set_zeit(double value);

		void set_aktion(int value);

		const int get_aktion() const;

	private:
	friend class Montagezeiten;

		int dn;

		int rohrtyp;

		int hoehe;

		double zeit;

		/// ID des Arbeitsschrittes
		int aktion;

	};


	Montagezeiten();

	virtual ~Montagezeiten();

	virtual void load(const std::string& db);

};



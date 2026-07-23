#pragma once

#include "StammDat.h"

/**
Dateninhalt der Tabelle Krantaetigkeit
*/
class WAXSTAMMDLL Krantaetigkeit : public StammDat
{
public:
	Krantaetigkeit();

	virtual void load(const std::string& db);

	enum INDIZES 
	{
		DN25_65 = 0, 
		DN80_125 = 1,
		DN150_200 = 2, 
		DN250_400 = 3,
		ZEIT_PRO_TRANSPORT = 4,
	};

	double getWert(int index);
	
	class WAXSTAMMDLL data : public CObject
	{
	public:
		data();

		const int get_index() const;

		const double get_wert() const;

	private:
		int index;

		double wert;

	friend class Krantaetigkeit;
	};
};

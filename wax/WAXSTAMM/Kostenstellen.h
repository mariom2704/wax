#pragma once

#include "StammDat.h"

class WAXSTAMMDLL Kostenstellen : public StammDat  
{
public:
	Kostenstellen();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data : 
public CObject  
	{
	public:
		const int get_id() const;

		data();

		const CString& get_bezeichnung() const;

		const double get_tarif() const;

		const double get_grenztarif() const;

		const double get_zuschlag() const;

		const double get_grenzZuschlag() const;

		const long get_nr() const;

	private:
		int id;

		long nr;

		CString bezeichnung;

		double tarif;

		double zuschlag;

		double grenztarif;

		double grenzZuschlag;

	friend class Kostenstellen;
	};


public:

	
	enum KOSTENSTELLEN 
	{
	
		AUSWAERTIGE_FERTIGUNG = 1,

		AV_STEUERUNG = 2,

		SCHWEISSEREI = 3,

		SCHWEISSZENTRUM = 4,

		VERTEILERROHRSTRASSEN = 5,

		SCHLOSSEREI = 6,

		LACKIEREREI = 7,

		BESCHICHTUNG = 8
	};


	double getTarif(long nr);

	double getGrenztarif(long nr);

	double getZuschlag(long nr);

	double getGrenzZuschlag(long nr);


	long getNr(int id);

};


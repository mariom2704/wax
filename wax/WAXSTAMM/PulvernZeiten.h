#pragma once

#include "StammDat.h"

class WAXSTAMMDLL PulvernZeiten : public StammDat  
{
public:
	PulvernZeiten();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data : 
public CObject  
	  
	{
	public:
		data();

		const int get_abglaenge_bis() const;

		const int get_abglaenge_von() const;

		const int get_dn() const;

		const int get_laenge_bis() const;

		const int get_laenge_von() const;

		const double get_zeit() const;

	private:
		int laenge_von;

		int laenge_bis;

		int abglaenge_von;

		int abglaenge_bis;

		int dn;

		double zeit;

	friend class PulvernZeiten;
	};
};


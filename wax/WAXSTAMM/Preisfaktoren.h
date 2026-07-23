#pragma once

#include "StammDat.h"

class WAXSTAMMDLL Preisfaktoren : public StammDat  
{
public:
	enum PREISFAKTOREN 
	{
		//Materialgemeinkosten (MGK) Herstelkosten
		HKMATERIALKOSTEN = 1,

		//Materialgemeinkosten (MGK) Grenzkosten
		GKMATREIALKOSTEN = 2,

		//Vertriebs- und Verwaltungskosten (V&V) Vollkosten
		VKVERWALTUNGSKOSTEN = 3,

		//Vertriebs- und Verwaltungskosten (V&V) Grenzkosten
		GKVERWALTUNGSKOSTEN = 4,


		//Strangrohrmuffenverschnitt. Aufschlag auf Material (Nur für SAP)
		STRANGROHRMUFFEN = 8,

		//Strangrohrverschnitt. Aufschlag für die Strangrohre für Materialentnahme.
		STRANGROHRVERSCHNITT = 9,

		//Verteilerrohrverschnitt. Aufschlag auf das Material wegen Rohrverschnitt
		VERTEILERVERSCHNITT = 11,

	};

	double getPreisfaktor(int index);

	class WAXSTAMMDLL data : public CObject  
	{
	private:
		int index;

		double prozent;

		CString text;

	public:
		const int get_index() const;

		const double get_prozent() const;

		const CString& get_text() const;

		data();

	friend class Preisfaktoren;
	};

	Preisfaktoren();

	virtual void load(const std::string& db);
};


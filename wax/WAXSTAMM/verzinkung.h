#pragma once

#include "StammDat.h"

/**
Tabelle mit Verzinkereien und deren Tarifen
*/
class WAXSTAMMDLL Verzinkung : public StammDat  
{
public:
	Verzinkung();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data : public CObject  
	{
		friend class Verzinkung;
	public:
		data();

		/// laufende Nummer
		int get_nr() const;

		/// Name der Verzinkerei
		CString get_name() const;

		/// Kosten Euro pro Kilo Zink
		double get_euro_pro_kg() const;

		double get_gewichtszuschlag() const;
		/// Prozentualer Aufschlag
		double get_zinkpreisausgleich() const;

		/// Stundensatz
		double get_stdsatz() const;

		/// Prozentualer Aufschlag auf VK
		double get_vkzuschlag() const;

		double get_mgk() const;
	private:
		int nr;

		CString name;

		double euro_pro_kg;
		double gewichtszuschlag;
		double zinkpreisausgleich; // vorher zuschlag

		double stdsatz;
	
		double vkzuschlag;
		double mgk;
	};

	const Verzinkung::data* query(int nr) const;
};



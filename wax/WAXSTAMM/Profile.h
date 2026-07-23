#pragma once
#include "StammDat.h"

//Stammdatentabelle Profile
class WAXSTAMMDLL Profile : public StammDat  
  
{
public:	
	Profile();

	virtual void load(const std::string& db);

	class WAXSTAMMDLL data : public CObject  
	{
	public:
		CString get_text();

		const double get_gewicht() const;

		const int get_elemid() const;

		const CString& get_kurztext() const;

		const CString& get_typ() const;

		const int get_a_h() const;

		const int get_b() const;

		const int get_s() const;

		const int get_artikelnr() const;

		const bool get_einkauf() const;

		const double get_festPreis() const;

		const double get_sapPreis() const;

		const bool get_schwarz() const;

		const bool get_halbzeug() const;

		const bool get_co2tikko() const;

	private:
		double gewicht;

		int elemid;

		CString kurztext;

		CString typ;

		int a_h;

		int b;

		int s;

		int artikelnr;

		bool einkauf;

		/// Preis
		double festPreis;

		/// SapPreis (GLD)
		double sapPreis;

		bool schwarz;

		bool halbzeug;

		bool co2tikko;

	friend class Profile;
	};

	
};

#pragma once

#include "StammDat.h"


/// Datensatzgruppe für Formstücke und Fittinge
class WAXSTAMMDLL FormstueckeUndFittinge : public StammDat  
{
public:
	/// Konstruktor
	FormstueckeUndFittinge();

	/// Laden der Tabellendaten
	virtual void load(const std::string& db);

	enum PREISART
	{
		festPreis,
		sapPreis,
		pulverPreis
	};
	double getPreis(int nr, FormstueckeUndFittinge::PREISART preisart);
	CString getKurztext(int nr);

	class WAXSTAMMDLL data : public CObject  
	{
		
	public:


		CString setKurztext(int elemid, CString kt = "", int gruppenid = 0, int dn1 = 0, int dn2 = 0, int dn3 = 0, int dn4 = 0, 
							CString iso = "", CString gf = "", bool schwarz = false, bool neunzigbar = false);


		const int get_zmass1() const;

		const int get_zmass2() const;
		
		const int get_zmass3() const;

		const int get_laenge() const;

		const int get_gruppenid() const;

		const bool get_einkauf() const;


		const bool get_halbzeug() const;

		const bool get_wasser_sprinkler() const;

		bool get_neunzigbar() const;

		CObject* theCObject;

		data();
		
		bool get_schwarz() const;

		const CString& get_iso() const;

		const CString& get_gfzeichen() const;

		int get_dn1() const;

		int get_dn2() const;

		int get_dn3() const;

		int get_dn4() const;

		int get_elemid() const;

		int get_artikelnr() const;

		const CString& get_gruppe() const;

		double get_gewicht() const;

		const CString& get_kurztext() const;

		double get_festPreis() const;

		double get_sapPreis() const;

		double get_pulverpreis() const;

	private:
		friend class FormstueckeUndFittinge;

		int gruppenid;

		bool einkauf;

		bool halbzeug;


		bool wasser_sprinkler;

		/// Gas 90 bar
		bool neunzigbar;

		/// Schwarzes oder verzinkte Elemente
		bool schwarz;

		/// Iso Kurzzeichen
		CString iso;

		CString gfzeichen;

		/// Durchmesser1
		int dn1;

		/// Durchmesser2
		int dn2;

		int dn3;

		int dn4;
	
		int elem_ID;
	
		double festPreis;

		double sapPreis;

		double pulverpreis;

		double gewicht;

		CString kurztext;

		int artikelnr;

		CString gruppe;

		int zmass1;

		int zmass2;
		
		int zmass3;
	
		int laenge;
	};

public:
	enum GRUPPENID 
	{
		UNKNOWN = 0,

		ANBOHRSCHELLE = 1,

		BOGEN = 2,

		DOPPELNIPPEL = 3,

		ENDKAPPE = 4,

		KAPPE = 5,

		KREUZ_STUECK = 6,

		KUPPLUNG = 7,

		F_MUFFE = 8,

		RED_KUPPLUNG = 9,

		RED_MUFFE = 10,

		RED_NIPPEL = 11,

		REDUZIERSTUECK = 12,

		STOPFEN = 13,

		T_STUECK = 14,

		VERLAENGERUNG = 15,

		VERSCHRAUBUNG = 16,

		WINKEL = 17,

		WINKEL_A1_45 = 18,

		WINKEL_A4 = 19,

		WINKELVERSCHRAUBUNG = 20,

		WINKEL_30 = 21,
	};
};

